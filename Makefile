NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g

SRCS = main.cpp Server.cpp User.cpp Channel.cpp utils.cpp\
		server/invite.cpp server/mode.cpp \
		server/pass.cpp server/topic.cpp \
		server/kick.cpp server/nick.cpp \
		server/privmsg.cpp server/user.cpp \
		server/join.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
