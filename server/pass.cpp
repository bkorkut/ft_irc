#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

// Parameters: <password>
void	Server::PASS(int fd, std::vector<std::string> params)
{
	if (params.size() < 2)
		return sendData(fd, ERR_NEEDMOREPARAMS(std::string("PASS")));
	if (_users[fd].registered || _users[fd].pass)
		return sendData(fd, ERR_ALREADYREGISTERED);
	
	_users[fd].pass = true;

	std::cout << "Command from user: " << this->_users[fd].getNick() << std::endl;
	for (size_t i = 0; i < params.size(); i++)
		std::cout << params[i] << std::endl;
}
