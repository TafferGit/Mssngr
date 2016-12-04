#pragma once
#include <string>
#include <iostream>
#include "ClientUserDataFile.h"

class RegistrationForm
{
	std::string *username = NULL;
	std::string *password = NULL;
	ClientUserDataFileInfo accountsDataFileInfo;
	std::string loadedUsername;
public:
	RegistrationForm(std::string *_username, std::string * _password);
	~RegistrationForm();
	void ShowForm();
	int Check();
	int SaveToFile();
};

