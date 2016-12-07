#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include "ClientUserDataFile.h"

#define LOGIN_OK 4
#define LOGIN_IN_USE 5
#define LOGIN_CHECK_PROBLEMS 6
#define LOGIN_OR_PASSWORD_INCORRECT 7

class LoginForm
{
private:
	std::string *enteredUsername = NULL;
	std::string *enteredPassword = NULL;
	ClientUserDataFileInfo accountsDatafileInfo;
	std::string loadedUsername;
	std::string loadedPassword;
	void CheckLoginReturnedCodes(int loginResult);
public:
	LoginForm();
	~LoginForm();
	void ShowForm();
	int CheckLoginResult();
};

