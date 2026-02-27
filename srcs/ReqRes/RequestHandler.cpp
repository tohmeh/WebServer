#include "RequestHandler.hpp"

bool RequestHandler::isReadable(const std::string& path) {
    return (access(path.c_str(), R_OK) == 0);
}
