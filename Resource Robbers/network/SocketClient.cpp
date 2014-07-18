#include <string>
#include <iostream>
#include <sstream>
#include "Log.hpp"
#include <exception>
#include "Exception.hpp"
#include "SocketException.hpp"
#include <winsock2.h>
#include "Socket.hpp"
#include "SocketClient.hpp"

using namespace RR;

SocketClient::SocketClient()
: Socket()
{

}

SocketClient::~SocketClient()
{

}

bool
SocketClient::connect(const std::string& addr, const std::string& port)
{
	struct sockaddr_in	sin;
	//struct hostent *hp = NULL;
	std::istringstream is(port);
	unsigned short	p;

	is >> p;
/* for dns purpose
	if ((hp = gethostbyname(addr.c_str())) == NULL)
		return (false);
	memcpy((char *)&sin.sin_addr, hp->h_addr, hp->h_length);
	sin.sin_family = hp->h_addrtype; 
	sin.sin_port = p;
*/
	memset(&sin,0,sizeof(sin));
	sin.sin_addr.s_addr = ::inet_addr(addr.c_str());
	sin.sin_family = AF_INET; 
	sin.sin_port = htons(p);
	if (::connect(_socket, (struct sockaddr *)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		Log::instance()<<"Error: Client connection: #"<<WSAGetLastError();
		return (false);
	}
	return (true);
}

bool
SocketClient::exec()
{
	const static struct timeval tv = {0, 0};
	fd_set	rd;
	fd_set	wr;

	try
	{
		FD_ZERO(&rd);
		FD_ZERO(&wr);
		FD_SET(_socket, &rd);
		FD_SET(_socket, &wr);
		if (select(0, &rd, &wr, NULL, &tv) == SOCKET_ERROR)
			throw (SocketException("Error with Select on client side"));
		if (FD_ISSET(_socket, &rd))
			recvAll();
		if (hasDataToWrite() && FD_ISSET(_socket, &wr))
			send();
		return (true);
	}
	catch (const SocketException& e)
	{
		RR::Log::instance()<<e.what();
		return (false);
	}
}