#include "ContactListNode.h"



ContactListNode::ContactListNode()
{
}


ContactListNode::~ContactListNode()
{
}

void ContactListNode::ShowMessages(Connection *c)
{
	int userInput = 0;
	char sendBuf[DEFAULT_BUFLEN];
	std::string message;
	std::cin.ignore();
	while (userInput != 27) {
		system("cls");
		for (size_t i = 0; i < messagesVec.size(); i++) {
			std::cout << this->GetName() << ": " << messagesVec.at(i) << std::endl;
		}
		printf("To write a message press [M].\n");
		printf("To exit to contact list press [Esc]\n");
		
		userInput = _getch();
		if (userInput == 'm' || userInput == 'M') {
			std::cout << "Enter your message: ";
			std::getline(std::cin, message);
			sprintf(sendBuf, "<rec>%s</rec>\n<msg>%s</msg>\n", this->GetName().c_str(), message.c_str());
			c->SendBuffer(sendBuf);
			message.clear();
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

void ContactListNode::PushMessageToVec(std::string msg)
{
	messagesVec.push_back(msg);
}
