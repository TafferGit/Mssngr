#pragma once
#include "MssgrServerMain.h"

class Parser
{
public:
	Parser();
	~Parser();
	std::string parse_login(char *buf);
	std::string parse_password(char *buf);
	std::string parse_contact_name(char * buf);
};

