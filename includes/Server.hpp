/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtohmeh <mtohmeh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 12:16:27 by mtohmeh           #+#    #+#             */
/*   Updated: 2025/10/15 18:35:23 by mtohmeh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Response.hpp"
#include "Parser.hpp"

typedef struct sockaddr_in socketAddress;

typedef struct serverSocketInfos
{
    int serverFd;
    socketAddress serverAddress;
    int port;
} serverSocketInfos;

class ServerInfos{
	private:
		std::vector<serverSocketInfos> serverSockets;
		std::vector<ClientSocketInfos*> clients;
		int maxFd;
		bool shouldShutdown;
		HttpConf config;

	public:
		void initializeSocket(int port);
		void initializeAllSockets(const HttpConf& httpConfig);
		void runServerLoop();
		void handleNewConnection(int serverFd, int serverPort);
		void removeClient(ClientSocketInfos* client);
		void checkTimeouts();
		void shutdown();
		void setShutdownFlag();
		Response handleHttpRequest(const Request& request, int clientPort);
		void processClientSockets(fd_set& currentReadfds, fd_set& currentWritefds);
		const ServerConf* findServerByPort(int port) const;
		const ServerConf* matchServerByHost(int port, const std::string& host) const;
};

extern ServerInfos* g_server;

void signalHandler(int signal);

#endif