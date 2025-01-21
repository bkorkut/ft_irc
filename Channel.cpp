#include "Channel.hpp"

// Flag management
uint16_t Channel::getFlags(ChannelFlags flag){
	return this->modeFlags;
}

bool Channel::hasFlag(ChannelFlags flag){
	return this->modeFlags & flag;
}
void Channel::setFlag(ChannelFlags flag){
	this->modeFlags |= flag;
}

void Channel::unsetFlag(ChannelFlags flag){
	this->modeFlags &= ~flag;
}

// Channel management
void Channel::addUser(User* user, bool isOperator = false) {
	users[user->getId()] = user;
	operators[user->getId()] = isOperator;
}

void Channel::removeUser(int fd) {
	users.erase(fd);
	operators.erase(fd);
}

bool Channel::isOperator(int fd) const {
	std::map<int, bool>::const_iterator it = operators.find(fd);
	return it != operators.end() && it->second;
}

// Generate user list with prefixes
std::string Channel::getUserList() const {
	std::string list;
	std::map<int, User*>::const_iterator it;
	for (it = users.begin(); it != users.end(); ++it) {
		if (operators.find(it->first) != operators.end() && operators.find(it->first)->second) {
			list += "@";
		}
		list += it->second->getNick() + " ";
	}
	return list;
}
