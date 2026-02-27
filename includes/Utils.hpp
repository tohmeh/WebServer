#ifndef UTILS_HPP
#define UTILS_HPP

#include "Parser.hpp"
#include "Request.hpp"
#include "CGI.hpp"

enum HttpMethod {
    METHOD_GET,
    METHOD_POST,
    METHOD_DELETE,
    METHOD_UNKNOWN
};

class MimeTypes {
    public: 
        static const std::map<std::string, std::string> mimeTypes;
        static std::string get(const std::string& filename);
};

struct ResolvedPath {
    std::string filesystemPath;
    const LocationConf* location;
    const ServerConf* server;
    
    bool isCgi;
    const std::string cgiPath;
    std::string interpreter;

    bool allowsMethod(const std::string& method) const;
};

struct RootResolution {
    std::string root;
    std::string relativeUri;
};

bool fileExists(const std::string& path);
bool dirExists(const std::string& path);

std::string joinPath(const std::string& root, const std::string& relative);
ResolvedPath resolve(const ServerConf &server, const Request& req);
std::string generateAutoIndex(const std::string& path, const std::string& uri);
std::string handleCgiPart(const Request& req, const ResolvedPath& resolvedPath);
std::string loadErrorPage(const ServerConf* server, const LocationConf* location, int statusCode);


// String to integer conversion (replacement for atoi)
int ft_stoi(const std::string& str);

// Integer to string conversion
std::string ft_itos(int n);

// Write string to file descriptor (replacement for printf/perror)
void ft_putstr_fd(const std::string& str, int fd);
void ft_putendl_fd(const std::string& str, int fd);

// Error message with strerror (replacement for perror)
void ft_error(const std::string& prefix);
std::string extractFileBody(const std::string& body, const std::string& boundary);
std::string extractFilename(const std::string& disposition);
std::string getExtensionFromMime(const std::string& mime);
std::string extractFilenameFromPart(const std::string& part);
std::string extractPartBody(const std::string& part);
char hexToChar(char high, char low);
std::string urlDecode(const std::string& encoded);

#endif // UTILS_HPP
