#include "TestClient.h"

int __cdecl main(int argc, char **argv)
{
	TestClient * tc = new TestClient(argc, argv);
	system("pause");
	return 0;
}

TestClient::TestClient(int argc, char ** argv)
{
	int usrChoice = 0;
	printf("I wanna REGISTER [R]! I wanna LOGIN [L]!\n");
	usrChoice = _getch();

	if (usrChoice == 'r' || usrChoice == 'R') {
		c = new Connection(argc, argv);
		user = new User(c);
		user->Register();
		user->Login();
	}
	else if (usrChoice == 'l' || usrChoice == 'L') {
		c = new Connection(argc, argv);
		user = new User(c);
		user->Login();
	}
	else {
		printf("Wrond choice bro!\n");
		system("pause");
		system("cls");
		TestClient(argc, argv);
	}
	pCL = new ContactList(c);
	delete pCL;
}

TestClient::~TestClient()
{
	delete c;
}
