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

#include "Client.hpp"

bool ClientSocketInfos::isConnected()
{
    return clientInformations.clientFd != -1 && !clientInformations.connectionClosed;
}

void ClientSocketInfos::closeConnection()
{
    close(clientInformations.clientFd);
}
