#include "TestClient.h"

Connection::Connection(int argc, char **argv)
{
	int result;

	result = InitializeWinSocket(argc, argv);
	ResolveAddressAndPort(argv);
	ConnectToAddress();
	SendUserName();
	MessageScreen();
}


Connection::~Connection()
{
	ConnectionCleanUp();
}


int Connection::InitializeWinSocket(int argc, char **argv) 
{
	sendbuf = new char[512];
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
	hints.ai_socktype = SOCK_STREAM ;
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

	u_long iMode = 1;
	ioctlsocket(ConnectSocket, FIONBIO, &iMode);

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
	
	WaitForReceive();
	return 0;
}

void Connection::WaitForReceive()
{
	// Receive until the peer closes the connection
	//do {

	iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	MessageScreen();
	printf("recv failed with error: %d\n", WSAGetLastError());
	//} while (1);
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
	iResult = shutdown(ConnectSocket, SD_BOTH);
	if (iResult == SOCKET_ERROR) 
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	return 0;
}

void Connection::MessageScreen()
{
	system("cls");
	if (iResult > 0) {
		for (int i = 0; i < iResult; i++) {
			printf("%c", recvbuf[i]);
		}
		printf("\nTo refresh the dialog press [R]. To write a message press [M].\n");
		printf("To exit client press [Esc]\n");
	}

	else if (iResult == -1) {
		printf("No new messages.\n");
		printf("\nTo refresh the dialog press [R]. To write a message press [M].\n");
		printf("To exit client press [Esc]\n");
	}

	int userChoice = _getch();

	if (userChoice == 114 || userChoice == 170) {
		WaitForReceive();
	}
	
	else if (userChoice == 109 || userChoice == 236) {
		system("cls");
		std::cout << "Enter receiver's username: ";
		std::getline(std::cin, receiver_username);
		std::cout << "Enter your message: ";
		std::getline(std::cin, message);
		sprintf(sendbuf, "<rec>%s</rec>\n<msg>%s</msg>\n", receiver_username.c_str(), message.c_str());
		SendBuffer();
	}

	else if (userChoice == 27) {
		ShutDownConnection();
		ConnectionCleanUp();
		exit(0);
	}

	else {
		printf("Incorrect input. Try again.\n");
		MessageScreen();
	}
}

void Connection::SendUserName()
{
	std::cout << "Enter your username: ";
	std::cin >> username;
	std::cin.ignore();
	sprintf(sendbuf, "!u!%s\n", username.c_str());

	SendBuffer();
}

