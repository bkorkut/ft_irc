#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/types.h> // data types for systemcalls require for socket and netinet
#include <sys/socket.h> // |sockaddr| structure
#include <netinet/in.h>  // |sockaddr_in| const&structs for internet domain addrs
#include <iostream>
#include <stdexcept>
#include <string>
#include <stdlib.h>
#include <unistd.h>

class Socket{
private:
  int _socketFD;
  int _newSocketFD;
  int _portNum;
  const std::string& _pswd; 
  struct sockaddr_in _serverAddress;

	// Preventing cononical form
  Socket& operator=(const Socket& other);
  Socket(const Socket& other);
  Socket();

public:
  Socket(int portNum, const std::string& pswd);
  ~Socket();
  int acceptClient();
};

#endif

