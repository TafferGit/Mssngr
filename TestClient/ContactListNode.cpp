#include "ContactListNode.h"



void ContactListNode::SendContactMessage(Connection *c)
{
	ContactMessage cm;
	char sendBuf[DEFAULT_BUFLEN];
	std::string message;
	std::cout << "Enter your message: ";
	std::getline(std::cin, message);
	cm.contactName = "Me"; cm.contactMessageBody = message;
	messagesVec.push_back(cm);																		//Pushes back our own message to the messages history vector
	sprintf(sendBuf, "<rec>%s</rec>\n<msg>%s</msg>\n", this->GetName().c_str(), message.c_str());  //Actually sends the message to the server
	c->SendBuffer(sendBuf);
	message.clear();
}

ContactListNode::ContactListNode()
{
}


ContactListNode::~ContactListNode()
{
}

void ContactListNode::ShowMessages(Connection *c)
{
	int userInput = 0;
	std::cin.ignore();
	while (userInput != 27) {
		system("cls");
		for (size_t i = 0; i < messagesVec.size(); i++) {
			std::cout << messagesVec.at(i).contactName << ": " << messagesVec.at(i).contactMessageBody << std::endl;
		}
		printf("To write a message press [M].\n");
		printf("To exit to contact list press [Esc]\n");
		
		userInput = _getch();
		if (userInput == 'm' || userInput == 'M' || userInput == 236 || userInput == 156) {
			SendContactMessage(c);
		}
	}
}

std::string ContactListNode::GetName()
{
	return this->name;
}

void ContactListNode::SetName(std::string n)
{
	this->name = n;
}

void ContactListNode::PushMessageToVec(std::string senderName, std::string msg)
{
	ContactMessage cm;
	cm.contactName = senderName;
	cm.contactMessageBody = msg;
	messagesVec.push_back(cm);
}
