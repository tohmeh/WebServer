/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-naki <mal-naki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 20:03:03 by mal-naki          #+#    #+#             */
/*   Updated: 2025/10/14 20:03:03 by mal-naki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP
#include "webserv.hpp"
#include "Token.hpp"
#include "ConfigAST.hpp"
#include "Node.hpp"

#include "webserv.hpp"
#include "Token.hpp"
#include "Node.hpp"
#include "ConfigAST.hpp"

// *Represents a location block configuration

struct LocationConf
{
    bool autoindex;
    std::string path;
    std::string root;
    bool upload_enable;
    std::string redirect;
    std::string upload_store;
    size_t client_max_body_size;
    std::vector<std::string> index;
    std::vector<std::string> methods;
    std::map<int, std::string> error_pages;
    std::map<std::string, std::string> cgi_pass;
};

// *Represents a server block configuration
struct ServerConf
{
    std::string root;
    std::vector<int> ports;
    size_t client_max_body_size;
    std::vector<std::string> index;
    std::vector<std::string> serverName;
    std::vector<LocationConf> locations;
    std::map<int, std::string> error_pages;
    std::map<std::string, std::string> cgi_pass;
};

//* Represents the HTTP configuration containing all servers
struct HttpConf
{
    std::vector<ServerConf> servers;
};

/*
 *Parser class to convert config files/strings into AST
 */
class Parser
{
private:
    std::string _src;
    size_t _i;
    size_t _line;   //? For Error Messages
    size_t _column; //? For Error Messages
    bool _hasLookaHead;
    Token _lookaHead;

    std::string parseUnquoted();
    std::string parseQuoted(char ch);

    static bool isIdentChar(int ch);

    Token next();
    Token peek();
    bool match(TokenType Token);
    void reset(const std::string &text);

    Node parseDirective();
    std::vector<Node> parseBlock();

    void syntaxError(const std::string &msg, size_t line, size_t column) const;

public:
    Parser();

    ConfigAST parseFile(const std::string &path);
    ConfigAST parseString(const std::string &text);
};

#endif //! PARSER

//! Mohamad AL-Nakib | Webserve