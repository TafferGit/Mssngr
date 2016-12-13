#include "User.h"


User::User(Connection *c)
{
	this->c = c;
}


User::~User()
{
}

void User::Register(void)
{
	RegistrationForm * rf = new RegistrationForm(&login, &password, c);
	rf->ShowForm();
	delete rf;
}

void User::Login(void)
{
	LoginForm * lf = new LoginForm(c);
	lf->ShowForm();
	delete lf;
}

std::string User::GetLogin()
{
	return this->login;
}
