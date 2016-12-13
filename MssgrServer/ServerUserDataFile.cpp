#include "ServerUserDataFile.h"

#include <cstdio>
#include <errno.h>
#include <locale>

char * ServerUserDataFile::CheckLoginInMsf(std::string username)
{
	size_t foundLogin(0), foundPassword(0), foundEndline(0); //Variables to save position of !login! and !password! in users.mcf
	ServerUserDataFile *pServerFile = new ServerUserDataFile(); //Create new UserDataFile class to open users.msf for reading
	ServerUserDataFileInfo *pAccsDataFileInfo = new ServerUserDataFileInfo; //Strict for actial data loaded from msf file
	int result; //Variable to store functions return values
	std::string loadedUsername;

	result = pServerFile->LoadAccountData(pAccsDataFileInfo);

	if (result == MCF_FILE_OK) {
		if (pAccsDataFileInfo->data != "") {
			//Iterate through loaded .mcf file to find if entered login is in user
			for (size_t i = 0; i < pAccsDataFileInfo->fileSize; i = foundEndline + 1) {

				foundLogin = pAccsDataFileInfo->data.find("!login!", i) + (size_t)sizeof("!login!") - 1; //Position where actual login starts
				foundPassword = pAccsDataFileInfo->data.find("!password!", i) + (size_t)sizeof("!password!") - 1; //Position where actual password starts
				foundEndline = pAccsDataFileInfo->data.find('\n', i);

				if (foundLogin != std::string::npos && foundPassword != std::string::npos && foundEndline != std::string::npos) {

					for (size_t j = foundLogin; j < foundPassword - (size_t)sizeof("!password!") + 1; j++) {
						loadedUsername += pAccsDataFileInfo->data.at(j);
					}

					//If we find a match, we return 1 which means that login is already in use
					if (loadedUsername.compare(username) == 0) {
						loadedUsername.clear();
						return LOGIN_IN_USE;
					}

					//Else we iterate further to the end of the file
					else
						loadedUsername.clear();

				}

				//Else if we reached the end of the file and haven't found any matches we return 0 which means login is free to use
				else if (foundEndline >= pAccsDataFileInfo->fileSize) {
					loadedUsername.clear();
					return LOGIN_OK;
				}
			}

			//Something went terribly wrong if we got here...
			return LOGIN_CHECK_PROBLEMS;
		}
	}

	else if (result == MCF_FILE_EMPTY) {
		std::cout << "Users.msf file is empty. Looks like program is running for the first time.\n";
		return LOGIN_OK;
	}

	else if (result == MCF_FILE_NOT_FOUND) {
		std::cout << "Users.msf file not found. Looks like the program is running for the first time.\n";
		return LOGIN_OK;
	}

	else if (result == MCF_FILE_INVALID_FORMAT) {
		std::cout << "Users.msf file is corrupted!\n";
		system("pause");
		exit(MCF_FILE_INVALID_FORMAT);
	}

	else return LOGIN_CHECK_PROBLEMS; //Unlikely to happen but...
}

ServerUserDataFile::ServerUserDataFile()
{
}


ServerUserDataFile::~ServerUserDataFile()
{
}

int ServerUserDataFile::SaveAccountDataToFile(std::string login, std::string password)
{
	fileStream.open("users.msf", std::fstream::out | std::fstream::app);
	if (!fileStream.fail()) {
		fileStream << "!login!" << login << "!password!" << password << "\n";
		fileStream.close();
		return 0;
	}

	else {
		std::cerr << "Error: " << strerror(errno);
		return 1;
	}
}

int ServerUserDataFile::LoadAccountData(ServerUserDataFileInfo * fileInfo)
{
	ServerUserDataFileInfo currentFileInfo;
	fileStream.open("users.msf", std::ios::in);
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
