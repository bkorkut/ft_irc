#include <sstream>
#include <vector>
#include <iostream>
#include "Server.hpp"

// Constructor
Server::Server() : _serverName(SERVER_NAME), _socketFD(-1), _newSocketFD(-1)
{
	fptr["NICK"] = &Server::NICK;
	fptr["USER"] = &Server::USER;
	fptr["PASS"] = &Server::PASS;
	fptr["KICK"] = &Server::KICK;
	fptr["MODE"] = &Server::MODE;
	fptr["PART"] = &Server::PART;
	fptr["QUIT"] = &Server::QUIT;
	fptr["TOPIC"] = &Server::TOPIC;
	fptr["INVITE"] = &Server::INVITE;
	fptr["PRIVMSG"] = &Server::PRIVMSG;
	fptr["JOIN"] = &Server::JOIN;
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

	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_port = htons(_portNum);
	_serverAddress.sin_addr.s_addr = INADDR_ANY;

	int en = 1;
	if (setsockopt(_socketFD, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)))
		throw(std::runtime_error("failed to set option (SO_REUSEADDR) on socket"));

	if (bind(_socketFD, (sockaddr *)&_serverAddress, sizeof(_serverAddress)))
		throw std::runtime_error("Couldn't bind ip address & the port to the socket");

	if (listen(_socketFD, SOMAXCONN))
		throw std::runtime_error("Listen failed");

	_newPollFD.fd = _socketFD;
	_newPollFD.events = POLLIN;
	_newPollFD.revents = 0;
	_fds.push_back(_newPollFD);
}

void Server::acceptClient() {
	struct sockaddr_in client;
	socklen_t client_len = sizeof(client);
	int comFD = accept(_socketFD, (struct sockaddr *)&client, &client_len);

	if (comFD == -1) {
		std::cerr << "Socket connection failed" << std::endl;
		return;
	}

	_newPollFD.fd = comFD;
	_newPollFD.events = POLLIN;
	_newPollFD.revents = 0;
	_fds.push_back(_newPollFD);
	_users.insert(std::make_pair(comFD, User(comFD)));

	std::cout << "Client with ID: " << comFD << " connected" << std::endl;
}

int Server::findClientIndex(int fd) {
	std::cout << "findClientIndex with fd " << fd << std::endl;
	for (size_t i = 0; i < _fds.size(); i++) {
		if (_fds[i].fd == fd)
			return i;
	}
	return -1;
}

void	Server::removeClient(int idx) {
	std::cout << "removeClient fd " << _fds[idx].fd << std::endl;
	close(_fds[idx].fd);
	std::map<int, User>::iterator user = _users.find(_fds[idx].fd);
	if (user != _users.end())
		_users.erase(user);
	_fds.erase(_fds.begin() + idx);
}

void Server::recieveData(int fd) {
	char buf[512];
	ssize_t bytes = recv(fd, buf, sizeof(buf) - 1, 0);

	if (bytes <= 0) {
		int idx = findClientIndex(fd);
		if (idx != -1)
			removeClient(idx);
	}
	else if (bytes > 0) {
		buf[bytes] = '\0';
		std::cout << "Recieved: " << buf << std::endl;
		this->commandParser(fd, buf);
	}
}

void Server::sendData(int fd, std::string data) {
	ssize_t result = send(fd, data.c_str(), data.size(), 0);
	if (result == -1) {
		std::cerr << "Error sending data to fd " << fd << std::endl;
		return;
	}
	std::cout << "Sent " << result << " bytes to fd " << fd << ":\n" << data << std::endl;
}

void Server::closeFds() {
	if(!_users.empty()){
		for(std::map<int, User>::iterator it = _users.begin(); it != _users.end(); ++it){
			std::cout << "User <" << it->first << "> Disconnected" << std::endl;
			close(it->first);
		}
	}

	if (this->_socketFD != -1) {
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
		int readyStatus = poll(_fds.data(), _fds.size(), -1);
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
			else if (_fds[i].revents & POLLHUP)
				removeClient(i);
		}
	}
}

void Server::commandParser(int fd, std::string input) {
	std::vector<std::string> commands;
	std::vector<std::string> params;
	std::string colon;
	std::string others;
	std::string command;

	std::cout << "\n\033[34m[New Client Message]\033[0m Client ID: " << fd << "\nMessage:\n" << input << std::endl;
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

	for (size_t i = 0; i < commands.size(); i++) {
		size_t pos = commands[i].find(" :");
		if (pos != std::string::npos) {
			others = commands[i].substr(0, pos);
			colon = commands[i].substr(pos + 2, commands[i].size());
			params = vecSplit(others, " ");
			params.push_back(colon);
		}
		else
			params = vecSplit(commands[i], " ");

		if (!params.empty() && fptr.find(toUpper(params[0])) != fptr.end())
			(this->*fptr[toUpper(params[0])])(fd, params);
	}
}
