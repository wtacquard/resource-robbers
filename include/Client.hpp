#ifndef CLIENT
# define CLIENT

namespace RR
{
	class Client
	{
	private:
		Client(const Client &){};
		Client& operator=(const Client&){return (*this);};

	private:
		Socket	_client;
	
	public:
		Client();
		Client(SOCKET);
		~Client();

		void connect(const std::string &addr, unsigned short port);
		virtual void disconnect(bool force);

		void	send(const std::string &str);
		void	recv(std::string &str);
	};
}

#endif //!CLIENT