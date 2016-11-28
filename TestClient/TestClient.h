#pragma once
#ifndef TEST_CLIENT_H
#define TEST_CLIENT_H
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS



#include <windows.h>

#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <process.h>
#include <thread>
#include <iostream>

#include "Connection.h"
#include "LoginMenu.h"



class TestClient
{
private:
	Connection * c = NULL;
	LoginMenu * lm = NULL;
public:
	TestClient(int argc, char **argv);
	~TestClient();
};

#endif // !TEST_CLIENT_H
