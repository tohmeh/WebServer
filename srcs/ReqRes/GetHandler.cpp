/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gakhoury <gakhoury@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 21:33:48 by gakhoury          #+#    #+#             */
/*   Updated: 2025/10/13 19:48:14 by gakhoury         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GetHandler.hpp"
#include "webserv.hpp"
#include "HttpStatus.hpp"
#include "CGI.hpp"
#include "Utils.hpp"

GetHandler::~GetHandler() {}

static Response createErrorResponse(const ResolvedPath& resolvedPath, HttpStatus status) {
    std::string errorPage = loadErrorPage(resolvedPath.server, resolvedPath.location, status.code);
    if (!errorPage.empty()) {
        return Response(status, errorPage);
    }
    return Response(status, status.reason);
}

Response GetHandler::handle(const Request& req, const ResolvedPath& resolvedPath) {
    const std::string& path = resolvedPath.filesystemPath;
    static const std::map<std::string, std::string> emptyHeaders;

    if (!fileExists(path) && !dirExists(path)) {
        return createErrorResponse(resolvedPath, Status::NotFound);
    }

    if (resolvedPath.isCgi) {
        std::string body = handleCgiPart(req, resolvedPath);
        return Response(Status::OK, body);
    }

    if (fileExists(path)) {
        return serveFile(path, resolvedPath);
    }

    else if (dirExists(path)) {
        return serveDirectory(req, path, resolvedPath);
    }

    return createErrorResponse(resolvedPath, Status::InternalServerError);
}

Response GetHandler::serveFile(const std::string& path, const ResolvedPath& resolvedPath) {
    static const std::map<std::string, std::string> emptyHeaders;

    if (!isReadable(path)) {
        return createErrorResponse(resolvedPath, Status::Forbidden);
    }

    std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
    if (!file) {
        return createErrorResponse(resolvedPath, Status::InternalServerError);
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string body = buffer.str();

    std::map<std::string, std::string> headers;
    std::ostringstream len;
    len << body.size();
    headers["Content-Length"] = len.str();
    headers["Content-Type"] = MimeTypes::get(path);

    return Response(Status::OK, body, headers);
}

Response GetHandler::serveDirectory(const Request& req, const std::string& path, const ResolvedPath& resolvedPath) {
    static const std::map<std::string, std::string> emptyHeaders;

    if (req.getUri().empty() || req.getUri()[req.getUri().size() - 1] != '/') {
        std::map<std::string, std::string> headers;
        headers["Location"] = req.getUri() + "/";
        return Response(Status::MovedPermanently, "Redirecting...");
    }

    // Determine which index list to use: location or server fallback
    const std::vector<std::string>* indexList = NULL;
    if (resolvedPath.location && !resolvedPath.location->index.empty()) {
        indexList = &resolvedPath.location->index;
    } else if (resolvedPath.server && !resolvedPath.server->index.empty()) {
        indexList = &resolvedPath.server->index;
    }

    // Try to find and serve an index file
    if (indexList) {
        for (std::vector<std::string>::const_iterator it = indexList->begin(); it != indexList->end(); ++it) {
            std::string indexPath = path + "/" + *it;
            if (fileExists(indexPath) && isReadable(indexPath)) {
                return serveFile(indexPath, resolvedPath);
            }
        }
    }

    // If autoindex is enabled, generate directory listing
    if (resolvedPath.location && resolvedPath.location->autoindex) {
        std::string listing = generateAutoIndex(path, req.getUri());
        std::map<std::string, std::string> headers;
        std::ostringstream len;
        len << listing.size();
        headers["Content-Length"] = len.str();
        headers["Content-Type"] = "text/html";
        return Response(Status::OK, listing, headers);
    }
    return createErrorResponse(resolvedPath, Status::Forbidden);
}
