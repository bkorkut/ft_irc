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

static std::vector<t_mode> parseModes(Channel *channel, std::vector<std::string> params)
{
	std::vector<t_mode> ret;
	std::vector<std::string>::iterator pit = params.begin();
	std::advance(pit, 2);
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
			else if (*sit == INVONLY || *sit == CHTOPIC)
				ret.push_back(t_mode(sign, *sit, ""));
			else if (pit == params.end())
				throw errorException(ERR_NEEDMOREPARAMS(std::string("MODE")));
			else
			{
				if (*sit == CHANOP && !channel->hasUser(*pit))
					throw errorException(ERR_USERNOTINCHANNEL(std::string("source"), "nick", channel->getName()));
				ret.push_back(t_mode(sign, *sit, *pit));
				pit++;
			}
		}
	}
	return ret;
}

static void executeMode(Channel *channel, t_mode mode, std::map<int, User> &users)
{
	switch (mode.mode){
		case INVONLY:
			// fall through
		case CHTOPIC:
			mode.sign ? channel->setFlag(mode.mode) : channel->unsetFlag(mode.mode);
			break ;
		case LIMIT:
			if (mode.sign)
			{
				// check if the number is valid maybe??
				channel->setFlag(mode.mode);
				channel->setLimit(std::atoi(mode.param.c_str()));
			}
			else
				channel->unsetFlag(mode.mode);
			break ;
		case PASSKEY:
			if (mode.sign)
			{
				channel->setFlag(mode.mode);
				channel->setPassword(mode.param);
			}
			else
				channel->unsetFlag(mode.mode);
			break ;
		case CHANOP:
			User *user = findUserWithNick(users, mode.param);
			if (!user)
				throw errorException(ERR_USERNOTINCHANNEL(mode.param, channel->getName()));
			if ((mode.sign && !channel->isOperator(user->getId())) || (!mode.sign && channel->isOperator(user->getId())))
				channel->toggleOperator(user->getId());
			break ;
	}
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
		return sendData(fd, RPL_CHANNELMODEIS(_users[fd].getNick(), params[1], "+l", "100")); // correct this to get correct modes
	if (false) // THE USER NOT A CHANOP
		return sendData(fd, ERR_CHANOPRIVSNEEDED(_serverName, params[1]));
	try
	{
		std::vector<t_mode> modes = parseModes(&channel->second, params);
		for (std::vector<t_mode>::iterator mit = modes.begin(); mit != modes.end(); ++mit)
			executeMode(&channel->second, *mit, _users);
	}
	catch(const std::exception& e)
	{
		std::cout << "Catched: " << e.what() << std::endl;
		return sendData(fd, e.what());
	}
}

		// e.g.: MODE #Finnish +imIbe *!*@*.fi user!*@* spamword -k secret
		// if (/*there is no user with the provided nick*/)
		// 	return sendData(fd, ERR_NOSUCHNICK(_serverName, "provided nick"));
		// if (/*Channel key is already set*/)
		// 	return sendData(fd, ERR_KEYSET(_serverName, "Channel"));
