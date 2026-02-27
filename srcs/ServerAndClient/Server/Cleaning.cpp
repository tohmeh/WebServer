/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cleaning.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtohmeh <mtohmeh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 00:00:00 by mtohmeh           #+#    #+#             */
/*   Updated: 2025/10/14 00:00:00 by mtohmeh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Utils.hpp"

void ServerInfos::removeClient(ClientSocketInfos* client)
{
    int clientFd;
    size_t i;

    clientFd = client->getClientFd();

    for (i = 0; i < clients.size(); i++)
    {
        if (clients[i] == client)
        {
            clients.erase(clients.begin() + i);
            break;
        }
    }

    client->closeConnection();
    delete client;

    if (clientFd == maxFd)
    {
        maxFd = 0;
        for (i = 0; i < serverSockets.size(); i++)
        {
            if (serverSockets[i].serverFd > maxFd)
                maxFd = serverSockets[i].serverFd;
        }
        for (i = 0; i < clients.size(); i++)
        {
            if (clients[i]->getClientFd() > maxFd)
                maxFd = clients[i]->getClientFd();
        }
    }
}

void ServerInfos::checkTimeouts()
{
    time_t now;
    size_t i;
    time_t lastActivity;
    std::string msg;

    now = time(NULL);

    for (i = 0; i < clients.size(); i++)
    {
        lastActivity = clients[i]->getLastActivityTime();
        if (now - lastActivity > TIMEOUT_SECONDS)
        {
            msg = "[Server] Client " + ft_itos(clients[i]->getClientFd()) +
                  " timed out (inactive for " + ft_itos(now - lastActivity) + " seconds)\n";
            ft_putstr_fd(msg, STDOUT_FILENO);
            removeClient(clients[i]);
            i--;
        }
    }
}
