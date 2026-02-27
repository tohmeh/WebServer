/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigAST.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-naki <mal-naki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 20:01:48 by mal-naki          #+#    #+#             */
/*   Updated: 2025/10/14 20:01:48 by mal-naki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

const Node *ConfigAST::findFirst(const std::string &name) const
{
    size_t i = 0;
    while (i < roots.size())
    {
        if (roots[i].name == name)
            return (&roots[i]);
        ++i;
    }
    return (NULL);
}

std::vector<const Node *> ConfigAST::findAll(const std::string &name) const
{
    size_t i = 0;
    std::vector<const Node *> out;
    while (i < roots.size())
    {
        if (roots[i].name == name)
            out.push_back(&roots[i]);
        ++i;
    }
    return (out);
}

//! Mohamad AL-Nakib | Webserve
