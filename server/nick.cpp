#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

// Parameters: <nickname>
void	Server::NICK(int fd, std::vector<std::string> params)
{
	if (params.size() < 2)
		return ; // ERR_NONICKNAMEGIVEN

	// needs several touppers everywhere
	if (params[1].find_first_not_of(NICKSET) != std::string::npos)
		return ; // ERR_ERRONEOUSNICKNAME
	for (std::map<int, User>::const_iterator i = users.begin(); i != users.end(); ++i)
	{
		if (i->second.getNick() == params[1])
			return ; // ERR_NICKNAMEINUSE
	}

	// save previous nick for reply
	users[fd].setNick(params[1]);

	std::cout << "Command from user:" << this->users[fd].getNick() << std::endl;
	for (size_t i = 0; i < params.size(); i++)
		std::cout << params[i] << std::endl;

	return ; // :previous_nickname NICK params[1]
}
