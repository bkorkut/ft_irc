#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <string>
#include "User.hpp"

enum ChannelModes
{
	ANONYMOUS	= 'a',
	INVONLY		= 'i',
	MODERATED	= 'm',
	NOOUTMSG	= 'n',
	QUIET		= 'q',
	PRIVATE		= 'p',
	SECRET		= 's',
	REOP		= 'r',
	CHTOPIC		= 't',
	LIMIT		= 'l',
	PASSKEY		= 'k',
	CREATOR		= 'O',
	CHANOP		= 'o',
	VOICE		= 'v',
	BANMASK		= 'b',
	BANEXCEPT	= 'e',
	INVMASK		= 'I'
};

enum	ChannelFlags
{
	B_ANONYMOUS	= 1 << 0,	// (00000000 00000001)
	B_INVONLY	= 1 << 1,	// (00000000 00000010)
	B_MODERATED	= 1 << 2,	// (00000000 00000100)
	B_NOOUTMSG	= 1 << 3,	// (00000000 00001000)
	B_QUIET		= 1 << 4,	// (00000000 00010000)
	B_PRIVATE	= 1 << 5,	// (00000000 00100000)
	B_SECRET	= 1 << 6,	// (00000000 01000000)
	B_REOP		= 1 << 7,	// (00000000 10000000)
	B_CHTOPIC	= 1 << 8,	// (00000001 00000000)
	B_LIMIT		= 1 << 9,	// (00000010 00000000)
	B_PASSKEY	= 1 << 10,	// (00000100 00000000)
};

class Channel {
private:
	std::string name;
	std::map<int, bool>		operators;	// fd -> is_operator
	std::map<int, User *>	users;		// fd -> User pointer
	uint16_t				modeFlags;
	std::string topic;

public:
	// Constructors
	Channel() {}
	Channel(const std::string& channelName) : name(channelName) {}

	// Flag management
	uint16_t			getFlags(void);
	bool				hasFlag(ChannelFlags flag);
	bool				hasFlag(char flag);
	void				setFlag(ChannelFlags flag);
	void				setFlag(char flag);
	void				unsetFlag(ChannelFlags flag);
	void				unsetFlag(char flag);

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
