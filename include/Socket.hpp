/*******************************************************************************

	Author:		Wilfried Tacquard
	Date:		04/06/2009
	Filename:	Socket.cpp
	Purpose:	Abstract class that provides bases to create more specified 
				sockets

*******************************************************************************/
#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#define BUFFER_SIZE 512

namespace RR
{
	class	Socket
	{
	private:
		WSADATA				_wsa;
		Socket(const Socket&);

	protected:
		std::string			_writeBuffer;
		std::string			_readBuffer;
		SOCKET				_socket;

	public:
		Socket();
		Socket(SOCKET);
		virtual ~Socket();

		int		recv();
		int		send();
		int		sendAll();
		int		recvAll();
		
		SOCKET				socket() const;
		void				readAllData(std::string &out);
		void				readData(std::string &out, std::string::size_type size);
		const std::string&	peekData() const;
		void				ignore(std::string::size_type size);
		void				clearReadBuffer();
		bool				hasDataToWrite() const;
		void				addDataToWrite(const std::string &);

		virtual	void		disconnect(bool force);
	};
}
#endif