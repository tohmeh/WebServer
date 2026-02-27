/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-naki <mal-naki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 13:32:00 by mtohmeh           #+#    #+#             */
/*   Updated: 2025/10/15 19:41:50 by mal-naki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Request.hpp"
# include "webserv.hpp"

typedef struct sockaddr_in	socketAddress;

typedef struct clientInfos
{
	int						clientFd;
	socketAddress			clientAddress;
	socklen_t				addressLength;
	int						serverPort;
	std::string				pendingResponse;    
	std::string				readBuffer;         
	bool					connectionClosed;  
	time_t					lastActivityTime;
	bool					bodyTooLarge;     
	size_t					maxBodySize;
}							clientInfos;

class ClientSocketInfos
{
  private:
	clientInfos clientInformations;
	Request currentRequest;

  public:
	void setClientInfo(int clientFd, socketAddress clientAddr, int serverPort);
	std::string readRequest();
	int getServerPort() const;
	void sendResponse(const std::string &response);
	bool flushPendingResponse();
	bool hasPendingResponse() const;
	void closeConnection();
	socketAddress getClientAddress();
	int getClientFd();
	bool isConnected();
	Request processRequest();
	time_t getLastActivityTime() const;
	void updateActivityTime();
	void setMaxBodySize(size_t size);
	bool isBodyTooLarge() const;
};

#endif