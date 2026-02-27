/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientInfo.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtohmeh <mtohmeh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 00:00:00 by mtohmeh           #+#    #+#             */
/*   Updated: 2025/10/14 00:00:00 by mtohmeh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void ClientSocketInfos::setClientInfo(int clientFd, socketAddress clientAddr, int serverPort)
{
    clientInformations.clientFd = clientFd;
    clientInformations.clientAddress = clientAddr;
    clientInformations.addressLength = sizeof(clientAddr);
    clientInformations.serverPort = serverPort;
    clientInformations.connectionClosed = false;
    clientInformations.lastActivityTime = time(NULL);
    clientInformations.bodyTooLarge = false;
    clientInformations.maxBodySize = 1048576;
}

int ClientSocketInfos::getServerPort() const
{
    return clientInformations.serverPort;
}

time_t ClientSocketInfos::getLastActivityTime() const
{
    return clientInformations.lastActivityTime;
}

void ClientSocketInfos::updateActivityTime()
{
    clientInformations.lastActivityTime = time(NULL);
}

socketAddress ClientSocketInfos::getClientAddress()
{
    return clientInformations.clientAddress;
}

int ClientSocketInfos::getClientFd()
{
    return clientInformations.clientFd;
}

void ClientSocketInfos::setMaxBodySize(size_t size)
{
    clientInformations.maxBodySize = size;
}

bool ClientSocketInfos::isBodyTooLarge() const
{
    return clientInformations.bodyTooLarge;
}
