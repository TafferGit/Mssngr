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
	delete rf;
}

void User::Login(void)
{
	LoginForm * lf = new LoginForm;
	lf->ShowForm();
	delete lf;
}
