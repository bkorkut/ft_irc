#include "../Server.hpp"

void Server::PRIVMSG(int fd, std::vector<std::string> params) {
	if (params.size() < 3)
		return sendData(fd, ERR_NEEDMOREPARAMS(std::string("PRIVMSG")));

	std::string target = params[1];
	std::string message = params[2];
	std::string prefix = _users[fd].getNick() + "!" + _users[fd].getUsername() + "@" + _serverName;

	// Kanal mesajı
	if (target[0] == '#') {
		std::map<std::string, Channel>::iterator channel = _channels.find(target);
		if (channel == _channels.end())
			return sendData(fd, ERR_NOSUCHCHANNEL(_users[fd].getNick(), target));

		// Kullanıcı kanalda mı kontrol et
		if (!channel->second.hasUser(fd))
			return sendData(fd, ERR_NOTONCHANNEL(_users[fd].getNick(), target));

		// Kanaldaki tüm kullanıcılara mesajı gönder (gönderen hariç)
		const std::map<int, User*>& users = channel->second.getUsers();
		std::string fullMessage = ":" + prefix + " PRIVMSG " + target + " :" + message + "\r\n";

		for (std::map<int, User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
			if (it->first != fd) { // Gönderen hariç herkese
				sendData(it->first, fullMessage);
			}
		}
	}
}
