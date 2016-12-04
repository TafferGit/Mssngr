#include "RegistrationForm.h"

void RegistrationForm::checkForNewlineStarts()
{
	if (username->at(0) == '\n' || password->at(0) == '\n') {
		std::cout << "Login and password cannot start with newline!\n";
		system("pause");
		system("cls");
		ShowForm();
	}

	else return;
}

void RegistrationForm::checkForEmptiness()
{
	if (*username == "" || *password == "") {
		std::cout << "Login and password cannot be empty!\n";
		system("pause");
		system("cls");
		ShowForm();
	}

	else return;
}

void RegistrationForm::checkForSpaceStarts()
{
	if (username->at(0) == ' ' || password->at(0) == ' ') {
		std::cout << "Login and password cannot start with a space!\n";
		system("pause");
		system("cls");
		ShowForm();
	}

	else return;
}

RegistrationForm::RegistrationForm(std::string * _username, std::string *_password)
{
	username = _username;
	password = _password;
}


RegistrationForm::~RegistrationForm()
{
	username = NULL;
	password = NULL;
}

void RegistrationForm::ShowForm()
{
	int checkResult = 0;

	std::cout << "Welcome to Messenger 0.9.8!\nPlease register an account\n";
	std::cout << "Please enter your login: ";
	std::cin >> *username;
	std::cout << "Please enter your password: ";
	std::cin >> *password;

	checkResult = CheckMCFFile();

	if (checkResult == LOGIN_OK){
		checkForEmptiness();
		checkForNewlineStarts();
		checkForSpaceStarts();
		SaveToFile();
	}

	else if (checkResult == LOGIN_IN_USE) {
		std::cout << "We are sorry, but the login you entered is in use. Please, provide antoher login.\n";
		system("pause");
		system("cls");
		ShowForm();
	}

	else if (checkResult == LOGIN_CHECK_PROBLEMS) {
		std::cout << "We are sorry, but something went terribly wrong. Please contact support: tafferredmine@gmail.com\n";
		system("pause");
		system("cls");
		exit(2);
	}
}

int RegistrationForm::CheckMCFFile()
{
	size_t foundLogin(0), foundPassword(0), foundEndline(0); //Variables to save position of !login! and !password! in users.mcf
	ClientUserDataFile *clientFile = new ClientUserDataFile(); //Create new UserDataFile class to open users.mcf for reading
	int result; //Variable to store functions return values

	result = clientFile->LoadAccountData(&accountsDataFileInfo);
	
	if (result == MCF_FILE_OK) {
		if (accountsDataFileInfo.data != "") {
			//Iterate through loaded .mcf file to find if entered login is in user
			for (size_t i = 0; i < accountsDataFileInfo.fileSize; i = foundEndline + 1) {

				foundLogin = accountsDataFileInfo.data.find("!login!", i) + (size_t)sizeof("!login!") - 1; //Position where actual login starts
				foundPassword = accountsDataFileInfo.data.find("!password!", i) + (size_t)sizeof("!password!") - 1; //Position where actual password starts
				foundEndline = accountsDataFileInfo.data.find('\n', i);

				if (foundLogin != std::string::npos && foundPassword != std::string::npos && foundEndline != std::string::npos) {

					for (size_t j = foundLogin; j < foundPassword - (size_t)sizeof("!password!") + 1; j++) {
						loadedUsername += accountsDataFileInfo.data.at(j);
					}

					//If we find a match, we return 1 which means that login is already in use
					if (loadedUsername.compare(*username) == 0) {
						loadedUsername.clear();
						return LOGIN_IN_USE;
					}

					//Else we iterate further to the end of the file
					else
						loadedUsername.clear();

				}

				//Else if we reached the end of the file and haven't found any matches we return 0 which means login is free to use
				else if (foundEndline >= accountsDataFileInfo.fileSize) {
					loadedUsername.clear();
					return LOGIN_OK;
				}
			}

			//Something went terribly wrong if we got here...
			return LOGIN_CHECK_PROBLEMS;
		}


	}

	else if (result == MCF_FILE_EMPTY) {
		std::cout << "Users.mcf file is empty. Looks like program is running for the first time.\n";
		return LOGIN_OK;
	}

	else if (result == MCF_FILE_NOT_FOUND) {
		std::cout << "Users.mcf file not found. Looks like the program is running for the first time.\n";
		return LOGIN_OK;
	}

	else if (result == MCF_FILE_INVALID_FORMAT) {
		std::cout << "Users.mcf file is corrupted!\n";
		system("pause");
		exit(MCF_FILE_INVALID_FORMAT);
	}

	else return 4; //Unlikely to happen but...
}

int RegistrationForm::SaveToFile()
{
	ClientFile * loginDataFile = new ClientUserDataFile();
	loginDataFile->SaveAccountDataToFile(*username, *password);
	return 0;
}
