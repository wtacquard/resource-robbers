#include <d3dx9.h>
#include "DXUT.h"
#include <iostream>

/**
	return all vertices from a mesh
	@param pMesh[in] The mesh to analyse
	@param ret[out] A non-initialized pointer in which an array is created and filled with vertices
	@return Numbers of vertices in array
**/
DWORD	getVertices(LPD3DXMESH pMesh, D3DXVECTOR3 vertices[])
{
	unsigned int	numVertices = pMesh->GetNumVertices();
	LPD3DXVECTOR3		  v = 0;

//	vertices = new D3DXVECTOR3[numVertices];
	pMesh->LockVertexBuffer(0, (void**)&v);
	for(UINT i = 0; i < numVertices; i++)
	{
		vertices[i].x = v[i].x;
		vertices[i].y = v[i].y;
		vertices[i].z = v[i].z;
	}
	pMesh->UnlockVertexBuffer();
	return (numVertices);
   /*// Extract indices
   pData->indices = new int[numFaces * 3];   
   WORD* ind = 0;
   pMesh->LockIndexBuffer(0,(void**)&ind);      
 
   
   //memcpy( &indices, &ind, sizeof(ind));   
   for (int i=0;i < numFaces; i++)
   {   
      pData->indices[i*3+0] = ind[i*3+0];
      pData->indices[i*3+1] = ind[i*3+1];
      pData->indices[i*3+2] = ind[i*3+2];      
   }

   pMesh->UnlockIndexBuffer();      

   int indexStride = 3 * sizeof(int);
   int vertStride = sizeof(btVector3);

   pData->indexVertexArrays = new btTriangleIndexVertexArray(numFaces, pData->indices, indexStride,
      numVertices, (btScalar*) &pData->vertices[0], sizeof(btScalar) * 3);

   bool useQuantizedAabbCompression = true;
   btBvhTriangleMeshShape *shape = new btBvhTriangleMeshShape(pData->indexVertexArrays, true);   
      
   //delete indices;
   //delete vertices;   
   //delete indexVertexArrays;   

   return shape;
*/
   }

typedef struct
{
	float radius;
	D3DXVECTOR3 center;
}sphere;

typedef struct
{
	D3DXVECTOR3 pos;
	sphere bounding;
	D3DXVECTOR3 dir;
	float mass;
} vehicule;

bool
init_vehicule(vehicule &v, const wchar_t *addr_mesh)
{
	LPD3DXMESH ppMeshSysMem; 
	LPD3DXBUFFER ppAdjacencyBuffer; 
	LPD3DXBUFFER ppD3DXMtrlBuffer; 
	DWORD NumMaterials;
	LPD3DXVECTOR3 buf;

	if (FAILED(D3DXLoadMeshFromX (addr_mesh, D3DXMESH_SYSTEMMEM, DXUTGetD3D9Device(),
		&ppAdjacencyBuffer, &ppD3DXMtrlBuffer, NULL,
		&NumMaterials, &ppMeshSysMem)))
		return (false);
	ppMeshSysMem->LockVertexBuffer(0, (void**)&buf);
	if (FAILED(D3DXComputeBoundingSphere(buf, ppMeshSysMem->GetNumVertices(),
		ppMeshSysMem->GetNumBytesPerVertex(), &v.bounding.center, &v.bounding.radius)))
		return (false);
	ppMeshSysMem->UnlockVertexBuffer();
ppMeshSysMem->Release();
ppD3DXMtrlBuffer->Release();
ppAdjacencyBuffer->Release();
	return (true);
}

void
delete_vehicule(vehicule &v)
{
//	delete[] v.vertices;
}

INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
	vehicule obj[2];

	DXUTInit();
	DXUTCreateDevice();
	for (int i = 0; i < 2; ++i)
		if (init_vehicule(obj[i], L"./Debug/test.x") == false)
			return (i + 1);
	for (int i = 0; i < 2; ++i)
		std::cout<<"("<<obj[i].pos.x<<";"<<obj[i].pos.y<<";"<<obj[i].pos.z<<")"<<std::endl;
	for (int i = 0; i < 2; ++i)
		delete_vehicule(obj[i]);
	return (0);
}

float
distance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	float x = pos2.x - pos1.x;
	float y = pos2.y - pos1.y;
	float z = pos2.z - pos1.z;

	return (sqrt(x*x + y*y + z*z));
}

bool
detect_collision(const vehicule &a, const vehicule &b)
{
	return (true);
}

bool
detect_basic_collision(const vehicule &a, const vehicule &b)
{
	D3DXVECTOR3 abs_center[2];

	abs_center[0] = a.bounding.center + a.pos;
	abs_center[0] = b.bounding.center + b.pos;
	if (distance(abs_center[0], abs_center[1]) <= a.bounding.radius + b.bounding.radius)
		return (true);
	return (false);
}

void
handle_collision(vehicule &a, vehicule &b)
{
	const float restitution = 1.f;
	D3DXVECTOR3 normal = (b.bounding.center + b.pos) - (a.bounding.center + a.pos);

	D3DXVec3Normalize(&normal, &normal);

	float j;
	D3DXVECTOR3 relativeVelocity = a.dir - b.dir;
	
	j = (D3DXVec3Dot(&relativeVelocity, &normal) * -(1.f + restitution))
		/ (D3DXVec3Dot(&normal, &normal) * 
		(1.f / a.mass + 1.f / b.mass));
	a.dir += (normal / a.mass) * j;
	b.dir -= (normal / b.mass) * j;
}

void collision(vehicule *obj, int num)
{
	for (int i = 0; i < num - 2; ++i)
		for (int j = 0; j < num - 1; ++j)
	{
		if (detect_basic_collision(obj[i], obj[j]) && detect_collision(obj[i], obj[j]))
		{
			std::clog<<i<<" collide with "<<j<<std::endl;
			handle_collision(obj[i], obj[j]);
		}
	}
}