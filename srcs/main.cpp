/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtohmeh <mtohmeh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 18:30:33 by mtohmeh           #+#    #+#             */
/*   Updated: 2025/10/15 18:30:33 by mtohmeh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Config.hpp"
#include "Server.hpp"
#include "webserv.hpp"

extern ServerInfos *g_server;

int	main(int argc, char **argv)
{
	Parser parser;
	std::string configFile = "configs/default.conf";
    ServerInfos server;
    ConfigAST ast;
    HttpConf httpConf;

	if (argc == 2)
	{
		configFile = argv[1];
	}
	else if (argc > 2)
	{
		std::cerr << "Usage: " << argv[0] << " [config_file]" << std::endl;
		return (1);
	}

	try
	{
		ast = parser.parseFile(configFile);
		httpConf = initHttp(ast);
		std::cout << "[Config] Loaded " << httpConf.servers.size() << " server(s)" << std::endl;
		g_server = &server;
		signal(SIGINT, signalHandler);
		signal(SIGTERM, signalHandler);
		server.initializeAllSockets(httpConf);
		server.runServerLoop();
	}
	catch (const std::exception &e)
	{
		std::cerr << "[Error] " << e.what() << std::endl;
		return (1);
	}

	return (0);
}
