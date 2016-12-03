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
	std::size_t foundLogin(0), foundPassword(0), foundEndline(0);
	ClientUserDataFile * clientFile = new ClientUserDataFile();

	accountsDatafileInfo = clientFile->LoadAccountData();

	/*Iterate through loaded account data to find if any of usernames and passwords in users.mcf match
	the entered username and password*/

	for (int i = 0; i < accountsDatafileInfo.fileSize; i = foundEndline + 1) {

		foundLogin = accountsDatafileInfo.data.find("!login!", i) + (std::size_t)sizeof("!login!") - 1;
		foundPassword = accountsDatafileInfo.data.find("!password!", i) + (std::size_t)sizeof"!password!" - 1;
		foundEndline = accountsDatafileInfo.data.find("\n", i);

		if (foundLogin != std::string::npos && foundPassword != std::string::npos && foundEndline != std::string::npos) {

			for (std::size_t i = foundLogin; i < foundPassword - 10; i++) {
				loadedUsername += accountsDatafileInfo.data.at(i);
			}

			for (std::size_t i = foundPassword; i < foundEndline; i++) {
				loadedPassword += accountsDatafileInfo.data.at(i);
			}

			//If we find a match - we return 0 which means that username and password entered by user are correct
			if (enteredPassword->compare(loadedPassword) == 0 && enteredUsername->compare(loadedUsername) == 0) {
				return 0;
			}
			else { loadedUsername.clear(); loadedPassword.clear(); }
		}

		else if (foundEndline >= accountsDatafileInfo.fileSize)
				return 2;
	}

	//Haven't found any matches. Returning 2.
	return 1;
}
