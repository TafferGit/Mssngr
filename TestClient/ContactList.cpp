#include "ContactList.h"



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

void ContactList::ShowContactsFromServer()
{
	system("cls");
	int selected = 0; int chGet = 0;
	char *sendBuf = new char[512];
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

void ContactList::ParseContactsFromServer(char * pInBuf, std::vector<std::string>* pVectorToParse)
{
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

ContactList::ContactList(Connection *c)
{
	pConnection = c;
	int userInput = 0;

	for (size_t i = 0; i < contactListNodesVec.size(); i++) {
		std::cout << contactListNodesVec.at(i).contactName << std::endl;
	}
	printf("To add new contacts please press [A] button.\n");
	userInput = _getch();

	if (userInput == 'A' || userInput == 'a' || userInput == 'ô' || userInput == 'Ô') {
		RequestContactsFromServer();
	}
	ShowContactsFromServer();
}


ContactList::~ContactList()
{
}
