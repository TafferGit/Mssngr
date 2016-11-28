#include "TestClient.h"


LoginMenu::LoginMenu()
{
	std::cout << "Please enter your login: ";
	std::cin >> login;
	std::cout << "\nPlease enter your password: ";
	std::cin >> password;
}


LoginMenu::~LoginMenu()
{
}

std::string LoginMenu::GetLogin()
{
	return login;
}

std::string LoginMenu::GetPassword()
{
	return password;
}
