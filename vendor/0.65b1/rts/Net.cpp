#include "stdafx.h"
// Net.cpp: implementation of the CNet class.
//
//////////////////////////////////////////////////////////////////////

#include "Net.h"
#include "stdio.h"
#include "filehandler.h"
#include "infoconsole.h"
#include "game.h"
#include "scripthandler.h"
#include "gamesetup.h"
#include "team.h"
#include "gameversion.h"
#include <direct.h>
#include ".\net.h"
//#include "mmgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define NETWORK_VERSION 1

CNet* net=0;
CNet* serverNet=0;
extern std::string stupidGlobalMapname;
extern int stupidGlobalMapId;

unsigned char netbuf[NETWORK_BUFFER_SIZE];	//buffer space for outgoing data

CNet::CNet()
{
	LARGE_INTEGER t,f;
	QueryPerformanceCounter(&t);
	QueryPerformanceFrequency(&f);
	curTime=double(t.QuadPart)/double(f.QuadPart);

	WORD wVersionRequested;
	WSADATA wsaData;int err; 
	wVersionRequested = MAKEWORD( 2, 2 ); 
	err = WSAStartup( wVersionRequested, &wsaData );if ( err != 0 ) {
		MessageBox(NULL,"Couldnt initialize winsock.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		return;
	} 
	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */ 
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
        HIBYTE( wsaData.wVersion ) != 2 ) {
		MessageBox(NULL,"Wrong WSA version.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		WSACleanup( );
		return; 
	}
	connected=false;

	for(int a=0;a<MAX_PLAYERS;a++){
		connections[a].active=false;
		connections[a].localConnection=0;
	}

	imServer=false;
	onlyLocal=false;
	inInitialConnect=false;

	recordDemo=0;
	playbackDemo=0;
	mySocket=0;

	netMutex=CreateMutex(0,false,"SpringNetLocalBufferMutex");
}

CNet::~CNet()
{
	if(connected && (imServer || !connections[0].localConnection)){
		unsigned char t=NETMSG_QUIT;
		SendData(&t,1);
		FlushNet();
	}
	if(mySocket)
		closesocket(mySocket);

	WSACleanup( );

	delete recordDemo;
	delete playbackDemo;

	for (int a=0;a<MAX_PLAYERS;a++){
		Connection* c=&connections[a];
		if(!c->active)
			continue;
		std::deque<Packet*>::iterator pi;
		for(pi=c->unackedPackets.begin();pi!=c->unackedPackets.end();++pi)
			delete (*pi);
		std::map<int,Packet*>::iterator pi2;
		for(pi2=c->waitingPackets.begin();pi2!=c->waitingPackets.end();++pi2)
			delete (pi2->second);
	}
	CloseHandle(netMutex);
}

int CNet::InitServer(int portnum)
{
	connected=true;
	waitOnCon=true;
	imServer=true;

	if ((mySocket= socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET ){ /* create socket */
		MessageBox(NULL,"Error initializing socket as server.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		connected=false;
		exit(0);
	}

	SOCKADDR_IN saMe;

	saMe.sin_family = AF_INET;
	saMe.sin_addr.s_addr = INADDR_ANY; // Let WinSock assign address
	saMe.sin_port = htons(portnum);	   // Use port passed from user

	if (bind(mySocket,(struct sockaddr *)&saMe,sizeof(struct sockaddr_in)) == SOCKET_ERROR) {
		closesocket(mySocket);
		MessageBox(NULL,"Error binding socket as server.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		exit(0);
	}

	u_long u=1;
	ioctlsocket(mySocket,FIONBIO,&u);

	return 0;
}

void CNet::StopListening()
{
	waitOnCon=false;
}

int CNet::InitClient(const char *server, int portnum,int sourceport,bool localConnect)
{
  LPHOSTENT lpHostEntry;

	LARGE_INTEGER t,f;
	QueryPerformanceCounter(&t);
	QueryPerformanceFrequency(&f);
	curTime=double(t.QuadPart)/f.QuadPart;

	if(FindDemoFile(server)){
		onlyLocal=true;
		connections[0].active=true;
		connections[0].readyLength=0;
		connected=true;
		gu->spectating=true;
		return 1;
	} else {
		if(!gameSetup || !gameSetup->hostDemo)
			CreateDemoFile();
	}

	SOCKADDR_IN saOther;

	saOther.sin_family = AF_INET;
	saOther.sin_port = htons(portnum);

	unsigned long ul;
	if((ul=inet_addr(server))!=INADDR_NONE){
		saOther.sin_addr.S_un.S_addr = 	ul;
	} else {
		lpHostEntry = gethostbyname(server);
		if (lpHostEntry == NULL)
		{
			MessageBox(NULL,"Error looking up server from dns.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			exit(0);
			return -1;
		}
		saOther.sin_addr = 	*((LPIN_ADDR)*lpHostEntry->h_addr_list);	
	}

	if ((mySocket= socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET ){ /* create socket */
		MessageBox(NULL,"Error initializing socket.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		exit(0);
		return -1;
	}

	if(strcmp(server,"localhost")!=0){
		SOCKADDR_IN saMe;
		saMe.sin_family = AF_INET;
		saMe.sin_addr.s_addr = INADDR_ANY; // Let WinSock assign address
		saMe.sin_port = htons(sourceport);	   // Use port passed from user

		int numTries=0;
		while (bind(mySocket,(struct sockaddr *)&saMe,sizeof(struct sockaddr_in)) == SOCKET_ERROR) {
			numTries++;
			if(numTries>10){
				closesocket(mySocket);
				MessageBox(NULL,"Error binding socket as client.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
				exit(0);
			}
			saMe.sin_port = htons(sourceport+numTries);
		}
	}

	u_long u=1;
	ioctlsocket(mySocket,FIONBIO,&u);

	waitOnCon=false;
	imServer=false;

	InitNewConn(&saOther,false,0);
	if(!localConnect){
		tempbuf[0]=NETMSG_ATTEMPTCONNECT;
		if(gameSetup)
			tempbuf[1]=gameSetup->myPlayer;
		else
			tempbuf[1]=0;
		tempbuf[2]=NETWORK_VERSION;
		SendData(tempbuf,3);
		FlushNet();
		inInitialConnect=true;
	}
	return 1;
}

int CNet::SendData(unsigned char *data, int length)
{
	int ret=1;			//becomes 0 if any connection return 0
	for (int a=0;a<MAX_PLAYERS;a++){
		Connection* c=&connections[a];
		if(c->active){
			ret&=SendData(data,length,a);
		}
	}
	return ret;
}

int CNet::SendData(unsigned char *data, int length,int connection)
{
	if(playbackDemo && !serverNet)
		return 1;

	if(length<=0)
		info->AddLine("Errenous send length in SendData %i",length);

	Connection* c=&connections[connection];
	if(c->active){
		if(c->localConnection){
			WaitForSingleObject(netMutex,INFINITE);
			Connection* lc=c->localConnection;
			if(lc->readyLength+length>=NETWORK_BUFFER_SIZE){
				info->AddLine("Overflow when sending to local connection %i %i %i %i %i",imServer,connection,lc->readyLength,length,NETWORK_BUFFER_SIZE);
				ReleaseMutex(netMutex);
				return 0;
			}
			memcpy(&lc->readyData[lc->readyLength],data,length);
			lc->readyLength+=length;
			ReleaseMutex(netMutex);
		} else {
			if(c->outgoingLength+length>=NETWORK_BUFFER_SIZE){
				info->AddLine("Overflow when sending to remote connection %i %i %i %i %i",imServer,connection,c->outgoingLength,length,NETWORK_BUFFER_SIZE);
				return 0;
			}
			memcpy(&c->outgoingData[c->outgoingLength],data,length);
			c->outgoingLength+=length;
		}
	}
	return 1;
}

int CNet::GetData(unsigned char *buf, int length,int conNum)
{
	if(connections[conNum].active){
		if(connections[conNum].localConnection)
			WaitForSingleObject(netMutex,INFINITE);

		int ret=connections[conNum].readyLength;
		if(length<=ret){
			if(connections[conNum].localConnection)
				ReleaseMutex(netMutex);
			return 0;
		}
		memcpy(buf,connections[conNum].readyData,connections[conNum].readyLength);
		connections[conNum].readyLength=0;

		if(recordDemo && ret>0)
			SaveToDemo(buf,ret);

		if(connections[conNum].localConnection)
			ReleaseMutex(netMutex);
		return ret;
	} else {
		return -1;
	}
}

void CNet::Update(void)
{
	LARGE_INTEGER t,f;
	QueryPerformanceCounter(&t);
	QueryPerformanceFrequency(&f);
	curTime=double(t.QuadPart)/double(f.QuadPart);

	if(playbackDemo)
		ReadDemoFile();
	if(onlyLocal){
		return;
	}
	sockaddr_in from;
	int fromsize=sizeof(from);
	int r;
	unsigned char inbuf[16000];
	if(connected)
	while(true){
		if((r=recvfrom(mySocket,(char*)inbuf,16000,0,(sockaddr*)&from,&fromsize))==SOCKET_ERROR){
			if(WSAGetLastError()==WSAEWOULDBLOCK || WSAGetLastError()==WSAECONNRESET) 
				break;
			char test[500];
			sprintf(test,"Error receiving data. %i %d",(int)imServer,WSAGetLastError());
			MessageBox(NULL,test,"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			exit(0);
		}
		int conn=ResolveConnection(&from);
		if(conn==-1){
			if(waitOnCon && r>=12 && (*(int*)inbuf)==0 && (*(int*)&inbuf[4])==-1 && inbuf[8]==0 && inbuf[9]==NETMSG_ATTEMPTCONNECT && inbuf[11]==NETWORK_VERSION){
				conn=InitNewConn(&from,false,inbuf[10]);
			} else {
				continue;
			}
		}
		inInitialConnect=false;
		int packetNum=(*(int*)inbuf);
		ProcessRawPacket(inbuf,r,conn);
	}

	for(int a=0;a<MAX_PLAYERS;++a){
		Connection* c=&connections[a];
		if(c->localConnection || !c->active)
			continue;
		std::map<int,Packet*>::iterator wpi;
		while((wpi=c->waitingPackets.find(c->lastInOrder+1))!=c->waitingPackets.end()){		//process all in order packets that we have waiting
			if(c->readyLength+wpi->second->length>=NETWORK_BUFFER_SIZE){
				info->AddLine("Overflow in incomming network buffer");
				break;
			}
			memcpy(&c->readyData[c->readyLength],wpi->second->data,wpi->second->length);
			c->readyLength+=wpi->second->length;
			delete wpi->second;
			c->waitingPackets.erase(wpi);
			c->lastInOrder++;
		}
		if(inInitialConnect && c->lastSendTime<curTime-1){		//server hasnt responded so try to send the connection attempt again
			SendRawPacket(a,c->unackedPackets[0]->data,c->unackedPackets[0]->length,0);
		}

		if(c->lastSendTime<curTime-5 && !inInitialConnect){		//we havent sent anything for a while so send something to prevent timeout
			tempbuf[0]=NETMSG_HELLO;
			SendData(tempbuf,1);
		}
		if(c->lastSendTime<curTime-0.2 && !c->waitingPackets.empty()){	//we have at least one missing incomming packet lying around so send a packet to ensure the other side get a nak
			tempbuf[0]=NETMSG_HELLO;
			SendData(tempbuf,1);
		}
		if(c->lastReceiveTime < curTime-(inInitialConnect ? 40 : 30)){		//other side has timed out
			c->active=false;
		}

		if(c->outgoingLength>0 && (c->lastSendTime < (curTime-0.2+c->outgoingLength*0.01) || c->lastSendFrame < gs->frameNum-1)){
			FlushConnection(a);
		}
	}
}

void CNet::ProcessRawPacket(unsigned char* data, int length, int conn)
{
	Connection* c=&connections[conn];
	c->lastReceiveTime=curTime;

	int packetNum=*(int*)data;
	int ack=*(int*)&data[4];
	int hsize=9;

	while(ack>=c->firstUnacked){
		delete c->unackedPackets.front();
		c->unackedPackets.pop_front();
		c->firstUnacked++;
	}
//	if(!imServer)
//		info->AddLine("Got packet %i %i %i %i %i %i",(int)imServer,packetNum,length,ack,c->lastInOrder,c->waitingPackets.size());
	if(data[8]==1){
		hsize=13;
		int nak=*(int*)&data[9];
//		info->AddLine("Got nak %i %i %i",nak,c->lastNak,c->firstUnacked);
		if(nak!=c->lastNak || c->lastNakTime < curTime-0.1){
			c->lastNak=nak;
			c->lastNakTime=curTime;
			for(int b=c->firstUnacked;b<=nak;++b){
				SendRawPacket(conn,c->unackedPackets[b-c->firstUnacked]->data,c->unackedPackets[b-c->firstUnacked]->length,b);
			}
		}
	}

	if(!c->active || c->lastInOrder>=packetNum || c->waitingPackets.find(packetNum)!=c->waitingPackets.end())
		return;

	Packet* p=new Packet(data+hsize,length-hsize);
	c->waitingPackets[packetNum]=p;
}

int CNet::ResolveConnection(sockaddr_in* from)
{
	unsigned int addr=from->sin_addr.S_un.S_addr;
	for(int a=0;a<MAX_PLAYERS;++a){
		if(connections[a].active && !connections[a].localConnection){
			if(connections[a].addr.sin_addr.S_un.S_addr==addr && connections[a].addr.sin_port==from->sin_port){
				return a;
			}
		}
	}
	return -1;
}

int CNet::InitNewConn(sockaddr_in* other,bool localConnect,int wantedNumber)
{
	int freeConn=0;
	if(wantedNumber){
		if(wantedNumber<0 || wantedNumber>=MAX_PLAYERS){
			info->AddLine("Warning attempt to connect to errenous connection number");
			wantedNumber=0;		
		}
		if(connections[wantedNumber].active){
			info->AddLine("Warning attempt to connect to already active connection number");
			wantedNumber=0;
		}
		freeConn=wantedNumber;
	}
	if(wantedNumber==0){
		for(freeConn=0;freeConn<MAX_PLAYERS;++freeConn){
			if(!connections[freeConn].active)
				break;
		}
	}

	gs->players[freeConn]->active=true;
	connections[freeConn].active=true;
	connections[freeConn].addr=*other;
	connections[freeConn].lastInOrder=-1;
	connections[freeConn].readyLength=0;
	connections[freeConn].waitingPackets.clear();
	connections[freeConn].firstUnacked=0;
	connections[freeConn].currentNum=0;
	connections[freeConn].outgoingLength=0;
	connections[freeConn].lastReceiveTime=curTime;
	connections[freeConn].lastSendFrame=0;
	connections[freeConn].lastSendTime=0;
	connections[freeConn].lastNak=-1;
	connections[freeConn].lastNakTime=0;

	connected=true;

	if(imServer){
		tempbuf[0]=NETMSG_SETPLAYERNUM;
		tempbuf[1]=freeConn;
		SendData(tempbuf,2,freeConn);

		tempbuf[0]=NETMSG_SCRIPT;
		tempbuf[1]=CScriptHandler::Instance()->chosenName.size()+3;
		for(unsigned int b=0;b<CScriptHandler::Instance()->chosenName.size();b++)
			tempbuf[b+2]=CScriptHandler::Instance()->chosenName.at(b);
		tempbuf[CScriptHandler::Instance()->chosenName.size()+2]=0;
		SendData(tempbuf,CScriptHandler::Instance()->chosenName.size()+3);

		tempbuf[0]=NETMSG_MAPNAME;
		tempbuf[1]=stupidGlobalMapname.size()+7;
		*(int*)&tempbuf[2]=stupidGlobalMapId;
		for(unsigned int a=0;a<stupidGlobalMapname.size();a++)
			tempbuf[a+6]=stupidGlobalMapname.at(a);
		tempbuf[stupidGlobalMapname.size()+6]=0;
		SendData(tempbuf,stupidGlobalMapname.size()+7);

		for(int a=0;a<MAX_PLAYERS;a++){
			if(!gs->players[a]->readyToStart)
				continue;
			tempbuf[0]=NETMSG_PLAYERNAME;
			tempbuf[1]=gs->players[a]->playerName.size()+4;
			tempbuf[2]=a;
			for(unsigned int b=0;b<gs->players[a]->playerName.size();b++)
				tempbuf[b+3]=gs->players[a]->playerName.at(b);
			tempbuf[gs->players[a]->playerName.size()+3]=0;
			SendData(tempbuf,gs->players[a]->playerName.size()+4);
		}
		if(gameSetup){
			for(int a=0;a<gs->activeTeams;a++){
				tempbuf[0]=NETMSG_STARTPOS;
				tempbuf[1]=a;
				tempbuf[2]=2;
				*(float*)&tempbuf[3]=gs->teams[a]->startPos.x;
				*(float*)&tempbuf[7]=gs->teams[a]->startPos.y;
				*(float*)&tempbuf[11]=gs->teams[a]->startPos.z;
				serverNet->SendData(tempbuf,15);
			}
		}
		FlushNet();
	}
	return freeConn;
}

void CNet::FlushNet(void)
{
	for (int a=0;a<MAX_PLAYERS;a++){
		Connection* c=&connections[a];
		if(c->active && c->outgoingLength>0){
			FlushConnection(a);
		}
	}
}

void CNet::FlushConnection(int conn)
{
	Connection* c=&connections[conn];
	c->lastSendFrame=gs->frameNum;
	c->lastSendTime=curTime;

	SendRawPacket(conn,c->outgoingData,c->outgoingLength,c->currentNum++);
	Packet* p=new Packet(c->outgoingData,c->outgoingLength);
	c->outgoingLength=0;
	c->unackedPackets.push_back(p);
}

void CNet::SendRawPacket(int conn, unsigned char* data, int length, int packetNum)
{
	Connection* c=&connections[conn];

	*(int*)&tempbuf[0]=packetNum;
	*(int*)&tempbuf[4]=c->lastInOrder;
	int hsize=9;
	if(!c->waitingPackets.empty() && c->waitingPackets.find(c->lastInOrder+1)==c->waitingPackets.end()){
		tempbuf[8]=1;
		*(int*)&tempbuf[9]=c->waitingPackets.begin()->first-1;
		hsize=13;
	} else {
		tempbuf[8]=0;
	}

	memcpy(&tempbuf[hsize],data,length);
//	if(rand()&7)
	if(sendto(mySocket,(char*)tempbuf,length+hsize,0,(sockaddr*)&c->addr,sizeof(c->addr))==SOCKET_ERROR){
		if(WSAGetLastError()==WSAEWOULDBLOCK)
			return;
		char test[100];
		sprintf(test,"Error sending data. %d",WSAGetLastError());
		MessageBox(NULL,test,"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		exit(0);
	}
}

void CNet::CreateDemoFile()
{
	// We want this folder to exist
	_mkdir("demos");

	if(gameSetup){
		struct tm *newtime;
		__time64_t long_time;

		_time64( &long_time );                /* Get time as long integer. */
		newtime = _localtime64( &long_time ); /* Convert to local time. */

		char buf[500];
		sprintf(buf,"%02i%02i%02i",newtime->tm_year%100,newtime->tm_mon+1,newtime->tm_mday);
		string name=string(buf)+"-"+gameSetup->mapname.substr(0,gameSetup->mapname.find_first_of("."));
/*		for(int a=0;a<gameSetup->numPlayers;++a){
			name+="-"+gs->players[a]->playerName;
		}*/
		name+=string("-")+VERSION_STRING;

		sprintf(buf,"demos\\%s.sdf",name.c_str());
		CFileHandler ifs(buf);
		if(ifs.FileExists()){
			for(int a=0;a<9999;++a){
				sprintf(buf,"demos\\%s-%i.sdf",name.c_str(),a);
				CFileHandler ifs(buf);
				if(!ifs.FileExists())
					break;
			}
		}
		demoName = buf;
		recordDemo=new ofstream(demoName.c_str(), ios::out|ios::binary);

		char c=1;
		recordDemo->write(&c,1);
		recordDemo->write((char*)&gameSetup->gameSetupTextLength,sizeof(int));
		recordDemo->write(gameSetup->gameSetupText,gameSetup->gameSetupTextLength);
	} else {
		demoName = "demos\\test.sdf";
		recordDemo=new ofstream(demoName.c_str(), ios::out|ios::binary);
		char c=0;
		recordDemo->write(&c,1);
	}
}

void CNet::SaveToDemo(unsigned char* buf,int length)
{
	recordDemo->write((char*)&gu->modGameTime,sizeof(double));
	recordDemo->write((char*)&length,sizeof(int));
	recordDemo->write((char*)buf,length);
	recordDemo->flush();
}

bool CNet::FindDemoFile(const char* name)
{
	string firstTry = name;
	firstTry = ".\\demos\\" + firstTry;
	playbackDemo=new CFileHandler(firstTry);
	if (!playbackDemo->FileExists()) {
		delete playbackDemo;
		playbackDemo = new CFileHandler(name);
	}

	if(playbackDemo->FileExists()){
		info->AddLine("Playing demo from %s",name);
		char c;
		playbackDemo->Read(&c,1);
		if(c){
			int length;
			playbackDemo->Read(&length,sizeof(int));
			char* buf=new char[length];
			playbackDemo->Read(buf,length);

			gameSetup=new CGameSetup();
			gameSetup->Init(buf,length);
			delete[] buf;
		}
		playbackDemo->Read(&demoTimeOffset,sizeof(double));
		demoTimeOffset=gu->modGameTime-demoTimeOffset;
		nextDemoRead=gu->modGameTime-0.01;
		return true;
	} else {
		delete playbackDemo;
		playbackDemo=0;
		return false;
	}
}

void CNet::ReadDemoFile(void)
{
//	if(!game)
//		return;
	if(gs->paused){
		return;
	}
	if(this==serverNet){
		while(nextDemoRead<gu->modGameTime){
			if(playbackDemo->Eof()){
				for(int a=0;a<MAX_PLAYERS;++a)
					connections[a].active=false;
				return;
			}
			int l;
			playbackDemo->Read(&l,sizeof(int));
			playbackDemo->Read(tempbuf,l);
			SendData(tempbuf,l);
			playbackDemo->Read(&nextDemoRead,sizeof(double));
			nextDemoRead+=demoTimeOffset;
			if(playbackDemo->Eof()){
				info->AddLine("End of demo");
				nextDemoRead=gu->modGameTime+4*gs->speedFactor;
			}
	//		info->AddLine("Read packet length %i ready %i time %.0f",l,connections[0].readyLength,nextDemoRead);
		}
	} else {
		while(connections[0].readyLength<500 && nextDemoRead<gu->modGameTime/**/){
			if(playbackDemo->Eof()){
				if(connections[0].readyLength<30 && (!game || game->inbuflength-game->inbufpos<30))
					connections[0].active=false;
				return;
			}
			int l;
			playbackDemo->Read(&l,sizeof(int));
			playbackDemo->Read(&connections[0].readyData[connections[0].readyLength],l);
			connections[0].readyLength+=l;
			playbackDemo->Read(&nextDemoRead,sizeof(double));
			nextDemoRead+=demoTimeOffset;
			if(playbackDemo->Eof()){
				info->AddLine("End of demo");
				nextDemoRead=gu->modGameTime+4*gs->speedFactor;
			}
	//		info->AddLine("Read packet length %i ready %i time %.0f",l,connections[0].readyLength,nextDemoRead);
		}
	}
}

void CNet::CreateDemoServer(std::string demoname)
{
	string firstTry = demoname;
	firstTry = ".\\demos\\" + firstTry;
	playbackDemo=new CFileHandler(firstTry);
	if (!playbackDemo->FileExists()) {
		delete playbackDemo;
		playbackDemo = new CFileHandler(demoname);
	}

	if(playbackDemo->FileExists()){
		char c;
		playbackDemo->Read(&c,1);
		if(c){
			int length;
			playbackDemo->Read(&length,sizeof(int));
			char* buf=new char[length];
			playbackDemo->Read(buf,length);

			//gameSetup=new CGameSetup();	//we have already initialized ...
			//gameSetup->Init(buf,length);
			delete[] buf;
			nextDemoRead=gu->modGameTime+100000000;
		}
	} else {
		info->AddLine("Couldnt find file to use as server demo");
		delete playbackDemo;
		playbackDemo=0;
	}
}

void CNet::StartDemoServer(void)
{
	playbackDemo->Read(&demoTimeOffset,sizeof(double));
	demoTimeOffset=gu->modGameTime-demoTimeOffset;
	nextDemoRead=gu->modGameTime-0.01;
}
