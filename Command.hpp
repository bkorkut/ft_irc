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
# define ERR_NOSUCHNICK(source, nick)                    "401 " + source + " " + nick + " :No such nick" + "\r\n"                                                                      //Belirtilen takma adın mevcut olmadığını bildirir.
# define ERR_NOSUCHSERVER(source, server)                "402 " + source + " " + server + " :No such server" + "\r\n"                                                                  //Belirtilen sunucunun mevcut olmadığını bildirir.
# define ERR_NOSUCHCHANNEL(source, channel)              "403 " + source + " " + channel + " :No such channel" + "\r\n"                                                                //Belirtilen kanalın mevcut olmadığını bildirir.
# define ERR_INVALIDCAPCMD(source)                       "410 " + source + " :Invalid CAP subcommand" + "\r\n"                                                                         //Geçersiz bir CAP alt komutu gönderildiğini belirtir.
# define ERR_NORECIPIENT(source)                         "411     " + source + " :No" + "\r\n"                                                                                         //Alıcı belirtilmediğini bildirir.
# define ERR_NOTEXTTOSEND(source)                        "412 " + source + " :No text to send" + "\r\n"                                                                                //Gönderilecek bir metin olmadığını belirtir.
# define ERR_NONICKNAMEGIVEN(source)                     "431 " + ((source).empty() ? "" : (source + " ")) + ":No nickname given\r\n"                                                  //Bir takma ad belirtilmediğini bildirir.
// # define ERR_NONICKNAMEGIVEN(source)                  "431 " + source + " :No nickname given" + "\r\n"                                                                              //Bir takma ad belirtilmediğini bildirir.
# define ERR_ERRONEUSNICKNAME(source, nick)              "432 " + ((source).empty() ? "" : (source + " ")) + nick + " :Erroneous nickname" + "\r\n"                                    //Belirtilen takma adın hatalı olduğunu bildirir.
# define ERR_NICKNAMEINUSE(source, nick)                 "433 " + source + " " + nick + " :Nickname is already in use" + "\r\n"                                                        //Belirtilen takma adın kullanımda olduğunu bildirir.
# define ERR_USERNOTINCHANNEL(source, nick, channel)     "441 " + source + " " + nick + " " + channel + " :They aren't on that channel" + "\r\n"                                       //Belirtilen kullanıcının kanalda olmadığını belirtir.
# define ERR_NOTONCHANNEL(source, channel)               "442 " + source + " " + channel + " :You're not on that channel" + "\r\n"                                                     //Kullanıcının belirtilen kanalda olmadığını belirtir.
# define ERR_USERONCHANNEL(source, channel)              "443 " + source + " " + channel + " :is already on channel" + "\r\n"                                                          //Kullanıcının belirtilen kanalda zaten olduğunu belirtir.
# define ERR_NEEDMOREPARAMS(source, command)             "461 " + source + " " + command + " :Not enough parameters" + "\r\n"                                                          //Komutun çalışması için daha fazla parametre gerektiğini belirtir.
# define ERR_ALREADYREGISTERED(source)                   "462 " + source + " :You may not register" + "\r\n"                                                                           //Kullanıcının zaten kayıtlı olduğunu belirtir.
# define ERR_CHANNELISFULL(source, channel)              "471 " + source + " " + channel + " :Cannot join channel (+l)" + "\r\n"                                                       //Kanalın dolu olduğunu ve daha fazla kullanıcı kabul edemediğini belirtir.
# define ERR_UNKNOWNMODE(nick, mode)                     "472 " + nick + " " + mode + " :is unknown mode char to me" + "\r\n"                                                          //Belirtilen modun bilinmeyen bir mod olduğunu belirtir.
# define ERR_INVITEONLYCHAN(nick, channel)               "473 " + nick + " " + channel + " :Cannot join channel (+i)" + "\r\n"                                                         //Bir kanala davet olmadan katılmaya çalışıldığını belirtir.
# define ERR_BADCHANNELKEY(source, channel)              "475 " + source + " " + channel + " :Cannot join channel (+k)" + "\r\n"                                                       //Kanalın bir anahtarla (şifre) korunduğunu belirtir.
# define ERR_CHANOPRIVSNEEDED(source, channel)         ": 482 " + source + " " + channel + " :You're not the channel operator" + "\r\n"                                                //Bir kanalda operatör ayrıcalıkları gerektiğini belirtir.

// Reply Codes
# define RPL_LISTSTART(nick, num_users)                   ": 321 " + nick + " Channel : "+ num_users + "\r\n"                                                                          //LIST komutunun başlangıcını belirtir.
# define RPL_LIST(source, channel, userCount, topic)     ": 322 " + source + " " + channel + " " + userCount + " " + topic + "\r\n"                                                    //Bir kanalın kullanıcı sayısını ve konusunu belirtir.
# define RPL_LISTEND(source)                             "323 " + source + " :End of /LIST" "\r\n"                                                                                     //LIST komutunun sonunu belirtir.
# define RPL_NOTOPIC(source, channel)                    "331 " + source + " " + channel + " :No topic is set" + "\r\n"                                                                //Bir kanalda konu belirlenmediğini bildirir.
# define RPL_TOPIC(source, channel, topic)               "332 " + source + " " + channel + " " + topic + "\r\n"                                                                        //Bir kanaldaki mevcut konuyu belirtir.
# define RPL_NAMREPLY(source, channel, nickList)         ": 353 " + source + " = " + channel + " :" + nickList + "\r\n"                                                                //Bir kanaldaki kullanıcıların listesini içerir.
# define RPL_ENDOFNAMES(source, channel)                 "366 " + source + " " + channel + " :End of /NAMES list." + "\r\n"                                                            //Bir kanalın kullanıcı listesinin sonunu belirtir.
# define RPL_JOIN(source, channel)                       ":" + source + " JOIN :" + channel + "\r\n"                                                                                   //Bir kullanıcının bir kanala katıldığını belirtir.
# define RPL_WELCOME(server, nick, prefix)               ":" + server  + " 001 " + nick +" :Welcome to the Internet Relay Network " + prefix + "\r\n"                                  //Sunucuya bağlanan kullanıcıya hoş geldiniz mesajını gönderir.
# define RPL_KICK(source, channel, target, reason)       ":" + source + " KICK " + channel + " " + target + " :" + reason + "\r\n"                                                     //Bir kullanıcının bir kanaldan atıldığını belirtir.
# define RPL_PART(prefix, channel, message)              ":" + prefix + " PART " + channel + " :" + message + "\r\n"                                                                   //Bir kullanıcının bir kanaldan ayrıldığını belirtir.
# define RPL_QUIT(source, message)                       " :" + source + " QUIT :Quit: " + message + "\r\n"                                                                            //Bir kullanıcının sunucudan ayrıldığını bildirir.

#endif
