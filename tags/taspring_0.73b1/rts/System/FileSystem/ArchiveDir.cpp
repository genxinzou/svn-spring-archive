/* Author: Tobi Vollebregt */

#include "StdAfx.h"
#include "ArchiveDir.h"
#include "Platform/FileSystem.h"

inline CFileHandler* CArchiveDir::GetFileHandler(int handle)
{
	std::map<int, CFileHandler*>::iterator it = fileHandles.find(handle);
	assert(it != fileHandles.end());
	return it->second;
}

inline std::vector<std::string>::iterator& CArchiveDir::GetSearchHandle(int handle)
{
	std::map<int, std::vector<std::string>::iterator>::iterator it = searchHandles.find(handle);
	assert(it != searchHandles.end());
	return it->second;
}

CArchiveDir::CArchiveDir(const string& archivename) :
		CArchiveBase(archivename),
		archiveName(archivename + '/'),
		curFileHandle(0),
		curSearchHandle(0)
{
	std::vector<std::string> found = filesystem.FindFiles(archiveName, "*", FileSystem::RECURSE);

	// because spring expects the contents of archives to be case independent,
	// we convert filenames to lowercase in every function, and keep a std::map
	// lcNameToOrigName to convert back from lowercase to original case.
	for (std::vector<std::string>::iterator it = found.begin(); it != found.end(); ++it) {
		// strip our own name off.. & convert to forward slashes
		std::string origName(*it, archiveName.length());
		filesystem.ForwardSlashes(origName);
		// convert to lowercase and store
		searchFiles.push_back(origName);
		lcNameToOrigName[StringToLower(origName)] = origName;
	}
}

CArchiveDir::~CArchiveDir(void)
{
}

bool CArchiveDir::IsOpen()
{
	return true;
}

int CArchiveDir::OpenFile(const std::string& fileName)
{
	CFileHandler* f = new CFileHandler(archiveName + lcNameToOrigName[StringToLower(fileName)]);

	if (!f || !f->FileExists())
		return 0;

	++curFileHandle;
	fileHandles[curFileHandle] = f;
	return curFileHandle;
}

int CArchiveDir::ReadFile(int handle, void* buffer, int numBytes)
{
	CFileHandler* f = GetFileHandler(handle);
	f->Read(buffer, numBytes);
	return 0;
}

void CArchiveDir::CloseFile(int handle)
{
	delete GetFileHandler(handle);
	fileHandles.erase(handle);
}

void CArchiveDir::Seek(int handle, int pos)
{
	GetFileHandler(handle)->Seek(pos);
}

int CArchiveDir::Peek(int handle)
{
	return GetFileHandler(handle)->Peek();
}

bool CArchiveDir::Eof(int handle)
{
	return GetFileHandler(handle)->Eof();
}

int CArchiveDir::FileSize(int handle)
{
	return GetFileHandler(handle)->FileSize();
}

int CArchiveDir::FindFiles(int cur, string* name, int* size)
{
	if (cur == 0) {
		cur = ++curSearchHandle;
		searchHandles[cur] = searchFiles.begin();
	}
	if (searchHandles[cur] == searchFiles.end()) {
		searchHandles.erase(cur);
		return 0;
	}

	std::vector<std::string>::iterator& it = GetSearchHandle(cur);

	*name = *searchHandles[cur];
	*size = filesystem.GetFilesize(archiveName + *name);

	++searchHandles[cur];
	return cur;
}
