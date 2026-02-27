/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtohmeh <mtohmeh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 16:02:46 by mtohmeh           #+#    #+#             */
/*   Updated: 2025/10/15 18:58:11 by mtohmeh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Utils.hpp"

void ServerInfos::runServerLoop()
{
	fd_set currentReadfds;
	fd_set currentWritefds;
    size_t i;
    int activity;

    ft_putendl_fd("[Server] Server loop started", STDOUT_FILENO);

    while (!shouldShutdown)
    {
        FD_ZERO(&currentReadfds);
        FD_ZERO(&currentWritefds);

        for (i = 0; i < serverSockets.size(); i++)
            FD_SET(serverSockets[i].serverFd, &currentReadfds);

        for (i = 0; i < clients.size(); i++)
        {
            FD_SET(clients[i]->getClientFd(), &currentReadfds);
            if (clients[i]->hasPendingResponse())
                FD_SET(clients[i]->getClientFd(), &currentWritefds);
        }

        activity = select(maxFd + 1, &currentReadfds, &currentWritefds, NULL, NULL);
        if (activity < 0)
        {
            continue;
        }

        if (!shouldShutdown)
            for (i = 0; i < serverSockets.size(); i++)
                if (FD_ISSET(serverSockets[i].serverFd, &currentReadfds))
                    handleNewConnection(serverSockets[i].serverFd, serverSockets[i].port);

        processClientSockets(currentReadfds, currentWritefds);

        checkTimeouts();
    }

    ft_putendl_fd("[Server] Shutting down...", STDOUT_FILENO);
    shutdown();
}
