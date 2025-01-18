#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

// Parameters: <username> 0 * <realname>
void Server::USER(int fd, std::vector<std::string> params) {
    if (params.size() < 5)
        return sendData(fd, ERR_NEEDMOREPARAMS(_users[fd].getNick(), "USER"));
    
    if (_users[fd].getIsRegistered())
        return sendData(fd, ERR_ALREADYREGISTERED(_users[fd].getNick()));

    std::cout << "\033[32m[USER Command]\033[0m Username: " << params[1] 
              << ", Realname: " << params[4] << std::endl;

    _users[fd].setUsername(params[1]);
    _users[fd].setRealname(params[4]);

    if (_users[fd].checkRegistration()) {
        std::string prefix = _users[fd].getNick() + "!" + _users[fd].getUsername() + "@" + _serverName;
        sendData(fd, RPL_WELCOME(_serverName, _users[fd].getNick(), prefix));
    }
}
