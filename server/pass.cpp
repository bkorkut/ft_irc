#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

// Parameters: <password>
void	Server::PASS(int fd, std::vector<std::string> params)
{
	if (params.size() < 2)
		return sendData(fd, ERR_NEEDMOREPARAMS(std::string("PASS")));
	if (_users[fd].registered)
		return sendData(fd, ERR_ALREADYREGISTERED);
	if (params[1] == _pswd)
		_users[fd].pass = true;
	else
		; // disconnect
}
