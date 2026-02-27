#ifndef POSTHANDLER_HPP
#define POSTHANDLER_HPP

#include "RequestHandler.hpp"
#include "Response.hpp"
#include "Request.hpp"	

class PostHandler: public RequestHandler {
	private:
		Response handleUpload(const Request& req, const ResolvedPath& resolvedPath);
	public: 
		bool saveUploadedFile(const Request& req, const std::string& uploadDir);
		Response handle(const Request& req, const ResolvedPath& resolvedPath);
		~PostHandler();
};

#endif
