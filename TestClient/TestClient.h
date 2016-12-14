#pragma once
#ifndef TEST_CLIENT_H
#define TEST_CLIENT_H
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <process.h>
#include <thread>
#include <iostream>
#include <conio.h>
#include <mutex>

#include "Connection.h"
#include "User.h"
#include "ContactList.h"



class TestClient
{
private:
	Connection * c = NULL;
	User * user = NULL;
	ContactList * pCL = NULL;
public:
	TestClient(int argc, char **argv);
	~TestClient();
};

#endif // !TEST_CLIENT_H
