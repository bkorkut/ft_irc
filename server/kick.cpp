#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

void Server::KICK(int fd, std::vector<std::string> params) {
    std::cout << "\033[32m[KICK Command]\033[0m" << std::endl;

    if (params.size() < 3)
        return sendData(fd, ERR_NEEDMOREPARAMS(std::string("KICK")));

    std::string channelName = params[1];
    std::string targetNick;
    std::string reason;

    // Parametreleri doğru şekilde ayır
    size_t reasonStart = 3;
    if (!params[2].empty() && params[2][0] == '@') {
        targetNick = params[2].substr(1); // @ işaretini kaldır
    } else {
        targetNick = params[2];
    }

    // Reason'ı birleştir
    if (params.size() > reasonStart) {
        reason = params[reasonStart];
        for (size_t i = reasonStart + 1; i < params.size(); ++i) {
            reason += " " + params[i];
        }
    } else {
        reason = "No reason given";
    }

    // Debug bilgisi
    std::cout << "Channel: " << channelName << std::endl;
    std::cout << "Target: " << targetNick << std::endl;
    std::cout << "Reason: " << reason << std::endl;

    // Kanal adını kontrol et ve düzelt
    if (!channelName.empty() && channelName[0] != '#') {
        channelName = "#" + channelName;
    }

    // Kanal var mı kontrol et
    std::map<std::string, Channel>::iterator channelIt = _channels.find(channelName);
    if (channelIt == _channels.end()) {
        std::cout << "Debug: Channel not found: " << channelName << std::endl;
        return sendData(fd, ERR_NOSUCHCHANNEL(_users[fd].getNick(), channelName));
    }

    // Komutu kullanan kullanıcı kanalda mı ve operatör mü kontrol et
    if (!channelIt->second.hasUser(fd)) {
        std::cout << "Debug: User not in channel" << std::endl;
        return sendData(fd, ERR_NOTONCHANNEL(_users[fd].getNick(), channelName));
    }

    if (!channelIt->second.isOperator(fd)) {
        std::cout << "Debug: User is not operator" << std::endl;
        return sendData(fd, ERR_CHANOPRIVSNEEDED(_users[fd].getNick(), channelName));
    }

    // Hedef kullanıcıyı bul
    int targetFd = -1;
    const std::map<int, User*>& users = channelIt->second.getUsers();
    for (std::map<int, User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
        std::cout << "Debug: Checking user: " << it->second->getNick() << " against target: " << targetNick << std::endl;
        if (it->second && toLower(it->second->getNick()) == toLower(targetNick)) {
            targetFd = it->first;
            break;
        }
    }

    if (targetFd == -1) {
        std::cout << "Debug: Target user not found: " << targetNick << std::endl;
        return sendData(fd, ERR_USERNOTINCHANNEL(targetNick, channelName));
    }

    // KICK mesajını oluştur
    std::string kickMsg = RPL_KICK(_users[fd].getNick() + "!" + _users[fd].getUsername() + "@" + _serverName,
                                  channelName, targetNick, reason);

    std::cout << "Debug: Sending kick message: " << kickMsg;

    // Kanaldaki herkese KICK mesajını gönder
    for (std::map<int, User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
        sendData(it->first, kickMsg);
    }

    // Kullanıcıyı kanaldan çıkar
    channelIt->second.removeUser(targetFd);

    std::cout << "Debug: KICK command completed successfully" << std::endl;
}
