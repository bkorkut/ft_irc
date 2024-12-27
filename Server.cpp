#include <sstream>
#include "Server.hpp"

void	Server::NICK(std::string params)
{
	std::cout << "in function nick" << std::endl;
	std::cout << params << std::endl;
}

void	Server::USER(std::string params)
{
	std::cout << "in function user" << std::endl;
	std::cout << params << std::endl;
}

void	Server::PASS(std::string params)
{
	std::cout << "in function pass" << std::endl;
	std::cout << params << std::endl;
}

void	Server::KICK(std::string params)
{
	std::cout << "in function kick" << std::endl;
	std::cout << params << std::endl;
}

Server::Server()
{
	fptr["NICK"] = &Server::NICK;
	fptr["USER"] = &Server::USER;
	fptr["PASS"] = &Server::PASS;
	fptr["KICK"] = &Server::KICK;
}

void	Server::commandParser(std::string input)
{
	std::istringstream	stream(input);
	std::string			command;
	std::string			params;

	stream >> command;
	std::getline(stream >> std::ws, params);
	if (fptr.find(command) != fptr.end())
		(this->*fptr[command])(params);
}
