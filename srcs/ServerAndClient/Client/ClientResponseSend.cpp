/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientResponseSend.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtohmeh <mtohmeh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 00:00:00 by mtohmeh           #+#    #+#             */
/*   Updated: 2025/10/14 00:00:00 by mtohmeh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void ClientSocketInfos::sendResponse(const std::string& response)
{
    if (!clientInformations.pendingResponse.empty())
    {
        clientInformations.pendingResponse += response;
        return;
    }

    int sent = send(clientInformations.clientFd, response.c_str(), response.length(), 0);

    if (sent < 0)
    {
        clientInformations.pendingResponse = response;
        return;
    }

    if (sent < (int)response.length())
    {
        clientInformations.pendingResponse = response.substr(sent);
    }
}

bool ClientSocketInfos::flushPendingResponse()
{
    if (clientInformations.pendingResponse.empty())
        return true;

    int sent = send(clientInformations.clientFd,
                    clientInformations.pendingResponse.c_str(),
                    clientInformations.pendingResponse.length(), 0);

    if (sent < 0)
    {
        return false;
    }

    if (sent < (int)clientInformations.pendingResponse.length())
    {
        clientInformations.pendingResponse = clientInformations.pendingResponse.substr(sent);
        return false;
    }
    clientInformations.pendingResponse.clear();
    return true;
}

bool ClientSocketInfos::hasPendingResponse() const
{
    return !clientInformations.pendingResponse.empty();
}
