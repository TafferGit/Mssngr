#include "ClientUserDataFile.h"
#include <cstdio>
#include <errno.h>
#include <locale>

ClientUserDataFile::ClientUserDataFile()
{
}


ClientUserDataFile::~ClientUserDataFile()
{
}

int ClientUserDataFile::SaveAccountDataToFile(std::string login, std::string password)
{
	fileStream.open("users.mcf", std::fstream::out | std::fstream::app);
	if (!fileStream.fail()) {
		fileStream << "!login!" << login << "!password!" << password << "\n";
		fileStream.close();
	}

	else {
		std::cerr << "Error: " << strerror(errno);
		return 1;
	}
}

ClientUserDataFileInfo ClientUserDataFile::LoadAccountData()
{
	ClientUserDataFileInfo currentFileInfo;
	fileStream.open("users.mcf", std::ios::in);
	fileStream.seekg(0, std::ios::end);
 	currentFileInfo.data.reserve(fileStream.tellg());
	currentFileInfo.fileSize = fileStream.tellg();
	fileStream.seekg(0, std::ios::beg);

	currentFileInfo.data.assign((std::istreambuf_iterator<char>(fileStream)),
		std::istreambuf_iterator<char>());

	return currentFileInfo;
}
