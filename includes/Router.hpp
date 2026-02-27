#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <map>
#include <string>
#include "Parser.hpp"

class RequestHandler;
class Response;
class Request;

class Router {
public:
	std::map<std::string, RequestHandler*> methods;
	Router();
	~Router();
	Response dispatch(const ServerConf& server, const Request& req);
};

#endif
