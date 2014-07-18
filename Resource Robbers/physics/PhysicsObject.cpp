#include "server/Object.h"
#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
: absoluteCartesianPosition(0.0f, 0.0f, 0.0f), orientation(0.0f, 0.0f, 0.0f, 1.0f)
{
}

PhysicsObject::PhysicsObject(const wchar_t *xfile)
{
	LPD3DXMESH ppMeshSysMem; 
	LPD3DXBUFFER ppAdjacencyBuffer; 
	LPD3DXBUFFER ppD3DXMtrlBuffer; 
	DWORD NumMaterials;
	LPD3DXVECTOR3 buf;

	if (FAILED(D3DXLoadMeshFromX(xfile, D3DXMESH_SYSTEMMEM, DXUTGetD3D9Device(),
		&ppAdjacencyBuffer, &ppD3DXMtrlBuffer, NULL,
		&NumMaterials, &ppMeshSysMem)))
		return ;
	ppMeshSysMem->LockVertexBuffer(0, (void**)&buf);
	if (FAILED(D3DXComputeBoundingSphere(buf, ppMeshSysMem->GetNumVertices(),
		ppMeshSysMem->GetNumBytesPerVertex(), &_sphere.center, &_sphere.radius)))
		return ;
	if (FAILED(D3DXComputeBoundingBox(buf, ppMeshSysMem->GetNumVertices(),
		ppMeshSysMem->GetNumBytesPerVertex(), &_box.lower_left, &_box.upper_right)))
		return ;
	ppMeshSysMem->UnlockVertexBuffer();
	ppMeshSysMem->Release();
	ppD3DXMtrlBuffer->Release();
	ppAdjacencyBuffer->Release();
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::update()
{
}

D3DXQUATERNION* PhysicsObject::getOrientation()
{
	return &orientation;
}

D3DXVECTOR3* PhysicsObject::getPosition()
{
	return &absoluteCartesianPosition;
}

std::string& 
PhysicsObject::serialize(std::string& str)
{
	std::string objXml;
	Object::serialize(objXml);
	std::ostringstream os;
	os << "<physics>" <<
			objXml.c_str() <<
			"<position>" << 
				"<x>" << absoluteCartesianPosition.x << "</x>" <<
				"<y>" << absoluteCartesianPosition.y << "</y>" <<
				"<z>" << absoluteCartesianPosition.z << "</z>" <<
			"</position>" <<
			"<orientation>" <<
				"<w>" << orientation.w << "</w>" <<
				"<x>" << orientation.x << "</x>" <<
				"<y>" << orientation.y << "</y>" <<
				"<z>" << orientation.z << "</z>" <<
			"</orientation>" <<
		"</physics>";
	return str += os.str();
}

Object*
PhysicsObject::deserialize(const std::string& str)
{
	return NULL;
}

const PhysicsObject::BoundingSphere&
PhysicsObject::getBoundingSphere() const
{
	return (_sphere);
}

const PhysicsObject::BoundingBox&
PhysicsObject::getBoundingBox() const
{
	return (_box);
}