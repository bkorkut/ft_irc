#include "Channel.hpp"

// Flag management
uint8_t Channel::getFlags(void){
	return this->modeFlags;
}

bool Channel::hasFlag(ChannelFlags flag){
	return this->modeFlags & flag;
}

bool Channel::hasFlag(char flag){
	switch (flag){
		case INVONLY:
			return this->modeFlags & B_INVONLY;
		case CHTOPIC:
			return this->modeFlags & B_CHTOPIC;
		case LIMIT:
			return this->modeFlags & B_LIMIT;
		case PASSKEY:
			return this->modeFlags & B_PASSKEY;
		default:
			return false;
	}
}

void Channel::setFlag(ChannelFlags flag){
	this->modeFlags |= flag;
}

void Channel::setFlag(char flag){
	switch (flag){
		case INVONLY:
			this->modeFlags |= B_INVONLY;
			break ;
		case CHTOPIC:
			this->modeFlags |= B_CHTOPIC;
			break ;
		case LIMIT:
			this->modeFlags |= B_LIMIT;
			break ;
		case PASSKEY:
			this->modeFlags |= B_PASSKEY;
			break ;
		default:
			break ;
	}
}

void Channel::unsetFlag(ChannelFlags flag){
	this->modeFlags &= ~flag;
}

void Channel::unsetFlag(char flag){
	switch (flag){
		case INVONLY:
			this->modeFlags &= ~B_INVONLY;
			break ;
		case CHTOPIC:
			this->modeFlags &= ~B_CHTOPIC;
			break ;
		case LIMIT:
			this->modeFlags &= ~B_LIMIT;
			break ;
		case PASSKEY:
			this->modeFlags &= ~B_PASSKEY;
			break ;
		default:
			break ;
	}
}

// Channel management
void Channel::addUser(User* user, bool isOperator) {
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
