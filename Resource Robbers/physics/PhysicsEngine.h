#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <set>
#include <DXUT.h>
#include "PhysicsObject.h"


using boost::shared_ptr;

class PhysicsEngine
{
public:
	typedef std::set<shared_ptr<PhysicsObject>> PhysicsObjects;
	typedef std::set<shared_ptr<PhysicsObject>>::iterator PhysicsObjectsIterator;
	typedef std::set<shared_ptr<PhysicsObject>>::const_iterator PhysicsObjectsConstIterator;
private:
	D3DXVECTOR3 gravity;
	PhysicsObjects _pobjects;
	boost::function<void(shared_ptr<PhysicsObject>, shared_ptr<PhysicsObject>, float forceMagnitude)> onCollideFunction;
	__int64 timeStep;
	__int64 lastUpdate;

	bool detectCollisionBasic(PhysicsObject &a, PhysicsObject &b);
	bool detectCollisionAdvanced(PhysicsObject &a, PhysicsObject &b);

public:
	PhysicsEngine();
	~PhysicsEngine();
	static void destroy();
	const D3DXVECTOR3& getGravity() const;
	float getTimeStep() const;
	PhysicsObjects& getPhysicsObjects();
	bool isReadyToUpdate();
	void onCollide(shared_ptr<PhysicsObject> first, shared_ptr<PhysicsObject> second);
	bool registerPhysicsObject(shared_ptr<PhysicsObject> obj);
	bool remove(boost::shared_ptr<PhysicsObject> obj);
	void reset();
	void setGravity(const D3DXVECTOR3& gravity);
	void setOnCollide(boost::function<void(shared_ptr<PhysicsObject>, shared_ptr<PhysicsObject>, float forceMagnitude)> callback);
	void setTimeStep(double time);
	bool update();
	void detectCollision();
};

#endif
