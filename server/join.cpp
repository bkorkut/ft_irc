#include "../Server.hpp"

void Server::JOIN(int fd, std::vector<std::string> parameters) {
    std::cout << "\033[32m[JOIN Command]\033[0m" << std::endl;
    
    // Debug için parametre kontrolü
    std::cout << "Parameters received:" << std::endl;
    for (size_t i = 0; i < parameters.size(); i++) {
        std::cout << "Param[" << i << "]: " << parameters[i] << std::endl;
    }
    
    if (parameters.size() < 2)
        return sendData(fd, ERR_NEEDMOREPARAMS(_users[fd].getNick(), "JOIN"));

    std::cout << "Debug: After params check" << std::endl;

    std::string channelName = parameters[1];
    if (channelName[0] != '#')
        channelName = "#" + channelName;

    std::cout << "Debug: Channel name: " << channelName << std::endl;

    // Kullanıcı kontrolü
    User* user = &_users[fd];
    if (!user->getIsRegistered()) {
        std::cout << "Debug: User not registered" << std::endl;
        return; // Kayıtlı olmayan kullanıcı kanala katılamaz
    }

    std::map<std::string, Channel>::iterator channelIt = _channels.find(channelName);
    bool isNewChannel = (channelIt == _channels.end());

    std::cout << "Debug: Is new channel: " << (isNewChannel ? "yes" : "no") << std::endl;

    // Kullanıcının zaten kanalda olup olmadığını kontrol et
    if (!isNewChannel && channelIt->second.hasUser(fd)) {
        std::cout << "Debug: User already in channel" << std::endl;
        return; // Kullanıcı zaten kanalda
    }

    if (isNewChannel) {
        Channel newChannel(channelName);
        channelIt = _channels.insert(std::make_pair(channelName, newChannel)).first;
        std::cout << "Debug: Created new channel" << std::endl;
    }

    // Kullanıcıyı kanala ekle
    channelIt->second.addUser(user, isNewChannel);
    std::cout << "Debug: Added user to channel" << std::endl;

    // JOIN mesajı
    std::string prefix = user->getNick() + "!" + user->getUsername() + "@" + _serverName;
    std::string joinMsg = ":" + prefix + " JOIN " + channelName + "\r\n";
    std::cout << "Debug: Join message: " << joinMsg;
    sendData(fd, joinMsg);

    // TOPIC durumu
    const std::string& topic = channelIt->second.getTopic();
    if (!topic.empty()) {
        std::string topicMsg = RPL_TOPIC(user->getNick(), channelName, topic);
        std::cout << "Debug: Topic message: " << topicMsg;
        sendData(fd, topicMsg);
    } else {
        std::string noTopicMsg = RPL_NOTOPIC(user->getNick(), channelName);
        std::cout << "Debug: No topic message: " << noTopicMsg;
        sendData(fd, noTopicMsg);
    }

    // NAMES listesi
    std::string userList = channelIt->second.getUserList();
    std::string namesMsg = RPL_NAMREPLY(user->getNick(), channelName, userList);
    std::string endNamesMsg = RPL_ENDOFNAMES(user->getNick(), channelName);
    
    std::cout << "Debug: Names message: " << namesMsg;
    std::cout << "Debug: End names message: " << endNamesMsg;
    
    sendData(fd, namesMsg);
    sendData(fd, endNamesMsg);

    // MODE mesajı
    if (isNewChannel) {
        std::string modeMsg = ":" + _serverName + " MODE " + channelName + " +o " + user->getNick() + "\r\n";
        std::cout << "Debug: Mode message: " << modeMsg;
        sendData(fd, modeMsg);
    }

    std::cout << "Debug: JOIN command completed" << std::endl;
}