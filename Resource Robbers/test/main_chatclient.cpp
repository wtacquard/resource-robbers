#include <set>
#include <string>

#include <iostream>
#include <sstream>
#include "Log.hpp"

#include <exception>
#include "Exception.hpp"
#include "SocketException.hpp"

#include <winsock2.h>
#include "Socket.hpp"
#include "ChatClient.hpp"


int	main(int argc, char **argv)
{
	RR::Log& log = RR::Log::instance();
	if (argc < 3)
	{
		log << "Bad parameters";
		log.destroy();
		exit(42);
	}
	if (argc == 4)
	{
		RR::ChatClient c(42, 4242, "phoenix");

		c.connect(argv[1], static_cast<unsigned short>(atoi(argv[2])));
		c.sendToAll("test to all");
		c.sendToGroup("test to group");
		c.sendToUser("test user1", 1);
		c.sendToUser("test user2", 2);
		c.sendToUser("test loopback", 42);
	}
	else
	{
		RR::ChatClient c2(1, 4242, "user1");
	}
	log.destroy();
	return (0);
}