/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-naki <mal-naki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 20:02:46 by mal-naki          #+#    #+#             */
/*   Updated: 2025/10/14 20:02:46 by mal-naki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include "webserv.hpp"

/*
 *Common Gateway Interface (CGI) is an interface specification that enables web servers
 *to execute an external program to process HTTP or HTTPS user requests.
 */
class CGI
{
private:
    std::string _path;
    std::string _interpreter;
    std::map<std::string, std::string> _env;

    std::vector<char *> initEnv() const;

public:
    CGI(const std::string &path, const std::string &interpreter,const std::map<std::string, std::string> &env);
    ~CGI();

    std::string execute(const std::string &body);
};

#endif //! CGI_HPP

//! Mohamad AL-Nakib | Webserve