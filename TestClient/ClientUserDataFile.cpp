#include "ClientUserDataFile.h"

ClientUserDataFile::ClientUserDataFile()
{
}


ClientUserDataFile::~ClientUserDataFile()
{
}

int ClientUserDataFile::SaveAccountDataToFile(std::string login, std::string password)
{
	fileStream.open("users.mcf", std::fstream::out);
	if (!fileStream.fail()) {
		fileStream << "!login!" << login << "!password!" << password << "\n";
		fileStream.close();
	}

	else {
		std::cerr << "Error: " << strerror(errno);
		return 1;
	}
}
