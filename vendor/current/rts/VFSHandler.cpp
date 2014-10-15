#include "stdafx.h"
#include "VFSHandler.h"
#include <io.h>
#include "ArchiveFactory.h"
#include <algorithm>
#include "mygl.h"
//#include "mmgr.h"

CVFSHandler* hpiHandler=0;

CVFSHandler::CVFSHandler(bool mapArchives)
{
	if (!mapArchives)
		return;

	string taDir;
	taDir=".\\";
	char t[500];
	sprintf(t,"Creating virtual file system from %s",taDir.c_str());
	//PrintLoadMsg(t);

	FindArchives(taDir + "*.gp4", taDir);
	FindArchives(taDir + "*.swx", taDir);
	FindArchives(taDir + "*.gp3", taDir);
	FindArchives(taDir + "*.ccx", taDir);
	FindArchives(taDir + "*.ufo", taDir);
	FindArchives(taDir + "*.hpi", taDir);
	FindArchives(taDir + "*.sdz", taDir);
	FindArchives(taDir + "*.sd7", taDir);
}

void CVFSHandler::FindArchives(const string& pattern, const string& path)
{
	struct _finddata_t files;    
	long hFile;
	int morefiles=0;

	if( (hFile = _findfirst( pattern.c_str(), &files )) == -1L ){
		morefiles=-1;
	}

	while(morefiles==0){
		AddArchive(path + files.name, false);		
		morefiles=_findnext( hFile, &files ); 
	}
}

// Override determines whether if conflicts overwrites an existing entry in the virtual filesystem or not
void CVFSHandler::AddArchive(string arName, bool override)
{
	transform(arName.begin(), arName.end(), arName.begin(), (int (*)(int))tolower);

	CArchiveBase* ar = archives[arName];
	if (!ar) {
		ar = CArchiveFactory::OpenArchive(arName);
		if (!ar)
			return;
		archives[arName] = ar;
	}

	int cur;
	string name;
	int size;

	for (cur = ar->FindFiles(0, &name, &size); cur != 0; cur = ar->FindFiles(cur, &name, &size)) {
		transform(name.begin(), name.end(), name.begin(), (int (*)(int))tolower);
		if ((!override) && (files.find(name) != files.end())) 
			continue;

		FileData d;
		d.ar = ar;
		d.size = size;
		files[name] = d;
	}
}

CVFSHandler::~CVFSHandler(void)
{
	for (map<string, CArchiveBase*>::iterator i = archives.begin(); i != archives.end(); ++i) {
		delete i->second;
	}
}

void CVFSHandler::MakeLower(string &s)
{
	for(unsigned int a=0;a<s.size();++a){
		if(s[a]>='A' && s[a]<='Z')
			s[a]=s[a]+'a'-'A';
	}
}

void CVFSHandler::MakeLower(char *s)
{
	for(int a=0;s[a]!=0;++a)
		if(s[a]>='A' && s[a]<='Z')
			s[a]=s[a]+'a'-'A';
}

int CVFSHandler::LoadFile(string name, void* buffer)
{
	transform(name.begin(), name.end(), name.begin(), (int (*)(int))tolower);
	FileData fd = files[name];

	int fh = fd.ar->OpenFile(name);
	if (!fh)
		return -1;
	fd.ar->ReadFile(fh, buffer, fd.size);
	fd.ar->CloseFile(fh);

	return fd.size;
}

int CVFSHandler::GetFileSize(string name)
{
	transform(name.begin(), name.end(), name.begin(), (int (*)(int))tolower);
	map<string, FileData>::iterator f = files.find(name);

	if (f == files.end())
		return -1;
	else
		return f->second.size;
}

// Returns all the files in the given (virtual) directory without the preceeding pathname
vector<string> CVFSHandler::GetFilesInDir(string dir)
{
	vector<string> ret;
	transform(dir.begin(), dir.end(), dir.begin(), (int (*)(int))tolower);
	
	// Non-empty directories to look in should have a trailing backslash
	if (dir.length() > 0) {
		if (dir[dir.length() - 1] != '\\')
			dir += "\\";
	}

	// Possible optimization: find the start iterator by inserting the dir and see where it ends up
	bool foundMatch = false;
	for (map<string, FileData>::iterator i = files.begin(); i != files.end(); ++i) {
		if (equal(dir.begin(), dir.end(), i->first.begin())) {

			// Strip pathname
			string name = i->first.substr(dir.length());

			// Do not return files in subfolders
			if (name.find('\\') == string::npos)
				ret.push_back(name);

			foundMatch = true;
		}
		else {
			// If we have had a match previously but this one isn't a match, there will be no more matches
			if (foundMatch)
				break;
		}
	}

	return ret;
}