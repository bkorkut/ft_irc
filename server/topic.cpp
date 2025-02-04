#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

void Server::TOPIC(int fd, std::vector<std::string> params) {
    std::cout << "\033[32m[TOPIC Command]\033[0m" << std::endl;

    if (params.size() < 2)
        return sendData(fd, ERR_NEEDMOREPARAMS(std::string("TOPIC")));

    std::string channelName = params[1];
    if (channelName[0] != '#')
        channelName = "#" + channelName;

    // Find the channel
    std::map<std::string, Channel>::iterator channelIt = _channels.find(channelName);
    if (channelIt == _channels.end())
        return sendData(fd, ERR_NOSUCHCHANNEL(_users[fd].getNick(), channelName));

    // Check if user is on the channel
    if (!channelIt->second.hasUser(fd))
        return sendData(fd, ERR_NOTONCHANNEL(_users[fd].getNick(), channelName));

    // If no topic parameter is provided, return current topic
    if (params.size() == 2) {
        std::string currentTopic = channelIt->second.getTopic();
        if (currentTopic.empty())
            return sendData(fd, RPL_NOTOPIC(_users[fd].getNick(), channelName));
        return sendData(fd, RPL_TOPIC(_users[fd].getNick(), channelName, currentTopic));
    }

    // Check if channel has topic restriction and user is not an operator
    if (channelIt->second.hasFlag(B_CHTOPIC) && !channelIt->second.isOperator(fd))
        return sendData(fd, ERR_CHANOPRIVSNEEDED(_users[fd].getNick(), channelName));

    // Set the new topic
    std::string newTopic = params[2];
    for (size_t i = 3; i < params.size(); ++i)
        newTopic += " " + params[i];

    channelIt->second.setTopic(newTopic);

    // Notify all users in the channel about the topic change
    std::string prefix = _users[fd].getNick() + "!" + _users[fd].getUsername() + "@" + _serverName;
    std::string topicMsg = ":" + prefix + " TOPIC " + channelName + " :" + newTopic + "\r\n";

    const std::map<int, User*>& channelUsers = channelIt->second.getUsers();
    for (std::map<int, User*>::const_iterator it = channelUsers.begin(); it != channelUsers.end(); ++it) {
        sendData(it->first, topicMsg);
    }

    std::cout << "Debug: TOPIC command completed successfully" << std::endl;
}