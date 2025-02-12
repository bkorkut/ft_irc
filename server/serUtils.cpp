#include "../Server.hpp"

void Server::msgAllUsers(const std::string& channelName, const std::string& msg){
	std::map<std::string, Channel>::iterator channelIt = _channels.find(channelName);
	const std::map<int, User*>& channelUsers = channelIt->second.getUsers();
	for (std::map<int, User*>::const_iterator it = channelUsers.begin(); it != channelUsers.end(); ++it) {
		sendData(it->first, msg);
	}
}

int Server::findClientIndex(int fd) {
	std::cout << "findClientIndex with fd " << fd << std::endl;
	for (size_t i = 0; i < _fds.size(); i++) {
		if (_fds[i].fd == fd)
			return i;
	}
	return -1;
}