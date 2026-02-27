/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
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

ServerInfos* g_server = NULL;

void signalHandler(int signal)
{
    std::string msg = "\n[Server] Received signal " + ft_itos(signal) + ". Shutting down gracefully...\n";
    ft_putstr_fd(msg, STDOUT_FILENO);
    if (g_server)
    {
        g_server->setShutdownFlag();
    }
}

static bool serverListensOnPort(const ServerConf& server, int targetPort)
{
    for (size_t j = 0; j < server.ports.size(); ++j)
    {
        int port = server.ports[j];
        if (port == targetPort)
            return true;
    }
    return false;
}

const ServerConf* ServerInfos::findServerByPort(int port) const
{
    size_t i;

    for (i = 0; i < config.servers.size(); ++i)
    {
        const ServerConf& server = config.servers[i];
        if (serverListensOnPort(server, port))
            return &server;
    }
    return NULL;
}

const ServerConf* ServerInfos::matchServerByHost(int port, const std::string& host) const
{
    const ServerConf* defaultServer;
    size_t i;
    size_t j;

    defaultServer = NULL;

    for (i = 0; i < config.servers.size(); ++i)
    {
        const ServerConf& server = config.servers[i];

        if (!serverListensOnPort(server, port))
            continue;

        if (defaultServer == NULL)
            defaultServer = &server;

        for (j = 0; j < server.serverName.size(); ++j)
        {
            if (server.serverName[j] == host)
                return &server;
        }
    }

    return defaultServer;
}
