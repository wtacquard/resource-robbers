#ifndef PHYSICS_MOVABLE_OBJECT
#define PHYSICS_MOVABLE_OBJECT

#include <DXUT.h>
#include <memory>
#include <utility>
#include <boost/ptr_container/ptr_vector.hpp>
#include "PhysicsObject.h"
#include "PhysicsMovableObjectInterface.h"

using std::auto_ptr;
using boost::ptr_vector;
using std::pair;

class PhysicsMovableObject :
	public PhysicsObject, public PhysicsMovableObjectInterface
{
public:
	typedef ptr_vector<pair<D3DXVECTOR3, D3DXVECTOR3>> VectorArray;
	typedef ptr_vector<pair<D3DXVECTOR3, D3DXVECTOR3>>::iterator VectorArrayIterator;
	typedef ptr_vector<pair<D3DXVECTOR3, D3DXVECTOR3>>::const_iterator VectorArrayConstIterator;

	/* for adding different components and masses
	typedef vector<auto_ptr<PhysicsMovableObject::Component>> ComponentArray;
	typedef vector<auto_ptr<PhysicsMovableObject::Component>>::iterator ComponentArrayIterator;
	typedef vector<auto_ptr<PhysicsMovableObject::Component>>::const_iterator ComponentArrayConstIterator;
	*/
	D3DXVECTOR3 absoluteCartesianVelocity;
	D3DXVECTOR3 absoluteCartesianAcceleration;
	D3DXVECTOR3 inertia;
	D3DXVECTOR3 eulerAngles;
	D3DXVECTOR3 localCenterOfGravity;
	D3DXVECTOR3 localCartesianVelocity;
	D3DXVECTOR3 localAngularVelocity;
	D3DXMATRIX momentOfInertia;
	D3DXMATRIX momentOfInertiaInverse;
	D3DXVECTOR3 totalForce;
	D3DXVECTOR3 totalTorque;
	float speed;
	float mass;

	VectorArray appliedForces;
	//ComponentArray components;

	void calculateLoads();
	void calculateMassProperties();

public:
	PhysicsMovableObject();
	PhysicsMovableObject(const wchar_t *);
	virtual ~PhysicsMovableObject();
	virtual void applyForce(const D3DXVECTOR3* position, const D3DXVECTOR3* force);
	float getMass();
	D3DXVECTOR3* getVelocity();
	virtual void setAbsoluteCartesianAcceleration(const D3DXVECTOR3& accel);
	virtual void update();
};

#endif //PHYSICS_MOVABLE_OBJECT
