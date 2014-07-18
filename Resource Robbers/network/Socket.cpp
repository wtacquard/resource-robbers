/*******************************************************************************

	Author:		Wilfried Tacquard
	Date:		04/06/2009
	Filename:	Socket.cpp
	Purpose:	Abstract class that provides bases to create more specified 
				sockets

*******************************************************************************/
#include <algorithm>
#include <string>

#include <exception>
#include "Exception.hpp"
#include "SocketException.hpp"

#include <winsock2.h>
#include "Socket.hpp"

using namespace RR;

Socket::Socket()
try
: _writeBuffer(std::string()), _readBuffer(std::string())
{
	if (WSAStartup(MAKEWORD(2,2), &_wsa) != 0)
		throw SocketException("Failed to start WSA");
	if ((_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
		throw (SocketException("Socket creation failed"));
}
catch (...)
{
	throw;
}

Socket::Socket(SOCKET sock)
try
: _writeBuffer(std::string()), _readBuffer(std::string()),
_socket(sock)
{
	if (WSAStartup(MAKEWORD(2,2), &_wsa) != 0)
		throw SocketException("Failed to start WSA");
}
catch (...)
{
	throw;
}

Socket::Socket(const Socket&)
{

}

Socket::~Socket()
{
	disconnect(false);
	WSACleanup();
}

void
Socket::disconnect(bool force)
{
	if (_socket != INVALID_SOCKET)
	{
		if (!force)
			shutdown(_socket, SD_BOTH);
		closesocket(_socket);
		_socket = INVALID_SOCKET;
	}
}

SOCKET
Socket::socket() const
{
	return (_socket);
}

void
Socket::readAllData(std::string &out)
{
	readData(out, std::string::npos);
}

void
Socket::readData(std::string &out, std::string::size_type size)
{
	out = _readBuffer.substr(0, size);
	_readBuffer.erase(0, size);
}

const std::string &
Socket::peekData() const
{
	return (_readBuffer);
}

void
Socket::ignore(std::string::size_type size)
{
	_readBuffer.erase(0, size);
}

bool
Socket::hasDataToWrite() const
{
	return (!_writeBuffer.empty());
}

void
Socket::addDataToWrite(const std::string &str)
{
	_writeBuffer += str;
}

void
Socket::clearReadBuffer()
{
	_readBuffer.clear();
}

int
Socket::recv()
{
	int			n;
	static char	buf[BUFFER_SIZE];

	if ((n = ::recv(_socket, buf, BUFFER_SIZE, 0)) == SOCKET_ERROR || n < 0)
		throw (SocketException("Receive failed"));
	_readBuffer.append(buf, min(n, BUFFER_SIZE));
	return (n);
}

int
Socket::send()
{
	int			n;
	static char	buf[BUFFER_SIZE];

	n = _writeBuffer.copy(buf, min(_writeBuffer.length(), BUFFER_SIZE));
	if ((n = ::send(_socket, buf, n, 0)) == SOCKET_ERROR)
		throw (SocketException("Send failed"));
	_writeBuffer.erase(0, n);
	return (n);
}

int
Socket::sendAll()
{
	int	n;

	n = 0;
	while (!_writeBuffer.empty())
		n += send();
	return (n);
}

int
Socket::recvAll()
{
	unsigned int	n;
	unsigned long	bytesAvailable;

	n = 0;
	if (ioctlsocket(_socket, FIONREAD, &bytesAvailable) == SOCKET_ERROR)
		throw (SocketException("ioctlsocket can't check if data are available to read"));
	while (bytesAvailable > n)
		n += recv();
	return (n);
}