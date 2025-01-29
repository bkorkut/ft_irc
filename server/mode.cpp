#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

typedef struct s_mode{
	char		mode;
	bool		sign;
	std::string	param;

	s_mode(char m, bool s, std::string p) : mode(m), sign(s), param(p) {}
}		t_mode;

static t_mode createMode(int *parcount, bool sign, char mode, std::vector<std::string>::iterator &param, bool end)
{
	// else if (mode == 'q') // FOR SERVER USE ONLY??
	// 	sign ? channel->setFlag(mode) : channel->unsetFlag(mode);
	if (mode == INVONLY || mode == CHTOPIC)
		return t_mode(sign, mode, NULL);
	if (end)
		throw ERR_NEEDMOREPARAMS(std::string("MODE"));
	t_mode ret(sign, mode, *param);
	param++;
	(*parcount)++;
	return ret;
}

static std::vector<t_mode> parseModes(Channel *channel, std::vector<std::string> params)
{
	std::vector<t_mode> ret;
	std::vector<std::string>::iterator pit = params.begin();
	std::advance(pit, 2);
	int	parcount = 0;
	for (; pit != params.end(); ++pit)
	{
		std::string &modes = *pit;
		std::string::iterator sit = modes.begin();
		bool	sign;
		bool	flag = false;
		++pit;
		for (; sit != modes.end(); ++sit)
		{
			if (*sit == '+' || *sit == '-')
			{
				if (flag)
					throw errorException(ERR_NEEDMOREPARAMS(std::string("MODE")));
				sign = (*sit == '+');
				flag = true;
			}
			else if (!flag)
				throw errorException(ERR_NEEDMOREPARAMS(std::string("MODE")));
			else if (!std::strchr(CH_MODESET, *sit))
				throw errorException(ERR_UNKNOWNMODE(std::string(SERVER_NAME), *sit, channel->getName()));
			else
			{
				ret.push_back(createMode(&parcount, sign, *sit, pit, pit == params.end()));
				if (parcount > 3)
					throw errorException(ERR_UNKNOWNMODE(std::string(SERVER_NAME), *sit, channel->getName()));
			}
		}
	}
	return ret;
}

static void executeMode(Channel *channel, t_mode mode)
{
	switch (mode.mode){
		case INVONLY:
			// fall through
		case CHTOPIC:
			mode.sign ? channel->setFlag(mode.mode) : channel->unsetFlag(mode.mode);
			break ;
			// fall through
		case LIMIT:
			mode.sign ? channel->setFlag(mode.mode) : channel->unsetFlag(mode.mode);
			// fall through
		case PASSKEY:
			mode.sign ? channel->setFlag(mode.mode) : channel->unsetFlag(mode.mode);
			// fall through
		case CHANOP:
			break ;
		default:
			throw errorException(ERR_UNKNOWNMODE(std::string(SERVER_NAME), mode.mode, channel->getName()));
	}
}

// Channel mode parameters:
// <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>] [<ban mask>]
// User mode parameters:
// <nickname> {[+|-]|i|w|s|o}
void Server::MODE(int fd, std::vector<std::string> params)
{
	std::cout << "\033[32m[MODE Command]\033[0m" << std::endl;
	if (params.size() < 3)
			return sendData(fd, ERR_NEEDMOREPARAMS(std::string("MODE")));
	else if (!std::strchr("#&+!", params[1][0]))
		return sendData(fd, ERR_NOSUCHCHANNEL(_serverName, "Channel"));
	std::map<std::string, Channel>::iterator channel = _channels.find(params[1]);
	if (channel == _channels.end())
		return sendData(fd, ERR_NOSUCHCHANNEL(_serverName, "Channel"));
	if (false) // - the user is not a chanop
		return sendData(fd, ERR_CHANOPRIVSNEEDED(_serverName, "Channel"));
	try
	{
		std::vector<t_mode> modes = parseModes(&channel->second, params);
		for (std::vector<t_mode>::iterator mit = modes.begin(); mit != modes.end(); ++mit)
			executeMode(&channel->second, *mit);
	}
	catch(const std::exception& e)
	{
		return sendData(fd, e.what());
	}
}

		// e.g.: MODE #Finnish +imIbe *!*@*.fi user!*@* spamword -k secret
		// if (/*User not on channel*/)
		// 	return sendData(fd, ERR_NOTONCHANNEL(_serverName, "Channel"));
		// if (/*there is no user with the provided nick*/)
		// 	return sendData(fd, ERR_NOSUCHNICK(_serverName, "provided nick"));
		// if (/*Channel key is already set*/)
		// 	return sendData(fd, ERR_KEYSET(_serverName, "Channel"));
