#include "MssgrServerMain.h"

void do_read(evutil_socket_t fd, short events, void *arg) {

}
void do_write(evutil_socket_t fd, short events, void * arg);

void readcb(struct bufferevent *bev, void *ctx) {
	struct evbuffer *input, *output;
	char *line;
	size_t n;
	input = bufferevent_get_input(bev);
	output = bufferevent_get_output(bev);

	while ((line = evbuffer_readln(input, &n, EVBUFFER_EOL_LF))) {
		evbuffer_add(output, line, n);
		evbuffer_add(output, "\n", 1);
		std::cout << line << std::endl;
		free(line);
	}
}

void errorcb(struct bufferevent *bev, short error, void *ctx) {
	if (error & BEV_EVENT_EOF) {
		printf("Connection was closed.\n");
		WSACleanup();
	}
	else if (error & BEV_EVENT_ERROR) {
		std::cerr << "Error: " << strerror(errno);
		system("pause");
	}
	else if (error & BEV_EVENT_TIMEOUT) {
		/* must be a timeout handle, handle it */
		/* ... */
	}
	bufferevent_free(bev);
}

void do_accept(evutil_socket_t listener, short event, void *arg) {
	struct event_base *base = (event_base*)arg;
	SOCKADDR_IN ss;
	char stringBuf[INET_ADDRSTRLEN];
	socklen_t slen = sizeof(ss);

	SOCKET clientSocket = accept(listener, (SOCKADDR*)&ss, &slen);
	if (clientSocket < 0) {
		perror("accept");
	}
	else if (clientSocket == INVALID_SOCKET) {
		std::cout << "Accept failed with error: " << WSAGetLastError();
		closesocket(listener);
		WSACleanup();
		system("pause");
	}
	else {
		std::cout << "Some connection accepted from: " << inet_ntop(AF_INET, &(ss.sin_addr), stringBuf, INET_ADDRSTRLEN) << std::endl;
		struct bufferevent *bev;
		evutil_make_socket_nonblocking(clientSocket);
		bev = bufferevent_socket_new(base, clientSocket, BEV_OPT_CLOSE_ON_FREE);
		bufferevent_setcb(bev, readcb, NULL, errorcb, NULL);
		bufferevent_setwatermark(bev, EV_READ, 0, DEFAULT_BUFLEN);
		bufferevent_enable(bev, EV_READ | EV_WRITE);
	}
}
int __cdecl main()
{
	WSADATA wsaData;
	int iResult;

	evutil_socket_t listener;
	struct event_base *base;
	struct event *listener_event;

	struct addrinfo hints;
	struct addrinfo *result;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStratup failed with error: %d\n", iResult);
		system("pause");
		return 1;
	}

	base = event_base_new();
	if (!base)
		return 1;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error %d\n", iResult);
		system("pause");
		WSACleanup();
		return 1;
	}


	listener = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	evutil_make_socket_nonblocking(listener);

	struct addrinfo;

	if (bind(listener, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(listener);
		WSACleanup();
		system("pause");
		return 1;
	}

	if (listen(listener, SOMAXCONN) == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listener);
		WSACleanup();
		system("pause");
		return 1;
	}

	listener_event = event_new(base, listener, EV_READ | EV_PERSIST, do_accept, (void*)base);
	event_add(listener_event, NULL);

	event_base_dispatch(base);
	//MssgrServer * msgSrv = new MssgrServer;
	system("pause");
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
