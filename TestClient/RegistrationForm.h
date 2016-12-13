#pragma once
#include <string>
#include <iostream>
#include "Connection.h"

#define LOGIN_OK '4'
#define LOGIN_IN_USE '5'
#define LOGIN_CHECK_PROBLEMS '6'

class RegistrationForm
{
	std::string *username = NULL;
	std::string *password = NULL;
	Connection *c = NULL;
	void checkForNewlineStarts();
	void checkForEmptiness();
	void checkForSpaceStarts();
public:
	RegistrationForm(std::string *_username, std::string * _password, Connection *c);
	~RegistrationForm();
	void ShowForm();
	char *RequestLoginCheck();
};

