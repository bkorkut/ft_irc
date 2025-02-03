#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <set>
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
	int						limit;
	std::set<int>			operators;
	std::map<int, User *>	users;
	uint8_t					modeFlags;

public:
	// Constructors
	Channel() {}
	Channel(const std::string& channelName) : name(channelName) {}

	// mode management
	int			getLimit();
	void		setLimit(int l);
	std::string	getTopic();
	void		setTopic(std::string t);
	std::string	getPassword();
	void		setPassword(std::string p);
	bool		isOperator(int uid);
	void		toggleOperator(int uid);

	// Flag management
	uint8_t		getFlags(void);
	std::string	getModes(void);
	bool		hasFlag(ChannelFlags flag);
	bool		hasFlag(char flag);
	void		setFlag(ChannelFlags flag);
	void		setFlag(char flag);
	void		unsetFlag(ChannelFlags flag);
	void		unsetFlag(char flag);

	// Channel management
	void addUser(User* user, bool isOperator = false);
	bool hasUser(int fd) const;
	bool hasUser(std::string nick);
	void removeUser(int fd);

	const std::string& getName() const { return name; }
	const std::map<int, User*>& getUsers() const { return users; }
	const std::string& getTopic() const { return topic; }
	void setTopic(const std::string& newTopic) { topic = newTopic; }
	void setName(const std::string& channelName) { name = channelName; }

	// Generate user list with prefixes
	std::string getUserList();
};

#endif
