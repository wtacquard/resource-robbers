#include <string>

#include <exception>
#include "Exception.hpp"
#include "SocketException.hpp"

#include <winsock2.h>
#include "Socket.hpp"
#include "Client.hpp"

using namespace RR;

Client::Client()
try
: _client()
{

}
catch (...)
{
	//log.append(e->what());
	throw;
}

Client::Client(SOCKET sock)
try
: _client(sock)
{

}
catch (...)
{
	//log.append(e->what());
	throw;
}

Client::~Client()
{

}

void
Client::connect(const std::string &addr, unsigned short port)
{
	struct sockaddr_in	sin;

	sin.sin_addr.s_addr = inet_addr(addr.c_str());
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	if (::connect(_client.socket(), (struct sockaddr *)&sin, sizeof(sin)) != 0)
		throw (SocketException((std::string("Can't connect to ") + addr).c_str()));
}

void
Client::disconnect(bool force)
{
	_client.disconnect(force);
}

void
Client::send(const std::string &str)
{
	_client.addDataToWrite(str);
	_client.sendAll();
}

void
Client::recv(std::string &str)
{
	_client.recvAll();
	_client.readAllData(str);
	_client.clearReadBuffer();
}
