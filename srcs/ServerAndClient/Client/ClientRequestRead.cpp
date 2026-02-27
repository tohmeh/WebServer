/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientRequestRead.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtohmeh <mtohmeh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 00:00:00 by mtohmeh           #+#    #+#             */
/*   Updated: 2025/10/14 00:00:00 by mtohmeh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Utils.hpp"

std::string ClientSocketInfos::readRequest()
{
    char buffer[1024];
    int valread = read(clientInformations.clientFd, buffer, sizeof(buffer) - 1);

    if (valread < 0)
    {
        return "";
    }

    if (valread == 0)
    {
        clientInformations.connectionClosed = true;
        return "";
    }

    clientInformations.lastActivityTime = time(NULL);
    clientInformations.readBuffer.append(buffer, valread);

    size_t headerEnd = clientInformations.readBuffer.find("\r\n\r\n");
    if (headerEnd == std::string::npos)
    {
        return "";
    }

    std::string headerSection = clientInformations.readBuffer.substr(0, headerEnd);
    size_t bodyStart = headerEnd + 4;
    size_t currentBodySize = clientInformations.readBuffer.length() - bodyStart;

    size_t contentLengthPos = headerSection.find("Content-Length:");
    if (contentLengthPos == std::string::npos)
        contentLengthPos = headerSection.find("content-length:");

    if (contentLengthPos != std::string::npos)
    {
        size_t valueStart = contentLengthPos + 15;
        while (valueStart < headerSection.length() &&
               (headerSection[valueStart] == ' ' || headerSection[valueStart] == '\t'))
            valueStart++;

        size_t valueEnd = headerSection.find("\r\n", valueStart);
        if (valueEnd == std::string::npos)
            valueEnd = headerSection.length();

        std::string lengthStr = headerSection.substr(valueStart, valueEnd - valueStart);
        int expectedBodySize = ft_stoi(lengthStr);

        if ((size_t)expectedBodySize > clientInformations.maxBodySize)
        {
            std::string errMsg = "[Client " + ft_itos(clientInformations.clientFd) +
                                "] Body size " + ft_itos(expectedBodySize) +
                                " exceeds limit " + ft_itos(clientInformations.maxBodySize) + "\n";
            ft_putstr_fd(errMsg, STDOUT_FILENO);
            clientInformations.bodyTooLarge = true;
            std::string completeRequest = clientInformations.readBuffer;
            clientInformations.readBuffer.clear();
            return completeRequest;
        }

        if (currentBodySize < (size_t)expectedBodySize)
        {
            return "";
        }
    }

    std::string completeRequest = clientInformations.readBuffer;
    clientInformations.readBuffer.clear();

    return completeRequest;
}

Request ClientSocketInfos::processRequest()
{
    std::string rawRequest = readRequest();

    if (clientInformations.connectionClosed)
    {
        return Request();
    }

    if (rawRequest.empty())
    {

        Request waitRequest;
        waitRequest.setMethod("__INCOMPLETE__");
        return waitRequest;
    }

    currentRequest = Request::parseFromString(rawRequest);

    std::string msg = "[Client " + ft_itos(clientInformations.clientFd) + "] " +
                     currentRequest.getMethod() + " " + currentRequest.getUri() + "\n";
    ft_putstr_fd(msg, STDOUT_FILENO);

    return currentRequest;
}
