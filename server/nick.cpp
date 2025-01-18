#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

// Parameters: <nickname>
void Server::NICK(int fd, std::vector<std::string> params) {
    if (params.size() < 2)
        return sendData(fd, ERR_NONICKNAMEGIVEN);

    std::cout << "\033[32m[NICK Command]\033[0m New nickname: " << params[1] << std::endl;

    if (toLower(params[1]).find_first_not_of(NICKSET) != std::string::npos)
        return sendData(fd, ERR_ERRONEUSNICKNAME(params[1]));

    for (std::map<int, User>::const_iterator i = _users.begin(); i != _users.end(); ++i)
        if (toLower(i->second.getNick()) == toLower(params[1]))
            return sendData(fd, ERR_NICKNAMEINUSE(params[1]));

    if (_users[fd].getNick().empty())
        sendData(fd, "NICK "+ params[1] + "\r\n");
    else
        sendData(fd, ":" + _users[fd].getNick() + " NICK " + params[1] + "\r\n");
    _users[fd].setNick(params[1]);
	_users[fd].nick = true;
}
