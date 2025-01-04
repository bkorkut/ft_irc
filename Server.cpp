#include <sstream>
#include <vector>
#include <list>
#include "Server.hpp"

void	Server::NICK(std::vector<std::string> params)
{
	// check if nickname is valid
	// is alphanumeric,
	// square and curly brackets ([]{}),
	// backslashes (\), and pipe (|) characters in nicknames,
	// and MAY disallow digits as the first character.
	// Servers MAY allow extra characters, as long as they do not introduce ambiguity in other commands, including:

	// no leading # character or other character advertized in CHANTYPES
	// no leading colon (:)
	// no ASCII space

	// check if nickname is already in use in a user
	// if not, set or change the prev one
	// if so, issue an ERR_NICKNAMEINUSE numeric and ignore nick
	std::cout << "Command:" << std::endl;
	for (size_t i = 0; i < params.size(); i++)
		std::cout << params[i] << std::endl;
}

void	Server::USER(std::vector<std::string> params)
{
	std::cout << "Command:" << std::endl;
	for (size_t i = 0; i < params.size(); i++)
		std::cout << params[i] << std::endl;
}

void	Server::PASS(std::vector<std::string> params)
{
	std::cout << "Command:" << std::endl;
	for (size_t i = 0; i < params.size(); i++)
		std::cout << params[i] << std::endl;
}

void	Server::KICK(std::vector<std::string> params)
{
	std::cout << "Command:" << std::endl;
	for (size_t i = 0; i < params.size(); i++)
		std::cout << params[i] << std::endl;
}

// Direct Client-to-Client (DDC) file transfer comes here too
void	Server::PRIVMSG(std::vector<std::string> params)
{
	// NEEDS CHANGE AFTER VECTOR SPLIT
	// if (params.size() > 3)
	// 	;
	std::cout << "Command:" << std::endl;
	for (size_t i = 0; i < params.size(); i++)
		std::cout << params[i] << std::endl;
	// std::istringstream	stream(params);
	// std::string					message;
	// std::string					targets;
	// std::vector<std::string>	target;

	// stream >> targets;
	// std::getline(stream >> std::ws, message);
	// if (!message.empty() && message[0] == ':')
	// 	message.erase(0, 1);
	// stream.clear();
	// stream.str(targets);
	// while (stream.good())
	// {
	// 	std::getline(stream, targets, ',');
	// 	target.push_back(targets);
	// }

	// std::cout << "in function privmsg" << std::endl;
	// for(size_t i = 0; i < target.size(); i++)
	// 	std::cout << target[i] << std::endl;
	// // find and send message to clients here
	// std::cout << message << std::endl;
}

Server::Server()
{
	fptr["NICK"] = &Server::NICK;
	fptr["USER"] = &Server::USER;
	fptr["PASS"] = &Server::PASS;
	fptr["KICK"] = &Server::KICK;
	fptr["PRIVMSG"] = &Server::PRIVMSG;
}

std::vector<std::string>	vecSplit(std::string toSplit, std::string septor)
{
	size_t						pos = 0;
	size_t						foundPos;
	std::string					tmp;
	std::vector<std::string>	retval;

	do
	{
		foundPos = toSplit.find(septor, pos);
		if (foundPos != std::string::npos)
			retval.push_back(toSplit.substr(pos, foundPos - pos));
		else
			retval.push_back(toSplit.substr(pos, toSplit.size()));
		pos = foundPos + septor.size();
	} while (foundPos != std::string::npos);
	return (retval);
}

// parses single perfect commands for now. splits em into parameters in a vector
void	Server::commandParser(std::string input)
{
	std::vector<std::string>	commands;
	std::vector<std::string>	params;
	std::string					colon;
	std::string					others;

	std::cout << input << std::endl;
	commands = vecSplit(input, "\r\n");
	for (size_t i = 0; i < commands.size(); i++)
	{
		size_t pos = commands[i].find(" :");
		if (pos != std::string::npos)
		{
			others = commands[i].substr(0, pos);
			colon = commands[i].substr(pos + 2, commands[i].size());
			params = vecSplit(others, " ");
			params.push_back(colon);
		}
		else
			params = vecSplit(commands[i], " ");

		// try
		// {
		if (!params.empty() && fptr.find(params[0]) != fptr.end())
				(this->*fptr[params[0]])(params);
		// }
		// catch(const std::exception& e)
		// {
		// 	std::cerr << e.what() << '\n';
		// }
	}
	exit(1);
}

// PRIVMSG burak hey :burak naber\r\n/PRIVMSG burak hey :burak naber\r\n
// CAP LS\r\nPASS 123\r\nNICK burak\r\nUSER a a a a
// PRIVMSG burak hey :burak naber\r\n
