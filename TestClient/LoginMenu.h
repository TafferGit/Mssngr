#pragma once
#include "TestClient.h"
#include "Connection.h"

class LoginMenu
{
private:
	std::string login;
	std::string password;
public:
	LoginMenu();
	~LoginMenu();
	std::string GetLogin();
	std::string GetPassword();
};

