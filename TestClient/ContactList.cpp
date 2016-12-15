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
		RequestContactsFromServer();
		ShowContactsFromServer();
		RequestContactListFromServer();
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
	std::string term_fu = "<fu>";
	std::string term_fu_end = "</fu>";
	std::string term_im = "<im>";
	std::string term_im_end = "</im>";
	//Temporary tracer vars
	std::fstream logStream;
	std::string logString;
	//End temporary tracer vars
	size_t fu_open_pos = 0;
	size_t fu_close_pos = 0;
	size_t im_open_pos = 0;
	size_t im_close_pos = 0;

	logStream.open("client.log", std::ios::out | std::ios::app);
	logStream << "Going to start parsing message!\n";
	logStream.close();
	while (fu_open_pos != std::string::npos && fu_close_pos != std::string::npos && im_open_pos != std::string::npos && im_close_pos != std::string::npos) {
		logStream.open("client.log", std::ios::out | std::ios::app);
		logStream << "Starting parsing!\n";
		int n = 0;
		fu_open_pos = bufString.find(term_fu, fu_open_pos);
		fu_close_pos = bufString.find(term_fu_end, fu_close_pos);
		im_open_pos = bufString.find(term_im, im_open_pos);
		im_close_pos = bufString.find(term_im_end, im_close_pos);

		size_t senderNameStart = fu_open_pos + 4;
		size_t senderNameSize = fu_close_pos - senderNameStart;
		size_t messageStart = im_open_pos + 4;
		size_t messageSize = im_close_pos - messageStart;
		if (fu_open_pos != std::string::npos && fu_close_pos != std::string::npos && im_open_pos != std::string::npos && im_close_pos != std::string::npos) {
			parsedSenderName = bufString.substr(senderNameStart, senderNameSize);
			parsedMessage = bufString.substr(messageStart, messageSize);
			while (this->contactListNodesVec.at(n).GetName() != parsedSenderName) {
				n++;
			}
			this->contactListNodesVec.at(n).PushMessageToVec(parsedSenderName, parsedMessage);
			fu_open_pos += term_fu.length(); fu_close_pos += term_fu_end.length(); im_open_pos += term_im.length(); im_close_pos += term_im_end.length();
			logStream << "Received buffer size " << bufString.size() << "\n";
			logStream << "Fu start found: " << fu_open_pos << " /fu " << fu_close_pos << " Sender's name length " << senderNameSize << " sender name start " << senderNameStart << "\n";
			logStream << "im start found: " << im_open_pos << " /im " << im_close_pos << " Message length " << messageSize << " Message start " << messageStart << "\n";
			logStream << "Sender \"" << parsedSenderName << "\" has been parsed\n";
			logStream << "Message \"" << parsedMessage << "\" has been parsed\n";
			logStream.close();
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
	std::fstream logStream;
	std::string logString;
	char recvBuf[DEFAULT_BUFLEN];
	while (this->isReceivingActive) {
		iResult = pConnection->WaitForReceiveSafe(recvBuf);

		if (recvBuf[0] == '<' && recvBuf[1] == 'f' && recvBuf[2] == 'u' && recvBuf[3] == '>') {
			logStream.open("client.log", std::ios::out | std::ios::app);
			if (!logStream.fail()) {
				logStream << "iResult: " << iResult << "\n";
				logString = recvBuf;
				logStream << "Received message: \"" << logString << "\" \n";
				logStream.close();
			}

			ParseIncomingMessage(recvBuf);
		}
		memset(recvBuf, 0, sizeof(recvBuf));
	}
	m.unlock();
}