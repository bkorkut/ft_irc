#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

// Parameters: <nickname>
void Server::NICK(int fd, std::vector<std::string> params) {
	std::cout << "\033[32m[NICK Command]\033[0m New nickname: " << params[1] << std::endl;
	if (params.size() < 2)
		return sendData(fd, ERR_NONICKNAMEGIVEN);

	if (toLower(params[1]).find_first_not_of(NICKSET) != std::string::npos)
		return sendData(fd, ERR_ERRONEUSNICKNAME(params[1]));

	if (findUserWithNick(_users, params[1]) != NULL)
			return sendData(fd, ERR_NICKNAMEINUSE(params[1]));

	if (!_users[fd].getNick().empty())
		sendData(fd, ":" + _users[fd].getNick() + " NICK " + params[1] + "\r\n");
	_users[fd].setNick(params[1]);
}
