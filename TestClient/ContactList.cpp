#include "ContactList.h"

#define NO_CONTACT_LIST_YET '1'

std::mutex m;

void ContactList::RequestContactsFromServer()
{
	char *outBuf = new char[DEFAULT_BUFLEN];
	char *recvBuf = new char[DEFAULT_BUFLEN];
	int iResult = 0;
	sprintf(outBuf, "<clreq>");
	pConnection->SendBuffer(outBuf);
	do {
		iResult = pConnection->WaitForReceive(recvBuf);
	} while (iResult <= 0);

	ParseContactsFromServer(recvBuf, &requestedContactNames);
}

void ContactList::RequestContactListFromServer()
{
	char *outBuf = new char[DEFAULT_BUFLEN];
	char *recvBuf = new char[DEFAULT_BUFLEN];
	int iResult = 0;
	sprintf(outBuf, "<cl>");
	pConnection->SendBuffer(outBuf);
	do {
		iResult = pConnection->WaitForReceive(recvBuf);
	} while (iResult <= 0);

	if (recvBuf[0] == NO_CONTACT_LIST_YET) {
		printf("You have no contacts!\n");
		system("pause");
	}
	else {
		ParseCLFromServer(recvBuf, &contactListNodesVec);
	}
}

void ContactList::ShowContactsFromServer()
{
	system("cls");
	int selected = 0; int chGet = 0;
	char *sendBuf = new char[DEFAULT_BUFLEN];
	for (size_t i = 0; i < requestedContactNames.size(); i++) {
		printf("%d. %s\n", i, requestedContactNames.at(i).c_str());
	}
	printf("Selected: %d. %s\n", selected, requestedContactNames.at(selected).c_str());
	
	while (chGet != 27 && chGet != 13) {
		chGet = _getch();

		if (chGet == 72) {
			if (selected > 0) {
				--selected;
				system("cls");
				for (size_t i = 0; i < requestedContactNames.size(); i++) {
					printf("%d. %s\n", i, requestedContactNames.at(i).c_str());
				}
				printf("Selected: %d. %s\n", selected, requestedContactNames.at(selected).c_str());
			}
		}
		else if (chGet == 80) {
			if (selected < requestedContactNames.size() - 1) {
				++selected;
				system("cls");
				for (size_t i = 0; i < requestedContactNames.size(); i++) {
					printf("%d. %s\n", i, requestedContactNames.at(i).c_str());
				}
				printf("Selected: %d. %s\n", selected, requestedContactNames.at(selected).c_str());
			}
		}
	}
	
	if (chGet == 13) {
		sprintf(sendBuf, "<cladd>%s</>", requestedContactNames.at(selected).c_str());
		pConnection->SendBuffer(sendBuf);
	}
}

void ContactList::ParseCLFromServer(char * pInBuf, std::vector<ContactListNode>* pContactListVec) {
	pContactListVec->clear();
	std::string bufString(pInBuf);
	ContactListNode contactListNode;
	size_t un_open = 0;
	size_t un_close = 0;

	while (un_open != std::string::npos && un_close != std::string::npos) {
		un_open = bufString.find("<un>", un_open);
		un_close = bufString.find("</un>", un_close);
		size_t contactNameStart = un_open + 4;
		size_t contactNameSize = un_close - contactNameStart;
		if (un_open != std::string::npos && un_close != std::string::npos) {
			contactListNode.SetName(bufString.substr(contactNameStart, contactNameSize));
			pContactListVec->push_back(contactListNode);
			un_open++; un_close++;
		}
		else break;
	}
}

void ContactList::ParseIncomingMessage(char * pInBuf)
{
	std::string bufString(pInBuf);
	std::string parsedSenderName;
	std::string parsedMessage;
	size_t fu_open = 0;
	size_t fu_close = 0;
	size_t im_open = 0;
	size_t im_close = 0;

	while (fu_open != std::string::npos && fu_close != std::string::npos && im_open != std::string::npos && im_close != std::string::npos) {
		int n = 0;
		fu_open = bufString.find("<fu>", fu_open);
		fu_close = bufString.find("</fu>", fu_close);
		im_open = bufString.find("<im>", im_open);
		im_close = bufString.find("</im>", im_close);
		size_t senderNameStart = fu_open + 4;
		size_t senderNameSize = fu_close - senderNameStart;
		size_t messageStart = im_open + 4;
		size_t messageSize = im_close - messageStart;
		if (fu_open != std::string::npos && fu_close != std::string::npos && im_open != std::string::npos && im_close != std::string::npos) {
			parsedSenderName = bufString.substr(senderNameStart, senderNameSize);
			parsedMessage = bufString.substr(messageStart, messageSize);
			while (this->contactListNodesVec.at(n).GetName() != parsedSenderName) {
				n++;
			}
			this->contactListNodesVec.at(n).PushMessageToVec(parsedMessage);
			++fu_open; ++fu_close; ++im_open; ++im_close;
		}
	}
}

void ContactList::ParseContactsFromServer(char * pInBuf, std::vector<std::string>* pVectorToParse)
{
	pVectorToParse->clear();
	std::string bufString(pInBuf);
	std::string parsedUsername;
	size_t un_open = 0;
	size_t un_close = 0;

	while (un_open != std::string::npos && un_close != std::string::npos) {
		un_open = bufString.find("<un>", un_open);
		un_close = bufString.find ("</un>", un_close);
		size_t loginStart = un_open + 4;
		size_t loginSize = un_close - loginStart;
		if (un_open != std::string::npos && un_close != std::string::npos) {
			parsedUsername = bufString.substr(loginStart, loginSize);
			pVectorToParse->push_back(parsedUsername);
			un_open++; un_close++;
		}
		else break;
	}
}

void ContactList::ShowContactList()
{
	int userInput(0), selected(0);
	system("cls");
	for (size_t i = 0; i < contactListNodesVec.size(); i++) {
		std::cout << i << ". " << contactListNodesVec.at(i).GetName() << std::endl;
	}
	printf("Selected: %d %s\n", selected, contactListNodesVec.at(selected).GetName().c_str());
	printf("To add new contacts please press [A] button.\n");
	while (userInput != 27 && userInput != 13 && userInput != 'A' && userInput != 'a'){
		userInput = _getch();

		if (userInput == 72) {
			if (selected > 0) {
				--selected;
				system("cls");
				for (size_t i = 0; i < contactListNodesVec.size(); i++) {
					std::cout << i << ". " << contactListNodesVec.at(i).GetName() << std::endl;
				}
				printf("Selected: %d %s\n", selected, contactListNodesVec.at(selected).GetName().c_str());
				printf("To add new contacts please press [A] button.\n");
			}
		}

		else if (userInput == 80) {
			if (selected < contactListNodesVec.size() - 1) {
				++selected;
				system("cls");
				for (size_t i = 0; i < contactListNodesVec.size(); i++) {
					std::cout << i << ". " << contactListNodesVec.at(i).GetName() << std::endl;
				}
				printf("Selected: %d %s\n", selected, contactListNodesVec.at(selected).GetName().c_str());
				printf("To add new contacts please press [A] button.\n");
			}
		}

		else if (userInput == 'A' || userInput == 'a' || userInput == 'ô' || userInput == 'Ô') {
			this->isReceivingActive = false;
			RequestContactsFromServer();
			ShowContactsFromServer();
			RequestContactListFromServer();
			ShowContactList();
		}

		else if (userInput == 13) {
			this->contactListNodesVec.at(selected).ShowMessages(pConnection);
			ShowContactList();
		}
	}
}

ContactList::ContactList(Connection *c)
{
	pConnection = c;
	RequestContactListFromServer();
	this->isReceivingActive = true;
	SpawnThreads();
}

void ContactList::SpawnThreads() {
	t1 = std::thread(&ContactList::ShowContactList, this);
	t2 = std::thread(&ContactList::ReceiveMsg, this);
	if (t1.joinable())
		t1.join();
	if (t2.joinable())
		t2.join();
}


ContactList::~ContactList()
{
	pConnection = NULL;
}

void ContactList::ReceiveMsg()
{
	m.lock();
	int iResult = 0;
	while (this->isReceivingActive) {
		char * recvBuf = new char[DEFAULT_BUFLEN];
		iResult = pConnection->WaitForReceiveSafe(recvBuf);

		if (recvBuf[0] == '<' && recvBuf[1] == 'f' && recvBuf[2] == 'u' && recvBuf[3] == '>') {
			ParseIncomingMessage(recvBuf);
		}
		delete recvBuf;
	}
	m.unlock();
}