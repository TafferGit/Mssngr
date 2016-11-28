#include "TestClient.h"

int __cdecl main(int argc, char **argv)
{
	TestClient * tc = new TestClient(argc, argv);
	return 0;
}

TestClient::TestClient(int argc, char ** argv)
{
	lm = new LoginMenu();
	c = new Connection(argc, argv);
}

TestClient::~TestClient()
{
	delete lm;
	delete c;
}
