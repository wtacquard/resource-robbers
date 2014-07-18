/*******************************************************************************

	Author:		Wilfried Tacquard
	Date:		04/06/2009
	Filename:	SocketException.cpp
	Purpose:	Provide exception for winsock.


*******************************************************************************/
#ifndef SOCKETEXCEPTION_HPP_
# define SOCKETEXCEPTION_HPP_

namespace RR
{
	class SocketException : public Exception
	{
	public:
		SocketException(const char *);
		~SocketException();

		virtual const char *what() const throw();
	};
}
#endif //!SOCKETEXCEPTION_HPP_
