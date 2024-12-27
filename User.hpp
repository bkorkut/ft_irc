#ifndef USER_HPP
# define USER_HPP

#include <string>

class	User
{
	int			id;
	std::string	username;
};

class	Metadata
{
	User	*User;
	bool	voice;
	bool	chCreator;
	bool	chOperator;
};

#endif
