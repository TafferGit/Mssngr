#include "TestClient.h"

Connection::Connection(int argc, char **argv)
{
	int result;

	result = InitializeWinSockConnection(argc, argv);
	ResolveAddressAndPort(argv);
	ConnectToAddress();
	SendBuffer();
	ShutDownConnection();
	WaitForReceive();
}


Connection::~Connection()
{
	ConnectionCleanUp();
}


int Connection::InitializeWinSockConnection(int argc, char **argv) 
{
	sendbuf = new char[DEFAULT_BUFLEN];
	sprintf(sendbuf, "%s", "Hello stranger!\n");

	// Validate the parameters
	if (argc != 2)
	{
		printf("usage: %s server-name\n", argv[0]);
		return 1;
	}

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	return 0;
}

int Connection::ResolveAddressAndPort(char ** argv)
{
	// Resolve the server address and port
	iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	if (iResult != 0) 
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	return 0;
}

int Connection::ConnectToAddress()
{
	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL;ptr = ptr->ai_next)
	{

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) 
		{
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}
		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) 
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) 
	{
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	return 0;
}

int Connection::SendBuffer()
{
	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, DEFAULT_BUFLEN, 0);
	if (iResult == SOCKET_ERROR) 
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);

	return 0;
}

void Connection::WaitForReceive()
{
	// Receive until the peer closes the connection
	do {
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());
	} while (iResult > 0);
}

void Connection::ConnectionCleanUp()
{
	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();
}

int Connection::ShutDownConnection()
{
	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) 
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	return 0;
}


