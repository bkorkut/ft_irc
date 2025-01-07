#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

// Parameters: <nickname> <channel>
void	Server::INVITE(int fd, std::vector<std::string> params)
{
	std::cout << "Command from user:" << this->_users[fd].getNick() << std::endl;
	for (size_t i = 0; i < params.size(); i++)
		std::cout << params[i] << std::endl;
}
