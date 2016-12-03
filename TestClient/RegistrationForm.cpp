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
	
	if (username->at(0) == '\n' || password->at(0) == '\n') {
		std::cout << "Login and password cannot start with newline!\n";
		system("pause");
		system("cls");
		ShowForm();
	}
	else if (*username == "" || *password == "") {
		std::cout << "Login and password cannot be empty!\n";
		system("pause");
		system("cls");
		ShowForm();
	}
}

int RegistrationForm::SaveToFile()
{
	ClientFile * loginDataFile = new ClientUserDataFile();
	loginDataFile->SaveAccountDataToFile(*username, *password);
	return 0;
}
