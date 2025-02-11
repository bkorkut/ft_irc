#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

// Parameters: <channel> [<topic>]
void	Server::TOPIC(int fd, std::vector<std::string> params)
{
	std::cout << "\033[32m[TOPIC Command]\033[0m" << std::endl;
	for (size_t i = 0; i < params.size(); i++)
		std::cout << params[i] << std::endl;

	if (params.size() < 2)
		return sendData(fd, ERR_NEEDMOREPARAMS(std::string("TOPIC")));

	// Find the channel
	std::map<std::string, Channel>::iterator channel = _channels.find(params[1]);
	if (channel == _channels.end())
		return sendData(fd, ERR_NOSUCHCHANNEL(_users[fd].getNick(), params[1]));

	// If no topic parameter is provided, return current topic
	if (params.size() < 3)
	{
		if (channel->second.getTopic().empty())
			return sendData(fd, RPL_NOTOPIC(_users[fd].getNick(), channel->second.getName()));
		return sendData(fd, RPL_TOPIC(_users[fd].getNick(), channel->second.getName(), channel->second.getTopic()));
	}

	// Check if user is on the channel
	else if (!channel->second.hasUser(fd))
		return sendData(fd, ERR_NOTONCHANNEL(_users[fd].getNick(), channel->second.getName()));

	// Check if channel has mode +t set and the user is an operator
	else if (channel->second.hasFlag(B_CHTOPIC) && !channel->second.isOperator(fd))
		return sendData(fd, ERR_CHANOPRIVSNEEDED(_users[fd].getNick(), channel->second.getName()));

	// Set the new topic
	if (params[2] == ":")
		channel->second.setTopic("");
	else
		channel->second.setTopic(params[2]);

	// Notify all users in the channel about the topic change
	std::string prefix = _users[fd].getNick() + "!" + _users[fd].getUsername() + "@" + _serverName;
	std::string topicMsg = ":" + prefix + " TOPIC " + channel->second.getName() + " :" + channel->second.getTopic() + "\r\n";

	const std::map<int, User*>& channelUsers = channel->second.getUsers();
	for (std::map<int, User*>::const_iterator it = channelUsers.begin(); it != channelUsers.end(); ++it) {
		sendData(it->first, topicMsg);
	}

	std::cout << "Debug: TOPIC command completed successfully" << std::endl;
}
