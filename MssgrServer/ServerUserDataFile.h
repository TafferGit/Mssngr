#pragma once
#include "ServerFile.h"
#include <string>
#include <streambuf>
#include <vector>

#define LOGIN_OK "4"
#define LOGIN_IN_USE "5"
#define LOGIN_CHECK_PROBLEMS "6"
#define LOGIN_OR_PASSWORD_INCORRECT "7"

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
	char *CheckLoginResult(std::string login, std::string password);
	char *CheckLoginInMsf(std::string username);
	void LoadAllUsernames(std::vector<std::string>*userNamesVec);
	int SaveAccountCLToFile(std::string login, std::string contact);
	int LoadAccountCLFromFile(std::string *buf, std::string login);
};