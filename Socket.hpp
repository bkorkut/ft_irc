#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/types.h> // data types for systemcalls require for socket and netinet
#include <sys/socket.h> // |sockaddr| structure
#include <netinet/in.h> // |sockaddr_in| const&structs for internet domain addrs
#include <iostream>
#include <stdexcept>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <vector>

class Socket{
private:
	int _socketFD;
	int _newSocketFD;
	int _portNum;
	std::string _pswd;
	std::vector<struct pollfd> _fds;
	struct sockaddr_in _serverAddress;
	struct pollfd _newPollFD;

	// Preventing cononical form by making these private
	Socket& operator=(const Socket& other);
	Socket(const Socket& other);

public:
	Socket();
	~Socket();
	void initialize(int portNum, const std::string &pswd);
	void closeFds();
	void run();
	void acceptClient();
	void recieveData(int fd);
};

#endif

