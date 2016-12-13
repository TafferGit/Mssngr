#pragma once
#include"Connection.h"
#include <vector>
#include <iostream>
#include <string>
class ContactListNode
{
	std::string name;
	std::vector<std::string> messagesVec;

public:
	ContactListNode();
	~ContactListNode();
	void ShowMessages();
	std::string GetName();
	void SetName(std::string n);
	void PushMessageToVec(std::string msg);
};

