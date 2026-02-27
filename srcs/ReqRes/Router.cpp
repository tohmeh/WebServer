#include "webserv.hpp"
#include "Router.hpp"
#include "GetHandler.hpp"
#include "PostHandler.hpp"
#include "DeleteHandler.hpp"

Router::Router() {
	methods["GET"] = new GetHandler();
	methods["POST"] = new PostHandler();
	methods["DELETE"] = new DeleteHandler();
}

Router::~Router() {
	for (std::map<std::string, RequestHandler*>::iterator it = methods.begin(); it!= methods.end(); ++it)
		delete it->second;
}

Response Router::dispatch(const ServerConf& server, const Request& req) {
	if (methods.find(req.getMethod()) != methods.end())
	{
		std::string path = req.getMethod();

		ResolvedPath resolvedPath = resolve(server, req);

		return methods[req.getMethod()]->handle(req, resolvedPath);
	}
	return Response(Status::MethodNotAllowed);
}
