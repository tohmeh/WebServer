#include "Utils.hpp"
#include "webserv.hpp"

bool fileExists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0 && S_ISREG(buffer.st_mode));
}

bool dirExists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode));
}

static const LocationConf* findBestLocation(const ServerConf& server, const std::string& uri) {
    const LocationConf* best = NULL;
    size_t bestLen = 0;

    for(size_t i = 0; i < server.locations.size(); ++i) {
        const LocationConf& loc = server.locations[i];
        if (uri.find(loc.path) == 0 && loc.path.length() > bestLen) {
            best = &loc;
            bestLen = loc.path.length();
        }
    }
    return best;
}

static RootResolution resolveRootAndUri(const ServerConf& server, const std::string& uri, const LocationConf* location)  {
    RootResolution rr;
    
    if (location && !location->root.empty()) {
        rr.root = location->root;
        rr.relativeUri = uri.substr(location->path.length());
    } else {
        rr.root = server.root;
        rr.relativeUri = uri;
    }
    return rr;
}

std::string joinPath(const std::string& root, const std::string& relative)  {
    if (root.empty()) return relative;
    if (relative.empty()) return root;
    if (root[root.size() - 1] == '/' && relative[0] == '/')
        return root + relative.substr(1);
    if (root[root.size() - 1] != '/' && relative[0] != '/')
        return root + "/" + relative;
    return root + relative;
}

ResolvedPath resolve(const ServerConf &server, const Request& req)  {
    std::string uri = req.getUri();
    uri = urlDecode(uri);
    const LocationConf* loc = findBestLocation(server, uri);
    RootResolution rr = resolveRootAndUri(server, uri, loc);

    std::string filesystemPath = joinPath(rr.root, rr.relativeUri);

    ResolvedPath rp;
    rp.filesystemPath = filesystemPath;
    rp.location = loc;
    rp.server = &server;

    rp.isCgi = false;
    if (loc && !loc->cgi_pass.empty()) {
        size_t dotPos = filesystemPath.find_last_of('.');
        if (dotPos != std::string::npos) {
            std::string ext = filesystemPath.substr(dotPos);
            if (loc->cgi_pass.find(ext) != loc->cgi_pass.end()) {
                rp.isCgi = true;
                rp.filesystemPath = filesystemPath;
                rp.interpreter = loc->cgi_pass.find(ext)->second;
            }
        }
    }
    return rp;
}

std::string generateAutoIndex(const std::string& path, const std::string& uri) {
    std::ostringstream html;
    
    html << "<!DOCTYPE html>\n<html>\n<head>\n";
    html << "<title>Index of " << uri << "</title>\n</head>\n<body>\n";
    html << "<h1>Index of " << uri << "</h1>\n<ul>\n";

    DIR* dir = opendir(path.c_str());
    if (!dir) {
        return "<h1>Could not open directory</h1>";
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        std::string name = entry->d_name;
        if (name == "." || name == "..") continue;

        std::string href = uri;
        if (uri[uri.size() - 1] != '/') href += "/";
        href += name;

        if (entry->d_type == DT_DIR) {
            href += "/";
            name += "/";
        }
        html << "<li><a href=\"" << href << "\">" << name << "</a></li>\n";
    }
    closedir(dir);
    html << "</ul>\n</body>\n</html>\n";
    return html.str();
}

bool ResolvedPath::allowsMethod(const std::string& method) const {
    if (!location) return false;
    const std::vector<std::string> &methods = location->methods;
    if (methods.empty()) return true;
    for (size_t i = 0; i < methods.size(); ++i) {
        if (methods[i] == method) return true;
    }
    return false;
}

static std::map<std::string, std::string> initMimeTypes() {
    std::map<std::string, std::string> m;
    m[".html"] = "text/html";
    m[".htm"]  = "text/html";
    m[".txt"]  = "text/plain";
    m[".css"]  = "text/css";
    m[".js"]   = "application/javascript";
    m[".json"] = "application/json";
    m[".png"]  = "image/png";
    m[".jpg"]  = "image/jpeg";
    m[".gif"]  = "image/gif";
    m[".ico"]  = "image/x-icon";
    m[".pdf"]  = "application/pdf";
    m[".zip"]  = "application/zip";
    m[".xml"]  = "application/xml";
    return m;
}

const std::map<std::string, std::string> MimeTypes::mimeTypes = initMimeTypes();

std::string MimeTypes::get(const std::string& path) {
    std::string ext;
    size_t dotPos = path.find_last_of('.');
    if (dotPos != std::string::npos) {
        ext = path.substr(dotPos);
    }
    std::map<std::string, std::string>::const_iterator it = mimeTypes.find(ext);
    if (it != mimeTypes.end()) {
        return it->second;
    }
    return "application/octet-stream";
}

std::string getExtensionFromMime(const std::string& mime) {
    for (std::map<std::string, std::string>::const_iterator it = MimeTypes::mimeTypes.begin(); it != MimeTypes::mimeTypes.end(); ++it) {
        if (it->second == mime) {
            return it->first;
        }
    }
    return "";
}

std::string handleCgiPart(const Request& req, const ResolvedPath& resolvedPath) {
    std::string scriptPath = resolvedPath.cgiPath;
    std::map<std::string, std::string> env;
    std::stringstream ss;
    ss << req.getBody().size();
    std::string contentLength = ss.str();
    env["REQUEST_METHOD"] = req.getMethod();
    env["CONTENT_LENGTH"] = contentLength;
    env["SCRIPT_FILENAME"] = resolvedPath.filesystemPath;
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["REDIRECT_STATUS"] = "200";
    env["PATH_INFO"] = resolvedPath.cgiPath;
    env["SERVER_PROTOCOL"] = req.getVersion();
    env["CONTENT_TYPE"] = req.getHeader("Content-Type");
    env["QUERY_STRING"] = req.getQueryString();
    env["SERVER_SOFTWARE"] = "webserv/1.0";

    CGI cgi(resolvedPath.filesystemPath, resolvedPath.interpreter, env);
    std::string result = "";
    try
    {
        result = cgi.execute(req.getBody());
    }
    catch (const std::exception &e)
    {
        std::cerr << "CGI execution failed: " << e.what() << std::endl;
    }
    return result;
}

std::string loadErrorPage(const ServerConf* server, const LocationConf* location, int statusCode) {
    std::string errorPagePath;

    // Try location error_pages first
    if (location) {
        std::map<int, std::string>::const_iterator it = location->error_pages.find(statusCode);
        if (it != location->error_pages.end()) {
            errorPagePath = it->second;
        }
    }

    // Fall back to server error_pages
    if (errorPagePath.empty() && server) {
        std::map<int, std::string>::const_iterator it = server->error_pages.find(statusCode);
        if (it != server->error_pages.end()) {
            errorPagePath = it->second;
        }
    }

    // If no error page configured or file doesn't exist, return empty string
    if (errorPagePath.empty()) {
        return "";
    }

    // Make path relative to server root if it starts with /
    std::string fullPath = errorPagePath;
    if (!errorPagePath.empty() && errorPagePath[0] == '/' && server && !server->root.empty()) {
        fullPath = server->root + errorPagePath;
    }

    // Try to read the error page file
    std::ifstream file(fullPath.c_str(), std::ios::in | std::ios::binary);
    if (!file) {
        return "";
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// String to integer conversion (replacement for atoi)
int ft_stoi(const std::string& str)
{
    int result = 0;
    int sign = 1;
    size_t i = 0;

    // Skip whitespace
    while (i < str.length() && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
        i++;

    // Handle sign
    if (i < str.length() && (str[i] == '+' || str[i] == '-'))
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }

    // Convert digits
    while (i < str.length() && str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    return result * sign;
}

// Integer to string conversion
std::string ft_itos(int n)
{
    if (n == 0)
        return "0";

    std::string result;
    bool negative = false;

    if (n < 0)
    {
        negative = true;
        n = -n;
    }

    while (n > 0)
    {
        result = static_cast<char>('0' + (n % 10)) + result;
        n /= 10;
    }

    if (negative)
        result = "-" + result;

    return result;
}

void ft_putstr_fd(const std::string& str, int fd)
{
    write(fd, str.c_str(), str.length());
}

void ft_putendl_fd(const std::string& str, int fd)
{
    write(fd, str.c_str(), str.length());
    write(fd, "\n", 1);
}

void ft_error(const std::string& prefix)
{
    std::string error_msg = prefix + ": " + strerror(errno) + "\n";
    write(STDERR_FILENO, error_msg.c_str(), error_msg.length());
}

std::string extractFilename(const std::string& disposition) {
    size_t pos = disposition.find("filename=");
    if (pos == std::string::npos) return "";
    pos += 9;
    if (disposition[pos] == '\"') ++pos;
    size_t end = disposition.find('\"', pos);
    if (end == std::string::npos) end = disposition.size();
    return disposition.substr(pos, end - pos);
}

std::string extractFileBody(const std::string& body, const std::string& boundary) {
    std::string start = "--" + boundary;
    size_t fileStart = body.find("\r\n\r\n");
    if (fileStart == std::string::npos) return "";
    fileStart += 4;
    size_t fileEnd = body.find(start, fileStart);
    if (fileEnd == std::string::npos) fileEnd = body.size();
    return body.substr(fileStart, fileEnd - fileStart - 2);
}

std::string extractFilenameFromPart(const std::string& part) {
    size_t headerEnd = part.find("\r\n\r\n");
    if (headerEnd == std::string::npos) return "";

    std::string headers = part.substr(0, headerEnd);
    std::istringstream stream(headers);
    std::string line;
    while (std::getline(stream, line)) {
        if (!line.empty() && line[line.size() - 1] == '\r') line.erase(line.size() - 1);

        std::string key = "Content-Disposition:";
        if (line.compare(0, key.size(), key) == 0) {
            size_t fnamePos = line.find("filename=\"");
            if (fnamePos != std::string::npos) {
                fnamePos += 10; 
                size_t endQuote = line.find("\"", fnamePos);
                if (endQuote != std::string::npos) {
                    return line.substr(fnamePos, endQuote - fnamePos);
                }
            }
        }
    }
    return "";
}

std::string extractPartBody(const std::string& part) {
    size_t pos = part.find("\r\n\r\n");
    if (pos == std::string::npos) return "";
    return part.substr(pos + 4);
} 
int hexDigit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
    return -1;
}

char hexToChar(char high, char low) {
    int hi = hexDigit(high);
    int lo = hexDigit(low);
    if (hi == -1 || lo == -1) throw std::runtime_error("Invalid percent-encoding");
    return static_cast<char>((hi << 4) | lo);
}

std::string urlDecode(const std::string& encoded) {
    std::string result;
    for (size_t i = 0; i < encoded.size(); ++i) {
        if (encoded[i] == '%') {
            if (i + 2 >= encoded.size()) throw std::runtime_error("Incomplete percent-encoding");
            result += hexToChar(encoded[i + 1], encoded[i + 2]);
            i += 2;
        } else if (encoded[i] == '+') {
            result += ' '; 
        } else {
            result += encoded[i];
        }
    }
    return result;
}