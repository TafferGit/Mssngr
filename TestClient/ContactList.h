#pragma once
#include "Connection.h"
#include <vector>
#include <conio.h>
#include <thread>
#include "ContactListNode.h"

class ContactList
{
private:
	Connection * pConnection;
	std::vector<std::string> requestedContactNames;
	std::vector<ContactListNode> contactListNodesVec;
	void RequestContactsFromServer();
	void RequestContactListFromServer();
	void ShowContactsFromServer();
	void ParseContactsFromServer(char *pInBuf, std::vector<std::string> *vectorToParse);
	void ContactList::ParseCLFromServer(char * pInBuf, std::vector<ContactListNode>* pContactListVec);
	void ReceiveMsg();
public:
	std::thread Spawn();
	void ShowContactList();
	ContactList(Connection *c);
	~ContactList();
};

