#include "../Server.hpp"

// Parameters: [ <Quit Message> ]
void Server::QUIT(int fd, std::vector<std::string> params) {
	std::cout << "\033[32m[QUIT Command]\033[0m" << std::endl;
	int idx = findClientIndex(fd);
	if (idx != -1)
	{
		removeClient(idx);
		// CHANGE HERE to send message to all users except the one leaving
		if (params.size() < 2)
			return (sendData(fd, _users[fd].getFullClientId() + " QUIT :Client Quit\r\n"));
		return (sendData(fd, _users[fd].getFullClientId() + " QUIT " + params[1] + "\r\n"));
	}
}
