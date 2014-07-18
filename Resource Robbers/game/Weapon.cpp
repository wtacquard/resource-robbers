#include <dxut.h>
#include "DisplayObject.h"
#include "Weapon.h"

Weapon::Weapon()
 : DisplayObject()
{
	//set starting location i.e. offset from main mesh
	startPos.x = 0;
	startPos.y = 0;
	startPos.z = 0;

	//set rotation
	rotation.x = 0;
	rotation.y = 3.14f/2;
	rotation.z = 0;
}

Weapon::~Weapon() {}

void Weapon::setOffset(D3DXVECTOR3 offset){
	startPos = offset;
	_m_pos = offset;

}