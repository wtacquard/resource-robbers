#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <map>
#include "common/Player.h"

class GameController
{
public:
	typedef std::map<void *, Player *> Players;

private:
	Players players;
	bool _isInGame;
	float damageCollisionMultiplier;
	__int64 timeStep;
	__int64 lastUpdate;

private:
	Player* addClient(void *id);
	void removeClient(void *id);

public:

	GameController();
	~GameController();
	Players& getPlayers();
	float getTimeStep() const;
	bool isInGame();
	bool isReadyToUpdate();
	void onCollide(boost::shared_ptr<PhysicsObject>, boost::shared_ptr<PhysicsObject>, float);
	void onConnect(void*);
	void onDisconnect(void*);
	void onReceive(void*, std::string&);
	void setTimeStep(double time);
	bool update();
};

#endif //GAME_CONTROLLER_H