#ifndef PHYSICS_MOVABLE_OBJECT_INTERFACE
#define PHYSICS_MOVABLE_OBJECT_INTERFACE

class PhysicsMovableObjectInterface
{
public:
	~PhysicsMovableObjectInterface() {};
	virtual void update() = 0;
	virtual void applyForce(const D3DXVECTOR3* position, const D3DXVECTOR3* force) = 0;
	virtual void setAbsoluteCartesianAcceleration(const D3DXVECTOR3& accel) = 0;
};

#endif //PHYSICS_MOVABLE_OBJECT_INTERFACE