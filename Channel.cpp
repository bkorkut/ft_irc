#include "Command.hpp"
#include "Channel.hpp"

// mode management
int	Channel::getLimit(){
	return (this->limit);
}

void	Channel::setLimit(int l){
	this->limit = l;
}

std::string	Channel::getTopic(){
	return (this->topic);
}
void	Channel::setTopic(std::string t){
	this->topic = t;
}

std::string	Channel::getPassword(){
	return (this->password);
}

void	Channel::setPassword(std::string p){
	this->password = p;
}

bool Channel::isOperator(int uid) {
	return this->operators.find(uid) != operators.end();
}

void Channel::toggleOperator(int uid) {
	if (isOperator(uid))
		this->operators.erase(uid);
	else
		this->operators.insert(uid);
}

// Flag management
uint8_t Channel::getFlags(void){
	return this->modeFlags;
}

std::string	Channel::getModes(void){
	std::string	ret("+");
	std::string	pass;
	std::string	limit;
	if (this->modeFlags & B_INVONLY)
		ret += INVONLY;
	if (this->modeFlags & B_CHTOPIC)
		ret += CHTOPIC;
	if (this->modeFlags & B_PASSKEY)
	{
		ret += PASSKEY;
		pass = this->getPassword();
	}
	if (this->modeFlags & B_LIMIT)
	{
		ret += LIMIT;
		std::stringstream ss;
		ss << this->limit;
		limit += ss.str();
	}
	if (ret != "+")
		return ret + (!pass.empty() ? " " + pass : "") + (!limit.empty() ? " " + limit : "");
	return (":0");
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
	if (isOperator)
		toggleOperator(user->getId());
	user->joinChannel(this->getName());
}

bool Channel::hasUser(int fd) const {
	return users.find(fd) != users.end();
}

bool Channel::hasUser(std::string nick){
	if (findUserWithNick(users, nick))
		return true;
	return false;
}

void Channel::removeUser(int fd) {
	users[fd]->partChannel(this->getName());
	users.erase(fd);
	operators.erase(fd);
}

// Generate user list with prefixes
std::string Channel::getUserList(){
	std::string list;
	std::map<int, User*>::iterator it;
	for (it = users.begin(); it != users.end(); ++it) {
		if (isOperator(it->first)) {
			list += "@";
		}
		list += it->second->getNick() + " ";
	}
	return list;
}