#include <string>
#include <sstream>
#include "Log.hpp"

#include <exception>
#include "Exception.hpp"
#include "SocketException.hpp"

#include <winsock2.h>
#include "Socket.hpp"
#include "ChatClient.hpp"

using namespace RR;

/* voir si on ouvre la socket immediatement */
ChatClient::ChatClient(const Id &uid, const Id &gid, const std::string &pseudo)
: _uid(uid), _gid(gid), _pseudo(pseudo), _sock()
{

}

/* ouvrir une nouvelle socket en gardant les memes autres parametres */
ChatClient::ChatClient(const ChatClient &copy)
: _uid(copy._uid), _gid(copy._gid), _pseudo(copy._pseudo), _sock()
{

}

ChatClient::~ChatClient()
{
	Log	&l = Log::instance();

	l << "ClientChat destroyed";
}

/*voir pour la socket ici aussi */
ChatClient&
ChatClient::operator=(const ChatClient &copy)
{
	_uid = copy._uid;
	_gid = copy._gid;
	_pseudo = copy._pseudo;
	return (*this);
}

void
ChatClient::connect(const std::string &addr, unsigned short port)
{
	struct sockaddr_in	sin;

	sin.sin_addr.s_addr = inet_addr(addr.c_str());
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	if (::connect(_sock.socket(), (struct sockaddr *)&sin, sizeof(sin)) != 0)
		throw (SocketException((std::string("Can't connect to ") + addr).c_str()));
	registerOnServer();
}

void
ChatClient::registerOnServer()
{
	std::ostringstream os;

	os << "#" << _uid << " " << _gid << " " << _pseudo.length() << "#" << _pseudo;
	sendToServer(os.str());
}

void
ChatClient::sendToServer(const std::string& str)
{
	_sock.addDataToWrite(str);
	_sock.sendAll();
}

void
ChatClient::sendToAll(const std::string &message)
{
	std::ostringstream os;

	os << "@" << message.length() << "@" << message;
	sendToServer(os.str());
}

void
ChatClient::sendToGroup(const std::string &message, const Id &gid)
{
	std::ostringstream os;

	os << "%" << gid << " " << message.length() << "%" << message;
	sendToServer(os.str());
}

void
ChatClient::sendToGroup(const std::string& message)
{
	sendToGroup(message, _gid);
}

void
ChatClient::sendToUser(const std::string &message, const Id &uid)
{
	std::ostringstream os;

	os << "$" << uid << " " << message.length() << "$" << message;
	sendToServer(os.str());
}

/*
bool
ChatClient::exec()
{
	fd_set	rd;
	fd_set	wr;

	FD_ZERO(&rd);
	FD_ZERO(&wr);
	FD_SET(, &rd);
	FD_SET(, &wr);

	if (select(0, &rd, &wr, NULL, NULL) == SOCKET_ERROR)
		return (false);
	if (FD_ISSET(, &rd))
		if (FD_ISSET(, &wr))
}*/