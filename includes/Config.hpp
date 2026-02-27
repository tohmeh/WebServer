/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-naki <mal-naki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 20:02:50 by mal-naki          #+#    #+#             */
/*   Updated: 2025/10/14 20:02:51 by mal-naki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "webserv.hpp"
#include "Token.hpp"
#include "Node.hpp"
#include "ConfigAST.hpp"
#include "Parser.hpp"

//* Functions to initialize configuration structs from AST nodes
HttpConf initHttp(const ConfigAST &ast);
ServerConf initServer(const Node &server);
LocationConf initLocation(const Node &loc);

#endif //! CONFIG_HPP

//! Mohamad AL-Nakib | Webserve