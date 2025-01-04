#ifndef USER_HPP
# define USER_HPP

#include <string>

class	User
{
	private:
		const int	id;
		std::string	nick;
	public:
							User(void);
							User(int fd);
		const std::string	&getNick(void);
		void				setNick(std::string nick);
};

// class	Metadata
// {
// 	private:
	//	User	*User;
	//	bool	voice;
	//	bool	chCreator;
	//	bool	chOperator;
// };

#endif
