#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>
# include <stdexcept>
# include <string>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <poll.h>
# include <vector>
# include <map>
# include <csignal>
# include "Channel.hpp"
# include "Command.hpp"

# define NICKSET "abcdefghiklmnoprstuvwxyz1234567890[]{}|\\"
# define CH_MODESET "itlko"
# define SERVER_NAME "ft_irc"

class Server
{
	private:
		// Attributes
		std::string					_serverName;
		int							_socketFD;
		int							_newSocketFD;
		int							_portNum;
		std::string					_pswd;
		std::vector<struct pollfd>	_fds;
		struct sockaddr_in			_serverAddress;
		struct pollfd				_newPollFD;
		static bool					Signal;
		std::map<int, User>			_users;
		std::map<std::string, Channel> _channels;
		std::map<std::string, void (Server::*)(int fd, std::vector<std::string> parameters)> fptr;

		// Member functions
		void	NICK(int fd, std::vector<std::string> params);
		void	USER(int fd, std::vector<std::string> params);
		void	PASS(int fd, std::vector<std::string> params);
		void	KICK(int fd, std::vector<std::string> params);
		void	MODE(int fd, std::vector<std::string> params);
		void	TOPIC(int fd, std::vector<std::string> params);
		void	INVITE(int fd, std::vector<std::string> params);
		void	PRIVMSG(int fd, std::vector<std::string> params);
		void	JOIN(int fd, std::vector<std::string> params);

		// Preventing canonical form by making these private
		Server(const Server& other);
		Server& operator=(const Server& other);

	public:
		Server();
		~Server();
		void	initialize(int portNum, const std::string &pswd);
		void	closeFds();
		void	run();
		void	acceptClient();
		void	recieveData(int fd);
		void	sendData(int fd, std::string data);
		void	commandParser(int fd, std::string input);
		static void SignalHandler(int signum);
};

#endif
