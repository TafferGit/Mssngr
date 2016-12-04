#pragma once
#include "ClientFile.h"
#include <string>
#include <streambuf>

struct ClientUserDataFileInfo {
	std::string data;
	size_t fileSize;
};

class ClientUserDataFile :
	public ClientFile
{
public:
	ClientUserDataFile();
	~ClientUserDataFile();
	int SaveAccountDataToFile(std::string login, std::string password);
	int LoadAccountData(ClientUserDataFileInfo * fileInfo);
};