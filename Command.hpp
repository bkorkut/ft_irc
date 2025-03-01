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
# include <vector>
# include <map>
# include "User.hpp"

typedef unsigned char uint8_t;

// Numeric error Codes
# define ERR_NOSUCHNICK(source, nick)					":ft_irc 401 " + source + " " + nick + " :No such nick\r\n"								//Belirtilen takma adın mevcut olmadığını bildirir.
# define ERR_NOSUCHCHANNEL(source, channel)				":ft_irc 403 " + source + " " + channel + " :No such channel\r\n"						//Belirtilen kanalın mevcut olmadığını bildirir.
# define ERR_NORECIPIENT(source)						":ft_irc 411 " + source + " :No recipient\r\n"											//Alıcı belirtilmediğini bildirir.
# define ERR_NOTEXTTOSEND(source)						":ft_irc 412 " + source + " :No text to send\r\n"										//Gönderilecek bir metin olmadığını belirtir.
# define ERR_NONICKNAMEGIVEN							":ft_irc 431 :No nickname given\r\n"													//Bir takma ad belirtilmediğini bildirir.
# define ERR_ERRONEUSNICKNAME(nick)						":ft_irc 432 " + nick + " :Erroneous nickname\r\n"										//Belirtilen takma adın hatalı olduğunu bildirir.
# define ERR_NICKNAMEINUSE(nick)						":ft_irc 433 " + nick + " :Nickname is already in use\r\n"								//Belirtilen takma adın kullanımda olduğunu bildirir.
# define ERR_USERNOTINCHANNEL(nick, channel)			":ft_irc 441 " + nick + " " + channel + " :They aren't on that channel\r\n"				//Belirtilen kullanıcının kanalda olmadığını belirtir.
# define ERR_NOTONCHANNEL(source, channel)				":ft_irc 442 " + source + " " + channel + " :You're not on that channel\r\n"			//Kullanıcının belirtilen kanalda olmadığını belirtir.
# define ERR_USERONCHANNEL(source, channel)				":ft_irc 443 " + source + " " + channel + " :is already on channel\r\n"					//Kullanıcının belirtilen kanalda zaten olduğunu belirtir.
# define ERR_NEEDMOREPARAMS(command)					":ft_irc 461 " + command + " :Not enough parameters\r\n"								//Komutun çalışması için daha fazla parametre gerektiğini belirtir.
# define ERR_ALREADYREGISTERED							":ft_irc 462 :Unauthorized command (already registered)\r\n"							//Kullanıcının zaten kayıtlı olduğunu belirtir.
# define ERR_KEYSET(channel)							":ft_irc 467 " + channel + " :Channel key already set\r\n"
# define ERR_CHANNELISFULL(source, channel)				":ft_irc 471 " + source + " " + channel + " :Cannot join channel (+l)\r\n"				//Kanalın dolu olduğunu ve daha fazla kullanıcı kabul edemediğini belirtir.
# define ERR_UNKNOWNMODE(char, channel)					":ft_irc 472 " + char + " :is unknown mode char to me for " + channel + "\r\n"			//Belirtilen modun bilinmeyen bir mod olduğunu belirtir.
# define ERR_INVITEONLYCHAN(nick, channel)				":ft_irc 473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n"				//Bir kanala davet olmadan katılmaya çalışıldığını belirtir.
# define ERR_BADCHANNELKEY(source, channel)				":ft_irc 475 " + source + " " + channel + " :Cannot join channel (+k)\r\n"				//Kanalın bir anahtarla (şifre) korunduğunu belirtir.
# define ERR_CHANOPRIVSNEEDED(source, channel)			":ft_irc 482 " + source + " " + channel + " :You're not the channel operator\r\n"		//Bir kanalda operatör ayrıcalıkları gerektiğini belirtir.

// Numeric reply Codes
# define RPL_WELCOME(nick, user)						":ft_irc 001 " + nick + " :Welcome to the Internet Relay Network " + user + "\r\n"		//Clientin servera başarıyla kayıt olduğunu haber verir.
// # define RPL_LISTSTART(nick, num_users)					":ft_irc 321 " + nick + " Channel : "+ num_users + "\r\n"										//LIST komutunun başlangıcını belirtir.
// # define RPL_LIST(source, channel, userCount, topic)	":ft_irc 322 " + source + " " + channel + " " + userCount + " " + topic + "\r\n"				//Bir kanalın kullanıcı sayısını ve konusunu belirtir.
// # define RPL_LISTEND(source)							":ft_irc 323 " + source + " :End of /LIST\r\n"													//LIST komutunun sonunu belirtir.
# define RPL_CHANNELMODEIS(target, channel, mode)		":ft_irc 324 " + target + " " + channel + " " + mode + "\r\n"
# define RPL_NOTOPIC(source, channel)					":ft_irc 331 " + source + " " + channel + " :No topic is set" + "\r\n"					//Bir kanalda konu belirlenmediğini bildirir.
# define RPL_TOPIC(source, channel, topic)				":ft_irc 332 " + source + " " + channel + " " + topic + "\r\n"							//Bir kanaldaki mevcut konuyu belirtir.
# define RPL_NAMREPLY(source, channel, nickList)		":ft_irc 353 " + source + " = " + channel + " :" + nickList + "\r\n"					//Bir kanaldaki kullanıcıların listesini içerir.
# define RPL_ENDOFNAMES(source, channel)				":ft_irc 366 " + source + " " + channel + " :End of /NAMES list.\r\n"					//Bir kanalın kullanıcı listesinin sonunu belirtir.

// Messages
// # define MSG_JOIN(source, channel)						":" + source + " JOIN :" + channel + "\r\n"												//Bir kullanıcının bir kanala katıldığını belirtir.
# define MSG_KICK(source, channel, target, reason)		":" + source + " KICK " + channel + " " + target + " :" + reason + "\r\n"				//Bir kullanıcının bir kanaldan atıldığını belirtir.
// # define MSG_PART(prefix, channel, message)				":" + prefix + " PART " + channel + " :" + message + "\r\n"								//Bir kullanıcının bir kanaldan ayrıldığını belirtir.
// # define MSG_QUIT(source, message)						":" + source + " QUIT :Quit: " + message + "\r\n"										//Bir kullanıcının sunucudan ayrıldığını bildirir.

// General exception for messages
class errorException : public std::exception{
	private:
		std::string message;
	public:
		errorException(std::string msg) : message(msg) {}
		~errorException() throw() {}
		const char *what() const throw() {
			return message.c_str();
		}
};

// Utilities
std::string	toLower(std::string str);
std::string	toUpper(std::string str);
User		*findUserWithNick(std::map<int, User> &users, std::string nick);
User		*findUserWithNick(std::map<int, User *> &users, std::string nick);
std::vector<std::string>	vecSplit(std::string toSplit, std::string septor);

#endif
