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
uint8_t User::getFlags(UserFlags flag){
	return this->modeFlags;
}

bool User::hasFlag(UserFlags flag){
	return this->modeFlags & flag;
}
void User::setFlag(UserFlags flag){
	this->modeFlags |= flag;
}

void User::unsetFlag(UserFlags flag){
	this->modeFlags &= ~flag;
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
