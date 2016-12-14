#pragma once
#include "Connection.h"
#include <vector>
#include <conio.h>
#include <thread>
#include <mutex>
#include "ContactListNode.h"

class ContactList
{
private:
	Connection * pConnection;
	bool isReceivingActive = false;
	std::vector<std::string> requestedContactNames;
	std::vector<ContactListNode> contactListNodesVec;
	std::thread t1;
	std::thread t2;
	void RequestContactsFromServer();
	void RequestContactListFromServer();
	void ShowContactsFromServer();
	void ParseContactsFromServer(char *pInBuf, std::vector<std::string> *vectorToParse);
	void ParseCLFromServer(char * pInBuf, std::vector<ContactListNode>* pContactListVec);
	void ParseIncomingMessage(char*pInBuf);
	void SpawnThreads();
public:
	void ReceiveMsg();
	void ShowContactList();
	ContactList(Connection *c);
	~ContactList();
};

