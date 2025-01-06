#include "Socket.hpp"

// Refactor isPort using findfirstnotof function this looks like C
int isPort(std::string portNum) {
  for (unsigned i = 0; i < portNum.length(); ++i) {
    if (portNum[i] < '0' || portNum[i] > '9')
      throw std::invalid_argument("The port has to be number");
  }
  int num = std::atoi(portNum.c_str());
  if (num < 0 || num > 65535) // maybe also block 80 and premade ports, it's gonna auto fail on binding though and that error is managed
    throw std::invalid_argument("The port number has to be between 0 & 65535");
  return num;
}

int main(int argc, char **argv) {
  Socket s;
  try {
    if (argc != 3)
      throw std::invalid_argument("./ircserv <port> <password>");

    s.initialize(isPort(argv[1]), argv[2]); // Sending portnumber and password
    s.run();

  } catch (const std::exception &e) {
    s.closeFds(); // you can make it from here or in the destructor of the class decide which is better 
    std::cerr << e.what() << std::endl;
  }
}







/*
 *
 *		Eski mainler dosyanin assagisinda silinmesi gerekli
 *
 */

// #include <netinet/in.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <poll.h>
// #include <iostream>
// #include <unistd.h>
// #include <stdio.h>
// #include <iostream>
// #include <cstdlib>	// for exit
// #include <cstring>	// for memset if needed

// int	main(/*int ac, char **av*/)
// {
// 	// if (ac == 3)
// 	// {
// 		Server	server;
// 		int	sd = socket(AF_INET, SOCK_STREAM, 0);
// 		if (sd == -1)
// 			return perror("Socket creation failed"),
// std::exit(EXIT_FAILURE), 1;
//
// 		sockaddr_in	sa;
// 		sa.sin_family = AF_INET;
// 		sa.sin_port = htons(6697);
// 		sa.sin_port = htons(6697);
// 		sa.sin_addr.s_addr = INADDR_ANY;
//
// 		if (bind(sd, (struct sockaddr *)(&sa), sizeof(sa)))
// 			return perror("Assigning a name to socket failed"),
// std::exit(EXIT_FAILURE), 1; 		if (listen(sd, 2)) 			return perror("Failed to
// listen"), std::exit(EXIT_FAILURE), 1;
//
// 		int				ready;
// 		int				num_fds = 1;
// 		socklen_t		sa_len = sizeof(sa);
// 		char			buf[512];
// 		struct pollfd	fds[100];
//
// 		fds[0].fd = sd;
// 		fds[0].events = POLLIN;
// 		while (1)
// 		{
// 			ready = poll(fds, num_fds, -1);
// 			if (ready == -1)
// 				return std::cout << "terminating program" <<
// std::endl, 1; 			for(int i = 0; i < num_fds; i++)
// 			{
// 				int	k = 0;
// 				if (fds[i].revents & POLLIN)
// 				{
// 					std::cout << "pollin" << std::endl;
// 					if (i == 0)
// 					{
// 						int	fd = accept(sd, (struct
// sockaddr *)(&sa), &sa_len); 						if (fd == -1) 							return perror("Failed to accept
// request"), std::exit(EXIT_FAILURE), 1; 						else
// 						{
// 							fds[num_fds].fd = fd;
// 							fds[num_fds].events =
// POLLIN; 							num_fds++; 							server.users.insert(std::make_pair(fd, User(fd)));
// 						}
// 					}
// 					else
// 					{
// 						ssize_t bytes = recv(fds[i].fd,
// buf, sizeof(buf) - 1, 0); 						if (bytes > 0)
// 						{
// 							buf[bytes] = '\0';
// 							server.commandParser(fds[i].fd,
// buf);
// 						}
// 					}
// 					k++;
// 				}
// 				else if (fds[i].revents & POLLHUP)
// 				{
// 					std::cout << "pollhup" << std::endl;
// 					fds[i].fd = -1;
// 					k++;
// 					exit(1);
// 				}
// 				if (k == ready)
// 					break ;
// 			}
// 		}
// 	close(sd);
// 	// }
// 	return 0;
// }

// int main(int ac, char **av)
// {
// 	Server a;
// 	// std::cout << std::boolalpha << a.channels.empty() << std::endl;
// 	if (ac == 2)
// 		a.commandParser(av[1]);
// }

// close, accept, listen, bind, htons,

// setsockopt, getsockname,
// getprotobyname, gethostbyname, getaddrinfo,
// freeaddrinfo, connect,
// htonl, ntohs, ntohl, inet_addr, inet_ntoa, send,
// recv, signal, sigaction, lseek, fstat, fcntl, poll (or equivalent)
