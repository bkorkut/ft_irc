#include "../Server.hpp"

// Parameters: [ <Quit Message> ]
void Server::QUIT(int fd, std::vector<std::string> params) {
	std::cout << "\033[32m[QUIT Command]\033[0m" << std::endl;

	std::string prefix;
	std::vector<std::string> channels;
	std::map<int, User>::iterator user = _users.find(fd);
	if (user != _users.end())
	{
		std::cout << "Debug: about to get user info" << std::endl;
		prefix = _users[fd].getFullClientId();
		channels = _users[fd].getJoinedChannels();
		std::cout << "Debug: channels are " << !channels.empty() << std::endl;
	}
	removeClient(fd);
	if (!channels.empty())
	{
		std::cout << "Debug: about to send quit message" << std::endl;
		if (params.size() < 2)
			params.push_back(" :Leaving");
		for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
			msgAllUsers(*it, ":" + prefix + " QUIT " + params[1] + "\r\n");
	}
}
