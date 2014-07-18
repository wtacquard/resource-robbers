#include <sstream>
#include <iostream>
#include "Log.hpp"

#include <exception>
#include "Exception.hpp"
#include "SocketException.hpp"

#include <string>
#include <vector>
#include <winsock2.h>
#include "Socket.hpp"
#include "AbstractServer.hpp"
#include <boost/function.hpp>
#include "Server.hpp"

#include <list>

using namespace RR;

Server::Server()
: AbstractServer(), _onConnect(NULL), _onDisconnect(NULL), _onReceive(NULL)
{

}


Server::~Server()
{
}

void
Server::broadcast(const std::string& msg)
{
	for (Clients::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
		(*it)->addDataToWrite(msg);
}

void
Server::initMonitoring()
{
	Clients::const_iterator	it;

	FD_ZERO(&_readfds);
	FD_ZERO(&_writefds);
	FD_SET(_server.socket(), &_readfds);
	for (it = _clients.begin(); it != _clients.end(); ++it)
	{
		FD_SET((*it)->socket(), &_readfds);
		if ((*it)->hasDataToWrite())
			FD_SET((*it)->socket(), &_writefds);
	}
}

void
Server::endMonitoring(int count)
{
	Log&	log = Log::instance();
	std::string		tmp;
	std::vector<Clients::size_type>	drop_clients;
	Socket*		new_socket = NULL;

	if (count && FD_ISSET(_server.socket(), &_readfds))
	{
		--count;
		new_socket = accept();
		log << "New client connected";
		if(_onConnect)
			_onConnect(new_socket);
	}
	for (Clients::size_type i = 0; count && i < _clients.size(); ++i)
	{
		try
		{
			if (FD_ISSET(_clients[i]->socket(), &_readfds))
			{
				--count;
				if (_clients[i]->recv() == 0)
				{
					FD_CLR(_clients[i]->socket(), &_writefds);
					drop_clients.push_back(i);
				}
				else
				{
					_clients[i]->readAllData(tmp);
					if(_onReceive)
						_onReceive(_clients[i], tmp);
				}
			}
			if (FD_ISSET(_clients[i]->socket(), &_writefds))
			{
				--count;
				_clients[i]->send();
			}
		}
		catch (const std::exception &e)
		{
			if (WSAGetLastError() == WSAECONNRESET)
			{
				FD_CLR(_clients[i]->socket(), &_writefds);
				drop_clients.push_back(i);
			}
			else
				log<<"\nException in socket\n" << e.what();
		}
	}
	for (std::vector<Clients::size_type>::iterator it = drop_clients.begin(); it != drop_clients.end(); ++it)
	{
		if(_onDisconnect)
		{
			Socket *test = *(_clients.begin() + *it);
			_onDisconnect(test);
		}
		disconnectClient(_clients.begin() + *it, false);
	}
}

bool
Server::exec()
{
	int	count;
	
	try
	{
		initMonitoring();
		if ((count = select(0, &_readfds, &_writefds, NULL, timeout())) == SOCKET_ERROR)
			throw (SocketException("Error in select while listening sockets"));
		endMonitoring(count);
	}
	catch (const Exception &e)
	{
		Log::instance() << e.what();
		return (false);
	}
	return (true);
}

void Server::setOnConnect(boost::function<void(void*)> onConnect)
{
	_onConnect = onConnect;
}
void Server::setOnDisconnect(boost::function<void(void*)> onDisconnect)
{
	_onDisconnect = onDisconnect;
}
void Server::setOnReceive(boost::function<void(void*, std::string& data)> onReceive)
{
	_onReceive = onReceive;
}