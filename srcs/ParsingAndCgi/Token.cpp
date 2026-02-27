/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-naki <mal-naki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 20:02:07 by mal-naki          #+#    #+#             */
/*   Updated: 2025/10/14 20:02:07 by mal-naki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Token::Token() : type(T_EOF), value(""), line(0), column(0) {}
Token::Token(TokenType t, const std::string &v, size_t l, size_t c) : type(t), value(v), line(l), column(c) {}

//! Mohamad AL-Nakib | Webserve
