#pragma once
#include "MssgrServerMain.h"
class ServerSocket
{
private:
	WSADATA	wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	char * dynamicBuf = NULL;
	int recvbuflen = DEFAULT_BUFLEN;

	int InitializeWinsock();
	int CreateSocket();
	int BindSocket();
	int Listen();
	int Accept();
	int OnReceive();
	int Shutdown();
	void Cleanup();
public:
	ServerSocket();
	~ServerSocket();
};

