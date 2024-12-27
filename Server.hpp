#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>
# include "Channel.hpp"
# include <iostream>

class Server
{
	private:
		void	NICK(std::string params);
		void	USER(std::string params);
		void	PASS(std::string params);
		void	KICK(std::string params);
		void	PRIVMSG(std::string params);
		std::map<std::string, void (Server::*)(std::string parameters)> fptr;
		std::map<std::string, Channel>	channels;
	public:
				Server();
		void	commandParser(std::string input);
};

#endif
