#pragma once
#ifndef MSSGR_SERVER_MAIN_H
#define MSSGR_SERVER_MAIN_H


#include <string>
#include <iostream>
#include <clocale>
#include <assert.h>
#include <vector>

//libevent
#include <event2\event.h>
#include <event2\listener.h>
#include <event2\buffer.h>
#include <event2\bufferevent.h>

#include "ServerSocket.h"

#pragma comment (lib, "event.lib")
#pragma comment (lib, "event_core.lib")
#pragma comment (lib, "event_extra.lib")

struct UserData
{
	evbuffer *in_buf;
	evbuffer *out_buf;
	int fd;
	std::string username;
};
class MssgrServer
{
private:
	ServerSocket * sock = NULL;
public:
	MssgrServer();
	~MssgrServer();
};

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
public:
	static void initialize_libevent();
};

#endif // !MSSGR_SERVER_MAIN_H
