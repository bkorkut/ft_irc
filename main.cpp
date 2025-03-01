#include "Server.hpp"

int isPort(std::string portNum) {
	for (unsigned i = 0; i < portNum.length(); ++i) {
		if (portNum[i] < '0' || portNum[i] > '9')
			throw std::invalid_argument("The port has to be number");
	}
	int	num = std::atoi(portNum.c_str());
	if (num < 0 || num > 65535) 
		throw std::invalid_argument("The port number has to be between 0 & 65535");
	return num;
}

int main(int argc, char **argv) {
	try {
		if (argc != 3)
			throw std::invalid_argument("./ircserv <port> <password>");

		signal(SIGINT, Server::SignalHandler);
		signal(SIGQUIT, Server::SignalHandler);

		Server s;

		s.initialize(isPort(argv[1]), argv[2]);
		s.run();

	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}
