#ifndef CHANNEL_HPP
# define CHANNEL_HPP

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
# include "User.hpp"

class	Channel
{
	// std::string	name; // needs to be limited to 50 chars
	// 				// and needs prefixes
	// // properties
	// bool	priv;
	// // members
	// std::map<std::string, Metadata> _users;
};

#endif
