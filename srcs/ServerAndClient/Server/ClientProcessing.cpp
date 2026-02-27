/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientProcessing.cpp                               :+:      :+:    :+:   */
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
#include "Router.hpp"

Response ServerInfos::handleHttpRequest(const Request& request, int clientPort)
{
    std::string host;
    const std::map<std::string, std::string>& headers = request.getHeaders();
    std::map<std::string, std::string>::const_iterator hostIt = headers.find("Host");
    if (hostIt != headers.end())
        host = hostIt->second;

    const ServerConf* serverConf = matchServerByHost(clientPort, host);
    if (!serverConf)
    {
        std::map<std::string, std::string> headers;
        headers["Content-Type"] = "text/html";
        headers["Connection"] = "close";
        return Response(Status::InternalServerError, "<html><body><h1>500 Internal Server Error</h1><p>No server configuration found</p></body></html>", headers);
    }

    ResolvedPath rp = resolve(*serverConf, request);
    Router router;
    Response response = router.dispatch(*serverConf, request);
    return response;
}

void ServerInfos::processClientSockets(fd_set& currentReadfds, fd_set& currentWritefds)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        int clientFd = clients[i]->getClientFd();

        if (FD_ISSET(clientFd, &currentWritefds))
        {
            clients[i]->flushPendingResponse();
        }

        if (FD_ISSET(clientFd, &currentReadfds))
        {
            Request request = clients[i]->processRequest();

            if (request.getMethod() == "__INCOMPLETE__")
            {
                continue;
            }

            if (request.getMethod().empty())
            {
                removeClient(clients[i]);
                i--;
            }
            else
            {
                int clientPort = clients[i]->getServerPort();

                Response response;

                if (clients[i]->isBodyTooLarge())
                {
                    std::map<std::string, std::string> headers;
                    headers["Content-Type"] = "text/html";
                    headers["Connection"] = "close";
                    response.setBody("<html><body><h1>413 Payload Too Large</h1><p>Request body exceeds maximum allowed size.</p></body></html>");
                    response.setStatus(Status::PayloadTooLarge);
                    response.setHeaders(headers);
                }
                else
                {
                    response = handleHttpRequest(request, clientPort);
                }

                std::string msg = "[Server] Response: " + ft_itos(response.getStatus().code) + " " + response.getStatus().reason + "\n";
                ft_putstr_fd(msg, STDOUT_FILENO);

                clients[i]->sendResponse(response.toString());

                removeClient(clients[i]);
                i--;
            }
        }
    }
}
