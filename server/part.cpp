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
	if (params[1].find(',') != std::string::npos)
	{
		std::vector<std::string> channels = vecSplit(params[1], ",");
		for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			std::map<std::string, Channel>::iterator channel = _channels.find(*it);
			if (channel == _channels.end())
				return sendData(fd, ERR_NOSUCHCHANNEL(_users[fd].getNick(), *it));
			if (!channel->second.hasUser(fd))
				return sendData(fd, ERR_NOTONCHANNEL(_users[fd].getNick(), *it));
			channel->second.removeUser(fd);
			if (params.size() < 3) {
				params.push_back(" :Leaving\rn");
			}
			std::string usersGet = ":" + _users[fd].getNick() + "!" + _users[fd].getUsername() + "@" + _users[fd].getClientIP() + " " + params[0] + " " + *it + " " + params[2] + "\r\n";
			msgAllUsers(*it,usersGet);

			std::string partMsg = ":" + _users[fd].getNick() + " PART " + *it + " " + params[2] + "\r\n";
			sendData(fd, partMsg);
		}
	}
	else
	{
		std::map<std::string, Channel>::iterator channel = _channels.find(params[1]);
		if (channel == _channels.end())
			return sendData(fd, ERR_NOSUCHCHANNEL(_users[fd].getNick(), params[1]));
		if (!channel->second.hasUser(fd))
			return sendData(fd, ERR_NOTONCHANNEL(_users[fd].getNick(), params[1]));
		channel->second.removeUser(fd);
		if (params.size() < 3) {
			params.push_back(" :Leaving\rn");
		}

		// notifiying all users that the current user left
		std::string usersGet = ":" + _users[fd].getNick() + "!" + _users[fd].getUsername() + "@" + _users[fd].getClientIP() + " " + params[0] + " " + params[1] + " " + params[2] + "\r\n";
		msgAllUsers(params[1],usersGet);

		std::string partMsg = ":" + _users[fd].getNick() + " PART " + params[1] + " " + params[2] + "\r\n";
		return (sendData(fd, partMsg));
	}
}
