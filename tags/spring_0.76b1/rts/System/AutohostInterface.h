#ifndef AUTOHOSTINTERFACE 
#define AUTOHOSTINTERFACE

#include <string>

namespace netcode {
class UDPHardlink;
}

/**
@brief Class to communicate with an autohost (or similar) using UDP over loopback
@author Karl-Robert Ernst
*/
class AutohostInterface
{
public:
	typedef unsigned char uchar;
	
	/**
	@brief Connects to a port on localhost
	@param localport port to use by this class
	@param remoteport the port of the autohost
	*/
	AutohostInterface(int localport, int remoteport);
	~AutohostInterface();
	
	void SendStart() const;
	void SendQuit() const;
	void SendStartPlaying() const;
	void SendGameOver() const;
	
	void SendPlayerJoined(uchar playerNum, const std::string& name) const;
	void SendPlayerLeft(uchar playerNum, uchar reason) const;
	void SendPlayerReady(uchar playerNum, uchar readyState) const;
	void SendPlayerChat(uchar playerNum, const std::string& msg) const;
	void SendPlayerDefeated(uchar playerNum) const;
	
	/**
	@brief Recieve a chat message from the autohost
	There should be only 1 message per UDP-Packet, and it will use the hosts playernumber to inject this message
	*/
	std::string GetChatMessage() const;
	
private:
	netcode::UDPHardlink* autohost;
};

#endif
