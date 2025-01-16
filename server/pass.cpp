#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

// Parameters: <password>
void Server::PASS(int fd, std::vector<std::string> params)
{
    if (params.size() < 2)
        return sendData(fd, ERR_NEEDMOREPARAMS(_users[fd].getNick(), "PASS"));
    
    if (_users[fd].getIsRegistered())
        return sendData(fd, ERR_ALREADYREGISTERED(_users[fd].getNick()));

    _users[fd].setPassword(params[1]);
    if (!_users[fd].authenticate(_pswd)) {
        // Wrong password handling can be added here
        return;
    }
}