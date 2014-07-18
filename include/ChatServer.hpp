#ifndef CHATSERVER_HPP_
# define CHATSERVER_HPP_

namespace RR
{
	class ChatServer : public AbstractServer
	{
		typedef unsigned int Id;

	private:
		typedef struct	s_client
		{
			Id			_uid;
			Id			_gid;
			std::string	_pseudo;
		}				t_client;

		std::map<Id, Socket*>		_usersByUid;
		std::multimap<Id, Socket*>	_usersByGid;
		std::map<Socket*, t_client>	_usersBySocket;

	private:
		ChatServer& operator=(const ChatServer &){return (*this);};
		ChatServer(const ChatServer &){};

		void	initMonitoring();
		void	endMonitoring(int count);
		void	readFromClient(Socket *client);

	public:
		ChatServer();
		~ChatServer();

		virtual bool	exec();
		virtual Socket*	accept();
		virtual void	disconnect(bool force);
	};
}

#endif //!CHATSERVER_HPP_
