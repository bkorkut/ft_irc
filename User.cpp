#include "User.hpp"

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
    if (!nick.empty() && !username.empty() && !realname.empty() && isAuthenticated) {
        isRegistered = true;
        return true;
    }
    return false;
}