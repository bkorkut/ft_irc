#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include "User.hpp"

typedef unsigned char uint8_t;  // C++98 uyumlu tanım

enum ChannelModes
{
	INVONLY		= 'i',
	CHTOPIC		= 't',
	PASSKEY		= 'k',
	CHANOP		= 'o',
	LIMIT		= 'l',
};

enum	ChannelFlags
{
	B_INVONLY	= 1 << 0,	// (00000001)
	B_PASSKEY	= 1 << 1,	// (00000010)
	B_CHTOPIC	= 1 << 2,	// (00000100)
	B_LIMIT		= 1 << 3,	// (00001000)
};

class Channel {
private:
	std::string				name;
	std::string				topic;
	std::string				password;
	std::map<int, bool>		operators;	// fd -> is_operator
	std::map<int, User *>	users;		// fd -> User pointer
	uint8_t					modeFlags;

public:
	// Constructors
	Channel() {}
	Channel(const std::string& channelName) : name(channelName) {}

	// Flag management
	uint8_t	getFlags(void);
	bool	hasFlag(ChannelFlags flag);
	bool	hasFlag(char flag);
	void	setFlag(ChannelFlags flag);
	void	setFlag(char flag);
	void	unsetFlag(ChannelFlags flag);
	void	unsetFlag(char flag);

	// Channel management
	void addUser(User* user, bool isOperator = false);
	bool hasUser(int fd) const {
		return users.find(fd) != users.end();
	}
	void removeUser(int fd);
	bool isOperator(int fd) const;

	void setOperator(int fd, bool status) {
		if (users.find(fd) != users.end()) {
			operators[fd] = status;
		}
	}

	const std::string& getName() const { return name; }
	const std::map<int, User*>& getUsers() const { return users; }
	const std::string& getTopic() const { return topic; }
	void setTopic(const std::string& newTopic) { topic = newTopic; }
	void setName(const std::string& channelName) { name = channelName; }

	// Generate user list with prefixes
	std::string getUserList() const;
};

#endif
