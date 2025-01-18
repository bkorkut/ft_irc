#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

// Parameters: <password>
void	Server::PASS(int fd, std::vector<std::string> params)
{
	if (params.size() < 2)
		return sendData(fd, ERR_NEEDMOREPARAMS(std::string("PASS")));
    if (_users[fd].getIsRegistered())
		return sendData(fd, ERR_ALREADYREGISTERED);

    std::cout << "\033[32m[PASS Command]\033[0m Password: " << params[1] << std::endl;
    _users[fd].setPassword(params[1]);
    if (!_users[fd].authenticate(_pswd)) {
        // Wrong password handling can be added here
        return;
    }
	// if (params[1] == _pswd)
	// 	_users[fd].pass = true;
	// else
	// 	; // disconnect
}
