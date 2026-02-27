#ifndef DELETEHANDLER_HPP
#define DELETEHANDLER_HPP

#include "webserv.hpp"
#include "RequestHandler.hpp"
#include "Response.hpp"
#include "Request.hpp"

class DeleteHandler: public RequestHandler {
	public:
		Response handle(const Request& req, const ResolvedPath& resolvedPath);
		~DeleteHandler();
};

#endif
