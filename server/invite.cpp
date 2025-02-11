#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

// Parameters: <nickname> <channel>
void Server::INVITE(int fd, std::vector<std::string> params) {
    std::cout << "\033[32m[INVITE Command]\033[0m" << std::endl;

    if (params.size() < 3)
        return sendData(fd, ERR_NEEDMOREPARAMS(std::string("INVITE")));

    std::string targetNick = params[1];
    std::string channelName = params[2];

    // Remove @ symbol if present
    if (!targetNick.empty() && targetNick[0] == '@') {
        targetNick = targetNick.substr(1);
    }

    // Add # prefix if not present
    if (channelName[0] != '#')
        channelName = "#" + channelName;

    // Find the channel
    std::map<std::string, Channel>::iterator channelIt = _channels.find(channelName);
    if (channelIt == _channels.end())
        return sendData(fd, ERR_NOSUCHCHANNEL(_users[fd].getNick(), channelName));

    // Check if user is on the channel
    if (!channelIt->second.hasUser(fd))
        return sendData(fd, ERR_NOTONCHANNEL(_users[fd].getNick(), channelName));

    // Check if user is operator if channel is invite-only
    if (channelIt->second.hasFlag(B_INVONLY) && !channelIt->second.isOperator(fd))
        return sendData(fd, ERR_CHANOPRIVSNEEDED(_users[fd].getNick(), channelName));

    // Find target user in server
    User* targetUser = findUserWithNick(_users, targetNick);
    if (!targetUser)
        return sendData(fd, ERR_NOSUCHNICK(_users[fd].getNick(), targetNick));

    // Check if user is already on channel
    if (channelIt->second.hasUser(targetUser->getId()))
        return sendData(fd, ERR_USERONCHANNEL(_users[fd].getNick(), channelName));

    // Add user to invite list
    channelIt->second.addInvite(targetUser->getId());

    // Create and send the INVITE message
    std::string prefix = _users[fd].getNick() + "!" + _users[fd].getUsername() + "@" + _serverName;
    std::string inviteMsg = ":" + prefix + " INVITE " + targetNick + " " + channelName + "\r\n";

    // Send to target user
    sendData(targetUser->getId(), inviteMsg);

    std::cout << "Debug: INVITE command completed successfully" << std::endl;
}
