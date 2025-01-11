#ifndef USER_HPP
# define USER_HPP

#include <string>

class	User
{
	private:
		const int	id;
		std::string	nickName;
		std::string	userName;
		std::string	realName;
	public:
		std::string	buffer;
		bool		registered;
		bool		pass;
		bool		user;
		bool		nick;
							User(void);
							User(int fd);
		const std::string	&getNick(void) const;
		void				setNick(std::string nick);
		const std::string	&getUname(void) const;
		void				setUname(std::string uname);
		const std::string	&getRname(void) const;
		void				setRname(std::string rname);
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
