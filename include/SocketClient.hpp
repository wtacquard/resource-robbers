#ifndef SOCKETCLIENT_HPP_
# define SOCKETCLIENT_HPP_

namespace RR
{
	class SocketClient : public Socket
	{
	private:
		bool	_connected;

	public:
		SocketClient();
		~SocketClient();

		virtual bool	exec();
		bool	connect(const std::string& addr, const std::string& port);
	};
}

#endif //!SOCKETCLIENT_HPP_