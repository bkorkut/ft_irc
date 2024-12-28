#include <sstream>
#include <vector>
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

// Direct Client-to-Client (DDC) file transfer comes here too
void	Server::PRIVMSG(std::string params)
{
	std::istringstream	stream(params);
	std::string					message;
	std::string					targets;
	std::vector<std::string>	target;

	stream >> targets;
	std::getline(stream >> std::ws, message);
	if (!message.empty() && message[0] == ':')
		message.erase(0, 1);
	stream.clear();
	stream.str(targets);
	while (stream.good())
	{
		std::getline(stream, targets, ',');
		target.push_back(targets);
	}
	std::cout << "in function privmsg" << std::endl;
	for(size_t i = 0; i < target.size(); i++)
		std::cout << target[i] << std::endl;
	// find and send message to clients here
	std::cout << message << std::endl;
}

Server::Server()
{
	fptr["NICK"] = &Server::NICK;
	fptr["USER"] = &Server::USER;
	fptr["PASS"] = &Server::PASS;
	fptr["KICK"] = &Server::KICK;
	fptr["PRIVMSG"] = &Server::PRIVMSG;
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
