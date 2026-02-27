/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shutdown.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtohmeh <mtohmeh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 00:00:00 by mtohmeh           #+#    #+#             */
/*   Updated: 2025/10/14 00:00:00 by mtohmeh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Utils.hpp"

void ServerInfos::setShutdownFlag()
{
    shouldShutdown = true;
}

void ServerInfos::shutdown()
{
    size_t i;
    std::string msg;

    for (i = 0; i < clients.size(); i++)
    {
        msg = "[Server] Closing client connection " + ft_itos(clients[i]->getClientFd()) + "\n";
        ft_putstr_fd(msg, STDOUT_FILENO);
        clients[i]->closeConnection();
        delete clients[i];
    }
    clients.clear();

    for (i = 0; i < serverSockets.size(); i++)
    {
        msg = "[Server] Closing server socket on port " + ft_itos(serverSockets[i].port) + "\n";
        ft_putstr_fd(msg, STDOUT_FILENO);
        close(serverSockets[i].serverFd);
    }
    serverSockets.clear();

    ft_putendl_fd("[Server] Shutdown complete", STDOUT_FILENO);
}
