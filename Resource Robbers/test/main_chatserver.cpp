
#include <string>
#include <set>
#include <map>
#include <iostream>
#include <sstream>
#include "Log.hpp"

#include <exception>
#include "Exception.hpp"
#include "SocketException.hpp"

#include <winsock2.h>
#include "Socket.hpp"
#include "AbstractServer.hpp"
#include "ChatServer.hpp"

int main(int argc, char **argv)
{
	RR::Log& log = RR::Log::instance();
	RR::ChatServer s;

	//log<<RR::Log::RR_DEBUG<<"test debug"<<RR::Log::RR_INFO<<"it's working?";
	
	s.bind_and_listen("127.0.0.1", 4242, 42);
	while (42)
	{
		s.exec();
		log.flush();
	}
	log.destroy();
	return (0);
}