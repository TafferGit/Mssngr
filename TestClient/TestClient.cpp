#include "TestClient.h"

int __cdecl main(int argc, char **argv)
{
	TestClient * tc = new TestClient(argc, argv);
	return 0;
}

TestClient::TestClient(int argc, char ** argv)
{
	//c = new Connection(argc, argv);
	user = new User();
	user->Register();
}

TestClient::~TestClient()
{
	//delete c;
}
