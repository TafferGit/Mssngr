#include "MssgrServerMain.h"

int __cdecl main()
{
	MssgrServer * msgSrv = new MssgrServer;
	return 0;
}

MssgrServer::MssgrServer()
{
	sock = new ServerSocket;
}

MssgrServer::~MssgrServer()
{
	delete sock;
}
