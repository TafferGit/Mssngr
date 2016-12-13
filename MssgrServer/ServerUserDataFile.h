#pragma once
#include "ServerFile.h"
#include <string>
#include <streambuf>

#define LOGIN_OK "4"
#define LOGIN_IN_USE "5"
#define LOGIN_CHECK_PROBLEMS "6"

struct ServerUserDataFileInfo {
	std::string data;
	size_t fileSize;
};

class ServerUserDataFile :
	public ServerFile
{
private:
	int LoadAccountData(ServerUserDataFileInfo * fileInfo);
public:
	ServerUserDataFile();
	~ServerUserDataFile();
	int SaveAccountDataToFile(std::string login, std::string password);
	char *CheckLoginInMsf(std::string username);
};