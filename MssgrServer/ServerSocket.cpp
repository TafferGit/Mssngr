#include "ServerSocket.h"


int ServerSocket::InitializeWinsock()
{
	//Initialize winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) 
	{
		printf("WSAStratup failed with error: %d\n", iResult);
		system("pause");
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	//Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error %d\n", iResult);
		system("pause");
		WSACleanup();
		return 1;
	}

	CreateSocket();
	Listen();
	OnReceive();

	return 0;
}

int ServerSocket::CreateSocket()
{
	//Create a SOCKET for connection to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		printf("Socket failed with error %ld\n", WSAGetLastError());
		system("pause");
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	BindSocket();

	return 0;
}

int ServerSocket::BindSocket()
{
	//Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) 
	{
		printf("bind failed with error: %d\n", WSAGetLastError);
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		system("pause");
		return 1;
	}

	return 0;
}

int ServerSocket::Listen()
{
	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		system("pause");
		return 1;
	}

	Accept();
	return 0;
}

int ServerSocket::Accept()
{
	//Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET)
	{
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		system("pause");
		return 1;
	}

	//No longer need server socket
	closesocket(ListenSocket);

	return 0;
}

int ServerSocket::OnReceive()
{
	//Receive until the peer shuts down the connection
	do {

		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) 
		{
			dynamicBuf = new char[iResult];
			for (int i = 0; i < iResult; i++)
			{
				dynamicBuf[i] = recvbuf[i];
			}

			int code = atoi(dynamicBuf);

			//Echo the buffer back to the sender
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) 
			{
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				system("pause");
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else 
		{
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
	} while (iResult > 0);

	return 0;
}

int ServerSocket::Shutdown()
{
	//Shutdown the connection since we are done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) 
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	return 0;
}

void ServerSocket::Cleanup()
{
	// cleanup
	closesocket(ClientSocket);
	WSACleanup();
}

ServerSocket::ServerSocket()
{
	InitializeWinsock();
}


ServerSocket::~ServerSocket()
{
	Shutdown();
	Cleanup();
}
