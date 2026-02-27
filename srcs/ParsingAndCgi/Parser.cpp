/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-naki <mal-naki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 20:02:02 by mal-naki          #+#    #+#             */
/*   Updated: 2025/10/14 20:02:02 by mal-naki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Parser::Parser() : _i(0), _line(1), _column(1), _hasLookaHead(false), _lookaHead(Token(T_EOF, "", 0, 0)) {}

static bool isSpace(int ch) { return (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n'); }

bool Parser::isIdentChar(int ch) { return !(isSpace(ch) || ch == '{' || ch == '}' || ch == ';' || ch == '#' || ch == '\0'); }

ConfigAST Parser::parseFile(const std::string &path)
{
    std::ifstream in(path.c_str());

    if (!in)
        throw std::runtime_error("Error -> Can't open config file " + path);
    //? returns a pointer to the file’s stream buffer
    std::ostringstream ss;
    ss << in.rdbuf();
    return (parseString(ss.str()));
}

ConfigAST Parser::parseString(const std::string &text)
{
    reset(text);
    ConfigAST ast;
    while (true)
    {
        Token t = peek();
        if (t.type == T_EOF)
            break;
        ast.roots.push_back(parseDirective());
    }
    return (ast);
}

void Parser::reset(const std::string &text)
{
    _i = 0;
    _line = 1;
    _column = 1;
    _src = text;
    _hasLookaHead = false;
}

Token Parser::peek()
{
    if (!_hasLookaHead)
    {
        _lookaHead = next();
        _hasLookaHead = true;
    }
    return (_lookaHead);
}

bool Parser::match(TokenType t)
{
    Token p = peek();
    if (p.type == t)
    {
        _hasLookaHead = false;
        return (true);
    }
    return (false);
}

Token Parser::next()
{
    while (_i < _src.size())
    {
        char ch = _src[_i];
        if (ch == '#')
        {
            while (_i < _src.size() && _src[_i] != '\n')
            {
                _i++;
                _column++;
            }
        }
        else if (isSpace(ch))
        {
            if (ch == '\n')
            {
                _line++;
                _i++;
                _column = 1;
            }
            else
            {
                _i++;
                _column++;
            }
        }
        else
            break;
    }

    if (_i >= _src.size())
        return (Token(T_EOF, "", _line, _column));

    char ch = _src[_i];

    if (ch == '{')
        return (Token(T_LBRACE, std::string(1, _src[_i++]), _line, _column++));

    if (ch == '}')
        return (Token(T_RBRACE, std::string(1, _src[_i++]), _line, _column++));

    if (ch == ';')
        return (Token(T_SEMI, std::string(1, _src[_i++]), _line, _column++));

    if (ch == '"' || ch == '\'')
    {
        size_t column = _column;
        size_t line = _line;
        std::string str = parseQuoted(ch);
        return (Token(T_STRING, str, line, column));
    }

    size_t column = _column;
    size_t line = _line;
    std::string str = parseUnquoted();
    return (Token(T_IDENT, str, line, column));
}

std::string Parser::parseUnquoted()
{
    std::string str;
    while (_i < _src.size() && isIdentChar(_src[_i]))
    {
        str.push_back(_src[_i]);
        _i++;
        _column++;
    }
    return (str);
}

std::string Parser::parseQuoted(char ch)
{
    _i++;
    _column++;
    std::string str;

    while (_i < _src.size())
    {
        char c = _src[_i++];
        _column++;
        if (c == ch)
            break;

        if (c == '\\' && _i < _src.size())
        {
            char n = _src[_i++];
            _column++;

            if (n == 'n')
                str.push_back('\n');
            else if (n == 't')
                str.push_back('\t');
            else
                str.push_back(n);
        }
        else
        {
            if (c == '\n')
            {
                _line++;
                _column = 1;
            }
            str.push_back(c);
        }

        if (_i >= _src.size())
            break;
    }
    return (str);
}

void Parser::syntaxError(const std::string &msg, size_t line, size_t column) const
{
    std::ostringstream oss;
    oss << "Error -> config syntax at line " << line
        << ", column " << column << " - " << msg;
    throw std::runtime_error(oss.str());
}

Node Parser::parseDirective()
{
    Token tokenName = peek();

    if (tokenName.type != T_IDENT)
        syntaxError("expected directive name", tokenName.line, tokenName.column);

    _hasLookaHead = false;

    Node node;
    node.line = tokenName.line;
    node.name = tokenName.value;
    node.column = tokenName.column;

    while (true)
    {
        Token t = peek();

        if (t.type == T_LBRACE || t.type == T_SEMI)
            break;

        if (t.type != T_IDENT && t.type != T_STRING)
            syntaxError("expected argument or block / semi", t.line, t.column);

        _hasLookaHead = false;

        node.args.push_back(t.value);
    }

    if (match(T_SEMI))
        return (node);

    if (!match(T_LBRACE))
        syntaxError("expected '{' or ';' after directive", peek().line, peek().column);

    node.children = parseBlock();

    if (!match(T_RBRACE))
        syntaxError("expected '}' to close block", peek().line, peek().column);

    return (node);
}

std::vector<Node> Parser::parseBlock()
{
    std::vector<Node> body;

    while (true)
    {
        Token t = peek();
        if (t.type == T_RBRACE || t.type == T_EOF)
            break;
        body.push_back(parseDirective());
    }

    return (body);
}

//! Mohamad AL-Nakib | Webserve
