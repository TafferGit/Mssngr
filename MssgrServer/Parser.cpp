#include "Parser.h"
#include <string>

Parser::Parser()
{
}


Parser::~Parser()
{
}

std::string Parser::parse_login(char *buf) {
	//Parse login
	std::string BufferToString(buf);
	size_t pos_start = 0;
	size_t pos_end = 0;
	std::string username;

	pos_start = BufferToString.find("<log>") + 5;
	pos_end = BufferToString.find("</log>");

	//If we were able to find <log> and </log> fill our username variable
	if (pos_start != std::string::npos && pos_end != std::string::npos) {
		size_t loginSize = pos_end - pos_start;
		username = BufferToString.substr(pos_start, loginSize);
	}
	
	return username;
}

std::string Parser::parse_password(char * buf)
{
	//Parse password
	std::string BufferToString(buf);
	size_t pos_start = 0;
	size_t pos_end = 0;
	std::string password;

	pos_start = BufferToString.find("<pas>") + 5;
	pos_end = BufferToString.find("</pas>");

	//If we were able to find <pas> and </pas> fill our password variable
	if (pos_start != std::string::npos && pos_end != std::string::npos) {
		size_t passwordSize = pos_end - pos_start;
		password = BufferToString.substr(pos_start, passwordSize);
	}

	return password;
}

std::string Parser::parse_contact_name(char * buf) {
	//Parse contact name
	std::string bufString(buf);
	std::string contactName;
	size_t cladd_pos = 0;
	size_t clend_pos = 0;

	while (cladd_pos != std::string::npos && clend_pos != std::string::npos) {
		cladd_pos = bufString.find("<cladd>", cladd_pos);
		clend_pos = bufString.find("</>", clend_pos);
		size_t contactNameStart = cladd_pos + 7;
		size_t contactNameLength = clend_pos - contactNameStart;
		if (cladd_pos != std::string::npos && clend_pos != std::string::npos) {
			contactName = bufString.substr(contactNameStart, contactNameLength);
			cladd_pos++; clend_pos++;
		}
		else break;
	}

	return contactName;
}