#include <winsock2.h>
#include <string>
#include <sstream>
#include <vector>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "Socket.hpp"
#include "AbstractServer.hpp"
#include "Server.hpp"
#include "PhysicsMovableObject.h"
#include "server/ServerController.h"
#include "common/MoreMath.h"
#include <Log.hpp>

PhysicsMovableObject::PhysicsMovableObject()
: absoluteCartesianVelocity(0.0f, 0.0f, 0.0f),
	absoluteCartesianAcceleration(0.0f, 0.0f, 0.0f),
	inertia(0.0f, 0.0f, 0.0f),
	eulerAngles(0.0f, 0.0f, 0.0f),
	localCenterOfGravity(0.0f, 0.0f, 0.0f),
	localCartesianVelocity(0.0f, 0.0f, 0.0f),
	localAngularVelocity(0.0f, 0.0f, 0.0f),
	momentOfInertia(),
	momentOfInertiaInverse(),
	totalForce(0.0f, 0.0f, 0.0f),
	totalTorque(0.0f, 0.0f, 0.0f),
	speed(0.0f),
	mass(1.0f)
{
	calculateMassProperties();
}

PhysicsMovableObject::PhysicsMovableObject(const wchar_t *xfile)
 : PhysicsObject(xfile)
{
	calculateMassProperties();
}

PhysicsMovableObject::~PhysicsMovableObject()
{
}

void PhysicsMovableObject::calculateMassProperties()
{
	float ixx = 0;
	float iyy = 0;
	float izz = 0;
	float ixy = 0;
	float ixz = 0;
	float iyz = 0;

	//for each component when we get there...
	ixx += inertia.x + mass * (localCenterOfGravity.y * localCenterOfGravity.y + localCenterOfGravity.z * localCenterOfGravity.z);
	iyy += inertia.y + mass * (localCenterOfGravity.x * localCenterOfGravity.x + localCenterOfGravity.z * localCenterOfGravity.z);
	izz += inertia.z + mass * (localCenterOfGravity.x * localCenterOfGravity.x + localCenterOfGravity.y * localCenterOfGravity.y);
	ixy += mass * localCenterOfGravity.x * localCenterOfGravity.y;
	ixz += mass * localCenterOfGravity.x * localCenterOfGravity.z;
	iyz += mass * localCenterOfGravity.y * localCenterOfGravity.z;

	momentOfInertia._11 = ixx;		momentOfInertia._12 = -ixy;		momentOfInertia._13 = -ixz;
	momentOfInertia._21 = -ixy;		momentOfInertia._22 = iyy;		momentOfInertia._23 = -iyz;
	momentOfInertia._31 = -ixz;		momentOfInertia._32 = -iyz;		momentOfInertia._33 = izz;

	D3DXMatrixInverse(&momentOfInertiaInverse, NULL, &momentOfInertia);
}

void PhysicsMovableObject::calculateLoads()
{
	VectorArrayIterator end;
	D3DXVECTOR3 totalTorque(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 totalForce(0.0f, 0.0f, 0.0f);
	for(VectorArrayIterator it = appliedForces.begin(), end = appliedForces.end(); it != end; it++)
	{
		D3DXVECTOR3 torque;
		D3DXVec3Cross(&torque, &it->first, &it->second);
		totalTorque += torque;
		totalForce += it->second;
	}
	totalForce += ServerController::instance().getPhysicsEngine().getGravity();
	this->totalTorque = totalTorque;
	this->totalForce = totalForce;
	appliedForces.clear();
}

//coordinates must be in local coordinates in respect to the object's center of gravity
void PhysicsMovableObject::applyForce(const D3DXVECTOR3* position, const D3DXVECTOR3* force)
{
	/*
	RR::Log& log = RR::Log::instance();
	log << "applying force x: " << position->x << 
		" y: " << position->y <<
		" z: " << position->z <<
		" and force x: " << force->x <<
		" y: " << force->y <<
		" z: " << force->z << std::endl;
		*/
		
	appliedForces.push_back(new std::pair<D3DXVECTOR3, D3DXVECTOR3>(*position, *force));
}
float PhysicsMovableObject::getMass() { return mass; }
D3DXVECTOR3* PhysicsMovableObject::getVelocity() { return &absoluteCartesianVelocity; }
void PhysicsMovableObject::setAbsoluteCartesianAcceleration(const D3DXVECTOR3& accel)
{
	absoluteCartesianVelocity = accel;
}

void PhysicsMovableObject::update()
{
	D3DXVECTOR3 acceleration;
	calculateLoads();
	acceleration = totalForce / mass;
	absoluteCartesianVelocity += acceleration * ServerController::instance().getPhysicsEngine().getTimeStep();
	absoluteCartesianPosition += absoluteCartesianVelocity * ServerController::instance().getPhysicsEngine().getTimeStep();

	//calculate the angular velocity of the object in body space
	//Look at the physics book p235? I don't know what's going on either.
	D3DXVECTOR3 temp1, temp2;
	matrixMultiplyByVector(&temp1, &momentOfInertia, &localAngularVelocity);
	D3DXVec3Cross(&temp2, &localAngularVelocity, &temp1);
	temp1 = totalTorque - temp2;
	matrixMultiplyByVector(&temp2, &momentOfInertiaInverse, &temp1);
	localAngularVelocity += temp2 * ServerController::instance().getPhysicsEngine().getTimeStep();

	//calcualte the new rotation quaternion
	D3DXQUATERNION result;
	orientation += *quaternionMultiplyByVector(&result, &orientation, &localAngularVelocity) * 
		0.5f * ServerController::instance().getPhysicsEngine().getTimeStep();
	/*
	localAngularVelocity += totalTorque * physics.getTimeStep();
	D3DXQuaternionRotationAxis(&orientation, &localAngularVelocity, 
		D3DXVec3Length(&localAngularVelocity) * physics.getTimeStep());
	*/
	D3DXQuaternionNormalize(&orientation, &orientation);
	D3DXQUATERNION inverseOrientation;
	D3DXVECTOR3 rotationResult;
	localCartesianVelocity += *quaternionVectorRotate(&rotationResult,
		D3DXQuaternionInverse(&inverseOrientation, &orientation), &absoluteCartesianVelocity);
	speed = D3DXVec3Length(&absoluteCartesianVelocity);
	quaternionToEulerAngles(&eulerAngles, &orientation);
}
