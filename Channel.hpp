#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <string>
#include "User.hpp"

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
            std::map<int, bool>::const_iterator op_it = operators.find(it->first);
            if (op_it != operators.end() && op_it->second) {
                list += "@";
            }
            list += it->second->getNick() + " ";
        }
        return list;
    }
};

#endif