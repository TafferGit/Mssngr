#include "MssgrServerMain.h"

void do_read(evutil_socket_t fd, short events, void *arg);
void do_write(evutil_socket_t fd, short events, void *arg);

struct fd_state {
	char buffer[DEFAULT_BUFLEN];
	size_t buffer_used;

	size_t n_written;
	size_t write_upto;

	struct event *read_event;
	struct event *write_event;
};

struct fd_state * alloc_fd_state(struct event_base *base, evutil_socket_t fd)
{
	struct fd_state *state = reinterpret_cast<fd_state*>( malloc(sizeof(struct fd_state)));
	if (!state) {
		return NULL;
	}
	state->read_event = event_new(base, fd, EV_READ | EV_PERSIST, do_read, state);
	if (!state->read_event) {
		free(state);
		return NULL;
	}

	state->write_event = event_new(base, fd, EV_WRITE | EV_PERSIST, do_write, state);

	if (!state->write_event) {
		event_free(state->read_event);
		free(state);
		return NULL;
	}

	state->buffer_used = state->n_written = state->write_upto = 0;

	assert(state->write_event);
	return state;
}

void free_fd_state(struct fd_state* state) {
	event_free(state->read_event);
	event_free(state->write_event);
	free(state);
}

void do_read(evutil_socket_t fd, short events, void *arg) {
	fd_state *state = (fd_state*)arg;
	char buf[1024];
	int i;
	SSIZE_T result;
	int size = sizeof(buf);
	while (1) {
		assert(state->write_event);
		result = recv(fd, buf, sizeof(buf), 0);
		if (result <= 0)
			break;

		for (i = 0; i < result; ++i) {
			if (state->buffer_used < sizeof(state->buffer)) {
				state->buffer[state->buffer_used++] = buf[i];
			}
			if (buf[i] == '\n') {
				assert(state->write_event);
				event_add(state->write_event, NULL);
				state->write_upto = state->buffer_used;
			}
		}
	}

	if (result == 0) {
		free_fd_state(state);
	}
	else if (result < 0) {
		if (errno == EAGAIN)
			return;
		perror("recv");
		free_fd_state(state);
	}
}
void do_write(evutil_socket_t fd, short events, void * arg) {
	struct fd_state *state = reinterpret_cast<fd_state*>(arg);

	while (state->n_written < state->write_upto) {
		SSIZE_T result = send(fd, state->buffer + state->n_written,
			state->write_upto - state->n_written, 0);

		if (result > 0) {
			if (errno == EAGAIN) { return; }
			free_fd_state(state);
			return;
		}
		assert(result != 0);

		state->n_written += result;
	}

	if (state->n_written == state->buffer_used) {
		state->n_written = state->write_upto = state->buffer_used = 1;
	}

	event_del(state->write_event);
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
		struct fd_state *state;
		evutil_make_socket_nonblocking(clientSocket);
		state = alloc_fd_state(base, clientSocket);
		assert(state);
		assert(state->write_event);
		event_add(state->read_event, NULL);
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
