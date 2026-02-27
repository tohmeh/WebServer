#include "Request.hpp"
#include "webserv.hpp"

Request::Request()
	: method(""), uri(""), version(""), headers(), body("") {}

Request::Request(std::string method, std::string uri, std::string version, std::map<std::string, std::string> headers, std::string body)
	: method(method), uri(uri), version(version), headers(headers), body(body) {}

Request::Request(const Request& other)
	: method(other.method), uri(other.uri), version(other.version), headers(other.headers), body(other.body) {}
Request& Request::operator=(const Request& other) {
	if (this != &other) {
		method = other.method;
		uri = other.uri;
		version = other.version;
		headers = other.headers;
		body = other.body;
	}
	return *this;
}
Request::~Request() {}

const std::string& Request::getMethod() const { return method; }
const std::string& Request::getUri() const { return uri; }
const std::string& Request::getVersion() const { return version; }
const std::map<std::string, std::string>& Request::getHeaders() const { return headers; }
const std::string& Request::getBody() const { return body; }

void Request::setMethod(const std::string& m) { method = m; }
void Request::setUri(const std::string& u) { uri = u; }
void Request::setVersion(const std::string& v) { version = v; }
void Request::setHeaders(const std::map<std::string, std::string>& h) { headers = h; }
void Request::setBody(const std::string& b) { body = b; }

void Request::setHeader(const std::string& key, const std::string& value) {
	headers[key] = value;
}

std::string Request::getQueryString() const {
    size_t pos = uri.find('?');
    if (pos == std::string::npos)
        return "";
    return uri.substr(pos + 1);
}

std::string Request::getHeader(const std::string &key) const {
	std::map<std::string, std::string> headers = getHeaders();
    std::map<std::string, std::string>::const_iterator it = headers.find(key);
    if (it != headers.end())
        return it->second;
    return "";
}

Request Request::parseFromString(const std::string& rawRequest)
{
	Request request;

	if (rawRequest.empty())
		return request;

	// Find the separator between headers and body
	size_t headerBodySeparator = rawRequest.find("\r\n\r\n");
	std::string headerSection;
	std::string bodySection;

	if (headerBodySeparator != std::string::npos)
	{
		headerSection = rawRequest.substr(0, headerBodySeparator);
		bodySection = rawRequest.substr(headerBodySeparator + 4);
	}
	else
	{
		headerSection = rawRequest;
	}

	// Parse request line (first line)
	size_t firstLineEnd = headerSection.find("\r\n");
	std::string requestLine;
	std::string remainingHeaders;

	if (firstLineEnd != std::string::npos)
	{
		requestLine = headerSection.substr(0, firstLineEnd);
		remainingHeaders = headerSection.substr(firstLineEnd + 2);
	}
	else
	{
		requestLine = headerSection;
	}

	// Parse method, URI, and version
	size_t methodEnd = requestLine.find(' ');
	if (methodEnd != std::string::npos)
	{
		request.setMethod(requestLine.substr(0, methodEnd));

		size_t uriStart = methodEnd + 1;
		size_t uriEnd = requestLine.find(' ', uriStart);
		if (uriEnd != std::string::npos)
		{
			request.setUri(requestLine.substr(uriStart, uriEnd - uriStart));
			request.setVersion(requestLine.substr(uriEnd + 1));
		}
	}

	// Parse headers
	size_t pos = 0;
	while (pos < remainingHeaders.length())
	{
		size_t lineEnd = remainingHeaders.find("\r\n", pos);
		if (lineEnd == std::string::npos)
			lineEnd = remainingHeaders.length();

		std::string headerLine = remainingHeaders.substr(pos, lineEnd - pos);
		if (!headerLine.empty())
		{
			size_t colonPos = headerLine.find(':');
			if (colonPos != std::string::npos)
			{
				std::string key = headerLine.substr(0, colonPos);
				std::string value = headerLine.substr(colonPos + 1);

				// Trim leading whitespace from value
				size_t valueStart = 0;
				while (valueStart < value.length() && (value[valueStart] == ' ' || value[valueStart] == '\t'))
					valueStart++;
				value = value.substr(valueStart);

				request.setHeader(key, value);
			}
		}

		pos = lineEnd + 2;
	}

	// Set body if exists
	if (!bodySection.empty())
	{
		request.setBody(bodySection);
	}

	return request;
}
