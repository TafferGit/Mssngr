#pragma once
#include <iostream>
#include <fstream>

#define MCF_FILE_OK 0
#define MCF_FILE_NOT_FOUND 1
#define MCF_FILE_EMPTY 2
#define MCF_FILE_INVALID_FORMAT 3

class ClientFile
{
protected:
	std::fstream fileStream;
public:
	ClientFile();
	~ClientFile();
	virtual int SaveAccountDataToFile(std::string login, std::string password);
};

