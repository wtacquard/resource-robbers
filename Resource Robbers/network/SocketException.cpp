/*******************************************************************************

	Author:		Wilfried Tacquard
	Date:		04/06/2009
	Filename:	SocketException.cpp
	Purpose:	Implement exception for winsock.


*******************************************************************************/
#include <string>
#include <sstream>
#include <exception>

#include <winsock2.h>
#include "Exception.hpp"
#include "SocketException.hpp"

using namespace RR;

SocketException::SocketException(const char *str)
: Exception(str)
{
	
}

SocketException::~SocketException()
{

}

const char *SocketException::what() const throw()
{
	static std::string err;
	std::ostringstream os;

	os<<WSAGetLastError();
	err = Exception::what();
	return (err.append(" Error #").append(os.str()).c_str());//.append(WSAGetLastError()));
}