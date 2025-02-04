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
	std::map<std::string, Channel>::iterator channel = _channels.find(params[1]);
	if (channel == _channels.end())
		return sendData(fd, ERR_NOSUCHCHANNEL(_serverName, params[1]));
	if (params.size() < 3)
	{
		if (!channel->second.getTopic().empty())
			return sendData(fd, RPL_TOPIC(_users[fd].getNick(), channel->second.getName(), channel->second.getTopic()));
		return sendData(fd, RPL_NOTOPIC(_users[fd].getNick(), channel->second.getName()));
	}
	else if (channel->second.hasFlag(B_CHTOPIC) && !channel->second.isOperator(fd))
		return sendData(fd, ERR_CHANOPRIVSNEEDED(_users[fd].getNick(), channel->second.getName()));
	else if (!channel->second.hasUser(fd))
		return sendData(fd, ERR_NOTONCHANNEL(_users[fd].getNick(), channel->second.getName()));
	if (params[2] == ":")
		channel->second.setTopic("");
	else
		channel->second.setTopic(params[2]);
}
