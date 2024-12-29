#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>
# include "Channel.hpp"
# include <iostream>

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
	public:
				Server();
		void	commandParser(std::string input);
};

#endif
