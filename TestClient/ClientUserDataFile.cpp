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

std::string ClientUserDataFile::LoadAccountData()
{
	std::string data;
	fileStream.open("users.mcf", std::ios::in);
	fileStream.seekg(0, std::ios::end);
	data.reserve(fileStream.tellg());
	fileStream.seekg(0, std::ios::beg);

	data.assign((std::istreambuf_iterator<char>(fileStream)),
		std::istreambuf_iterator<char>());

	return data;
}
