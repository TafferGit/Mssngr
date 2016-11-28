#pragma once
#ifndef MSSGR_SERVER_MAIN_H
#define MSSGR_SERVER_MAIN_H


#include <string>
#include <iostream>

#include "ServerSocket.h"

class MssgrServer
{
private:
	ServerSocket * sock = NULL;
public:
	MssgrServer();
	~MssgrServer();
};

#endif // !MSSGR_SERVER_MAIN_H
