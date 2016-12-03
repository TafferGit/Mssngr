#include "LoginForm.h"



LoginForm::LoginForm()
{
	enteredUsername = new std::string();
	enteredPassword = new std::string();
}


LoginForm::~LoginForm()
{
	enteredUsername = NULL;
	enteredUsername = NULL;
}

void LoginForm::ShowForm()
{
	std::cout << "Please enter your login: ";
	std::cin >> *enteredUsername;
	std::cout << "Please enter your password: ";
	std::cin >> *enteredPassword;
}

int LoginForm::Check()
{
	std::size_t foundLogin(NULL), foundPassword(NULL), foundEndline(NULL);
	ClientUserDataFile * clientFile = new ClientUserDataFile();

	accountsData = clientFile->LoadAccountData();
	foundLogin = accountsData.find("!login!") + (std::size_t)sizeof("!login!") - 1;
	foundPassword = accountsData.find("!password!") + (std::size_t)sizeof"!password!" - 1;
	foundEndline = accountsData.find("\n");

	if (foundLogin != std::string::npos) {
		for (std::size_t i = foundLogin; i < foundPassword - 10; i++) {
			loadedUsername += accountsData.at(i);
		}
	}
	else return 1;

	if (foundPassword != std::string::npos && foundEndline != std::string::npos) {
		for (std::size_t i = foundPassword; i < foundEndline; i++) {
			loadedPassword += accountsData.at(i);
		}
	}
	else return 1;

	if (enteredPassword->compare(loadedPassword) == 0 && enteredUsername->compare(loadedUsername) == 0) {
		return 0;
	}
	else return 2;
}
