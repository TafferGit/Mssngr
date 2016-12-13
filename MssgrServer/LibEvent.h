#pragma once
#ifndef LIBEVENT_H
#define LIBEVENT_H

#include "MssgrServerMain.h"

#pragma comment (lib, "event.lib")
#pragma comment (lib, "event_core.lib")
#pragma comment (lib, "event_extra.lib")

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class LibEvent
{
private:
	static WSADATA wsaData;
	static int iResult, port;
	static struct event_base *base;
	static evconnlistener *listener;
	static SOCKADDR_IN sock_addr;
	static void data_read_cb(struct bufferevent *buf_ev, void *arg);
	static void data_event_cb(struct bufferevent *buf_ev, short events, void *arg);
	static void accept_connection_cb(struct evconnlistener *listener, evutil_socket_t fd, SOCKADDR *addr, int sock_len, void *arg);
	static void accept_error_cb(evconnlistener *listener, void *arg);
	static int check_fd_existence(bufferevent *buf_ev);
	static void on_message_receive(bufferevent *buf_ev, char *buf);
	static void on_registration_receive(UserData *ud, char *buf);
	static char * on_login_receive(UserData *ud, char * logCode);
	static void on_clreq_receive(UserData *ud, char *buf);
	static void on_cladd_receive(UserData *ud, char *buf);
public:
	LibEvent();
	~LibEvent();
	static void initialize_libevent();
};

#endif // !LIBEVENT_H