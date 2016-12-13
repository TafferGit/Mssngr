#pragma once
#include "MssgrServerMain.h"

class Parser
{
public:
	Parser();
	~Parser();
	std::string parse_login_for_registration(char *buf);
	std::string parse_password_for_registration(char *buf);
};

