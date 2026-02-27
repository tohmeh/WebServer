#ifndef RESPONSE_HPP
#define RESPONSE_HPP

# include "webserv.hpp"
# include "Request.hpp"
# include "HttpStatus.hpp"

class Response {
	private:
		HttpStatus							status;
		std::map<std::string, std::string>	headers;
		std::string							body;
	public:
		Response();
		Response(const HttpStatus& s, 
			const std::string& body = "",
			const std::map<std::string, std::string>& headers = getDefaultHeaders());
		Response(const Response& other);
		Response& operator=(const Response& other);
		~Response();
		
		// Getters
		static std::map<std::string, std::string> getDefaultHeaders();
		HttpStatus getStatus() const;
		const std::map<std::string, std::string>& getHeaders() const;
		const std::string& getBody() const;

		// Setters
		void setStatus(HttpStatus code);
		void setHeaders(const std::map<std::string, std::string>& h);
		void setBody(const std::string& b);

		std::string toString() const;
};

#endif //! RESPONSE_HPP
