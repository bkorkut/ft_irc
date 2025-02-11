#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

void Server::USER(int fd, std::vector<std::string> params) {
	std::cout << "\033[32m[USER Command]\033[0m" << std::endl;

	// Kullanıcı komutunun format kontrolü
	if (params.size() < 5) {
		std::cout << "Debug: Not enough parameters. Expected: USER <username> <hostname> <servername> :<realname>" << std::endl;
		std::cout << "Debug: Received parameters:" << std::endl;
		for (size_t i = 0; i < params.size(); i++) {
			std::cout << "Param[" << i << "]: " << params[i] << std::endl;
		}
		return sendData(fd, ERR_NEEDMOREPARAMS(std::string("USER")));
	}

	if (_users[fd].getIsRegistered()) {
		std::cout << "Debug: User already registered" << std::endl;
		return sendData(fd, ERR_ALREADYREGISTERED);
	}

	std::cout << "Debug: Setting username and realname" << std::endl;
	std::cout << "Username: " << params[1] << std::endl;
	std::cout << "Realname: " << params[4] << std::endl;

	_users[fd].setUsername(params[1]);
	_users[fd].setRealname(params[4]);

	// Kayıt durumunu kontrol et
	if (_users[fd].checkRegistration()) {
		std::string prefix = _users[fd].getUsername() + "@" + getClientIP();
		sendData(fd, RPL_WELCOME(_users[fd].getNick(), prefix));
		std::cout << "Debug: Registration completed successfully" << std::endl;
	} else {
		std::cout << "Debug: Registration failed. Current state:" << std::endl;
		std::cout << "Nick: " << _users[fd].getNick() << std::endl;
		std::cout << "Username: " << _users[fd].getUsername() << std::endl;
		std::cout << "Realname: " << _users[fd].getRealname() << std::endl;
		std::cout << "IsAuthenticated: " << (_users[fd].getIsAuthenticated() ? "yes" : "no") << std::endl;
	}
}
