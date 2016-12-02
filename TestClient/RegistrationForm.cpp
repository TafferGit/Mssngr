#include "RegistrationForm.h"



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
	std::cout << "Please enter your login: ";
	std::cin >> *username;
	std::cout << "Please enter your password: ";
	std::cin >> *password;
}

int RegistrationForm::SaveToFile()
{
	ClientFile * loginDataFile = new ClientUserDataFile();
	loginDataFile->SaveAccountDataToFile(*username, *password);
	return 0;
}
