#include "User.hpp"

User::User(void) : id(-1)
{
	(void)id;
}

User::User(int fd) : id(fd), registered(false), pass(false), user(false), nick(false)
{
	(void)id;
}

const std::string	&User::getNick(void) const
{
	return (this->nickName);
}

void	User::setNick(std::string nickName)
{
	this->nickName = nickName;
}

const std::string	&User::getUname(void) const
{
	return (this->userName);
}

void	User::setUname(std::string userName)
{
	this->userName = userName;
}

const std::string	&User::getRname(void) const
{
	return (this->realName);
}

void	User::setRname(std::string realName)
{
	this->realName = realName;
}
