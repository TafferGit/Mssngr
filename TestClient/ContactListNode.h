#pragma once
#include"Connection.h"
#include <vector>
#include <iostream>
#include <string>
#include <conio.h>

//Contact message structure
//Contains message sender and message body itself
struct ContactMessage
{
	std::string contactName;
	std::string contactMessageBody;
};

class ContactListNode
{
	std::string name;
	std::vector<ContactMessage> messagesVec;
	void SendContactMessage(Connection *c);
public:
	ContactListNode();
	~ContactListNode();
	void ShowMessages(Connection *c);
	std::string GetName();
	void SetName(std::string n);
	void PushMessageToVec(std::string senderName, std::string msg);
};

