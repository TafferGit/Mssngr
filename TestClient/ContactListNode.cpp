#include "ContactListNode.h"



ContactListNode::ContactListNode()
{
}


ContactListNode::~ContactListNode()
{
}

void ContactListNode::ShowMessages()
{
	for (size_t i = 0; i < messagesVec.size(); i++) {
		std::cout << messagesVec.at(i) << std::endl;
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
