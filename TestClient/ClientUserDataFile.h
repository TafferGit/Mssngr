#pragma once
#include "ClientFile.h"
#include <string>

class ClientUserDataFile :
	public ClientFile
{
public:
	ClientUserDataFile();
	~ClientUserDataFile();
	int SaveAccountDataToFile(std::string login, std::string password);
};

