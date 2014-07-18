#include <winsock2.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "Socket.hpp"
#include "AbstractServer.hpp"
#include "Server.hpp"
#include "ServerController.h"
#include "GameController.h"
#include <log.hpp>

using std::string;
using boost::shared_ptr;
using RR::Log;

GameController::GameController()
: damageCollisionMultiplier(1), _isInGame(false), timeStep(0)
{
	Log &log = Log::instance();
	setTimeStep(1.0 / 30.0);
	if(!QueryPerformanceCounter((LARGE_INTEGER*)(&lastUpdate)))
		log << "Physics Engine could not get high resolution timer";
}

GameController::~GameController()
{
	for(Players::iterator it = players.begin(); it != players.end(); ++it)
		delete it->second;
}

GameController::Players&
GameController::getPlayers()
{
	return players;
}

float GameController::getTimeStep() const
{
	__int64 frequency;
	Log& log = Log::instance();
	if(!QueryPerformanceFrequency((LARGE_INTEGER*)&frequency))
		log << "Physics Engine could not get high resolution timer";
	return (float)((double)timeStep / frequency);
}
bool GameController::isInGame() { return _isInGame; }
bool GameController::isReadyToUpdate()
{
	__int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	return lastUpdate + timeStep < currentTime;
}
void GameController::onCollide(shared_ptr<PhysicsObject> first, shared_ptr<PhysicsObject> second, float forceMagnitude)
{
	Hovercraft* h1 = dynamic_cast<Hovercraft*>(first.get());
	Hovercraft* h2 = dynamic_cast<Hovercraft*>(second.get());
	if(h1)
	{
		h1->applyDamage((int)(forceMagnitude * damageCollisionMultiplier));
	}
	if(h2)
	{
		h2->applyDamage((int)(forceMagnitude * damageCollisionMultiplier));
	}
}

void GameController::onConnect(void* id)
{
	Player *newone;
	std::string buffer;
	ServerController& serverController = ServerController::instance();

	newone = addClient(id);
	Hovercraft* vehicle = newone->getVehicle().get();
	serverController.getPhysicsEngine().registerPhysicsObject(newone->getVehicle());
	vehicle->serializeGameState(buffer);
	serverController.getServer().broadcast(buffer);
}

void
GameController::onDisconnect(void* id)
{
	removeClient(id);
}

void GameController::onReceive(void* id, std::string& data)
{
	//as of now, all data received by the server is just client's input
	players.find(id)->second->getKeyState().deserialize(data);
}

void GameController::setTimeStep(double time)
{
	__int64 frequency;
	Log& log = Log::instance();
	if(!QueryPerformanceFrequency((LARGE_INTEGER*)&frequency))
		log << "Physics Engine could not get high resolution timer";
	timeStep = (__int64)(time * frequency);
}
bool GameController::update()
{
	__int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	if(lastUpdate + timeStep >= currentTime)
		return false;
	return true;
}

Player*
GameController::addClient(void *id)
{
	Player* newPlayer = new Player(id);
	players.insert(std::make_pair<void *, Player *>(id, newPlayer));
	return newPlayer;
}

void
GameController::removeClient(void *id)
{
	Players::iterator it = players.find(id);
	ServerController::instance().getPhysicsEngine().remove(it->second->getVehicle());
	delete (it->second);
	players.erase(it);
}