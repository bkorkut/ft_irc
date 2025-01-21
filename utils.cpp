#include <vector>
#include <string>
#include <map>
#include "Channel.hpp"

// Converts string toLower
std::string	toLower(std::string str)
{
	for(std::string::iterator c = str.begin(); c != str.end(); c++)
		*c = std::tolower(*c);
	return str;
}

std::string	flagsToString(uint16_t flags)
{
	std::string ret;
	if (flags & B_ANONYMOUS)
		ret += ANONYMOUS;
	if (flags & B_INVONLY)
		ret += INVONLY;
	if (flags & B_MODERATED)
		ret += MODERATED;
	if (flags & B_NOOUTMSG)
		ret += NOOUTMSG;
	if (flags & B_QUIET)
		ret += QUIET;
	if (flags & B_PRIVATE)
		ret += PRIVATE;
	if (flags & B_SECRET)
		ret += SECRET;
	if (flags & B_REOP)
		ret += REOP;
	if (flags & B_TOPIC)
		ret += TOPIC;
	if (flags & B_LIMIT)
		ret += LIMIT;
	if (flags & B_PASSKEY)
		ret += PASSKEY;
	return ret;
}

std::string	flagsToString(uint8_t flags)
{
	std::string ret;
	if (flags & B_INVISIBLE)
		ret += INVISIBLE;
	if (flags & B_SERVNOTICE)
		ret += SERVNOTICE;
	if (flags & B_WALLOPS)
		ret += WALLOPS;
	if (flags & B_OPERATOR)
		ret += OPERATOR;
	if (flags & B_AWAY)
		ret += AWAY;
	if (flags & B_RESTRICTED)
		ret += RESTRICTED;
	return ret;
}

// Return a pointer to a User if there is a match, NULL if not
User *findUserWithNick(std::map<int, User> &users, std::string nick)
{
	for (std::map<int, User>::iterator i = users.begin(); i != users.end(); ++i)
		if (toLower(i->second.getNick()) == toLower(nick))
			return &i->second;
	return NULL;
}

// Split using vectors in C++
std::vector<std::string>	vecSplit(std::string toSplit, std::string septor)
{
	size_t						pos = 0;
	size_t						foundPos;
	std::string					tmp;
	std::vector<std::string>	retval;

	do
	{
		foundPos = toSplit.find(septor, pos);
		if (foundPos != std::string::npos)
			retval.push_back(toSplit.substr(pos, foundPos - pos));
		else
			retval.push_back(toSplit.substr(pos, toSplit.size()));
		pos = foundPos + septor.size();
	} while (foundPos != std::string::npos);
	return (retval);
}
