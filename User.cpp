#include "User.hpp"
#include "iostream"

User::User(void) : id(-1), isAuthenticated(false), isRegistered(false) {}

User::User(int fd) : id(fd), isAuthenticated(false), isRegistered(false) {}

const std::string& User::getNick() const {
	return nick;
}

const std::string& User::getUsername() const {
	return username;
}

const std::string& User::getRealname() const {
	return realname;
}

bool User::getIsAuthenticated() const {
	return isAuthenticated;
}

bool User::getIsRegistered() const {
	return isRegistered;
}

int User::getId() const {
	return id;
}

// Flag management
uint8_t User::getFlags(void){
	return this->modeFlags;
}

bool User::hasFlag(UserFlags flag){
	return this->modeFlags & flag;
}

bool User::hasFlag(char flag){
	switch (flag){
		case INVISIBLE:
			return this->modeFlags & B_INVISIBLE;
		case SERVNOTICE:
			return this->modeFlags & B_SERVNOTICE;
		case WALLOPS:
			return this->modeFlags & B_WALLOPS;
		case OPERATOR:
			return this->modeFlags & B_OPERATOR;
		case AWAY:
			return this->modeFlags & B_AWAY;
		case RESTRICTED:
			return this->modeFlags & B_RESTRICTED;
		default:
			return false;
	}
}

void User::setFlag(UserFlags flag){
	this->modeFlags |= flag;
}

void User::setFlag(char flag){
	switch (flag){
		case INVISIBLE:
			this->modeFlags |= B_INVISIBLE;
			break ;
		case SERVNOTICE:
			this->modeFlags |= B_SERVNOTICE;
			break ;
		case WALLOPS:
			this->modeFlags |= B_WALLOPS;
			break ;
		case OPERATOR:
			this->modeFlags |= B_OPERATOR;
			break ;
		case AWAY:
			this->modeFlags |= B_AWAY;
			break ;
		case RESTRICTED:
			this->modeFlags |= B_RESTRICTED;
			break ;
		default:
			break ;
	}
}

void User::unsetFlag(UserFlags flag){
	this->modeFlags &= ~flag;
}

void User::unsetFlag(char flag){
	switch (flag){
		case INVISIBLE:
			this->modeFlags &= ~B_INVISIBLE;
			break ;
		case SERVNOTICE:
			this->modeFlags &= ~B_SERVNOTICE;
			break ;
		case WALLOPS:
			this->modeFlags &= ~B_WALLOPS;
			break ;
		case OPERATOR:
			this->modeFlags &= ~B_OPERATOR;
			break ;
		case AWAY:
			this->modeFlags &= ~B_AWAY;
			break ;
		case RESTRICTED:
			this->modeFlags &= ~B_RESTRICTED;
			break ;
		default:
			break ;
	}
}

void User::setNick(std::string newNick) {
	nick = newNick;
}

void User::setUsername(std::string newUsername) {
	username = newUsername;
}

void User::setRealname(std::string newRealname) {
	realname = newRealname;
}

void User::setPassword(std::string newPassword) {
	password = newPassword;
}

bool User::authenticate(const std::string& serverPassword) {
	if (password == serverPassword) {
		isAuthenticated = true;
		return true;
	}
	return false;
}

bool User::checkRegistration() {
	std::cout << "Debug: Checking registration..." << std::endl;
	std::cout << "Nick: " << (nick.empty() ? "empty" : nick) << std::endl;
	std::cout << "Username: " << (username.empty() ? "empty" : username) << std::endl;
	std::cout << "Realname: " << (realname.empty() ? "empty" : realname) << std::endl;
	std::cout << "IsAuthenticated: " << (isAuthenticated ? "yes" : "no") << std::endl;

	if (!nick.empty() && !username.empty() && !realname.empty() && isAuthenticated) {
		isRegistered = true;
		std::cout << "Debug: Registration successful" << std::endl;
		return true;
	}
	std::cout << "Debug: Registration failed" << std::endl;
	return false;
}
