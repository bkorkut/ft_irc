#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

// Parameters: <channel> *( "," <channel> ) [ <Part Message> ]
// others recieve :via3!~via@159.146.29.219 PART #41 :I'm done
//                :<nickname>!<username>@<host>
void Server::PART(int fd, std::vector<std::string> params) {
	std::cout << "\033[32m[PART Command]\033[0m" << std::endl;
	if (params.size() < 2)
		return sendData(fd, ERR_NEEDMOREPARAMS(std::string("PART")));
	std::vector<std::string> channels = vecSplit(params[1], ",");
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		std::map<std::string, Channel>::iterator channel = _channels.find(*it);
		if (channel == _channels.end())
		{
			sendData(fd, ERR_NOSUCHCHANNEL(_users[fd].getNick(), *it));
			continue ;
		}
		if (!channel->second.hasUser(fd))
		{
			sendData(fd, ERR_NOTONCHANNEL(_users[fd].getNick(), *it));
			continue ;
		}
		if (params.size() < 3)
			params.push_back(" :Leaving");

		// notify all users that the current user left
		std::string usersGet = ":" + _users[fd].getFullClientId() + " PART " + *it + " " + params[2] + "\r\n";
		msgAllUsers(*it,usersGet);

		// remove user from channel
		channel->second.removeUser(fd);

		// delete channel if its empty
		if (channel->second.getUsers().empty())
		{
			std::cout << "Deleting channel " << channel->second.getName() << std::endl;
			_channels.erase(channel);
		}
	}
}
