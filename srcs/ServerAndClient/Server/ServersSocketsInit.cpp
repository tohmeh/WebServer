/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServersSocketsInit.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-naki <mal-naki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:37:40 by mtohmeh           #+#    #+#             */
/*   Updated: 2025/10/15 19:44:25 by mal-naki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Server.hpp"
#include "Utils.hpp"

static std::vector<int> getUniquePorts(const HttpConf &config)
{
	int		port;
	bool	found;

	std::vector<int> uniquePorts;
	for (size_t i = 0; i < config.servers.size(); ++i)
	{
		const ServerConf &server = config.servers[i];
		for (size_t j = 0; j < server.ports.size(); ++j)
		{
			port = server.ports[j];
			found = false;
			for (size_t k = 0; k < uniquePorts.size(); ++k)
			{
				if (uniquePorts[k] == port)
				{
					found = true;
					break ;
				}
			}
			if (!found)
				uniquePorts.push_back(port);
		}
	}
	return (uniquePorts);
}

void ServerInfos::initializeAllSockets(const HttpConf &httpConfig)
{
	maxFd = 0;
	shouldShutdown = false;
	config = httpConfig;
	std::vector<int> uniquePorts = getUniquePorts(config);
	for (size_t i = 0; i < uniquePorts.size(); ++i)
		initializeSocket(uniquePorts[i]);
}
int createSocket()
{
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1)
    {
        ft_error("[Server] Error creating socket");
        throw std::runtime_error("Failed to create socket");
    }
    return fd;
}

void setSocketOptions(int fd)
{
    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        ft_error("[Server] setsockopt SO_REUSEADDR failed");
        throw std::runtime_error("Failed to set socket options");
    }

    if (fcntl(fd, F_SETFL, O_NONBLOCK | FD_CLOEXEC) < 0)
    {
        ft_error("[Server] fcntl failed");
        throw std::runtime_error("Failed to set socket to non-blocking");
    }
}

void bindSocket(int fd, int port, socketAddress &address)
{
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        ft_error("[Server] Bind failed");
        throw std::runtime_error("Failed to bind socket");
    }
}

void listenSocket(int fd)
{
    if (listen(fd, 3) < 0)
    {
        ft_error("[Server] Listen failed");
        throw std::runtime_error("Failed to listen on socket");
    }
}

void ServerInfos::initializeSocket(int port)
{
    serverSocketInfos serverInfo;
    serverInfo.serverFd = createSocket();
    setSocketOptions(serverInfo.serverFd);
    bindSocket(serverInfo.serverFd, port, serverInfo.serverAddress);
    listenSocket(serverInfo.serverFd);

    serverInfo.port = port;
    serverSockets.push_back(serverInfo);

    if (serverInfo.serverFd > maxFd)
        maxFd = serverInfo.serverFd;
}
