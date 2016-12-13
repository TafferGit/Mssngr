#include "LoginForm.h"


LoginForm::LoginForm(Connection *c)
{
	enteredUsername = new std::string();
	enteredPassword = new std::string();
	pConnection = c;
}


LoginForm::~LoginForm()
{
	enteredUsername = NULL;
	enteredUsername = NULL;
	pConnection = NULL;
}

void LoginForm::ShowForm()
{
	system("cls");
	std::cout << "Complete the login procedure.\n";
	std::cout << "Please enter your login: ";
	std::cin >> *enteredUsername;
	std::cout << "Please enter your password: ";
	std::cin >> *enteredPassword;
	
	CheckLoginResult();
}

void LoginForm::CheckLoginResult()
{
	outBuf = new char[DEFAULT_BUFLEN];
	recvBuf = new char[DEFAULT_BUFLEN];
	sprintf(outBuf, "<reqlog>\n<log>%s</log>\n<pas>%s</pas>", enteredUsername->c_str(), enteredPassword->c_str());
	pConnection->SendBuffer(outBuf);

	do {
		result = pConnection->WaitForReceive(recvBuf);
	} while (result <= 0);

	if (recvBuf[0] == LOGIN_OK) {
		std::cout << "The login and password you entered are correct!\n";
		system("pause");
	}

	else if (recvBuf[0] == LOGIN_OR_PASSWORD_INCORRECT) {
		std::cout << "We are sorry, but the login or password you've entered are incorrect. Please, try again!";
		ShowForm();
	}

	else if (recvBuf[0] == LOGIN_CHECK_PROBLEMS) {
		std::cout << "We are sorry, but something went terribly wrong. Please contact support: tafferredmine@gmail.com\n";
		system("pause");
		system("cls");
		exit(2);
	}
}
