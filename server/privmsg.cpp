#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

// Direct Client-to-Client (DDC) file transfer comes here too
// Parameters: <target>{,<target>} <text to be sent>
void	Server::PRIVMSG(int fd, std::vector<std::string> params)
{
	// NEEDS CHANGE AFTER VECTOR SPLIT
	// if (params.size() > 3)
	// 	;
	std::cout << "Command from user:" << this->users[fd].getNick() << std::endl;
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
