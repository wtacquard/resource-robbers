#include <winsock2.h>
#include <string>
#include <sstream>
#include <tinyxml.h>
#include <vector>
#include <map>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "Socket.hpp"
#include "AbstractServer.hpp"
#include "Server.hpp"
#include "ServerController.h"
#include <Log.hpp>

using std::string;
using RR::Server;
using std::set;
using boost::shared_ptr;

ServerController* ServerController::_instance = NULL;

ServerController::ServerController()
{
	networkServer.timeout(0,0);
	networkServer.setOnConnect(boost::bind<void>(&GameController::onConnect, boost::ref(gameController), _1));
	networkServer.setOnDisconnect(boost::bind<void>(&GameController::onDisconnect, boost::ref(gameController), _1));
	networkServer.setOnReceive(boost::bind<void>(&GameController::onReceive, boost::ref(gameController), _1, _2));
	physicsEngine.setOnCollide(boost::bind<void>(&GameController::onCollide, boost::ref(gameController), _1, _2, _3));
}

ServerController::~ServerController()
{
}

void ServerController::applyUserInput()
{
	GameController::Players::iterator it;
	GameController::Players::iterator end;
	GameController::Players& players = gameController.getPlayers();
	for(it = players.begin(), end = players.end(); it != end; it++)
	{
		D3DXVECTOR3 direction;
		it->second->getVehicle()->applyThrust(it->second->getMovementDirection(&direction));
	}
}
void ServerController::bindAndListen(const char *ip, unsigned short port)
{
	networkServer.bind_and_listen(ip, port, 42);
}
void ServerController::destroy()
{
	delete _instance;
	_instance = NULL;
}

void ServerController::exec()
{
	D3DXVECTOR3 direction;
	std::string buffer;

//	networkServer.exec();
	if (physicsEngine.isReadyToUpdate())
	{
		GameController::Players& players = gameController.getPlayers();
		for(GameController::Players::iterator it = players.begin(); it != players.end(); ++it)
		{			
			direction = *it->second->getVehicle()->getPosition();
			RR::Log::instance() << "position x: " << direction.x << " y: " << direction.y << " z: " << direction.z << std::endl;			
			it->second->getVehicle()->applyThrust(it->second->getMovementDirection(&direction));
			//log << "thrust x: " << direction.x << " y: " << direction.y << " z: " << direction.z << std::endl;
		}
		physicsEngine.update();
		physicsToString(buffer);
		if (!buffer.empty())
			networkServer.broadcast(buffer);
	}
	networkServer.exec();
}

ServerController& ServerController::instance()
{
	return _instance ? *_instance : *(_instance = new ServerController());
}

PhysicsEngine& ServerController::getPhysicsEngine() { return physicsEngine; }
GameController& ServerController::getGameController() { return gameController; }
Server& ServerController::getServer() { return networkServer; }

//todo: optimize that stuff to send only updated stuff
string& ServerController::physicsToString(string& buffer)
{
	std::string temp;

	buffer.clear();
	PhysicsEngine::PhysicsObjectsIterator it;
	PhysicsEngine::PhysicsObjectsIterator end;
	for(it = physicsEngine.getPhysicsObjects().begin(), end = physicsEngine.getPhysicsObjects().end();
		it != end; ++it)
	{
		buffer += (*it)->serialize(temp);
	}
	return (buffer);
}

/*
string&
ServerController::gameToString(string& buffer)
{
	buffer.clear();
	GameEngine::GameObjectListIterator it;
	for(it = gameEngine.getGameObjects().begin(), end = gameEngine.getGameObjects().end();
		it != end; ++it)
	{
		buffer += (*it)->serialize(temp);
	}
	return (buffer);
}
*/