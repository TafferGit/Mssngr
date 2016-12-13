#include "TestClient.h"

int __cdecl main(int argc, char **argv)
{
	SetConsoleCP(866);
	TestClient * tc = new TestClient(argc, argv);
	system("pause");
	return 0;
}

TestClient::TestClient(int argc, char ** argv)
{
	c = new Connection(argc, argv);

	user = new User(c);
	user->Register();
	//user->Login();
}

TestClient::~TestClient()
{
	delete c;
}
