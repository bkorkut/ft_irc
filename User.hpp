#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>

class User {
private:
	const int					id;
	std::string					nick;
	std::string					username;
	std::string					realname;
	std::string					_clientIp;
	std::string					password;
	bool						isAuthenticated;
	bool						isRegistered;
	std::vector<std::string>	joinedChannels;

public:
	std::string buffer;

	User(void) : id(-1), isAuthenticated(false), isRegistered(false) {}
	User(int fd) : id(fd), isAuthenticated(false), isRegistered(false) {}

	// Getters
	int					getId() const;
	const std::string	&getNick() const;
	const std::string	&getUsername() const;
	const std::string	&getRealname() const;
	const std::string	getClientIP() const;
	const std::string	getFullClientId() const;
	bool				getIsAuthenticated() const;
	bool				getIsRegistered() const;

	// Setters
	void setNick(std::string newNick);
	void setUsername(std::string newUsername);
	void setRealname(std::string newRealname);
	void setPassword(std::string newPassword);
	void setClientIP(const char* ip);

	// Channel methods
	void	joinChannel(std::string channel);
	void	partChannel(std::string channel);
	std::vector<std::string> getJoinedChannels() const;

	// Authentication methods
	bool authenticate(const std::string &serverPassword);
	bool checkRegistration();
};

#endif
