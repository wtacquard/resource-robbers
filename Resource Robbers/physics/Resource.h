#ifndef RESOURCE_H
#define RESOURCE_H

#include "PhysicsObject.h"

class Resource : public PhysicsObject
{
	int amount;
public:
	Resource(int amount);
	int getAmount();
};

#endif RESOURCE_H