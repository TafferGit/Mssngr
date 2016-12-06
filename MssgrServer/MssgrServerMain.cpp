#include "MssgrServerMain.h"


static void echo_read_cb(struct bufferevent *buf_ev, void *arg)
{
	struct evbuffer *buf_input = bufferevent_get_input(buf_ev);
	struct evbuffer *buf_output = bufferevent_get_output(buf_ev);
	evbuffer_add_buffer(buf_output, buf_input);
}

static void echo_event_cb(struct bufferevent *buf_ev, short events, void *arg)
{
	if (events & BEV_EVENT_ERROR) {
		perror("bufferevent obect error");
		if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
			bufferevent_free(buf_ev);
		}
	}
}

static void accept_connection_cb(struct evconnlistener *listener, evutil_socket_t fd, SOCKADDR *addr, int sock_len, void *arg) {
	struct event_base *base = evconnlistener_get_base(listener);
	struct bufferevent *buf_ev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	char inetAddress[14];
	SOCKADDR_IN *inAddr = (SOCKADDR_IN*)addr;

	printf("Incoming connection from: %s\n", inet_ntop(inAddr->sin_family, &inAddr->sin_addr, inetAddress, 14));
	bufferevent_setcb(buf_ev, echo_read_cb, NULL, echo_event_cb, NULL);
	bufferevent_enable(buf_ev, (EV_READ | EV_WRITE));
}

static void accept_error_cb(evconnlistener *listener, void *arg) {
	struct event_base *base = evconnlistener_get_base(listener);
	int error = EVUTIL_SOCKET_ERROR();
	fprintf(stderr, "Error %d (%s) in connections monitor. Shutting down.\n", error, evutil_socket_error_to_string(error));
	event_base_loopexit(base, NULL);
}

int __cdecl main()
{
	WSADATA wsaData;
	int iResult;
	struct event_base *base;
	evconnlistener *listener;
	SOCKADDR_IN sockIn;
	int port = 27015;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStratup failed with error: %d\n", iResult);
		system("pause");
		return 1;
	}

	base = event_base_new();
	if (!base)
		return 1;

	ZeroMemory(&sockIn, sizeof(sockIn));
	sockIn.sin_family = AF_INET;
	sockIn.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	sockIn.sin_port = htons(port);

	//Creating libevent listener for the above created socket
	listener = evconnlistener_new_bind(base, accept_connection_cb, NULL, (LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE), -1, (SOCKADDR*)&sockIn, sizeof(sockIn));

	if (!listener) {
		perror("Error creating evconnlistener object");
		return -1;
	}

	//Setting libevent error callback function
	evconnlistener_set_error_cb(listener, accept_error_cb);

	event_base_dispatch(base); //Starting libevent main loop

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
