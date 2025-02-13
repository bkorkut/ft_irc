#include "../Server.hpp"

// Parameters: ( <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] ) / "0"
void Server::JOIN(int fd, std::vector<std::string> parameters) {
	std::cout << "\033[32m[JOIN Command]\033[0m" << std::endl;

	if (parameters.size() < 2)
		return sendData(fd, ERR_NEEDMOREPARAMS(std::string("JOIN")));

	User* user = &_users[fd];
	if (!user->getIsRegistered()) {
		std::cout << "Debug: User not registered" << std::endl;
		return ;
	}

	size_t	passidx = 0;
	std::vector<std::string> passes;
	if (!(parameters.size() < 3))
		passes = vecSplit(parameters[2], ",");
	std::cout << "Debug: Passes size" << passes.size() << std::endl;
	std::vector<std::string> channels = vecSplit(parameters[1], ",");
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		std::string channelName = *it;
		if (channelName[0] != '#') {
			sendData(fd, ERR_NOSUCHCHANNEL(_users[fd].getNick(), channelName));
			continue ;
		}

		std::map<std::string, Channel>::iterator channelIt = _channels.find(channelName);
		bool isNewChannel = (channelIt == _channels.end());

		// Eğer kanal zaten varsa
		if (!isNewChannel)
		{
			// Kullanıcı içindeyse
			if (channelIt->second.hasUser(fd)) {
				std::cout << "Debug: User already in channel" << std::endl;
				if (channelIt->second.hasFlag(B_PASSKEY))
					passidx++;
				continue ;
			}

			// Limit modu aktifse
			if (channelIt->second.hasFlag(B_LIMIT)) {
				size_t currentUsers = channelIt->second.getUsers().size();
				if (currentUsers >= static_cast<size_t>(channelIt->second.getLimit())) {
					std::cout << "Debug: Channel is full (Limit: " << channelIt->second.getLimit()
							  << ", Current: " << currentUsers << ")" << std::endl;
					sendData(fd, ERR_CHANNELISFULL(user->getNick(), channelName));
					continue ;
				}
			}

			// İnvite-only modundaysa kontrol et
			if (channelIt->second.hasFlag(B_INVONLY) && !channelIt->second.isInvited(fd)) {
				std::cout << "Debug: Channel is invite-only and user is not invited" << std::endl;
				sendData(fd, ERR_INVITEONLYCHAN(user->getNick(), channelName));
				continue ;
			}

			if (channelIt->second.hasFlag(B_PASSKEY)) {
				if ((passes.size() <= passidx) || (channelIt->second.getPassword() != passes[passidx] && ++passidx))
				{
					std::cout << "Debug: Channel password is wrong" << std::endl;
					sendData(fd, ERR_BADCHANNELKEY(user->getNick(), channelName));
					continue ;
				}
				else
					passidx++;
			}

			// Mevcut kanala katıl
			std::cout << "Debug: Adding user to existing channel" << std::endl;
			channelIt->second.addUser(user, false);
			channelIt->second.removeInvite(fd);
		}
		else
		{
			// Channel oluştur
			std::cout << "Debug: Creating new channel" << std::endl;
			Channel newChannel(channelName);
			newChannel.addUser(user, true);  // İlk kullanıcıyı operatör olarak ekle
			channelIt = _channels.insert(std::make_pair(channelName, newChannel)).first;
		}

		// JOIN mesajını kanaldaki tüm kullanıcılara gönder
		std::string joinMsg = ":" + user->getFullClientId() + " JOIN " + channelName + "\r\n";
		msgAllUsers(channelName, joinMsg);

		// Topic mesajını gönder
		const std::string& topic = channelIt->second.getTopic();
		if (!topic.empty())
			sendData(fd, RPL_TOPIC(user->getNick(), channelName, topic));

		// NAMES listesini gönder
		std::string userList = channelIt->second.getUserList();
		std::cout << "Debug: Sending NAMES list: " << userList << std::endl;
		sendData(fd, RPL_NAMREPLY(user->getNick(), channelName, userList));
		sendData(fd, RPL_ENDOFNAMES(user->getNick(), channelName));

		// Yeni kanalsa MODE mesajını gönder
		if (isNewChannel) {
			std::string modeMsg = ":" + _serverName + " MODE " + channelName + " +o " + user->getNick() + "\r\n";
			sendData(fd, modeMsg);
		}
	}

	std::cout << "Debug: JOIN command completed successfully" << std::endl;
}
