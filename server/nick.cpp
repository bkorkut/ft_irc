#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"
#include "../Command.hpp"

// Parameters: <nickname>
void	Server::NICK(int fd, std::vector<std::string> params)
{
	if (params.size() < 2)
		return sendData(fd, ERR_NONICKNAMEGIVEN(_users[fd].getNick()));

	// needs several touppers everywhere
	if (params[1].find_first_not_of(NICKSET) != std::string::npos)
		return sendData(fd, ERR_ERRONEUSNICKNAME(_users[fd].getNick(), params[1]));
	// What to return, nick in use or nick collision??
	for (std::map<int, User>::const_iterator i = _users.begin(); i != _users.end(); ++i)
	{
		if (i->second.getNick() == params[1])
			return ; // ERR_NICKNAMEINUSE
	}

	// save previous nick for reply!
	_users[fd].setNick(params[1]);

	std::cout << "Command from user:" << this->_users[fd].getNick() << std::endl;
	for (size_t i = 0; i < params.size(); i++)
		std::cout << params[i] << std::endl;

	return ; // :previous_nickname NICK params[1]
}
