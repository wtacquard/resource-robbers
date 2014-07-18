#include <sstream>
#include <Log.hpp>
#include "Command.h"
#include "tinyxml.h"
#include <boost/bind.hpp>

using std::string;
using boost::function;
using RR::Socket;
using std::istringstream;
using RR::Log;


Command::Command(const string& name, InputController::VirtualKey key, InputController::CallbackFunctionKeyboard callback)
: name(name), callback(callback), key(key)
{
}

void Command::bindToNetworkSend(Socket& socket)
{
	setCallback(boost::bind<void>(&Command::sendToServer, this, boost::ref(socket), _1, _2, _3));
}

void Command::bindToSetDirection(Player* player)
{
	setCallback(boost::bind<void>(&Command::setUserInputDirection, this, player, _1, _2, _3));
}

void Command::setUserInputDirection(Player* player, bool isDown, bool isAltDown, void*)
{
	KeyState& keyState = player->getKeyState();
	if(name.compare("moveForward") == 0)
		keyState.up = isDown;
	else if(name.compare("moveBackward") == 0)
		keyState.down = isDown;
	else if(name.compare("moveLeft") == 0)
		keyState.left = isDown;
	else if(name.compare("moveRight") == 0)
		keyState.right = isDown;
	else if(name.compare("fire") == 0)
		keyState.fire = isDown;
	RR::Log::instance() << "Input: " << name << "\n";
}
InputController::CallbackFunctionKeyboard& Command::getCallback()
{
	return callback;
}
InputController::VirtualKey Command::getKey() const
{
	return key;
}
bool Command::operator<(const Command& that) const
{
	return this->name < that.name;
}
void Command::sendToServer(Socket& socket, bool isDown, bool isAltDown, void*)
{
	string s;

	socket.addDataToWrite(toXml(s, isDown, isAltDown));
}

void Command::setCallback(InputController::CallbackFunctionKeyboard callback)
{
	this->callback = callback;
}
string& Command::toXml(string& result, bool isDown, bool isAltDown)
{
	result = "<command><name>" + name + "</name><isDown>" + (isDown ? "true" : "false") + "</isDown><isAltDown>" +
		(isAltDown ? "true" : "false") + "</isAltDown></command>";
	RR::Log::instance() << "Input: " << name << "\n";
	return result;
}

