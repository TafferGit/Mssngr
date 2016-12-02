#pragma once
#include <string>
#include <iostream>

#include "RegistrationForm.h"

class User
{
	std::string login;
	std::string password;
public:
	User();
	~User();
	void Register(void);
};

