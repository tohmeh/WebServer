/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigAST.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-naki <mal-naki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 20:02:53 by mal-naki          #+#    #+#             */
/*   Updated: 2025/10/14 20:02:53 by mal-naki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFAST_HPP
#define CONFIGFAST_HPP

#include "webserv.hpp"
#include "Node.hpp"

/*
 *Represents the Abstract Syntax Tree (AST) of the configuration
 */
class ConfigAST
{
public:
    std::vector<Node> roots;

    const Node *findFirst(const std::string &name) const;
    std::vector<const Node *> findAll(const std::string &name) const;
};

#endif //! CONFIGFAST_HPP

//! Mohamad AL-Nakib | Webserve
