#pragma once
#include <iostream>
#include <fstream>

class ClientFile
{
protected:
	std::fstream fileStream;
public:
	ClientFile();
	~ClientFile();
	virtual int SaveAccountDataToFile(std::string login, std::string password);
};

