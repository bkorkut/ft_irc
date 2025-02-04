#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

void Server::PASS(int fd, std::vector<std::string> params) {
	std::cout << "\033[32m[PASS Command]\033[0m" << std::endl;

	if (params.size() < 2)
		return sendData(fd, ERR_NEEDMOREPARAMS(std::string("PASS")));

	if (_users[fd].getIsRegistered())
		return sendData(fd, ERR_ALREADYREGISTERED);

	std::cout << "Debug: Password received: " << params[1] << std::endl;
	std::cout << "Debug: Server password: " << _pswd << std::endl;

	_users[fd].setPassword(params[1]);
	if (_users[fd].authenticate(_pswd)) {
		std::cout << "Debug: Password authentication successful" << std::endl;
	} else {
		std::cout << "Debug: Password authentication failed" << std::endl;
	}
}
