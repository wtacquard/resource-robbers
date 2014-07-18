#ifndef SERVER_HPP_
# define SERVER_HPP_

namespace RR
{
	class Server : public AbstractServer
	{
	private:
		Server(const Server &){};
		Server& operator=(const Server &){return (*this);};

	private:
		boost::function<void(void*)> _onConnect;
		boost::function<void(void*)> _onDisconnect;
		boost::function<void(void*, std::string& data)> _onReceive;

		void	initMonitoring();
		void	endMonitoring(int count);
		
	public:
		Server();
		~Server();
		
		void			broadcast(const std::string& msg);
		virtual bool	exec();
		void setOnConnect(boost::function<void(void*)>);
		void setOnDisconnect(boost::function<void(void*)>);
		void setOnReceive(boost::function<void(void*, std::string& data)>);
	};
}

#endif //!SERVER_HPP_
