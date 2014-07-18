#ifndef HOVERCRAFT_H
#define HOVERCRAFT_H

#include "PhysicsMovableObject.h"

class Hovercraft : public PhysicsMovableObject
{
	unsigned int life;
	unsigned int maxLife;
	float thrust;
public:
	Hovercraft();
	~Hovercraft();
	void applyDamage(int damage);
	void applyThrust(D3DXVECTOR3* direction);
	std::string& serializeGameState(std::string& buffer);
};

#endif //HOVERCRAFT_H