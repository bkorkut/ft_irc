#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

// Channel mode parameters:
// <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>] [<ban mask>]
// User mode parameters:
// <nickname> {[+|-]|i|w|s|o}
void	Server::MODE(int fd, std::vector<std::string> params)
{
	std::cout << "\033[32m[MODE Command]\033[0m" << std::endl;
	if (params.size() < 2)
		return sendData(fd, ERR_NEEDMOREPARAMS(std::string("MODE")));
	if (std::strchr("#&+!", params[1][0]) != NULL)
	{
		if (/*The given channel is unknown*/)
			return sendData(fd, ERR_NOSUCHCHANNEL(_serverName, "Channel"));
		if (/*User not on channel*/)
			return sendData(fd, ERR_NOTONCHANNEL(_serverName, "Channel"));
		if (_users.find(fd)->second.hasFlag(OPERATOR)) // the user hasnt operator privileges
			return sendData(fd, ERR_CHANOPRIVSNEEDED(_serverName, "Channel"));
		if (/*there is no user with the provided nick*/)
			return sendData(fd, ERR_NOSUCHNICK(_serverName, "provided nick"));
		if (/*The given mode is not known*/)
			return sendData(fd, ERR_UNKNOWNMODE(_serverName, "Char", "Channel"));
		if (/*Channel key is already set*/)
			return sendData(fd, ERR_KEYSET(_serverName, "Channel"));
	}
	else // User MODE
	{
		if (/*The given mode is not known*/)
			return sendData(fd, ERR_UMODEUNKNOWNFLAG(_serverName));
		if (/*A user tries to change the mode of a user other than themselves*/)
			return sendData(fd, ERR_USERSDONTMATCH(_serverName));
	}
	// for (size_t i = 0; i < params.size(); i++)
	// 	std::cout << params[i] << std::endl;
}
