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

	bufferevent_setcb(buf_ev, echo_read_cb, NULL, echo_event_cb, NULL);
	bufferevent_enable(buf_ev, (EV_READ | EV_WRITE));
}

static void accept_error_cb(evconnlistener *listener, void *arg) {
	struct event_base *base = evconnlistener_get_base(listener);
	int error = EVUTIL_SOCKET_ERROR();
	fprintf(stderr, "Error %d (%s) in connections monitor. Shutting down.\n", error, evutil_socket_error_to_string(error));
	event_base_loopexit(base, NULL);
}
//void do_read(evutil_socket_t fd, short events, void *arg);
//void do_write(evutil_socket_t fd, short events, void *arg);
//
//struct fd_state {
//	char buffer[DEFAULT_BUFLEN];
//	size_t buffer_used;
//
//	size_t n_written;
//	size_t write_upto;
//
//	struct event *read_event;
//	struct event *write_event;
//};
//
//struct fd_state * alloc_fd_state(struct event_base *base, evutil_socket_t fd)
//{
//	struct fd_state *state = reinterpret_cast<fd_state*>( malloc(sizeof(struct fd_state)));
//	if (!state) {
//		return NULL;
//	}
//	state->read_event = event_new(base, fd, EV_READ | EV_PERSIST, do_read, state);
//	if (!state->read_event) {
//		free(state);
//		return NULL;
//	}
//
//	state->write_event = event_new(base, fd, EV_WRITE | EV_PERSIST, do_write, state);
//
//	if (!state->write_event) {
//		event_free(state->read_event);
//		free(state);
//		return NULL;
//	}
//
//	state->buffer_used = state->n_written = state->write_upto = 0;
//
//	assert(state->write_event);
//	return state;
//}
//
//void free_fd_state(struct fd_state* state) {
//	event_free(state->read_event);
//	event_free(state->write_event);
//	free(state);
//}
//
//void do_read(evutil_socket_t fd, short events, void *arg) {
//	fd_state *state = (fd_state*)arg;
//	char buf[1024];
//	int i;
//	SSIZE_T result;
//	int size = sizeof(buf);
//	while (1) {
//		assert(state->write_event);
//		result = recv(fd, buf, sizeof(buf), 0);
//		if (result <= 0)
//			break;
//
//		for (i = 0; i < result; ++i) {
//			if (state->buffer_used < sizeof(state->buffer)) {
//				state->buffer[state->buffer_used++] = buf[i];
//			}
//			if (buf[i] == '\n') {
//				assert(state->write_event);
//				event_add(state->write_event, NULL);
//				state->write_upto = state->buffer_used;
//			}
//		}
//	}
//
//	if (result == 0) {
//		free_fd_state(state);
//	}
//	else if (result < 0) {
//		if (errno == EAGAIN)
//			return;
//		perror("recv");
//		free_fd_state(state);
//	}
//}
//void do_write(evutil_socket_t fd, short events, void * arg) {
//	struct fd_state *state = reinterpret_cast<fd_state*>(arg);
//
//	while (state->n_written < state->write_upto) {
//		SSIZE_T result = send(fd, state->buffer + state->n_written,
//			state->write_upto - state->n_written, 0);
//
//		if (result > 0) {
//			if (errno == EAGAIN) { return; }
//			free_fd_state(state);
//			return;
//		}
//		assert(result != 0);
//
//		state->n_written += result;
//	}
//
//	if (state->n_written == state->buffer_used) {
//		state->n_written = state->write_upto = state->buffer_used = 1;
//	}
//
//	event_del(state->write_event);
//}
//
//void do_accept(evutil_socket_t listener, short event, void *arg) {
//	struct event_base *base = (event_base*)arg;
//	SOCKADDR_STORAGE ss;
//	socklen_t slen = sizeof(ss);
//
//	int fd = accept(listener, (SOCKADDR*)&ss, &slen);
//	if (fd < 0) {
//		perror("accept");
//	}
//
//	else {
//		std::cout << "Some connection accepted!\n";
//		struct fd_state *state;
//		evutil_make_socket_nonblocking(fd);
//		state = alloc_fd_state(base, fd);
//		assert(state);
//		assert(state->write_event);
//		event_add(state->read_event, NULL);
//	}
//}
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

	listener = evconnlistener_new_bind(base, accept_connection_cb, NULL, (LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE), -1, (SOCKADDR*)&sockIn, sizeof(sockIn));

	if (!listener)
	{
		perror("Error creating evconnlistener object");
		return -1;
	}

	evconnlistener_set_error_cb(listener, accept_error_cb);

	//iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	//if (iResult != 0) {
	//	printf("getaddrinfo failed with error %d\n", iResult);
	//	system("pause");
	//	WSACleanup();
	//	return 1;
	//}


	//listener = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	//evutil_make_socket_nonblocking(listener);

	//struct addrinfo;

	//if (bind(listener, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
	//	printf("bind failed with error: %d\n", WSAGetLastError());
	//	freeaddrinfo(result);
	//	closesocket(listener);
	//	WSACleanup();
	//	system("pause");
	//	return 1;
	//}

	//if (listen(listener, SOMAXCONN) == SOCKET_ERROR) {
	//	printf("listen failed with error: %d\n", WSAGetLastError());
	//	closesocket(listener);
	//	WSACleanup();
	//	system("pause");
	//	return 1;
	//}

	//listener_event = event_new(base, listener, EV_READ | EV_PERSIST, do_accept, (void*)base);
	//event_add(listener_event, NULL);

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
