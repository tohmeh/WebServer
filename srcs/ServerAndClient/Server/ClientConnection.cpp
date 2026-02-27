/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientConnection.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtohmeh <mtohmeh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 00:00:00 by mtohmeh           #+#    #+#             */
/*   Updated: 2025/10/14 00:00:00 by mtohmeh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Config.hpp"
#include "Utils.hpp"

static size_t getMaxBodySizeForPort(const HttpConf& config, int targetPort)
{
    size_t maxBodySize = 1048576;

    for (size_t i = 0; i < config.servers.size(); ++i)
    {
        const ServerConf& server = config.servers[i];
        for (size_t j = 0; j < server.ports.size(); ++j)
        {
            int port = server.ports[j];
            if (port == targetPort && server.client_max_body_size > maxBodySize)
                maxBodySize = server.client_max_body_size;
        }
    }
    return maxBodySize;
}

void ServerInfos::handleNewConnection(int serverFd, int serverPort)
{
    int newClientFd;
    socketAddress clientAddress;
    socklen_t addrlen;
    std::string msg;
    ClientSocketInfos* newClient;
    size_t maxBodySize;

    newClientFd = 0;
    addrlen = sizeof(clientAddress);

    while ((newClientFd = accept(serverFd, (struct sockaddr *)&clientAddress, &addrlen)) >= 0)
    {
        if (fcntl(newClientFd, F_SETFL, O_NONBLOCK | FD_CLOEXEC) < 0)
        {
            ft_error("[Server] fcntl on client socket failed");
            close(newClientFd);
            continue;
        }

        msg = "[Server] Client connected on port " + ft_itos(serverPort) + " (fd: " + ft_itos(newClientFd) + ")\n";
        ft_putstr_fd(msg, STDOUT_FILENO);

        newClient = new ClientSocketInfos();
        newClient->setClientInfo(newClientFd, clientAddress, serverPort);

        maxBodySize = getMaxBodySizeForPort(config, serverPort);
        newClient->setMaxBodySize(maxBodySize);

        clients.push_back(newClient);

        if (newClientFd > maxFd)
            maxFd = newClientFd;
    }

}
