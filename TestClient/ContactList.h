#pragma once
#include "Connection.h"
#include <vector>
#include <conio.h>

struct ContactListNode {
	std::string contactName;
	bool isOnline;
};

class ContactList
{
private:
	Connection * pConnection;
	std::vector<std::string> requestedContactNames;
	std::vector<ContactListNode> contactListNodesVec;
	void RequestContactsFromServer();
	void ShowContactsFromServer();
	void ParseContactsFromServer(char *pInBuf, std::vector<std::string> *vectorToParse);
public:
	ContactList(Connection *c);
	~ContactList();
};

