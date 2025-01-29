#include "Channel.hpp"

// Flag management
uint16_t Channel::getFlags(void){
	return this->modeFlags;
}

bool Channel::hasFlag(ChannelFlags flag){
	return this->modeFlags & flag;
}

bool Channel::hasFlag(char flag){
	switch (flag){
		case ANONYMOUS:
			return this->modeFlags & B_ANONYMOUS;
		case INVONLY:
			return this->modeFlags & B_INVONLY;
		case MODERATED:
			return this->modeFlags & B_MODERATED;
		case NOOUTMSG:
			return this->modeFlags & B_NOOUTMSG;
		case QUIET:
			return this->modeFlags & B_QUIET;
		case PRIVATE:
			return this->modeFlags & B_PRIVATE;
		case SECRET:
			return this->modeFlags & B_SECRET;
		case REOP:
			return this->modeFlags & B_REOP;
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
		case ANONYMOUS:
			this->modeFlags |= B_ANONYMOUS;
			break ;
		case INVONLY:
			this->modeFlags |= B_INVONLY;
			break ;
		case MODERATED:
			this->modeFlags |= B_MODERATED;
			break ;
		case NOOUTMSG:
			this->modeFlags |= B_NOOUTMSG;
			break ;
		case QUIET:
			this->modeFlags |= B_QUIET;
			break ;
		case PRIVATE:
			this->modeFlags |= B_PRIVATE;
			break ;
		case SECRET:
			this->modeFlags |= B_SECRET;
			break ;
		case REOP:
			this->modeFlags |= B_REOP;
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
		case ANONYMOUS:
			this->modeFlags &= ~B_ANONYMOUS;
			break ;
		case INVONLY:
			this->modeFlags &= ~B_INVONLY;
			break ;
		case MODERATED:
			this->modeFlags &= ~B_MODERATED;
			break ;
		case NOOUTMSG:
			this->modeFlags &= ~B_NOOUTMSG;
			break ;
		case QUIET:
			this->modeFlags &= ~B_QUIET;
			break ;
		case PRIVATE:
			this->modeFlags &= ~B_PRIVATE;
			break ;
		case SECRET:
			this->modeFlags &= ~B_SECRET;
			break ;
		case REOP:
			this->modeFlags &= ~B_REOP;
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
