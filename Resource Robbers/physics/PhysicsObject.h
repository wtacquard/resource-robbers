#ifndef PHYSICS_OBJECT_H
# define PHYSICS_OBJECT_H

#include <DXUT.h>
#include "server/Object.h"
class PhysicsObject : public Object
{
public:
	typedef struct {
		float radius;
		D3DXVECTOR3 center;
	}BoundingSphere;
	typedef struct {
		D3DXVECTOR3 lower_left;
		D3DXVECTOR3 upper_right;
	}BoundingBox;

protected:
	D3DXVECTOR3 absoluteCartesianPosition;
	D3DXQUATERNION orientation;
	BoundingSphere _sphere;
	BoundingBox _box;

public:
	PhysicsObject();
	PhysicsObject(const wchar_t *);
	virtual ~PhysicsObject();
	virtual void update();
	virtual D3DXQUATERNION* getOrientation();
	virtual D3DXVECTOR3* getPosition();
	const BoundingSphere& getBoundingSphere() const;
	const BoundingBox& getBoundingBox() const;

	virtual std::string& serialize(std::string& str);
	virtual Object* deserialize(const std::string& str);
};
#endif //PHYSICS_OBJECT_H