#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

// Channel mode parameters:
// <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>] [<ban mask>]
// User mode parameters:
// <nickname> {[+|-]|i|w|s|o}
void Server::MODE(int fd, std::vector<std::string> params)
{
	std::cout << "\033[32m[MODE Command]\033[0m" << std::endl;
	if (params.size() < 2)
		return sendData(fd, ERR_NEEDMOREPARAMS(std::string("MODE")));
	if (std::strchr("#&+!", params[1][0]) != NULL)
	{
		if (params.size() < 3)
			return sendData(fd, ERR_NEEDMOREPARAMS(std::string("MODE")));
		std::map<std::string, Channel>::iterator channel = _channels.find(params[1]);
		if (channel == _channels.end())
			return sendData(fd, ERR_NOSUCHCHANNEL(_serverName, "Channel"));
		User *user = &_users.find(fd)->second;
		if (!user->hasFlag(B_OPERATOR)) // ADD: || the user is not a chanop!!
			return sendData(fd, ERR_CHANOPRIVSNEEDED(_serverName, "Channel"));
		std::vector<std::string>::iterator pit = params.begin();
		std::advance(pit, 2);
		for (; pit != params.end(); ++pit)
		{
			std::string &modes = *pit;
			std::string::iterator sit = modes.begin();
			bool	sign;
			bool	flag = false;
			for (; sit != modes.end(); ++sit)
			{
				if (*sit == '+' || *sit == '-')
				{
					if (flag)
						return sendData(fd, ERR_NEEDMOREPARAMS(std::string("MODE")));
					sign = (*sit == '+');
					flag = true;
				}
				else if (flag)
				{
					if (!std::strchr(CH_MODESET, *sit))
						return sendData(fd, ERR_UNKNOWNMODE(_serverName, "Char", "Channel"));
					else if (*sit == ANONYMOUS)
						sign ? channel->second.setFlag(*sit) : channel->second.unsetFlag(*sit);
					else if (*sit == INVONLY)
						sign ? channel->second.setFlag(*sit) : channel->second.unsetFlag(*sit);
					else if (*sit == MODERATED)
						sign ? channel->second.setFlag(*sit) : channel->second.unsetFlag(*sit);
					else if (*sit == NOOUTMSG)
						sign ? channel->second.setFlag(*sit) : channel->second.unsetFlag(*sit);
					// else if (*sit == 'q') // FOR SERVER USE ONLY??
					// 	sign ? channel->second.setFlag(*sit) : channel->second.unsetFlag(*sit);
					else if (*sit == PRIVATE)
						sign && !channel->second.hasFlag(SECRET) ? channel->second.setFlag(*sit) : channel->second.unsetFlag(*sit);
					else if (*sit == SECRET)
						sign && !channel->second.hasFlag(PRIVATE) ? channel->second.setFlag(*sit) : channel->second.unsetFlag(*sit);
					else if (*sit == REOP)
					{
						if (channel->second.getName()[0] != '!')
							return sendData(fd, ERR_UNKNOWNMODE(_serverName, "Char", "Channel"));
						sign ? channel->second.setFlag(*sit) : channel->second.unsetFlag(*sit);
					}
					else if (*sit == CHTOPIC)
						sign ? channel->second.setFlag(*sit) : channel->second.unsetFlag(*sit);
					// the remaining channel modes
				}
				else
					return sendData(fd, ERR_NEEDMOREPARAMS(std::string("MODE")));
			}
		}
	}
	else // User MODE
	{
		User *user = findUserWithNick(_users, params[1]);
		if (user == NULL)
			return sendData(fd, ERR_NOSUCHNICK(_serverName, params[1]));
		if (_users[fd].getNick() != user->getNick())
			return sendData(fd, ERR_USERSDONTMATCH(_serverName));
		if (params.size() < 3)
			return sendData(fd, RPL_UMODEIS(_serverName, userFlagsToString(_users[fd].getFlags())));
		if (!std::strchr(USR_MODESET, params[2][1])) // needs string to flag converter util
			return sendData(fd, ERR_UMODEUNKNOWNFLAG(_serverName));
		if (params[2][0] == '+')
			user->setFlag(switchToUserMode(params[2][1]));
		else if (params[2][0] == '-')
			user->unsetFlag(switchToUserMode(params[2][1]));
		// Take care of case no matching flag (default)!
		// Take care of case o+ (users need to use cmd OPER)!
		// Take care of multiple commands!!
	}
}

		// e.g.: MODE #Finnish +imIbe *!*@*.fi user!*@* spamword -k secret
		// }
		// if (/*User not on channel*/)
		// 	return sendData(fd, ERR_NOTONCHANNEL(_serverName, "Channel"));
		// if (/*there is no user with the provided nick*/)
		// 	return sendData(fd, ERR_NOSUCHNICK(_serverName, "provided nick"));
		// if (/*Channel key is already set*/)
		// 	return sendData(fd, ERR_KEYSET(_serverName, "Channel"));
