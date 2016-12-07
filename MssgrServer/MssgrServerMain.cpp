#include "MssgrServerMain.h"

//Read callback function
//We create the input buffer evbuffer
//We get it's size
//We copy data from the buffer
//And we send the buffer back to client
//@param buf_ev the evbuffer where we read data
//@param arg
static void echo_read_cb(struct bufferevent *buf_ev, void *arg)
{
	char buf[512];
	int n = 0;
	struct evbuffer *buf_input = bufferevent_get_input(buf_ev);
	size_t buf_input_size = evbuffer_get_length(buf_input);
	evbuffer_copyout(buf_input, buf, sizeof(buf));

	while (buf[n] != '\n') {
		++n;
	}
	++n;
	fwrite(buf, 1, n, stdout);

	struct evbuffer *buf_output = bufferevent_get_output(buf_ev);
	evbuffer_add_buffer(buf_output, buf_input);
}

//Echo event callback function
//Buffer event callback function
//@param buf_ev the evbuffer structure connected to the event
//@param events event flags passed to the function
//@arg argument pointer passed to a function
static void echo_event_cb(struct bufferevent *buf_ev, short events, void *arg)
{
	if (events & BEV_EVENT_ERROR) {
		perror("bufferevent object error");
		if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
			bufferevent_free(buf_ev);
		}
	}
}

//Accept connection callback function
//Is called when there is a new incoming connection to the server
//@param listener evconnlistener object where the incoming connection arrived
//@param fd connection file descriptor number
//@param addr SOCKADDR structure with incoming connection info
//@param sock_len
//@param arg
static void accept_connection_cb(struct evconnlistener *listener, evutil_socket_t fd, SOCKADDR *addr, int sock_len, void *arg) {
	struct event_base *base = evconnlistener_get_base(listener);
	struct bufferevent *buf_ev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	fd_vec.push_back(fd);

	char inetAddress[INET_ADDRSTRLEN];
	SOCKADDR_IN *inAddr = (SOCKADDR_IN*)addr;
	int fd_vec_size = fd_vec.size();

	std::cout << "There are now " << fd_vec_size << " open connections:" << std::endl;
	for (int i = 0; i < fd_vec_size; i++) {
		std::cout << "Connection " << i << " file descriptor:" << fd_vec.at(i) << std::endl;
	}
	printf("Incoming connection from: %s\n", inet_ntop(inAddr->sin_family, &inAddr->sin_addr, inetAddress, 14));
	bufferevent_setcb(buf_ev, echo_read_cb, NULL, echo_event_cb, NULL);
	bufferevent_enable(buf_ev, (EV_READ | EV_WRITE));
}

//Error callback function
//Is called when some error occured
//@param evconnlistener listener where error occured
//@arg
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
	int port = 0;

	std::cout << "Please enter port number where server will listen.\n";
	std::cin >> port;

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
	listener = evconnlistener_new_bind(base, accept_connection_cb, NULL, LEV_OPT_REUSEABLE, -1, (SOCKADDR*)&sockIn, sizeof(sockIn));

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
