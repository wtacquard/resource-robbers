#include "DXUT.h"
#include "server/Object.h"
#include "WorldObject.h"


WorldObject::WorldObject()
 : Object(), _m_pos(), _m_rotation()
{

}

WorldObject::WorldObject(ID id)
: Object(id), _m_pos(), _m_rotation()
{

}

WorldObject::WorldObject(const WorldObject& copy)
 : Object(copy), _m_pos(copy._m_pos), _m_rotation(copy._m_rotation)
{

}

WorldObject::~WorldObject()
{

}

WorldObject&
WorldObject::operator=(const WorldObject& copy)
{
	_m_pos = copy._m_pos;
	_m_rotation = copy._m_rotation;
	return (*this);
}

D3DXVECTOR3
WorldObject::m_pos() const
{
	return (_m_pos);
}

void
WorldObject::m_pos(D3DXVECTOR3 pos)
{
	_m_pos = pos;
}

D3DXQUATERNION
WorldObject::m_rotation() const
{
	return (_m_rotation);
}

void
WorldObject::m_rotation(D3DXQUATERNION rot)
{
	_m_rotation = rot;
}
