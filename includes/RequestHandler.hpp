/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-naki <mal-naki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 20:03:12 by mal-naki          #+#    #+#             */
/*   Updated: 2025/10/14 20:03:13 by mal-naki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "webserv.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Utils.hpp"


class RequestHandler {
	public: 
		virtual bool isReadable(const std::string& path);
		virtual ~RequestHandler() {}
		virtual Response handle(const Request& req, const ResolvedPath& resolvedPath) = 0;
};

#endif //! REQUESTHANDLER_HPP
