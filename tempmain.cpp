#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

// close, accept, listen, bind, htons,

// setsockopt, getsockname,
// getprotobyname, gethostbyname, getaddrinfo,
// freeaddrinfo, connect,
// htonl, ntohs, ntohl, inet_addr, inet_ntoa, send,
// recv, signal, sigaction, lseek, fstat, fcntl, poll (or equivalent)

#include "Server.hpp"

int main()
{
	Server a;
	std::cout << "hey" << std::endl;
	std::cout << std::boolalpha << a.channels.empty() << std::endl;
}

int	main(/*int ac, char **av*/)
{
	// if (ac == 3)
	// {
		int	sd;

		sd = socket(AF_INET, SOCK_STREAM, 0);
		if (sd == -1)
		{
			perror("Socket creation failed");
			exit(EXIT_FAILURE);
		}

		sockaddr_in	sa;
		sa.sin_family = AF_INET;
		sa.sin_port = htons(4242);
		sa.sin_addr.s_addr = INADDR_ANY;

		if (bind(sd, (struct sockaddr *)(&sa), sizeof(sa)))
		{
			perror("Assigning a name to socket failed");
			exit(EXIT_FAILURE);
		}

		if (listen(sd, 2))
		{
			perror("Failed to listen");
			exit(EXIT_FAILURE);
		}

		socklen_t	sa_len = sizeof(sa);
		char	buf[512];
		while (1)
		{
			if (accept(sd, (struct sockaddr *)(&sa), &sa_len))
			{
				perror("Failed to accept request");
				exit(EXIT_FAILURE);
			}

			if (recv(sd, buf, 512, 0))
				perror("Failed to recieve message");
		}
		close(sd);
	// }
	return 0;
}
