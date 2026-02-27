#include "DeleteHandler.hpp"
#include "webserv.hpp"
#include "Utils.hpp"

DeleteHandler::~DeleteHandler() {}

static Response createErrorResponse(const ResolvedPath& resolvedPath, HttpStatus status) {
    std::string errorPage = loadErrorPage(resolvedPath.server, resolvedPath.location, status.code);
    if (!errorPage.empty()) {
        return Response(status, errorPage);
    }
    return Response(status, status.reason);
}

Response DeleteHandler::handle(const Request& req, const ResolvedPath& resolvedPath) {
    if (!resolvedPath.allowsMethod("DELETE")) {
        return createErrorResponse(resolvedPath, Status::MethodNotAllowed);
    }

    const std::string& target = resolvedPath.filesystemPath;

    if (resolvedPath.location && !resolvedPath.location->root.empty()) {
        const std::string& root = resolvedPath.location->root;
        if (target.find(root) != 0) {
            return createErrorResponse(resolvedPath, Status::Forbidden);
        }
    }

    struct stat st;
    if (stat(target.c_str(), &st) != 0) {
        return createErrorResponse(resolvedPath, Status::NotFound);
    }

    if (S_ISREG(st.st_mode)) {
        if (access(target.c_str(), W_OK) != 0) {
            return createErrorResponse(resolvedPath, Status::Forbidden);
        }

        if (remove(target.c_str()) != 0) {
            return createErrorResponse(resolvedPath, Status::InternalServerError);
        }

        return Response(Status::NoContent, "File deleted successfully");
    } else if (S_ISDIR(st.st_mode)) {
        const std::string& uri = req.getUri();
        bool endsWithSlash = !uri.empty() && uri[uri.size() - 1] == '/';

        if (!endsWithSlash) {
            return createErrorResponse(resolvedPath, Status::Conflict);
        }
        if (access(target.c_str(), W_OK) != 0) {
            return createErrorResponse(resolvedPath, Status::Forbidden);
        }
        if (rmdir(target.c_str()) != 0) {
            return createErrorResponse(resolvedPath, Status::InternalServerError);
        }
        return Response(Status::NoContent, "Directory deleted successfully");
    }
    return createErrorResponse(resolvedPath, Status::Forbidden);
}
