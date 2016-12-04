#pragma once
#include <string>
#include <iostream>
#include "ClientUserDataFile.h"

#define LOGIN_OK 4
#define LOGIN_IN_USE 5
#define LOGIN_CHECK_PROBLEMS 6

class RegistrationForm
{
	std::string *username = NULL;
	std::string *password = NULL;
	ClientUserDataFileInfo accountsDataFileInfo;
	std::string loadedUsername;

	void checkForNewlineStarts();
	void checkForEmptiness();
	void checkForSpaceStarts();
public:
	RegistrationForm(std::string *_username, std::string * _password);
	~RegistrationForm();
	void ShowForm();
	int CheckMCFFile();
	int SaveToFile();
};

