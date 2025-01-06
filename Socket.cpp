#include "Socket.hpp"
#include <stdexcept>
#include <sys/socket.h>

Socket::Socket() {}

Socket::~Socket() {
  // this->closeFds();
}

void Socket::initialize(int portNum, const std::string &pswd) {
  _portNum = portNum;
  _pswd = pswd;

  _socketFD = socket(AF_INET, SOCK_STREAM, 0);
  if (_socketFD == -1)
    throw std::runtime_error("Socket is not created");

  // need to bind an ip address and port to the socket
  // _serverAddress is a struct (sockaddr_in _serverAddress inside of the class)
  // | it was gonna be sockaddr_in6 if it was for ipv6
  _serverAddress.sin_family = AF_INET;
  _serverAddress.sin_port =
      htons(_portNum); // converting portnumber to big endian
  _serverAddress.sin_addr.s_addr = INADDR_ANY;

  int en = 1;
  if (setsockopt(_socketFD, SOL_SOCKET, SO_REUSEADDR, &en,
                 sizeof(en))) // Making the port re-usable without waiting
    throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));

  /*  //commented to see if it's gonna block the whole connection uncomment this later
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

  _newPollFD.fd = _socketFD;
  _newPollFD.events = POLLIN;
  _newPollFD.revents = 0;
  _fds.push_back(_newPollFD);
}

void Socket::acceptClient() {
  struct sockaddr_in client; // maybe use memset after it && maybe put it inside the class
  socklen_t client_len = sizeof(client); //& ref so it can update the variables
  int comFD = accept(_socketFD, (struct sockaddr *)&client, &client_len);

  if (comFD == -1) {
    std::cerr << "Socket connection failed" << std::endl;
    return;
  }

  /*  //commented to see if it's gonna block the whole connection uncomment this later
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

  // TODO: Maybe save the client info for the commands here
  std::cout << "Client <" << comFD << "> Connected" << std::endl; // might delete this message later
}

void Socket::recieveData(int fd) {
  // TODO

    char buf[512]; // maybe memset the area

    ssize_t bytes = recv(fd, buf, sizeof(buf) - 1, 0);
    if (bytes > 0) {
      buf[bytes] = '\0';
    }
    std::cout << "Message: " << buf << "Bytes: " << bytes << std::endl;
}

void Socket::closeFds() {
  // TODO: close client fds

  if (this->_socketFD != -1) { // if the socketFd is created therefore it isn't -1
    std::cout << "Server <" << _socketFD << "> Disconnected" << std::endl; // might delete this message later
    close(this->_socketFD);
  }
}

void Socket::run() {
  while (1 /* TODO: && the signal is not interupting*/) {
    if (poll(_fds.data(), _fds.size(), -1) == -1) // -1 is the timeout paramater
      throw std::runtime_error("poll() function failed");
    for (size_t i = 0; i < _fds.size(); i++) {
      if (_fds[i].revents & POLLIN) {
        if (_fds[i].fd == _socketFD)
          acceptClient();
        else
          recieveData(_fds[i].fd);
      }
    }
  }
}
