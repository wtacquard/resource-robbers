#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include "Log.hpp"

#include <exception>
#include "Exception.hpp"
#include "SocketException.hpp"

#include <vector>
#include <set>
#include <map>
#include <winsock2.h>
#include "Socket.hpp"
#include "AbstractServer.hpp"
#include "ChatServer.hpp"

#include <list>

using namespace RR;

ChatServer::ChatServer()
{

}


ChatServer::~ChatServer()
{

}

void
ChatServer::initMonitoring()
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
ChatServer::endMonitoring(int count)
{
	Clients::iterator			it;
	std::list<Clients::iterator>	drop_clients;

	if (count && FD_ISSET(_server.socket(), &_readfds))
	{
		accept();
	}
	for (it = _clients.begin(); count && it != _clients.end(); ++it)
	{
		if (count && FD_ISSET((*it)->socket(), &_readfds))
		{
			if ((*it)->recv() == 0)
			{
				FD_CLR((*it)->socket(), &_writefds);
				drop_clients.push_back(it);
			}
			else
				readFromClient(*it);
			--count;
		}
		if (count && FD_ISSET((*it)->socket(), &_writefds))
		{
			(*it)->send();
			--count;
		}
	}
	for (std::list<Clients::iterator>::iterator it2 = drop_clients.begin(); it2 != drop_clients.end(); ++it2)
		disconnectClient(*it2, false);
}

bool
ChatServer::exec()
{
	int	count;
	
	initMonitoring();
	if ((count = ::select(0, &_readfds, &_writefds, NULL, timeout())) == SOCKET_ERROR)
		throw (SocketException("Error in select while listening sockets"));
	endMonitoring(count);
	return (true);
}

Socket*
ChatServer::accept()
{
	return AbstractServer::accept();
}


void
ChatServer::disconnect(bool force)
{
	AbstractServer::disconnect(force);
	_usersByUid.clear();
	_usersByGid.clear();
	_usersBySocket.clear();
}

void
ChatServer::readFromClient(Socket* client)
{
	Log&				log = Log::instance();
	const std::string&	buf = client->peekData();
	std::istringstream	is(buf);
	char				op;
	unsigned int		len;
	char*				message;
	t_client			user;
	unsigned int		header_len;
	
	is >> op;
	if (op == '#')
	{
		is >> user._uid >> user._gid >> len >> op;
		header_len = buf.find_first_of(op, 1) + 1;
		if (buf.length() - header_len < len)
			return ;
		message = new char[len + 1];
		is.read(message, len);
		message[len] = 0;
		user._pseudo = message;
		log << "New user registered.\nUid: " << user._uid << " Gid: "<< user._gid 
			<< " Pseudo: " << user._pseudo << std::endl;
		_usersByUid.insert(std::make_pair<Id, Socket *>(user._uid, client));
		_usersByGid.insert(std::make_pair<Id, Socket *>(user._gid, client));
		_usersBySocket.insert(std::make_pair<Socket *, t_client>(client, user));
		client->ignore(header_len + len);
	}
	else if (op == '@')
	{
		is >> len >> op;
		header_len = buf.find_first_of(op, 1) + 1;
		if (buf.length() - header_len < len)
			return ;
		message = new char[len + 1];
		is.read(message, len);
		message[len] = 0;
		client->ignore(header_len + len);
		for (std::map<Socket*, t_client>::iterator it = _usersBySocket.begin(); it != _usersBySocket.end(); ++it)
		{
			if (it->first != client)
				it->first->addDataToWrite(message);
		}
		log << "Broadcast message: " << message << std::endl;
	}
	else if (op == '$')
	{
		std::map<Id, Socket*>::iterator it;
		is >> user._uid >> len >> op;
		header_len = buf.find_first_of(op, 1) + 1;
		if (buf.length() - header_len < len)
			return ;
		message = new char[len + 1];
		is.read(message, len);
		message[len] = 0;
		if ((it = _usersByUid.find(user._uid)) == _usersByUid.end())
			return ;
		it->second->addDataToWrite(message);
		client->ignore(header_len + len);
		log << "Private message: " << message << std::endl;
	}
	else if (op == '%')
	{
		std::pair<std::multimap<Id, Socket*>::iterator, std::multimap<Id, Socket*>::iterator> ret;
		is >> user._uid >> len >> op;
		header_len = buf.find_first_of(op, 1) + 1;
		if (buf.length() - header_len < len)
			return ;
		message = new char[len + 1];
		is.read(message, len);
		message[len] = 0;
		client->ignore(header_len + len);
		ret = _usersByGid.equal_range(user._gid);
		//if (ret == std::multimap<Id, Socket *>::end)
		//	return ;
		for (std::multimap<Id, Socket*>::iterator it = ret.first; it != ret.second; ++it)
		{
			it->second->addDataToWrite(message);
		}
		log << "Group message: " << message << std::endl;
	}
	else
		client->clearReadBuffer();
	if (!client->peekData().empty())
		readFromClient(client);
}

/*
void
ChatServer::registerClient()
{
	client->
}*/