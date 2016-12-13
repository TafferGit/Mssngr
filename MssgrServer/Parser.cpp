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
	//Parse login
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
