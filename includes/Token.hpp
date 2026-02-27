/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-naki <mal-naki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 20:03:20 by mal-naki          #+#    #+#             */
/*   Updated: 2025/10/14 20:03:20 by mal-naki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN
#define TOKEN

#include "webserv.hpp"

// *Types of tokens in the configuration
enum TokenType
{
    T_IDENT,  //? Word
    T_STRING, //? Quoted Word
    T_LBRACE, //? {
    T_RBRACE, //? }
    T_SEMI,   //? ;
    T_EOF     //? End of File
};

/*
 *Represents a token in the parser
 */
class Token
{
public:
    TokenType type;
    std::string value;
    size_t line;
    size_t column;

    Token();
    Token(TokenType t, const std::string &v, size_t l, size_t c);
};

#endif //! TOKEN

//! Mohamad AL-Nakib | Webserve