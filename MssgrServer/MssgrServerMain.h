#pragma once
#ifndef MSSGR_SERVER_MAIN_H
#define MSSGR_SERVER_MAIN_H

#define DEFAULT_BUFLEN 512

#include <string>
#include <iostream>
#include <clocale>
#include <assert.h>
#include <vector>
#include <WinSock2.h>

//libevent
#include <event2\event.h>
#include <event2\listener.h>
#include <event2\buffer.h>
#include <event2\bufferevent.h>

struct UserData
{
	evbuffer *in_buf;
	evbuffer *out_buf;
	int fd;
	std::string username;
};

#endif // !MSSGR_SERVER_MAIN_H
