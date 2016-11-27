#pragma once
#include "TestClient.h"

class Connection
{
private:
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	char *sendbuf;
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	int ResolveAddressAndPort(char **argv);
	int ConnectToAddress();
	int SendBuffer();
	int ShutDownConnection();
	void WaitForReceive();
	void ConnectionCleanUp();
public:
	Connection(int argc, char **argv);
	~Connection();
	int InitializeWinSockConnection(int argc, char **argv);
};



