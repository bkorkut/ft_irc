#include "Socket.hpp"

Socket::~Socket() {}

Socket::Socket(int portNum, const std::string &pswd) : _portNum(portNum), _pswd(pswd) {
  _socketFD = socket(AF_INET, SOCK_STREAM, 0);
  if (_socketFD == -1)
    throw std::runtime_error("Socket is not created"); // Change runtime it's too generic

  // need to bind an ip address and port to the socket
  _serverAddress.sin_family = AF_INET;
  _serverAddress.sin_port = htons(_portNum);
  _serverAddress.sin_addr.s_addr = INADDR_ANY;

  if (bind(_socketFD, (sockaddr *)&_serverAddress, sizeof(_serverAddress))) {
    close(_socketFD); // make a function for it and close it on the main
    throw std::runtime_error( "Couldn't bind ip address & the port to the socket");
  }

  if (listen(_socketFD, 42)) { // 42 = how many clients can connect
    close(_socketFD);
    throw std::runtime_error("Listen failed");
  }
}

int Socket::acceptClient(){
  struct sockaddr_in client;
  socklen_t client_len = sizeof(client);  //& ref so it can update the variables
  if(accept(_socketFD, (struct sockaddr*) &client, &client_len)){
    std::cerr << "Socket connection failed" << std::endl;
    return -1;
  };
  // POLL STUFF

  return 1;
}
