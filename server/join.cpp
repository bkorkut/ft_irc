#include "../Server.hpp"

void Server::JOIN(int fd, std::vector<std::string> parameters) {
	std::cout << "\033[32m[JOIN Command]\033[0m" << std::endl;

	if (parameters.size() < 2)
		return sendData(fd, ERR_NEEDMOREPARAMS(std::string("JOIN")));

	std::string channelName = parameters[1];
	if (channelName[0] != '#')
		return sendData(fd, ERR_NOSUCHCHANNEL(_users[fd].getNick(), channelName));

	User* user = &_users[fd];
	if (!user->getIsRegistered()) {
		std::cout << "Debug: User not registered" << std::endl;
		return;
	}

	std::map<std::string, Channel>::iterator channelIt = _channels.find(channelName);
	bool isNewChannel = (channelIt == _channels.end());

	// Eğer kanal zaten varsa ve kullanıcı içindeyse
	if (!isNewChannel && channelIt->second.hasUser(fd)) {
		std::cout << "Debug: User already in channel" << std::endl;
		return;
	}

	// Limit kontrolü - eğer kanal varsa ve limit modu aktifse
	if (!isNewChannel && channelIt->second.hasFlag(B_LIMIT)) {
		size_t currentUsers = channelIt->second.getUsers().size();
		if (currentUsers >= static_cast<size_t>(channelIt->second.getLimit())) {
			std::cout << "Debug: Channel is full (Limit: " << channelIt->second.getLimit()
					  << ", Current: " << currentUsers << ")" << std::endl;
			return sendData(fd, ERR_CHANNELISFULL(user->getNick(), channelName));
		}
	}

	// Eğer kanal varsa ve invite-only modundaysa kontrol et
	if (!isNewChannel && channelIt->second.hasFlag(B_INVONLY)) {
		if (!channelIt->second.isInvited(fd)) {
			std::cout << "Debug: Channel is invite-only and user is not invited" << std::endl;
			return sendData(fd, ERR_INVITEONLYCHAN(user->getNick(), channelName));
		}
	}

	// Channel oluştur veya mevcut kanala katıl
	if (isNewChannel) {
		std::cout << "Debug: Creating new channel" << std::endl;
		Channel newChannel(channelName);
		newChannel.addUser(user, true);  // İlk kullanıcıyı operatör olarak ekle
		channelIt = _channels.insert(std::make_pair(channelName, newChannel)).first;
	} else {
		std::cout << "Debug: Adding user to existing channel" << std::endl;
		channelIt->second.addUser(user, false);
		channelIt->second.removeInvite(fd);
	}

	// JOIN mesajını kanaldaki tüm kullanıcılara gönder
	std::string prefix = user->getNick() + "!" + user->getUsername() + "@" + getClientIP();
	std::string joinMsg = ":" + prefix + " JOIN " + channelName + "\r\n";

	const std::map<int, User*>& channelUsers = channelIt->second.getUsers();
	for (std::map<int, User*>::const_iterator it = channelUsers.begin(); it != channelUsers.end(); ++it) {
		sendData(it->first, joinMsg);
	}

	// Topic mesajını gönder
	const std::string& topic = channelIt->second.getTopic();
	if (!topic.empty()) {
		sendData(fd, RPL_TOPIC(user->getNick(), channelName, topic));
	} else {
		sendData(fd, RPL_NOTOPIC(user->getNick(), channelName));
	}

	// NAMES listesini gönder
	std::string userList = channelIt->second.getUserList();
	std::cout << "Debug: Sending NAMES list: " << userList << std::endl;

	std::string namesReply = ":" + _serverName + " 353 " + user->getNick() + " = " + channelName + " :" + userList + "\r\n";
	std::string namesEnd = ":" + _serverName + " 366 " + user->getNick() + " " + channelName + " :End of /NAMES list.\r\n";

	sendData(fd, namesReply);
	sendData(fd, namesEnd);

	// Yeni kanalsa MODE mesajını gönder
	if (isNewChannel) {
		std::string modeMsg = ":" + _serverName + " MODE " + channelName + " +o " + user->getNick() + "\r\n";
		sendData(fd, modeMsg);
	}

	std::cout << "Debug: JOIN command completed successfully" << std::endl;
}
