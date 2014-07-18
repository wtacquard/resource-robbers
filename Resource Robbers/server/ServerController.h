#ifndef SERVER_CONTROLLER_H
#define SERVER_CONTROLLER_H

#include <Server.hpp>
#include <boost/ptr_container/ptr_set.hpp>
#include "common/Player.h"
#include "physics/PhysicsEngine.h"
#include "GameController.h"

class ServerController
{
private:
	static ServerController* _instance;
	RR::Server networkServer;
	PhysicsEngine physicsEngine;
	GameController gameController;

	ServerController();
	ServerController(const ServerController& that){} //don't use
	~ServerController();
	ServerController& operator=(const ServerController& that) {return *this;}	//don't use
	std::string& physicsToString(std::string& buffer);
//	std::string& gameToString(std::string& buffer);
	void applyUserInput();

public:
	static ServerController& instance();

	void bindAndListen(const char* ip, unsigned short port);
	void destroy();
	void exec();
	PhysicsEngine& getPhysicsEngine();
	GameController& getGameController();
	RR::Server& getServer();
};
#endif //SERVER_CONTROLLER_H