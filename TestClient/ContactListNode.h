#pragma once
#include"Connection.h"
#include <vector>
#include <iostream>
#include <string>
#include <conio.h>

class ContactListNode
{
	std::string name;
	std::vector<std::string> messagesVec;

public:
	ContactListNode();
	~ContactListNode();
	void ShowMessages(Connection *c);
	std::string GetName();
	void SetName(std::string n);
	void PushMessageToVec(std::string msg);
};

