CXX       := c++
CXXFLAGS  := -Wall -Wextra -Werror -std=c++98 -g -Iincludes

NAME      := webserv
OBJ_DIR   := obj

SRCS := \
	srcs/main.cpp \
	srcs/Utils.cpp \
	srcs/ParsingAndCgi/Config.cpp \
	srcs/ParsingAndCgi/Token.cpp \
	srcs/ParsingAndCgi/CGI.cpp \
	srcs/ParsingAndCgi/Parser.cpp \
	srcs/ParsingAndCgi/ConfigAST.cpp \
	srcs/ParsingAndCgi/Node.cpp \
	srcs/ReqRes/Router.cpp \
	srcs/ReqRes/Request.cpp \
	srcs/ReqRes/Response.cpp \
	srcs/ReqRes/GetHandler.cpp \
	srcs/ReqRes/PostHandler.cpp \
	srcs/ReqRes/DeleteHandler.cpp \
	srcs/ReqRes/RequestHandler.cpp \
	srcs/ReqRes/HttpStatus.cpp \
	srcs/ServerAndClient/Server/Server.cpp \
	srcs/ServerAndClient/Server/ServersSocketsInit.cpp \
	srcs/ServerAndClient/Server/Shutdown.cpp \
	srcs/ServerAndClient/Server/Cleaning.cpp \
	srcs/ServerAndClient/Server/ClientConnection.cpp \
	srcs/ServerAndClient/Server/ClientProcessing.cpp \
	srcs/ServerAndClient/Server/ServerUtils.cpp \
	srcs/ServerAndClient/Client/ClientInfo.cpp \
	srcs/ServerAndClient/Client/ClientRequestRead.cpp \
	srcs/ServerAndClient/Client/ClientResponseSend.cpp \
	srcs/ServerAndClient/Client/ClientConnection.cpp

OBJS := $(SRCS:srcs/%.cpp=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: srcs/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
