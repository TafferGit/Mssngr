#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <process.h>
#include <thread>
#include <iostream>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "80"

class Connection
{
private:
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	char recvbuf[DEFAULT_BUFLEN];
	char *sendbuf;
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;
	std::string username;
	std::string receiver_username;
	std::string message;

	int ResolveAddressAndPort(char **argv);
	int ConnectToAddress();
	int SendBuffer();
	int ShutDownConnection();
	void SendUserName();
	void MessageScreen();
	void WaitForReceive();
	void ConnectionCleanUp();
public:
	Connection(int argc, char **argv);
	~Connection();
	int InitializeWinSocket(int argc, char **argv);
};



