#include <vector>
#include <string>
#include <map>
#include "Channel.hpp"

// Converts string toLower
std::string	toLower(std::string str)
{
	for (std::string::iterator c = str.begin(); c != str.end(); c++)
		*c = std::tolower(*c);
	return str;
}

// Converts string toUpper
std::string	toUpper(std::string str)
{
	for (std::string::iterator c = str.begin(); c != str.end(); c++)
		*c = std::toupper(*c);
	return str;
}

// Return a pointer to a User if there is a match, NULL if not
User *findUserWithNick(std::map<int, User> &users, std::string nick)
{
	for (std::map<int, User>::iterator i = users.begin(); i != users.end(); ++i)
		if (toLower(i->second.getNick()) == toLower(nick))
			return &i->second;
	return NULL;
}

// Return a pointer to a User if there is a match, NULL if not
User *findUserWithNick(std::map<int, User *> &users, std::string nick)
{
	for (std::map<int, User *>::iterator i = users.begin(); i != users.end(); ++i)
		if (toLower(i->second->getNick()) == toLower(nick))
			return i->second;
	return NULL;
}

// Split using vectors in C++
std::vector<std::string>	vecSplit(std::string toSplit, std::string septor)
{
	size_t						pos = 0;
	size_t						foundPos;
	std::vector<std::string>	retval;

	do
	{
		foundPos = toSplit.find(septor, pos);
		if (foundPos != std::string::npos)
		{
			if (foundPos > pos)
				retval.push_back(toSplit.substr(pos, foundPos - pos));
		}
		else if (pos < toSplit.size())
			retval.push_back(toSplit.substr(pos, toSplit.size()));
		pos = foundPos + septor.size();
	} while (foundPos != std::string::npos);
	return (retval);
}