#pragma once
#include <string>
#include <iostream>

#include "RegistrationForm.h"
#include "LoginForm.h"

class User
{
	std::string login;
	std::string password;
public:
	User();
	~User();
	void Register(void);
	void Login(void);
};

