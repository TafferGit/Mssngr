#pragma once
#define WIN32_LEAN_AND_MEAN
#undef UNICODE

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

//Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
//#pragma comment (lib, "Mswsock.lib")

//Request codes
#define LOGIN_REQUEST 1
#define REGISTRATION_REQUEST 2
#define CONTACT_LIST_REQUEST 3
#define SEND_MESSAGE_REQUEST 4

//Return codes
#define LOGIN_SUCCESSFUL 10
#define LOGIN_NOT_EXIST 11
#define PASSWORD_INCORRECT 12
#define CONTACT_LIST_REQUEST_SUCCESS 13
#define CONTACT_LIST_REQUEST_FAIL 14
#define SEND_MESSAGE_REQUEST_SUCCESS 15
#define SEND_MESSAGE_REQUEST_FAIL 16

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

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

