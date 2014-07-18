#ifndef CHATCLIENT_HPP_
# define CHATCLIENT_HPP_

namespace RR
{
	class ChatClient
	{
		typedef unsigned int Id;

	private:
		Id			_uid;
		Id			_gid;
		std::string	_pseudo;
		Socket		_sock;

	private:
		void	registerOnServer();
		void	sendToServer(const std::string& str);

	public:
		ChatClient(const Id &uid, const Id &gid, const std::string &pseudo);
		ChatClient(const ChatClient &);
		~ChatClient();

		ChatClient &operator=(const ChatClient &);

		void	connect(const std::string &addr, unsigned short port);
		void	sendToAll(const std::string &);
		void	sendToGroup(const std::string &, const Id&);
		void	sendToGroup(const std::string&);
		void	sendToUser(const std::string &, const Id&);
	};
}

#endif //!CHATCLIENT_HPP_