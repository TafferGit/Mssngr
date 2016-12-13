#include "ServerUserDataFile.h"

#include <cstdio>
#include <errno.h>
#include <locale>

char * ServerUserDataFile::CheckLoginResult(std::string login, std::string password)
{
	int result; //Variable to store operations results
	std::size_t foundLogin(0), foundPassword(0), foundEndline(0);
	ServerUserDataFileInfo *pAccsDataFileInfo = new ServerUserDataFileInfo;
	std::string loadedUsername; std::string loadedPassword;

	result = LoadAccountData(pAccsDataFileInfo); //Loading account data from users.msf

	if (result == MCF_FILE_OK) {
		/*Iterate through loaded account data to find if any of usernames and passwords in users.msf match
		the entered username and password*/

		for (size_t i = 0; i < pAccsDataFileInfo->fileSize; i = foundEndline + 1) {

			foundLogin = pAccsDataFileInfo->data.find("!login!", i) + (std::size_t)sizeof("!login!") - 1;
			foundPassword = pAccsDataFileInfo->data.find("!password!", i) + (std::size_t)sizeof"!password!" - 1;
			foundEndline = pAccsDataFileInfo->data.find("\n", i);

			if (foundLogin != std::string::npos && foundPassword != std::string::npos && foundEndline != std::string::npos) {

				for (std::size_t j = foundLogin; j < foundPassword - 10; j++) {
					loadedUsername += pAccsDataFileInfo->data.at(j);
				}

				for (std::size_t j = foundPassword; j < foundEndline; j++) {
					loadedPassword += pAccsDataFileInfo->data.at(j);
				}

				//If we find a match - we return LOGIN_OK which means that username and password entered by user are correct
				if (password.compare(loadedPassword) == 0 && login.compare(loadedUsername) == 0) {
					return LOGIN_OK;
				}
				else { loadedUsername.clear(); loadedPassword.clear(); } //Clear variables and iterate further
			}

			//If we've reached file's end and hadn't found any mathches we tell that login or password or both are incorrect
			else if (foundEndline >= pAccsDataFileInfo->fileSize)
				return LOGIN_OR_PASSWORD_INCORRECT;
		}
	}

	else if (result == MCF_FILE_NOT_FOUND) {
		std::cout << "Users.mcf file was not found!\n";
		return LOGIN_CHECK_PROBLEMS;
	}

	else if (result == MCF_FILE_EMPTY) {
		std::cout << "Users.mcf file is empty!\n";
		return LOGIN_CHECK_PROBLEMS;
	}

	return LOGIN_CHECK_PROBLEMS; //Unlikely result, something must go terribly wrong...
}

char * ServerUserDataFile::CheckLoginInMsf(std::string username)
{
	size_t foundLogin(0), foundPassword(0), foundEndline(0); //Variables to save position of !login! and !password! in users.mcf
	ServerUserDataFileInfo *pAccsDataFileInfo = new ServerUserDataFileInfo; //Strict for actial data loaded from msf file
	int result; //Variable to store functions return values
	std::string loadedUsername;

	result = LoadAccountData(pAccsDataFileInfo);

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

void ServerUserDataFile::LoadAllUsernames(std::vector<std::string> *userNamesVec)
{
	int result = 0; //Variable to store msf file operations return codes
	ServerUserDataFileInfo * pSrvUsrDataFileInfo = new ServerUserDataFileInfo;
	std::string loadedUsername;
	size_t pos_start = 0;
	size_t pos_end = 0;
	size_t pos_endline = 0;

	result = LoadAccountData(pSrvUsrDataFileInfo);

	if (result == MCF_FILE_OK) {
		for (size_t i = 0; i <= pSrvUsrDataFileInfo->fileSize; i = pos_endline + 2) {
			//If we have been able to find !login! and !password! we start to fill our vector
			pos_start = pSrvUsrDataFileInfo->data.find("!login!", pos_start);
			pos_end = pSrvUsrDataFileInfo->data.find("!password!", pos_end);
			pos_endline = pSrvUsrDataFileInfo->data.find("\n", pos_endline);
			size_t loginStart = pos_start + 7;
			size_t loginSize = pos_end - loginStart;
			if (pos_start != std::string::npos && pos_end != std::string::npos && pos_endline != std::string::npos) {
				loadedUsername = pSrvUsrDataFileInfo->data.substr(loginStart, loginSize);
				userNamesVec->push_back(loadedUsername);
				++pos_start; ++pos_end; ++pos_endline;
			}
			else break;
		}
	}
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

int ServerUserDataFile::SaveAccountCLToFile(std::string login, std::string contact) {
	login.append(".hsf");
	fileStream.open(login, std::fstream::out | std::fstream::app);
	if (!fileStream.fail()) {
		fileStream << "<un>" << contact << "</un>\n";
		fileStream.close();
		return 0;
	}

	else {
		std::cerr << "Error: " << strerror(errno) << std::endl;
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
