#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

// Parameters: <nickname>
void	Server::NICK(int fd, std::vector<std::string> params)
{
	if (params.size() < 2)
		return ; // ERR_NONICKNAMEGIVEN
	if (params[1].find_first_not_of(NICKSET) != std::string::npos)
		return ; // ERR_ERRONEOUSNICKNAME

	// check if nickname is already in use in a user (NICKNAMEINUSE OR NICKCOLLISION)
	// if not, set or change the prev one
	// if so, issue an ERR_NICKNAMEINUSE numeric and ignore nick
	std::cout << "Command from user:" << this->users[fd].getNick() << std::endl;
	for (size_t i = 0; i < params.size(); i++)
		std::cout << params[i] << std::endl;
}
