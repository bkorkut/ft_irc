#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

typedef struct s_mode{
	bool		sign;
	char		mode;
	std::string	param;

	s_mode(bool s, char m, std::string p) : sign(s), mode(m), param(p) {}
}		t_mode;

static t_mode createMode(int *parcount, bool sign, char mode, std::vector<std::string>::iterator &param, bool end)
{
	if (mode == INVONLY || mode == CHTOPIC || (!sign && mode != CHANOP))
		return t_mode(sign, mode, "");
	if (end)
		throw errorException(ERR_NEEDMOREPARAMS(std::string("MODE")));
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
	for (; pit != params.end();)
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
				throw errorException(ERR_UNKNOWNMODE(std::string(1, *sit), channel->getName()));
			else
			{
				ret.push_back(createMode(&parcount, sign, *sit, pit, pit == params.end()));
				if (parcount > 3)
					throw errorException(ERR_UNKNOWNMODE(std::string(1, *sit), channel->getName()));
			}
		}
	}
	return ret;
}

static void executeMode(Channel *channel, t_mode mode)
{
	std::cout << "sign:" << mode.sign << " mode: " << mode.mode << " param: " << mode.param << std::endl;
	channel->getName();
	// switch (mode.mode){
	// 	case INVONLY:
	// 		// fall through
	// 	case CHTOPIC:
	// 		mode.sign ? channel->setFlag(mode.mode) : channel->unsetFlag(mode.mode);
	// 		break ;
	// 		// fall through
	// 	case LIMIT:
	// 		mode.sign ? channel->setFlag(mode.mode) : channel->unsetFlag(mode.mode);
	// 		// fall through
	// 	case PASSKEY:
	// 		mode.sign ? channel->setFlag(mode.mode) : channel->unsetFlag(mode.mode);
	// 		// fall through
	// 	case CHANOP:
	// 		break ;
	// 	default:
	// 		throw errorException(ERR_UNKNOWNMODE(std::string(SERVER_NAME), mode.mode, channel->getName()));
	// }
}

// Channel mode parameters:
// <channel> {[+|-]|i|t|k|o|l} [<limit>] [<user>]
void Server::MODE(int fd, std::vector<std::string> params)
{
	std::cout << "\033[32m[MODE Command]\033[0m" << std::endl;
	for (size_t i = 0; i < params.size(); i++)
		std::cout << params[i] << std::endl;
	if (params.size() < 2)
		return sendData(fd, ERR_NEEDMOREPARAMS(std::string("MODE")));
	else if (!std::strchr("#&+!", params[1][0]))
		return sendData(fd, ERR_NOSUCHCHANNEL(_serverName, params[1]));
	std::map<std::string, Channel>::iterator channel = _channels.find(params[1]);
	if (channel == _channels.end())
		return sendData(fd, ERR_NOSUCHCHANNEL(_serverName, params[1]));
	if (params.size() < 3)
	{
		std::cout << "Debug: three arguments" << std::endl;
		return sendData(fd, RPL_CHANNELMODEIS(_users[fd].getNick(), params[1], "+l", "100"));
	}
	if (false) // - the user is not a chanop
		return sendData(fd, ERR_CHANOPRIVSNEEDED(_serverName, params[1]));
	try
	{
		std::vector<t_mode> modes = parseModes(&channel->second, params);
		for (std::vector<t_mode>::iterator mit = modes.begin(); mit != modes.end(); ++mit)
			executeMode(&channel->second, *mit);
	}
	catch(const std::exception& e)
	{
		std::cout << "Catched: " << e.what() << std::endl;
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

// parser needs fix for spaces
