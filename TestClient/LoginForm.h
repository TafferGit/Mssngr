#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include "Connection.h"

#define LOGIN_OK '4'
#define LOGIN_IN_USE '5'
#define LOGIN_CHECK_PROBLEMS '6'
#define LOGIN_OR_PASSWORD_INCORRECT '7'

class LoginForm
{
private:
	std::string *enteredUsername = NULL;
	std::string *enteredPassword = NULL;
	Connection *pConnection;
	int result = 0;
	char *outBuf;
	char *recvBuf;
public:
	LoginForm(Connection *c);
	~LoginForm();
	void ShowForm();
	void CheckLoginResult();
};

