#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <boost/function.hpp>
#include "InputController.h"
#include <Socket.hpp>
#include "common/Player.h"

class Command
{
	std::string name;
	InputController::CallbackFunctionKeyboard callback;
	InputController::VirtualKey key;

public:
	Command(const std::string& name, InputController::VirtualKey key, InputController::CallbackFunctionKeyboard callback);
	Command(const std::string& xml);
	void bindToNetworkSend(RR::Socket&);
	void bindToSetDirection(Player* player);
	InputController::VirtualKey getKey() const;
	InputController::CallbackFunctionKeyboard& getCallback();
	bool operator<(const Command &that) const;
	void sendToServer(RR::Socket&, bool isDown, bool isAltDown, void*);
	void setUserInputDirection(Player* player, bool isDown, bool isAltDown, void*);
	void setCallback(InputController::CallbackFunctionKeyboard callback);
	std::string& toXml(std::string& result, bool isDown, bool isAltDown);
};
#endif COMMAND_H