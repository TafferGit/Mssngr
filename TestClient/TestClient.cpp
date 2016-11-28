#include "Connection.h"

int __cdecl main(int argc, char **argv)
{
	Connection * connection = new Connection(argc, argv);
	delete connection;
	return 0;
}
