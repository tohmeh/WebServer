#include "PostHandler.hpp"
#include "webserv.hpp"
#include "Utils.hpp"

PostHandler::~PostHandler() {}

static Response createErrorResponse(const ResolvedPath& resolvedPath, HttpStatus status) {
    std::string errorPage = loadErrorPage(resolvedPath.server, resolvedPath.location, status.code);
    if (!errorPage.empty()) {
        return Response(status, errorPage);
    }
    return Response(status, status.reason);
}

Response PostHandler::handle(const Request& req, const ResolvedPath& resolvedPath) {
    if (!resolvedPath.allowsMethod("POST")) {
        return createErrorResponse(resolvedPath, Status::MethodNotAllowed);
    }

    if (resolvedPath.isCgi) {
        std::string output = handleCgiPart(req, resolvedPath);
        return Response(Status::Created, output);
    }

    if (resolvedPath.location->upload_enable) {
        return handleUpload(req, resolvedPath);
    }

    return createErrorResponse(resolvedPath, Status::Forbidden);
}

Response PostHandler::handleUpload(const Request& req, const ResolvedPath& resolvedPath) {
    std::string uploadDir = resolvedPath.location->upload_store;
    if (uploadDir.empty()) {
        return createErrorResponse(resolvedPath, Status::InternalServerError);
    }

    bool success = saveUploadedFile(req, uploadDir);
    if (!success) {
        return createErrorResponse(resolvedPath, Status::BadRequest);
    }

    std::string disposition = req.getHeader("Content-Disposition");
    std::string filename = extractFilename(disposition);
    return Response(Status::Created, "File'" + filename + "' uploaded successfully");
}

bool PostHandler::saveUploadedFile(const Request& req, const std::string& uploadDir) {
    struct stat st;
    if (stat(uploadDir.c_str(), &st) != 0 || !S_ISDIR(st.st_mode)) return false;

    std::string contentType = req.getHeader("Content-Type");
    std::string filename;

    if (contentType.find("multipart/form-data") != std::string::npos) {
        size_t pos = contentType.find("boundary=");
        if (pos == std::string::npos) return false;
        std::string boundary = "--" + contentType.substr(pos + 9); 
        std::string body = req.getBody();

        size_t searchPos = 0;
        bool anySaved = false;
        
        while (true) {
            size_t partStart = body.find(boundary, searchPos);
            if (partStart == std::string::npos) break;
            partStart += boundary.size();
            if (body.compare(partStart, 2, "\r\n") == 0)
                partStart += 2;
            size_t partEnd = body.find(boundary, partStart);
            if (partEnd == std::string::npos) break;
            if (body.compare(partEnd - 2, 2, "--") == 0)
                break;
            std::string part = body.substr(partStart, partEnd - partStart);
            filename = extractFilenameFromPart(part);
            if (filename.empty()) filename = "upload_" + ft_itos(time(NULL)) + ".dat";
            std::string fileData = extractPartBody(part);
            if (!fileData.empty()) {
                std::ofstream out((uploadDir + "/" + filename).c_str(), std::ios::binary);
                if (out.is_open()) {
                    out.write(fileData.c_str(), fileData.size());
                    anySaved = true;
                }
            }
            searchPos = partEnd;
        }
        return anySaved;
    }

    std::string ext = getExtensionFromMime(contentType);
    if (!ext.empty() && ext[0] != '.') ext = "." + ext;
    filename = "upload_" + ft_itos(time(NULL)) + (ext.empty() ? ".dat" : ext);

    std::ofstream out((uploadDir + "/" + filename).c_str(), std::ios::binary);
    if (!out.is_open()) return false;

    const std::string& body = req.getBody();
    out.write(body.c_str(), body.size());
    return out.good();
}
