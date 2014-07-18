#ifndef PLAYER_H
#define PLAYER_H

#include <DXUT.h>
#include <boost/shared_ptr.hpp>
#include "controller/KeyState.h"
#include "physics/Hovercraft.h"

class Player
{
	void* id;
	KeyState keyState;
	boost::shared_ptr<Hovercraft> vehicle;
public:
	Player(void* id);
	~Player();

	KeyState& getKeyState();
	D3DXVECTOR3* getMovementDirection(D3DXVECTOR3*);
	boost::shared_ptr<Hovercraft> getVehicle();
	bool operator<(const Player& that) const;
};

#endif //PLAYER_H