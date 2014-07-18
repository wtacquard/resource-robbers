#include <vector>
#include <string>

#include <iostream>
#include <sstream>
#include "Log.hpp"

#include <exception>
#include "Exception.hpp"
#include "SocketException.hpp"

#include <winsock2.h>
#include "Socket.hpp"
#include "AbstractServer.hpp"
#include <boost/function.hpp>
#include "Server.hpp"
#include "server/ServerController.h"

/*specifie mainCRTStartup en tant que point d'entree */
int main(int argc, char **argv)
{
	RR::Log &log = RR::Log::instance();

	try
	{
		ServerController& s = ServerController::instance();
		if (argc < 3)
		{	
			log<<log.level(RR::Log::RR_ERROR)
			<<"no enought parameters\n";
			log.flush();
			log.destroy();
			exit(42);
		}
		s.bindAndListen(argv[1], (unsigned short)atoi(argv[2]));
		while (42)
			s.exec();
	}
	catch (const RR::SocketException &e)
	{
		log << e.what();
	}
	catch (const std::exception &e)
	{
		log << e.what();
	}
	catch (...)
	{
		log << "Unexcepted Exception!!!";
	}
	log.flush();
	log.destroy();
	return 0;
}

