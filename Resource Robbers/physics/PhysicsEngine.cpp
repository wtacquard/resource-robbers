#include "PhysicsEngine.h"
#include <boost/exception/exception.hpp>
#include <sstream>
#include <log.hpp>
#include "PhysicsMovableObject.h"
#include "common/MoreMath.h"

using RR::Log;
using boost::exception;

PhysicsEngine::PhysicsEngine() :
gravity(0, 0.000000000f, 0.0f), _pobjects(), onCollideFunction(NULL), timeStep(0)
{
	Log &log = Log::instance();
	setTimeStep(1.0 / 30.0);
	if(!QueryPerformanceCounter((LARGE_INTEGER*)(&lastUpdate)))
		log << "Physics Engine could not get high resolution timer";
}

PhysicsEngine::~PhysicsEngine()
{
}

const D3DXVECTOR3& PhysicsEngine::getGravity() const
{
	return gravity;
}

PhysicsEngine::PhysicsObjects&
PhysicsEngine::getPhysicsObjects()
{
	return (_pobjects);
}

float PhysicsEngine::getTimeStep() const
{
	__int64 frequency;
	Log& log = Log::instance();
	if(!QueryPerformanceFrequency((LARGE_INTEGER*)&frequency))
		log << "Physics Engine could not get high resolution timer";
	return (float)((double)timeStep / frequency);
}

bool PhysicsEngine::isReadyToUpdate()
{
	__int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	return lastUpdate + timeStep < currentTime;
}

void PhysicsEngine::onCollide(shared_ptr<PhysicsObject> first, shared_ptr<PhysicsObject> second)
{
	PhysicsMovableObject* movable1;
	PhysicsMovableObject* movable2;
	movable1 = dynamic_cast<PhysicsMovableObject*>(first.get());
	movable2 = dynamic_cast<PhysicsMovableObject*>(second.get());
	if (!movable1 && !movable2)
		return;
	if (!movable1 && movable2)
	{
		onCollide(second, first);
		return;
	}
	D3DXVECTOR3 zero(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 firstNormal;
	D3DXVECTOR3 secondNormal;
	D3DXVec3Subtract(&firstNormal, second->getPosition(), first->getPosition());
	D3DXVec3Subtract(&secondNormal, first->getPosition(), second->getPosition());
	D3DXVec3Normalize(&firstNormal, &firstNormal);
	D3DXVec3Normalize(&secondNormal, &secondNormal);
	D3DXVECTOR3 forceMagnitudeVector;
	if (movable1 && !movable2)
	{
		D3DXVECTOR3 firstForce;
		D3DXVec3Scale(&firstForce, &firstNormal, D3DXVec3Dot(&firstNormal, movable1->getVelocity()) * movable1->getMass());
		forceMagnitudeVector = firstForce;
		firstForce *= -2;
		movable1->applyForce(&zero, &firstForce);
	}
	if (movable1 && movable2)
	{
		D3DXVECTOR3 firstForce;
		D3DXVECTOR3 secondForce;
		D3DXVec3Scale(&firstForce, &firstNormal, D3DXVec3Dot(&firstNormal, movable1->getVelocity()) * movable1->getMass());
		D3DXVec3Scale(&secondForce, &secondNormal, D3DXVec3Dot(&secondNormal, movable2->getVelocity()) * movable2->getMass());
		movable1->applyForce(&zero, &secondForce);
		movable2->applyForce(&zero, &firstForce);
		firstForce *= -1;
		forceMagnitudeVector = firstForce + secondForce;
		secondForce *= -1;
		movable1->applyForce(&zero, &firstForce);
		movable2->applyForce(&zero, &secondForce);
	}
	float forceMagnitude = D3DXVec3Length(&forceMagnitudeVector);
	if (onCollideFunction)
		onCollideFunction(first, second, forceMagnitude);
}

bool PhysicsEngine::registerPhysicsObject(boost::shared_ptr<PhysicsObject> obj)
{
	_pobjects.insert(obj);
	return (true);
}

bool PhysicsEngine::remove(boost::shared_ptr<PhysicsObject> obj)
{
	_pobjects.erase(_pobjects.find(obj));
	return (true);
}

void PhysicsEngine::reset()
{

}

void
PhysicsEngine::setOnCollide(boost::function<void(shared_ptr<PhysicsObject>, shared_ptr<PhysicsObject>, float forceMagnitude)> callback)
{
	onCollideFunction = callback;
}

void
PhysicsEngine::setTimeStep(double time)
{
	__int64 frequency;
	Log& log = Log::instance();
	if(!QueryPerformanceFrequency((LARGE_INTEGER*)&frequency))
		log << "Physics Engine could not get high resolution timer";
	timeStep = (__int64)(time * frequency);
}
/**
 * @return true if the objects status was actually updated.
 */
bool PhysicsEngine::update()
{
	__int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	if(lastUpdate + timeStep >= currentTime)
		return (false);
	for(PhysicsObjectsIterator it = _pobjects.begin(); it != _pobjects.end(); ++it)
	{
		(*it)->update();
	}
	lastUpdate += timeStep;
	detectCollision();
	return (true);
}

void
PhysicsEngine::detectCollision()
{
	PhysicsObjectsIterator it2;
	for (PhysicsObjectsIterator it1 = _pobjects.begin(); it1 != _pobjects.end(); ++it1)
	{
		it2 = it1;
		for (++it2; it2 != _pobjects.end(); ++it2)
		{
			if (detectCollisionBasic(**it1, **it2))
				onCollide(*it1, *it2);
		}
	}
}

bool
PhysicsEngine::detectCollisionBasic(PhysicsObject& a, PhysicsObject& b)
{
	D3DXVECTOR3 abs_center[2];

	abs_center[0] = a.getBoundingSphere().center + *a.getPosition();
	abs_center[1] = b.getBoundingSphere().center + *b.getPosition();
	if (distance(abs_center[0], abs_center[1]) <= a.getBoundingSphere().radius + b.getBoundingSphere().radius)
		return (true);
	return (false);
}

bool
PhysicsEngine::detectCollisionAdvanced(PhysicsObject& a, PhysicsObject& b)
{
	return (true);
}
