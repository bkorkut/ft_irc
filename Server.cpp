#include "Server.hpp"

void	Server::NICK();

void	Server::USER();

void	Server::PASS();

void	Server::KICK();

void	Server::commandParser(std::string input)
{
	std::string	command;
	std::string	parameters;

	// after parsing done
	if (fptr.find(command) != fptr.end())
		(this->*fptr[command])();
}
