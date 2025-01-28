#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <exception>
# include <fstream>
# include <unistd.h>
# include <sstream>
# include <ctime>

// Error Codes
# define ERR_NOSUCHNICK(source, nick)                   "401 " + source + " " + nick + " :No such nick\r\n"
# define ERR_NOSUCHSERVER(source, server)               "402 " + source + " " + server + " :No such server\r\n"
# define ERR_NOSUCHCHANNEL(source, channel)             "403 " + source + " " + channel + " :No such channel\r\n"
# define ERR_INVALIDCAPCMD(source)                      "410 " + source + " :Invalid CAP subcommand\r\n"
# define ERR_NORECIPIENT(source)                        "411 " + source + " :No recipient\r\n"
# define ERR_NOTEXTTOSEND(source)                       "412 " + source + " :No text to send\r\n"
# define ERR_NONICKNAMEGIVEN                           "431 :No nickname given\r\n"
# define ERR_ERRONEUSNICKNAME(nick)                     "432 " + nick + " :Erroneous nickname\r\n"
# define ERR_NICKNAMEINUSE(nick)                        "433 " + nick + " :Nickname is already in use\r\n"
# define ERR_USERNOTINCHANNEL(source, nick, channel)    "441 " + source + " " + nick + " " + channel + " :They aren't on that channel\r\n"
# define ERR_NOTONCHANNEL(source, channel)              "442 " + source + " " + channel + " :You're not on that channel\r\n"
# define ERR_USERONCHANNEL(source, channel)             "443 " + source + " " + channel + " :is already on channel\r\n"
# define ERR_NEEDMOREPARAMS(source, command)            "461 " + source + " " + command + " :Not enough parameters\r\n"
# define ERR_ALREADYREGISTERED(source)                  "462 " + source + " :Unauthorized command (already registered)\r\n"
# define ERR_CHANNELISFULL(source, channel)             "471 " + source + " " + channel + " :Cannot join channel (+l)\r\n"
# define ERR_UNKNOWNMODE(nick, mode)                    "472 " + nick + " " + mode + " :is unknown mode char to me\r\n"
# define ERR_INVITEONLYCHAN(nick, channel)              "473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n"
# define ERR_BADCHANNELKEY(source, channel)             "475 " + source + " " + channel + " :Cannot join channel (+k)\r\n"
# define ERR_CHANOPRIVSNEEDED(source, channel)          "482 " + source + " " + channel + " :You're not channel operator\r\n"

// Reply Codes
# define RPL_LISTSTART(nick, num_users)                 "321 " + nick + " Channel : "+ num_users + "\r\n"
# define RPL_LIST(source, channel, userCount, topic)    "322 " + source + " " + channel + " " + userCount + " " + topic + "\r\n"
# define RPL_LISTEND(source)                           "323 " + source + " :End of /LIST\r\n"
# define RPL_NOTOPIC(source, channel)                   "331 " + source + " " + channel + " :No topic is set\r\n"
# define RPL_TOPIC(source, channel, topic)              "332 " + source + " " + channel + " :" + topic + "\r\n"
# define RPL_NAMREPLY(source, channel, nickList)        "353 " + source + " = " + channel + " :" + nickList + "\r\n"
# define RPL_ENDOFNAMES(source, channel)                "366 " + source + " " + channel + " :End of /NAMES list.\r\n"
# define RPL_JOIN(source, channel)                      ":" + source + " JOIN " + channel + "\r\n"
# define RPL_WELCOME(server, nick, prefix)              ":" + server + " 001 " + nick + " :Welcome to the Internet Relay Network " + prefix + "\r\n"
# define RPL_KICK(source, channel, target, reason)      ":" + source + " KICK " + channel + " " + target + " :" + reason + "\r\n"
# define RPL_PART(prefix, channel, message)             ":" + prefix + " PART " + channel + " :" + message + "\r\n"
# define RPL_QUIT(source, message)                      ":" + source + " QUIT :Quit: " + message + "\r\n"

std::string toLower(std::string str);
std::vector<std::string> vecSplit(std::string toSplit, std::string septor);

#endif