#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <string>
#include "User.hpp"

enum ChannelFlags {
    CHAN_CREATOR = 'O',
    CHAN_OPERATOR = 'o',
    CHAN_VOICE = 'v',
    CHAN_ANONYMOUS = 'a',
    CHAN_INVITE = 'i',
    CHAN_MODERATED = 'm',
    CHAN_NOOUTMSG = 'n',
    CHAN_QUIET = 'q',
    CHAN_PRIVATE = 'p',
    CHAN_SECRET = 's',
    CHAN_REOP = 'r',
    CHAN_TOPIC = 't',
    CHAN_PASSKEY = 'k',
    CHAN_USRLIMIT = 'l',
    CHAN_BANMASK = 'b',
    CHAN_BANEXCEPT = 'e',
    CHAN_INVMASK = 'I'
};

class Channel {
private:
    std::string name;
    std::map<int, bool> operators;  // fd -> is_operator
    std::map<int, User*> users;     // fd -> User pointer
    std::string topic;
    
public:
    Channel() {}
    Channel(const std::string& channelName) : name(channelName) {}

    void addUser(User* user, bool isOperator = false) {
        if (user) {
            users[user->getId()] = user;
            operators[user->getId()] = isOperator;
        }
    }

    bool hasUser(int fd) const {
        return users.find(fd) != users.end();
    }

    void removeUser(int fd) {
        users.erase(fd);
        operators.erase(fd);
    }

    bool isOperator(int fd) const {
        std::map<int, bool>::const_iterator it = operators.find(fd);
        return it != operators.end() && it->second;
    }

    void setOperator(int fd, bool status) {
        if (users.find(fd) != users.end()) {
            operators[fd] = status;
        }
    }

    const std::string& getName() const { return name; }
    const std::map<int, User*>& getUsers() const { return users; }
    const std::string& getTopic() const { return topic; }
    void setTopic(const std::string& newTopic) { topic = newTopic; }
    void setName(const std::string& channelName) { name = channelName; }

    std::string getUserList() const {
        std::string list;
        for (std::map<int, User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
            if (operators.find(it->first) != operators.end() && operators.find(it->first)->second) {
                list += "@";
            }
            list += it->second->getNick() + " ";
        }
        return list;
    }
};

#endif