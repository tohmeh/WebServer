/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gakhoury <gakhoury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 21:41:22 by gakhoury          #+#    #+#             */
/*   Updated: 2025/10/12 14:02:25 by gakhoury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "webserv.hpp"

Response::Response(): status(Status::OK), headers(getDefaultHeaders()), body("") {}

Response::Response(const HttpStatus& s,
		const std::string& b,
		const std::map<std::string, std::string>& h)
		: status(s), headers(h), body(b) {}
 
Response::Response(const Response& other): 
			status(other.status),  
			headers(other.headers), 
			body(other.body) {}

Response& Response::operator=(const Response& other) {
	if (this != &other) {
		status = other.status;
		headers = other.headers;
		body = other.body;
	}
	return *this;
}

Response::~Response() {}

HttpStatus Response::getStatus() const { return status; }
const std::map<std::string, std::string>& Response::getHeaders() const { return headers; }
const std::string& Response::getBody() const { return body; }

void Response::setStatus(HttpStatus code) { status = code; }
void Response::setHeaders(const std::map<std::string, std::string>& h) { headers = h; }
void Response::setBody(const std::string& b) { body = b; }

std::string Response::toString() const {
	std::ostringstream oss;
	oss << "HTTP/1.0 " << status.code << " " << status.reason << "\r\n";
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
		oss << it->first << ": " << it->second << "\r\n";
	oss << "\r\n" << body;
	return oss.str();
}

std::map<std::string, std::string> Response::getDefaultHeaders() {
	std::map<std::string, std::string> h;
	h["Content-Type"] = "text/html";
	return h;
}
