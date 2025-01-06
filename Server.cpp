#include <sstream>
#include <vector>
#include <iostream>
#include "Server.hpp"

// Constructor
Server::Server()
{
	fptr["CAP"] = &Server::CAP;
	fptr["NICK"] = &Server::NICK;
	fptr["USER"] = &Server::USER;
	fptr["PASS"] = &Server::PASS;
	fptr["KICK"] = &Server::KICK;
	fptr["MODE"] = &Server::MODE;
	fptr["TOPIC"] = &Server::TOPIC;
	fptr["INVITE"] = &Server::INVITE;
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

# include <cstdlib>	// for exit
// parses single perfect commands for now. splits em into parameters in a vector
void	Server::commandParser(int fd, std::string input)
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
				(this->*fptr[params[0]])(fd, params);
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
