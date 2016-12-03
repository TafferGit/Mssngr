#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include "ClientUserDataFile.h"

class LoginForm
{
private:
	std::string *enteredUsername = NULL;
	std::string *enteredPassword = NULL;
	std::string accountsData;
	std::string loadedUsername;
	std::string loadedPassword;
public:
	LoginForm();
	~LoginForm();
	void ShowForm();
	int Check();
};
