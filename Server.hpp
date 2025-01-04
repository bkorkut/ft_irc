#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>
# include "Channel.hpp"
# include <iostream>
# include <vector>
# include <cstdlib>	// for exit

# define ERR_NOSUCHNICK 401
# define ERR_NOSUCHCHANNEL 403
# define ERR_CANNOTSENDTOCHAN 404
# define ERR_TOOMANYTARGETS 407
# define ERR_NORECIPIENT 411
# define ERR_NOTEXTTOSEND 412
# define ERR_NOTOPLEVEL 413
# define ERR_WILDTOPLEVEL 414
# define ERR_NONICKNAMEGIVEN 431
# define ERR_ERRONEUSNICKNAME 432
# define ERR_NICKNAMEINUSE 433
# define ERR_NICKCOLLISION 436
# define ERR_NOTONCHANNEL 442
# define ERR_NEEDMOREPARAMS 461
# define ERR_ALREADYREGISTRED 462
# define ERR_BADCHANMASK 476
# define ERR_CHANOPRIVSNEEDED 482

# define RPL_AWAY 301

# define NICKSET "abcdefghiklmnoprstuvwxyz1234567890[]{}|\\"

// # define ERR_TOOMANYCHANNELS 405
// # define ERR_WASNOSUCHNICK 406
// # define ERR_NOORIGIN 409
// # define ERR_NOSUCHSERVER 402
// # define ERR_UNKNOWNCOMMAND 421
// # define ERR_NOMOTD 422
// # define ERR_NOADMININFO 423
// # define ERR_FILEERROR 424
// # define ERR_USERNOTINCHANNEL 441
// # define ERR_USERONCHANNEL 443
// # define ERR_NOLOGIN 444
// # define ERR_SUMMONDISABLED 445
// # define ERR_USERSDISABLED 446
// # define ERR_NOTREGISTERED 451
// # define ERR_NOPERMFORHOST 463
// # define ERR_PASSWDMISMATCH 444
// # define ERR_YOUREBANNEDCREEP 444
// # define ERR_KEYSET 444
// # define ERR_CHANNELISFULL 444
// # define ERR_UNKNOWNMODE 444

class Server
{
	private:
		void	NICK(std::vector<std::string> params);
		void	USER(std::vector<std::string> params);
		void	PASS(std::vector<std::string> params);
		void	KICK(std::vector<std::string> params);
		void	PRIVMSG(std::vector<std::string> params);
		std::map<std::string, void (Server::*)(std::vector<std::string> parameters)> fptr;
		std::map<std::string, Channel>	channels;
		std::map<unsigned int, User>	users;
	public:
				Server();
		void	commandParser(std::string input);
};

#endif
