#include <sstream>
#include <vector>
#include <iostream>
#include "../Server.hpp"

typedef struct s_mode {
    bool        sign;       // true for +, false for -
    char        mode;       // mode character (i,t,k,o,l)
    std::string param;      // parameter if needed
    s_mode(bool s, char m, std::string p) : sign(s), mode(m), param(p) {}
} t_mode;

static std::vector<t_mode> parseModes(Channel *channel, std::vector<std::string> params) {
    std::vector<t_mode> ret;
    std::vector<std::string>::iterator pit = params.begin();
    std::advance(pit, 2); // Skip command and channel name

    for (; pit != params.end();) {
        std::string &modes = *pit;
        std::string::iterator sit = modes.begin();
        bool sign;
        bool flag = false;
        ++pit;

        for (; sit != modes.end(); ++sit) {
            if (*sit == '+' || *sit == '-') {
                if (flag)
                    throw errorException(ERR_NEEDMOREPARAMS(std::string("MODE")));
                sign = (*sit == '+');
                flag = true;
            }
            else if (!flag)
                throw errorException(ERR_NEEDMOREPARAMS(std::string("MODE")));
            else if (!std::strchr(CH_MODESET, *sit))
                throw errorException(ERR_UNKNOWNMODE(std::string(1, *sit), channel->getName()));
            else if (*sit == INVONLY || *sit == CHTOPIC || (!sign && (*sit == LIMIT || *sit == PASSKEY)))
                ret.push_back(t_mode(sign, *sit, ""));
            else if (pit == params.end())
                throw errorException(ERR_NEEDMOREPARAMS(std::string("MODE")));
            else {
                // Special handling for limit mode
                if (*sit == LIMIT && sign) {
                    // Check if limit parameter is valid
                    std::string limit_str = *pit;
                    for (size_t i = 0; i < limit_str.length(); i++) {
                        if (!isdigit(limit_str[i]))
                            throw errorException(ERR_UNKNOWNMODE(std::string(1, *sit), channel->getName()));
                    }
                    int limit = std::atoi(limit_str.c_str());
                    if (limit <= 0)
                        throw errorException(ERR_UNKNOWNMODE(std::string(1, *sit), channel->getName()));
                }
                
                if (*sit == CHANOP && !channel->hasUser(*pit))
                    throw errorException(ERR_USERNOTINCHANNEL(*pit, channel->getName()));
                
                ret.push_back(t_mode(sign, *sit, *pit));
                pit++;
            }
        }
    }
    return ret;
}

static void executeMode(Channel *channel, t_mode mode, std::map<int, User> &users) {
    switch (mode.mode) {
        case INVONLY:
        case CHTOPIC:
            mode.sign ? channel->setFlag(mode.mode) : channel->unsetFlag(mode.mode);
            break;
            
        case LIMIT:
            if (mode.sign) {
                int limit = std::atoi(mode.param.c_str());
                if (limit > 0) {
                    channel->setFlag(mode.mode);
                    channel->setLimit(limit);
                }
            }
            else {
                channel->unsetFlag(mode.mode);
            }
            break;
            
        case PASSKEY:
            if (mode.sign && channel->hasFlag(PASSKEY))
                throw errorException(ERR_KEYSET(channel->getName()));
            else if (mode.sign) {
                channel->setFlag(mode.mode);
                channel->setPassword(mode.param);
            }
            else
                channel->unsetFlag(mode.mode);
            break;
            
        case CHANOP:
            User *user = findUserWithNick(users, mode.param);
            if (!user)
                throw errorException(ERR_USERNOTINCHANNEL(mode.param, channel->getName()));
            if ((mode.sign && !channel->isOperator(user->getId()))
                || (!mode.sign && channel->isOperator(user->getId())))
                channel->toggleOperator(user->getId());
            break;
    }
}

void Server::MODE(int fd, std::vector<std::string> params) {
    std::cout << "\033[32m[MODE Command]\033[0m" << std::endl;
    for (size_t i = 0; i < params.size(); i++)
        std::cout << params[i] << std::endl;

    // Basic parameter check
    if (params.size() < 2)
        return sendData(fd, ERR_NEEDMOREPARAMS(std::string("MODE")));

    // Check if channel exists
    if (!std::strchr("#&+!", params[1][0]))
        return sendData(fd, ERR_NOSUCHCHANNEL(_serverName, params[1]));
    
    std::map<std::string, Channel>::iterator channel = _channels.find(params[1]);
    if (channel == _channels.end())
        return sendData(fd, ERR_NOSUCHCHANNEL(_serverName, params[1]));

    // If no mode parameter, return current modes
    if (params.size() < 3) {
        std::string modes = channel->second.getModes();
        if (modes.empty())
            return sendData(fd, RPL_CHANNELMODEIS(_users[fd].getNick(), params[1], ":0"));
        return sendData(fd, RPL_CHANNELMODEIS(_users[fd].getNick(), params[1], modes));
    }

    // Check if user is operator
    if (!channel->second.isOperator(fd))
        return sendData(fd, ERR_CHANOPRIVSNEEDED(_serverName, params[1]));

    try {
        std::vector<t_mode> modes = parseModes(&channel->second, params);
        for (std::vector<t_mode>::iterator mit = modes.begin(); mit != modes.end(); ++mit)
            executeMode(&channel->second, *mit, _users);
            
        // Send mode update message to all users in channel
        std::string modeStr = ":" + _users[fd].getNick() + " MODE " + params[1];
        for (size_t i = 2; i < params.size(); ++i)
            modeStr += " " + params[i];
        modeStr += "\r\n";
        
        const std::map<int, User*>& channelUsers = channel->second.getUsers();
        for (std::map<int, User*>::const_iterator it = channelUsers.begin(); it != channelUsers.end(); ++it) {
            sendData(it->first, modeStr);
        }
    }
    catch(const std::exception& e) {
        std::cout << "Caught error: " << e.what() << std::endl;
        return sendData(fd, e.what());
    }
}
