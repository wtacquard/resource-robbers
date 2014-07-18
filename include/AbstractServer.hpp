#ifndef ABSTRACTSERVER_HPP_
# define ABSTRACTSERVER_HPP_

#include <vector>

namespace RR
{
	class AbstractServer
	{
	private:
		struct timeval*				_timeout;

	protected:
		typedef std::vector<Socket *> Clients;
		Socket		_server;
		Clients		_clients;
		fd_set		_readfds;
		fd_set  	_writefds;
		fd_set		_errorfds;
		

	private:
		AbstractServer(const AbstractServer&) {};
		AbstractServer& operator=(const AbstractServer&){return (*this);};

	protected:
		void	disconnectClient(const Clients::iterator &it, bool force = false);

	public:
		AbstractServer();
		virtual ~AbstractServer();

		virtual bool	exec() = 0;

		void	bind_and_listen(const char *addr, unsigned short port, unsigned int queue);
		virtual Socket*	accept();
		virtual void	disconnect(bool force);
		void	setNoTimeout();
		void	timeout(unsigned int sec, unsigned int usec);
		const struct timeval* timeout() const;
	};
}

#endif //!ABSTRACTSERVER_HPP_
