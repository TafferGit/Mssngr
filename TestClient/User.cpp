#include "User.h"


User::User()
{
}


User::~User()
{
}

void User::Register(void)
{
	RegistrationForm * rf = new RegistrationForm(&login, &password);
	rf->ShowForm();
	rf->SaveToFile();
	delete rf;
}

void User::Login(void)
{
	int loginResult;
	LoginForm * lf = new LoginForm;
	lf->ShowForm();
	loginResult = lf->Check();
	if (loginResult == 0) {
		std::cout << "Login and password are correct!\n";
	}
	else if (loginResult == 1) {
		std::cout << "There were problems checking your login and password!\n";
	}
	else if (loginResult == 2) {
		std::cout << "Incorrect login and/or password!\n";
	}
}
