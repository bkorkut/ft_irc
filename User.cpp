#include <algorithm>
#include "User.hpp"
#include "iostream"

// Getters
const std::string& User::getNick() const {
	return nick;
}

const std::string& User::getUsername() const {
	return username;
}

const std::string& User::getRealname() const {
	return realname;
}

const std::string User::getFullClientId() const{
	return nick + "!" + username + "@" + _clientIp;
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

// Setters
void User::setNick(std::string newNick) {
	nick = newNick;
}

void User::setUsername(std::string newUsername) {
	username = newUsername;
}

void User::setClientIP(const char* ip){
	_clientIp = ip ? std::string(ip) : "0.0.0.0";
}

const std::string User::getClientIP() const {
	return _clientIp;
}

void User::setRealname(std::string newRealname) {
	realname = newRealname;
}

void User::setPassword(std::string newPassword) {
	password = newPassword;
}

// Channel methods
void	User::joinChannel(std::string channel){
	joinedChannels.push_back(channel);
}

void	User::partChannel(std::string channel){
	std::vector<std::string>::iterator it = std::find(joinedChannels.begin(), joinedChannels.begin(), channel);
		if (it != joinedChannels.end())
			joinedChannels.erase(it);
}

std::vector<std::string> User::getJoinedChannels() const{
	return joinedChannels;
}

// Authentication methods
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
