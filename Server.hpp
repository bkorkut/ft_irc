#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>
# include "Channel.hpp"
# include <iostream>

class Server
{
		std::map<std::string, void (Server::*)(std::string parameters)> fptr;
		void	commadParser(std::string	input);
	public:
		std::map<std::string, Channel>	channels;
};

#endif
