#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <string>
#include "User.hpp"

enum ChannelFlags
{
	CREATOR = 'O',
	OPERATOR = 'o',
	VOICE = 'v',
	ANONYMOUS = 'a',
	INVITE = 'i',
	MODERATED = 'm',
	NOOUTMSG = 'n',
	QUIET = 'q',
	PRIVATE = 'p',
	SECRET = 's',
	REOP = 'r',
	TOPIC = 't',
	PASSKEY = 'k',
	USRLIMIT = 'l',
	BANMASK = 'b',
	BANEXCEPT = 'e',
	INVMASK = 'I'
};

class Channel {
private:
	std::string name;
	std::map<int, bool>		operators; // fd -> is_operator
	std::map<int, User*>	users;	// fd -> User pointer
	std::string				topic;

public:
	// Default constructor
	Channel() {}

	// Constructor with channel name
	Channel(const std::string& channelName) : name(channelName) {}

	// Channel management
	void addUser(User* user, bool isOperator = false) {
		users[user->getId()] = user;
		operators[user->getId()] = isOperator;
	}

	void removeUser(int fd) {
		users.erase(fd);
		operators.erase(fd);
	}

	bool isOperator(int fd) const {
		std::map<int, bool>::const_iterator it = operators.find(fd);
		return it != operators.end() && it->second;
	}

	// Getters
	const std::string& getName() const { return name; }
	const std::map<int, User*>& getUsers() const { return users; }
	const std::string& getTopic() const { return topic; }
	void setName(const std::string& channelName) { name = channelName; }

	// Generate user list with prefixes
	std::string getUserList() const {
		std::string list;
		std::map<int, User*>::const_iterator it;
		for (it = users.begin(); it != users.end(); ++it) {
			if (operators.find(it->first) != operators.end() && operators.find(it->first)->second) {
				list += "@";
			}
			list += it->second->getNick() + " ";
		}
		return list;
	}
};

#endif
