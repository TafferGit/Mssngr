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
