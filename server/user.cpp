#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

// Parameters: <user> <mode> <unused> <realname>
void	Server::USER(int fd, std::vector<std::string> params)
{
	if (params.size() < 5)
		return sendData(fd, ERR_NEEDMOREPARAMS(std::string("USER")));
	if (_users[fd].registered == true)
		return sendData(fd, ERR_ALREADYREGISTERED);
	_users[fd].setUname(params[1]);
	_users[fd].setRname(params[4]);
	// check user mods to set and use <mode>
	_users[fd].user = true;
}
