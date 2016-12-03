#pragma once
#include "ClientFile.h"
#include <string>
#include <streambuf>

class ClientUserDataFile :
	public ClientFile
{
public:
	ClientUserDataFile();
	~ClientUserDataFile();
	int SaveAccountDataToFile(std::string login, std::string password);
	std::string LoadAccountData();
};

