#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>
# include "Channel.hpp"
# include <iostream>
# include <vector>
# include <cstdlib>	// for exit

# define NICKSET "abcdefghiklmnoprstuvwxyz1234567890[]{}|\\"

class Server
{
	private:
		void	CAP(int fd, std::vector<std::string> params);
		void	NICK(int fd, std::vector<std::string> params);
		void	USER(int fd, std::vector<std::string> params);
		void	PASS(int fd, std::vector<std::string> params);
		void	KICK(int fd, std::vector<std::string> params);
		void	MODE(int fd, std::vector<std::string> params);
		void	TOPIC(int fd, std::vector<std::string> params);
		void	INVITE(int fd, std::vector<std::string> params);
		void	PRIVMSG(int fd, std::vector<std::string> params);
		std::map<std::string, void (Server::*)(int fd, std::vector<std::string> parameters)> fptr;
		std::map<std::string, Channel>	channels;
	public:
				Server();
		std::map<int, User>	users;
		void	commandParser(int fd, std::string input);
};

#endif

