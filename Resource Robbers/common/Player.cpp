#include "Player.h"
#include <sstream>
#include <string>
#include <log.hpp>

Player::Player(void* id)
: id(id), vehicle(new Hovercraft())
{
}

Player::~Player()
{
}

KeyState& Player::getKeyState()
{
	return keyState;
}

D3DXVECTOR3* Player::getMovementDirection(D3DXVECTOR3* result)
{
	result->x = 0;
	result->y = 0;
	result->z = 0;
	if(keyState.up)
		result->z -= 1;
	if(keyState.down)
		result->z += 1;
	if(keyState.left)
		result->x -= 1;
	if(keyState.right)
		result->x += 1;
	return result;
}

boost::shared_ptr<Hovercraft> Player::getVehicle()
{
	return vehicle;
}

bool Player::operator<(const Player& that) const
{
	return this->id < that.id;
}