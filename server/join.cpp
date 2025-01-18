#include "../Server.hpp"

void Server::JOIN(int fd, std::vector<std::string> params) {
    std::cout << "\033[32m[JOIN Command]\033[0m" << std::endl;
    
    if (params.size() < 2)
        return sendData(fd, ERR_NEEDMOREPARAMS(_users[fd].getNick(), "JOIN"));

    std::string channelName = params[1];
    if (channelName[0] != '#')
        channelName = "#" + channelName;

    // Check if channel exists
    std::map<std::string, Channel>::iterator channelIt = _channels.find(channelName);
    bool isNewChannel = (channelIt == _channels.end());

    // If channel doesn't exist, create it
    if (isNewChannel) {
        std::cout << "Creating new channel: " << channelName << std::endl;
        std::pair<std::map<std::string, Channel>::iterator, bool> result;
        result = _channels.insert(std::make_pair(channelName, Channel(channelName)));
        channelIt = result.first;
    }

    // Add user to channel
    User* user = &_users[fd];
    channelIt->second.addUser(user, isNewChannel); // Make them operator if they created the channel

    // Send join confirmation to all users in channel
    std::string joinMsg = ":" + user->getNick() + " JOIN " + channelName + "\r\n";
    
    const std::map<int, User*>& channelUsers = channelIt->second.getUsers();
    std::map<int, User*>::const_iterator userIt;
    for (userIt = channelUsers.begin(); userIt != channelUsers.end(); ++userIt) {
        sendData(userIt->first, joinMsg);
    }

    // Send channel info to the user who joined
    std::string userList = channelIt->second.getUserList();
    sendData(fd, RPL_NAMREPLY(user->getNick(), channelName, userList));
    sendData(fd, RPL_ENDOFNAMES(user->getNick(), channelName));

    std::cout << "User " << user->getNick() << " joined channel " << channelName 
              << (isNewChannel ? " (as operator)" : "") << std::endl;
}