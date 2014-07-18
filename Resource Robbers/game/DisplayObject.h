#ifndef DISPLAYOBJECT_H_
# define DISPLAYOBJECT_H_

#include <string>
#include "DXUT.h"

#include "server/Object.h"
#include "game/WorldObject.h"

using namespace std;

class DisplayObject : public WorldObject
{
public:
	DisplayObject();
	DisplayObject(ID);
	DisplayObject(const DisplayObject& copy);
	~DisplayObject();

	void Update();
	void setTranslations(D3DXVECTOR3 pos, D3DXQUATERNION rotation);
	//void SetFile(LPCTSTR fname);
	HRESULT LoadMesh(LPCTSTR fname);
	void UnLoadMesh();

	virtual void Render();
	void CleanUp();
	virtual void MoveMesh(D3DXVECTOR3 pos, D3DXQUATERNION rotation);
	virtual void MoveMesh();
	
	//public access functions
	LPCTSTR GetMeshFilePath()				{return m_meshFilePath;};
	LPCTSTR GetTextureFilePath()			{return m_textureFilePath;};
	LPD3DXMESH GetMesh()					{return m_mesh;};
	D3DXMATRIX GetTransMatrix()				{return m_transformation;};
	D3DXMATRIX GetWorldMatrix()				{return m_world;};
	DWORD GetNumMaterials()					{return m_numMaterials;};
	D3DMATERIAL9* GetMeshMaterials()		{return m_meshMaterials;};
	LPDIRECT3DTEXTURE9* GetMeshTextures()	{return m_meshTextures;};

private:
	D3DXMATRIX m_transformation;			//Transformation matrix
	D3DXMATRIX m_world;						//Objects world matrix;
	LPCTSTR m_meshFilePath;					//File path to mesh on drive
	LPCTSTR m_textureFilePath;				//File path to texture on drive
	LPD3DXMESH m_mesh;						//Mesh	
	DWORD	m_numMaterials;					//Number of mesh materials
	D3DMATERIAL9*       m_meshMaterials;	// Materials for our mesh
	LPDIRECT3DTEXTURE9* m_meshTextures;		// Textures for our mesh
};

#endif //!DISPLAYOBJECT_H_
