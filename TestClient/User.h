#pragma once
#include <string>
#include <iostream>

#include "RegistrationForm.h"
#include "LoginForm.h"
#include "Connection.h"

class User
{
	std::string login;
	std::string password;
	Connection *c;
public:
	User(Connection *c);
	~User();
	void Register(void);
	void Login(void);
	std::string GetLogin();
};

