//#include "LoginForm.h"
//
//
//LoginForm::LoginForm()
//{
//	enteredUsername = new std::string();
//	enteredPassword = new std::string();
//}
//
//
//LoginForm::~LoginForm()
//{
//	enteredUsername = NULL;
//	enteredUsername = NULL;
//}
//
//void LoginForm::ShowForm()
//{
//	int loginResult; //Variable to store login result return value
//
//	system("cls");
//	std::cout << "Complete the login procedure.\n";
//	std::cout << "Please enter your login: ";
//	std::cin >> *enteredUsername;
//	std::cout << "Please enter your password: ";
//	std::cin >> *enteredPassword;
//
//	loginResult = CheckLoginResult();
//	CheckLoginReturnedCodes(loginResult);
//}

//int LoginForm::CheckLoginResult()
//{
//	int result; //Variable to store operations results
//	std::size_t foundLogin(0), foundPassword(0), foundEndline(0);
//	ClientUserDataFile *clientFile = new ClientUserDataFile();
//
//	result = clientFile->LoadAccountData(&accountsDatafileInfo); //Loading account data from users.mcf
//
//	if (result == MCF_FILE_OK) {
//		/*Iterate through loaded account data to find if any of usernames and passwords in users.mcf match
//		the entered username and password*/
//
//		for (size_t i = 0; i < accountsDatafileInfo.fileSize; i = foundEndline + 1) {
//
//			foundLogin = accountsDatafileInfo.data.find("!login!", i) + (std::size_t)sizeof("!login!") - 1;
//			foundPassword = accountsDatafileInfo.data.find("!password!", i) + (std::size_t)sizeof"!password!" - 1;
//			foundEndline = accountsDatafileInfo.data.find("\n", i);
//
//			if (foundLogin != std::string::npos && foundPassword != std::string::npos && foundEndline != std::string::npos) {
//
//				for (std::size_t j = foundLogin; j < foundPassword - 10; j++) {
//					loadedUsername += accountsDatafileInfo.data.at(j);
//				}
//
//				for (std::size_t j = foundPassword; j < foundEndline; j++) {
//					loadedPassword += accountsDatafileInfo.data.at(j);
//				}
//
//				//If we find a match - we return LOGIN_OK which means that username and password entered by user are correct
//				if (enteredPassword->compare(loadedPassword) == 0 && enteredUsername->compare(loadedUsername) == 0) {
//					return LOGIN_OK;
//				}
//				else { loadedUsername.clear(); loadedPassword.clear(); } //Clear variables and iterate further
//			}
//
//			//If we've reached file's end and hadn't found any mathches we tell that login or password or both are incorrect
//			else if (foundEndline >= accountsDatafileInfo.fileSize)
//				return LOGIN_OR_PASSWORD_INCORRECT;
//		}
//	}
//
//	else if (result == MCF_FILE_NOT_FOUND) {
//		std::cout << "Users.mcf file was not found!\n";
//		return LOGIN_CHECK_PROBLEMS;
//	}
//
//	else if (result == MCF_FILE_EMPTY) {
//		std::cout << "Users.mcf file is empty!\n";
//		return LOGIN_CHECK_PROBLEMS;
//	}
//
//	return 4; //Unlikely result, something must go terribly wrong...
//}
//
//void LoginForm::CheckLoginReturnedCodes(int loginResult) {
//
//	if (loginResult == LOGIN_OK) {
//		std::cout << "Login and password are correct!\n";
//		/*Go to the next part. Presumably, requesting contact list from the server*/
//	}
//	else if (loginResult == LOGIN_OR_PASSWORD_INCORRECT) {
//		std::cout << "Incorrect login and/or password!\n";
//		ShowForm(); //Just showing the form again for user to reenter login and password
//	}
//	else if (loginResult == LOGIN_CHECK_PROBLEMS) {
//		std::cout << "Some problems occured when checking login and password!\n"; //That's enough for client-side
//	}
//}