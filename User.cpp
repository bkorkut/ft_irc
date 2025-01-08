#include "User.hpp"

User::User(void) : id(-1)
{
	(void)id;
}

User::User(int fd) : id(fd)
{
	(void)id;
}

const std::string	&User::getNick(void) const
{
	return (this->nick);
}

void	User::setNick(std::string nick)
{
	this->nick = nick;
}
