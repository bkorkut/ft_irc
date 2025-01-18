#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

// Channel mode parameters:
// <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>] [<ban mask>]
// User mode parameters:
// <nickname> {[+|-]|i|w|s|o}
void	Server::MODE(int fd, std::vector<std::string> params)
{
    std::cout << "\033[32m[MODE Command]\033[0m" << std::endl;
    if (params.size() < 2)
        return sendData(fd, ERR_NEEDMOREPARAMS(std::string("MODE")));

	// for (size_t i = 0; i < params.size(); i++)
	// 	std::cout << params[i] << std::endl;
}
