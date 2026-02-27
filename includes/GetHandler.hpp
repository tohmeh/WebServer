#ifndef GETHANDLER_HPP
#define GETHANDLER_HPP

# include "RequestHandler.hpp"
# include "Response.hpp"
# include "Request.hpp" 

class GetHandler: public RequestHandler {
	public:
		virtual Response handle(const Request& req, const ResolvedPath& resolvedPath);
		Response serveDirectory(const Request& req, const std::string& path, const ResolvedPath& resolvedPath);
		Response serveFile(const std::string& path, const ResolvedPath& resolvedPath);
		~GetHandler();
};

#endif
