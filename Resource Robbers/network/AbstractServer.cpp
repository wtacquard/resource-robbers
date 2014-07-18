#include <string>
#include <iostream>
#include <sstream>
#include "Log.hpp"

#include <exception>
#include "Exception.hpp"
#include "SocketException.hpp"

#include <vector>
#include <winsock2.h>
#include "Socket.hpp"
#include "AbstractServer.hpp"

using namespace RR;

AbstractServer::AbstractServer()
: _server(), _clients(), _timeout(NULL)
{

}

AbstractServer::~AbstractServer()
{
	setNoTimeout();
	disconnect(false);
}

void
AbstractServer::disconnect(bool force)
{
	for (Clients::iterator it = _clients.begin(); it != _clients.end(); ++it)
		(*it)->disconnect(force);
	_server.disconnect(force);
}

const struct timeval*
AbstractServer::timeout() const
{
	return (_timeout);
}

void
AbstractServer::timeout(unsigned int sec, unsigned int usec)
{
	if (!_timeout)
		_timeout = new (struct timeval);
	_timeout->tv_sec = sec;
	_timeout->tv_usec = usec;
}

void
AbstractServer::setNoTimeout()
{
	if (_timeout)
		delete (_timeout);
	_timeout = NULL;
}

Socket*
AbstractServer::accept()
{
	SOCKET				sock;
	Socket*				sock_real;
	struct sockaddr_in	sock_addr;
	int					sock_addr_len;
	Log					&log = Log::instance();

	sock_addr_len = sizeof(sock_addr);
	if ((sock = ::accept(_server.socket(), (struct sockaddr *)&sock_addr, &sock_addr_len)) == INVALID_SOCKET)
		throw (SocketException("Invalid socket at accepting time"));
	sock_real = new Socket(sock);
	_clients.push_back(sock_real);
	log << "New Client connected" << std::endl;
	return (sock_real);
	//std::cout<<"New client from "<<sock_addr.sin_addr.S_un<<":"<<ntohs(sock_addr.sin_port)<<std::endl;
}

void
AbstractServer::disconnectClient(const Clients::iterator &it, bool force)
{
	(*it)->disconnect(force);
	delete (*it);
	_clients.erase(it);
}

void
AbstractServer::bind_and_listen(const char *host, unsigned short port, unsigned int queue)
{
	struct sockaddr_in	sin;
  
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(host);
	sin.sin_port = htons(port);
	if (::bind(_server.socket(), (struct sockaddr *)&sin, sizeof(sin)) != 0)
		throw (SocketException("Bind failed"));
	if (listen(_server.socket(), queue) != 0)
		throw (SocketException("Listen failed"));
}