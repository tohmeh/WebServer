#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Request.hpp"
#include "Response.hpp"

class Request {
	private:
		std::string method;
		std::string uri;
		std::string version;
		std::map<std::string, std::string> headers;
		std::string body;

	public:
		Request();
		Request(std::string method, std::string uri, std::string version, std::map<std::string, std::string> headers, std::string body);
		Request(const Request& other);
		Request& operator=(const Request& other);
		~Request();

		const std::string& getMethod() const;
		const std::string& getUri() const;
		const std::string& getVersion() const;
		const std::map<std::string, std::string>& getHeaders() const;
		const std::string& getBody() const;

		void setMethod(const std::string& m);
		void setUri(const std::string& u);
		void setVersion(const std::string& v) ;
		void setHeaders(const std::map<std::string, std::string>& h);
		void setBody(const std::string& b) ;
		void setHeader(const std::string& key, const std::string& value);
		std::string getQueryString() const;
		std::string getHeader(const std::string &key) const;

		static Request parseFromString(const std::string& rawRequest);
};

#endif //! REQUEST_HPP