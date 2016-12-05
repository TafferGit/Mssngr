#pragma once
#ifndef MSSGR_SERVER_MAIN_H
#define MSSGR_SERVER_MAIN_H


#include <string>
#include <iostream>
#include <clocale>

//libevent
#include <event2\event.h>
#include <event2\buffer.h>
#include <event2\bufferevent.h>

#include "ServerSocket.h"

#pragma comment (lib, "event.lib")
#pragma comment (lib, "event_core.lib")
#pragma comment (lib, "event_extra.lib")

class MssgrServer
{
private:
	ServerSocket * sock = NULL;
public:
	MssgrServer();
	~MssgrServer();
};

#endif // !MSSGR_SERVER_MAIN_H
