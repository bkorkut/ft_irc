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
		if (params.size() < 4 /*and is a no parameter taking flag command*/)
		{
			// channel flags
		}
		else if (/*check if its a nick taking command*/)
		{
			// Member status modes
		}
		else if (/*check if its a mask taking command*/)
		{
			// channel access control
		}
		// if (/*The given channel is unknown*/)
		// 	return sendData(fd, ERR_NOSUCHCHANNEL(_serverName, "Channel"));
		// if (/*User not on channel*/)
		// 	return sendData(fd, ERR_NOTONCHANNEL(_serverName, "Channel"));
		// if (_users.find(fd)->second.hasFlag(B_OPERATOR)) // the user hasnt operator privileges
		// 	return sendData(fd, ERR_CHANOPRIVSNEEDED(_serverName, "Channel"));
		// if (/*there is no user with the provided nick*/)
		// 	return sendData(fd, ERR_NOSUCHNICK(_serverName, "provided nick"));
		// if (/*The given mode is not known*/)
		// 	return sendData(fd, ERR_UNKNOWNMODE(_serverName, "Char", "Channel"));
		// if (/*Channel key is already set*/)
		// 	return sendData(fd, ERR_KEYSET(_serverName, "Channel"));
	}
	else // User MODE
	{
		User *user = findUserWithNick(_users, params[1]);
		if (user == NULL)
			return sendData(fd, ERR_NOSUCHNICK(_serverName, params[1]));
		if (_users[fd].getNick() != user->getNick())
			return sendData(fd, ERR_USERSDONTMATCH(_serverName));
		if (params.size() < 3)
			return sendData(fd, RPL_UMODEIS(_serverName, flagsToString(_users[fd].getFlags())));
		if (std::strchr(USR_MODESET, params[2][1])) // needs string to flag converter util
			return sendData(fd, ERR_UMODEUNKNOWNFLAG(_serverName));
		if (params[2][0] == '+')
			user->setFlag(switchToUserMode(params[2][1]));
		else if (params[2][0] == '+')
			user->unsetFlag(switchToUserMode(params[2][1]));
		// Take care of case no matching flag (default)!
		// Take care of case o+ (users need to use cmd OPER)!
	}
	// for (size_t i = 0; i < params.size(); i++)
	// 	std::cout << params[i] << std::endl;
}
