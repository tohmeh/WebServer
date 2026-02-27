/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-naki <mal-naki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 20:01:27 by mal-naki          #+#    #+#             */
/*   Updated: 2025/10/15 19:12:32 by mal-naki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Utils.hpp"

static std::string getFirstOrEmpty(const std::vector<std::string> &v)
{
    if (v.empty())
        return ("");
    else
        return (v[0]);
}

static std::string joinArgs(const std::vector<std::string> &v)
{
    size_t i = 0;
    std::string out;

    while (i < v.size())
    {
        if (i != 0)
            out += " ";
        out += v[i];
        ++i;
    }
    return (out);
}

LocationConf initLocation(const Node &Loc)
{
    LocationConf L;

    if (Loc.name != "location")
        throw std::runtime_error("Error -> Not a location node");

    if (Loc.args.empty())
        L.path = "/";
    else
        L.path = Loc.args[0];

    //? Init default 1MB
    L.client_max_body_size = 1048576;

    bool rootFound = false;
    bool indexFound = false;
    bool methodsFound = false;
    size_t i = 0;
    while (i < Loc.children.size())
    {
        const Node &n = Loc.children[i];
        if (n.name == "root")
        {
            if (rootFound)
                throw std::runtime_error("Error -> Duplicate 'root' directive in location block");
            if (n.args.size() != 1)
                throw std::runtime_error("Error -> 'root' directive in location block must have exactly one argument");
            rootFound = true;
            L.root = getFirstOrEmpty(n.args);
        }
        else if (n.name == "index")
        {
            if (indexFound)
                throw std::runtime_error("Error -> Duplicate 'index' directive in location block");
            if (n.args.empty())
                throw std::runtime_error("Error -> 'index' directive in location block must have at least one argument");
            indexFound = true;
            L.index = n.args;
        }
        else if (n.name == "limit_except")
        {
            if (methodsFound)
                throw std::runtime_error("Error -> Duplicate 'limit_except' directive in location block");
            if (n.args.empty())
                throw std::runtime_error("Error -> 'limit_except' directive in location block must have at least one argument");
            static const char *validMethods[] = {"GET", "POST", "DELETE"}; // TODO Make as enum
            size_t mi = 0;
            while (mi < n.args.size())
            {
                bool found = false;
                size_t vi = 0;
                while (vi < sizeof(validMethods) / sizeof(validMethods[0]))
                {
                    if (n.args[mi] == validMethods[vi])
                    {
                        found = true;
                        break;
                    }
                    ++vi;
                }
                if (!found)
                    throw std::runtime_error("Error -> Invalid HTTP method in 'limit_except': " + n.args[mi]);

                ++mi;
            }

            methodsFound = true;
            L.methods = n.args;
        }
        else if (n.name == "cgi_pass")
        {
            if (n.args.size() != 2)
                throw std::runtime_error("Error -> 'cgi_pass' must have extension and handler");
            L.cgi_pass[n.args[0]] = n.args[1];
        }
        else if (n.name == "error_page")
        {
            if (n.args.size() != 2)
                throw std::runtime_error("Error -> 'error_page' must have code and file");
            int code = ft_stoi(n.args[0]);
            L.error_pages[code] = n.args[1];
        }
        else if (n.name == "return")
        {
            if (n.args.empty())
                throw std::runtime_error("Error -> 'return' directive must have at least one argument");
            L.redirect = joinArgs(n.args);
        }
        else if (n.name == "autoindex")
        {
            if (n.args.size() != 1)
                throw std::runtime_error("Error -> 'autoindex' must have one argument");
            L.autoindex = (n.args[0] == "on");
        }
        else if (n.name == "upload_enable")
        {
            if (n.args.size() != 1)
                throw std::runtime_error("Error -> 'upload_enable' must have one argument");
            L.upload_enable = (n.args[0] == "on");
        }
        else if (n.name == "upload_store")
        {
            if (n.args.size() != 1)
                throw std::runtime_error("Error -> 'upload_store' must have one argument");
            L.upload_store = n.args[0];
        }
        else if (n.name == "client_max_body_size")
        {
            if (n.args.size() != 1)
                throw std::runtime_error("Error -> 'client_max_body_size' must have one argument");
            L.client_max_body_size = ft_stoi(n.args[0]);
        }
        else
        {
            throw std::runtime_error("Error -> Unknown block directive '" + n.name + "' in location block");
        }
        ++i;
    }
    if (L.root.empty())
    {
        throw std::runtime_error("Error -> 'root' directive is required in location block");
    }
    return (L);
}

ServerConf initServer(const Node &server)
{
    if (server.name != "server")
        throw std::runtime_error("Error -> Not a server node");

    ServerConf S;

    // default 1MB
    S.client_max_body_size = 1048576;

    bool rootFound = false;
    bool indexFound = false;
    bool listenFound = false;
    bool serverNameFound = false;
    size_t i = 0;
    while (i < server.children.size())
    {
        const Node &n = server.children[i];
        if (n.name == "listen")
        {
            if (listenFound)
                throw std::runtime_error("Error -> Duplicate 'listen' directive in server block");
            if (n.args.empty())
                throw std::runtime_error("Error -> 'listen' directive in server block must have at least one argument");

            size_t li = 0;
            while (li < n.args.size())
            {
                const std::string &arg = n.args[li];
                size_t colon = arg.find(":");
                std::string portStr = (colon == std::string::npos) ? arg : arg.substr(colon + 1);
                int port = ft_stoi(portStr);
                if (port < 1 || port > 65535)
                    throw std::runtime_error("Error -> Invalid port in 'listen' directive: " + arg);
                bool found = false;
                for (size_t pi = 0; pi < S.ports.size(); ++pi)
                {
                    if (S.ports[pi] == port)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                    S.ports.push_back(port);
                ++li;
            }
            listenFound = true;
        }
        else if (n.name == "server_name")
        {
            if (serverNameFound)
                throw std::runtime_error("Error -> Duplicate 'server_name' directive in server block");
            if (n.args.empty())
                throw std::runtime_error("Error -> 'server_name' directive in server block must have at least one argument");
            serverNameFound = true;
            S.serverName.insert(S.serverName.end(), n.args.begin(), n.args.end());
        }
        else if (n.name == "root")
        {
            if (rootFound)
                throw std::runtime_error("Error -> Duplicate 'root' directive in server block");
            if (n.args.size() != 1)
                throw std::runtime_error("Error -> 'root' directive in server block must have exactly one argument");
            rootFound = true;
            S.root = getFirstOrEmpty(n.args);
        }
        else if (n.name == "index")
        {
            if (indexFound)
                throw std::runtime_error("Error -> Duplicate 'index' directive in server block");
            if (n.args.empty())
                throw std::runtime_error("Error -> 'index' directive in server block must have at least one argument");
            indexFound = true;
            S.index = n.args;
        }
        else if (n.name == "location")
        {
            S.locations.push_back(initLocation(n));
        }
        else if (n.name == "error_page")
        {
            if (n.args.size() != 2)
                throw std::runtime_error("Error -> 'error_page' must have code and file");
            int code = ft_stoi(n.args[0]);
            S.error_pages[code] = n.args[1];
        }
        else if (n.name == "cgi_pass")
        {
            if (n.args.size() != 2)
                throw std::runtime_error("Error -> 'cgi_pass' must have extension and handler");
            S.cgi_pass[n.args[0]] = n.args[1];
        }
        else if (n.name == "client_max_body_size")
        {
            if (n.args.size() != 1)
                throw std::runtime_error("Error -> 'client_max_body_size' must have one argument");
            S.client_max_body_size = ft_stoi(n.args[0]);
        }
        else
            throw std::runtime_error("Error -> Unknown block directive '" + n.name + "' in server block");
        ++i;
    }
    if (S.ports.empty())
        throw std::runtime_error("Error -> 'listen' directive is required in server block");
    if (S.root.empty())
        throw std::runtime_error("Error -> 'root' directive is required in server block");
    return (S);
}

HttpConf initHttp(const ConfigAST &ast)
{
    HttpConf H;

    const Node *http = ast.findFirst("http");
    if (!http)
        throw std::runtime_error("Error -> No `http { ... }` block found");

    size_t i = 0;
    while (i < http->children.size())
    {
        const Node &n = http->children[i];

        if (n.name == "server")
            H.servers.push_back(initServer(n));
        else if (n.name == "location")
        {
            std::ostringstream oss;
            oss << "Error -> 'location' directive is not allowed at http block level (must be inside server) at line " << n.line << ", column " << n.column;
            throw std::runtime_error(oss.str());
        }
        else
        {
            throw std::runtime_error("Error -> Unknown directive '" + n.name + "' in http block");
        }
        ++i;
    }
    return (H);
}

//! Mohamad AL-Nakib | Webserve
