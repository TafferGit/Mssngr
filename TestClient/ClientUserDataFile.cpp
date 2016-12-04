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

int ClientUserDataFile::LoadAccountData(ClientUserDataFileInfo * fileInfo)
{
	ClientUserDataFileInfo currentFileInfo;
	fileStream.open("users.mcf", std::ios::in);
	if (fileStream.fail()) {
		std::cerr << "Error: " << strerror(errno) << "\n";
		return MCF_FILE_NOT_FOUND ;
	}

	else {
		fileStream.seekg(0, std::ios::end);
		currentFileInfo.data.reserve(fileStream.tellg());
		currentFileInfo.fileSize = fileStream.tellg();
		fileStream.seekg(0, std::ios::beg);

		currentFileInfo.data.assign((std::istreambuf_iterator<char>(fileStream)),
			std::istreambuf_iterator<char>());
		fileStream.close();

		*fileInfo = currentFileInfo;

		if (currentFileInfo.data != "") {
			if (currentFileInfo.data.at(0) != '!') { return MCF_FILE_INVALID_FORMAT; }
			return MCF_FILE_OK;
		}
		else return MCF_FILE_EMPTY;
	}
}
