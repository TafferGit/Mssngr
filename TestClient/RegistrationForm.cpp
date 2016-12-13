#include "TestClient.h"
#include "RegistrationForm.h"

void RegistrationForm::checkForNewlineStarts()
{
	if (username->at(0) == '\n' || password->at(0) == '\n') {
		std::cout << "Login and password cannot start with newline!\n";
		system("pause");
		system("cls");
		ShowForm();
	}

	else return;
}

void RegistrationForm::checkForEmptiness()
{
	if (*username == "" || *password == "") {
		std::cout << "Login and password cannot be empty!\n";
		system("pause");
		system("cls");
		ShowForm();
	}

	else return;
}

void RegistrationForm::checkForSpaceStarts()
{
	if (username->at(0) == ' ' || password->at(0) == ' ') {
		std::cout << "Login and password cannot start with a space!\n";
		system("pause");
		system("cls");
		ShowForm();
	}

	else return;
}

RegistrationForm::RegistrationForm(std::string * _username, std::string *_password, Connection *c)
{
	username = _username;
	password = _password;
	this->c = c;
}


RegistrationForm::~RegistrationForm()
{
	username = NULL;
	password = NULL;
}

void RegistrationForm::ShowForm()
{
	int result = 0;
	char *outBuf = new char[DEFAULT_BUFLEN];
	char *recvBuf = new char[DEFAULT_BUFLEN];

	std::cout << "Welcome to Messenger 0.9.8!\nPlease register an account\n";
	std::cout << "Please enter your login: ";
	std::cin >> *username;
	std::cout << "Please enter your password: ";
	std::cin >> *password;

	sprintf(outBuf, "<reg>\n<log>%s</log>\n<pas>%s</pas>", username->c_str(), password->c_str());
	c->SendBuffer(outBuf);
	
	do {
		result = c->WaitForReceive(recvBuf);
	} while (result <= 0);
	
	if (recvBuf[0] == LOGIN_OK) {
		std::cout << "The login you entered is not in use on the server!\n";
		system("pause");
	}

	else if (recvBuf[0] == LOGIN_IN_USE) {
		std::cout << "We are sorry, but the login you entered is in use. Please, provide antoher login.\n";
		system("pause");
		system("cls");
		ShowForm();
	}

	else if (recvBuf[0] == LOGIN_CHECK_PROBLEMS) {
		std::cout << "We are sorry, but something went terribly wrong. Please contact support: tafferredmine@gmail.com\n";
		system("pause");
		system("cls");
		exit(2);
	}
}

char * RegistrationForm::RequestLoginCheck()
{
	return nullptr;
}