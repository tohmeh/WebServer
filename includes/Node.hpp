/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Node.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-naki <mal-naki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 20:03:00 by mal-naki          #+#    #+#             */
/*   Updated: 2025/10/14 20:03:00 by mal-naki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NODE
#define NODE

#include "webserv.hpp"

/*
 *Represents a configuration element (node) in the AST
 */
class Node
{
public:
    size_t line;
    size_t column;
    std::string name;
    std::vector<Node> children;
    std::vector<std::string> args;

    bool isBlock();
};

#endif //! NODE

//! Mohamad AL-Nakib | Webserve
