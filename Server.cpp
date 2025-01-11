#include <sstream>
#include <vector>
#include <iostream>
#include "Server.hpp"

// Constructor
Server::Server()
{
	fptr["NICK"] = &Server::NICK;
	fptr["USER"] = &Server::USER;
	fptr["PASS"] = &Server::PASS;
	fptr["KICK"] = &Server::KICK;
	fptr["MODE"] = &Server::MODE;
	fptr["TOPIC"] = &Server::TOPIC;
	fptr["INVITE"] = &Server::INVITE;
	fptr["PRIVMSG"] = &Server::PRIVMSG;
}

// Destructor
Server::~Server() {
	this->closeFds();
}

void Server::initialize(int portNum, const std::string &pswd) {
	_portNum = portNum;
	_pswd = pswd;

	_socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketFD == -1)
		throw std::runtime_error("Socket is not created");

	// need to bind an ip address and port to the socket
	// _serverAddress is a struct (sockaddr_in _serverAddress inside of the class)
	// | it was gonna be sockaddr_in6 if it was for ipv6
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_port = htons(_portNum); // converting portnumber to big endian
	_serverAddress.sin_addr.s_addr = INADDR_ANY;

	int en = 1;
	if (setsockopt(_socketFD, SOL_SOCKET, SO_REUSEADDR, &en,
								 sizeof(en))) // Making the port re-usable without waiting
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));

	/*	//commented to see if it's gonna block the whole connection uncomment this later
	if (fcntl(_socketFD, F_SETFL, O_NONBLOCK) == -1) // making the socket using a nonblock connection for recv(read) or send(write) to not make the connection wait
	throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
	*/

	// sockaddr is a general structure for both ipv4 and ipv6, we're using one
	// bind function instead of two seperate ones
	if (bind(_socketFD, (sockaddr *)&_serverAddress, sizeof(_serverAddress)))
		throw std::runtime_error(
				"Couldn't bind ip address & the port to the socket");

	if (listen(_socketFD, SOMAXCONN)) // SOMAXCONN = max amount of clients that can connect
		throw std::runtime_error("Listen failed");

	_newPollFD.fd = _socketFD; // NOT: Gördüğüm kadarıyla bu değişkenler tek seferlik atama için kullanılıyor ve classtan ulaşılmasına ihtiyaç yok ~betül
	_newPollFD.events = POLLIN;
	_newPollFD.revents = 0;
	_fds.push_back(_newPollFD);
}

void Server::acceptClient() {
	struct sockaddr_in client; // maybe use memset after it && maybe put it inside the class
	socklen_t client_len = sizeof(client); //& ref so it can update the variables
	int comFD = accept(_socketFD, (struct sockaddr *)&client, &client_len);

	if (comFD == -1) {
		std::cerr << "Socket connection failed" << std::endl;
		return;
	}

	/*	//commented to see if it's gonna block the whole connection uncomment this later
	if (fcntl(comFD, F_SETFL, O_NONBLOCK)){
	close(comFD); // maybe close the comFD in the closeFD function?
	std::cerr << "fcntl() failed" << std::endl;
		return;
	}
	*/

	_newPollFD.fd = comFD;
	_newPollFD.events = POLLIN;
	_newPollFD.revents = 0;
	_fds.push_back(_newPollFD);
	_users.insert(std::make_pair(comFD, User(comFD)));

	// might delete this message later
	std::cout << "Client with ID: " << comFD << " connected" << std::endl;
}

void Server::recieveData(int fd) {

	char buf[512]; // maybe memset the area

	ssize_t bytes = recv(fd, buf, sizeof(buf) - 1, 0);
	if (bytes <= 0){
		//might delete printing message later
		std::cout << "User <" << fd << "> Disconnected" << " cause of recv() func" << std::endl;
		close(fd);
		_users.erase(fd);
	}
	else if (bytes > 0){
	buf[bytes] = '\0';
	// delete this later
	std::cout << "Message:\n" << buf << "Bytes: " << bytes << std::endl;
	this->commandParser(fd, buf);
	}
}

void Server::sendData(int fd, std::string data){
	if(send(fd, data.c_str(), data.size(), 0) == -1)
		std::cerr << "send() failed" << std::endl;

	//should delete this later
	std::cout << "Message to client ID " << fd << ":\n" << data << std::endl;
}

void Server::closeFds() {
	if(!_users.empty()){
		for(std::map<int, User>::iterator it = _users.begin(); it != _users.end(); ++it){
		//might delete printing message later
		std::cout << "User <" << it->first << "> Disconnected" << std::endl;
			close(it->first);
		}
	}

	if (this->_socketFD != -1) { // if the socketFd is created therefore it isn't -1
		//might delete printing message later
		std::cout << "Server <" << _socketFD << "> Disconnected" << std::endl;
		close(this->_socketFD);
	}
}

bool Server::Signal = false;
void Server::SignalHandler(int signum) {
	(void)signum;
	Server::Signal = true;
}

void Server::run() {
	while (!Server::Signal) {
	int readyStatus = poll(_fds.data(), _fds.size(), -1);  // -1 is the timeout paramater
		if (readyStatus == -1 && !Server::Signal)
			throw std::runtime_error("poll() function failed");
		for (size_t i = 0; i < _fds.size() && readyStatus > 0; i++) {
			if (_fds[i].revents & POLLIN) {
				if (_fds[i].fd == _socketFD)
					acceptClient();
				else
					recieveData(_fds[i].fd);
			readyStatus--;
			}
		}
	}
}

// Ctrl D (eof) still needs handling
// parses and sendscommands to their appropriate functions
void	Server::commandParser(int fd, std::string input)
{
	std::vector<std::string>	commands;
	std::vector<std::string>	params;
	std::string					colon;
	std::string					others;
	std::string					command;

	command = _users.find(fd)->second.buffer.append(input);
	commands = vecSplit(command, "\r\n");
	if (command.size() < 2 || !(command[command.size() - 2 ] == '\r'
		&& command[command.size() - 1] == '\n'))
	{
		_users.find(fd)->second.buffer = commands.back();
		commands.pop_back();
	}
	else
		_users.find(fd)->second.buffer.clear();
	for (size_t i = 0; i < commands.size(); i++)
	{
		size_t pos = commands[i].find(" :");
		if (pos != std::string::npos)
		{
			others = commands[i].substr(0, pos);
			colon = commands[i].substr(pos + 2, commands[i].size());
			params = vecSplit(others, " ");
			params.push_back(colon);
		}
		else
			params = vecSplit(commands[i], " ");

		// try
		// {
		if (!params.empty() && fptr.find(params[0]) != fptr.end())
				(this->*fptr[params[0]])(fd, params);
		// }
		// catch(const std::exception& e)
		// {
		// 	std::cerr << e.what() << '\n';
		// }
	}
}
