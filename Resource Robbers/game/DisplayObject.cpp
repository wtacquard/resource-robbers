#include "DXUT.h"
#include "server/Object.h"
#include "game/WorldObject.h"
#include "common/typedef.h"
#include "DisplayObject.h"

DisplayObject::DisplayObject()
: WorldObject(), m_mesh(NULL)
{
	D3DXMatrixIdentity(&m_world);
}
DisplayObject::DisplayObject(ID id)
: WorldObject(id), m_mesh(NULL)
{
	D3DXMatrixIdentity(&m_world);
}

DisplayObject::DisplayObject(const DisplayObject& copy)
: WorldObject(copy), m_mesh(NULL)
{

}

DisplayObject::~DisplayObject()
{
	CleanUp();
}

void DisplayObject::Update()
{
	MoveMesh();
}

void DisplayObject::setTranslations(D3DXVECTOR3 pos, D3DXQUATERNION rotation){
	_m_pos = pos;
	_m_rotation = rotation;
}

void DisplayObject::UnLoadMesh()
{
	if (m_mesh)
	{
		m_mesh->Release();
		m_mesh = NULL;
	}
}

HRESULT DisplayObject::LoadMesh(LPCTSTR fname)
{
	LPD3DXBUFFER		pD3DXMtrlBuffer;

	if (m_mesh)
		UnLoadMesh();
	m_numMaterials = 0L;  
	//LPDIRECT3DTEXTURE9* texture;    // a pointer to a texture


	//LPCTSTR tempstr = L m_meshFilePath.c_str();
    // Load the mesh from the specified file
    //if( FAILED( D3DXLoadMeshFromX( L"../assests/3dmodels/smalltank1-6.x", D3DXMESH_SYSTEMMEM,
	if (FAILED(D3DXLoadMeshFromX(fname, D3DXMESH_SYSTEMMEM,
                                   DXUTGetD3D9Device(), NULL,
                                   &pD3DXMtrlBuffer, NULL, &m_numMaterials,
                                   &m_mesh)))
    {
        // If model is not in current folder, try parent folder
        if (FAILED(D3DXLoadMeshFromX(L"..\\smalltank1-6.x", D3DXMESH_SYSTEMMEM,
                                       DXUTGetD3D9Device(), NULL,
                                       &pD3DXMtrlBuffer, NULL, &m_numMaterials,
                                       &m_mesh)))
        {
            MessageBox( NULL, L"Could not find .x file", L"Meshes.exe", MB_OK );
            return E_FAIL;
        }
    }
    // We need to extract the material properties and texture names from the 
    // pD3DXMtrlBuffer
    D3DXMATERIAL* d3dxMaterials = ( D3DXMATERIAL* )pD3DXMtrlBuffer->GetBufferPointer();
    m_meshMaterials = new D3DMATERIAL9[m_numMaterials];
    if( m_meshMaterials == NULL )
        return E_OUTOFMEMORY;
    m_meshTextures = new LPDIRECT3DTEXTURE9[m_numMaterials];
    if( m_meshTextures == NULL )
        return E_OUTOFMEMORY;

    for( DWORD i = 0; i < m_numMaterials; i++ )
    {
        // Copy the material
        m_meshMaterials[i] = d3dxMaterials[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
        m_meshMaterials[i].Ambient = m_meshMaterials[i].Diffuse;

        m_meshTextures[i] = NULL;
        if( d3dxMaterials[i].pTextureFilename != NULL &&
            lstrlenA( d3dxMaterials[i].pTextureFilename ) > 0 )
        {
            // Create the texture
            if( FAILED( D3DXCreateTextureFromFileA( DXUTGetD3D9Device(),
                                                    d3dxMaterials[i].pTextureFilename,
                                                    &m_meshTextures[i] ) ) )
            {
                // If texture is not in current folder, try parent folder
                const CHAR* strPrefix = "../assests/3dmodels/";
                CHAR strTexture[MAX_PATH];
                strcpy_s( strTexture, MAX_PATH, strPrefix );
                strcat_s( strTexture, MAX_PATH, d3dxMaterials[i].pTextureFilename );
                // If texture is not in current folder, try parent folder
                if( FAILED( D3DXCreateTextureFromFileA( DXUTGetD3D9Device(),
                                                        strTexture,
                                                        &m_meshTextures[i] ) ) )
                {
                    MessageBox( NULL, L"Could not find texture map", L"Meshes.exe", MB_OK );
                }
            }
        }
    }

    // Done with the material buffer
    pD3DXMtrlBuffer->Release();
	return S_OK;
}



void DisplayObject::Render(){
			
    //Meshes are divided into subsets, one for each material. Render them in
    //a loop
	DXUTGetD3D9Device()->SetTransform(D3DTS_WORLD, &(m_world));	//Set world matrix
    for( DWORD i = 0; i < m_numMaterials; i++ )
    {
        // Set the material and texture for this subset
        DXUTGetD3D9Device()->SetMaterial( &m_meshMaterials[i] );
        DXUTGetD3D9Device()->SetTexture( 0, m_meshTextures[i] );

        // Draw the mesh subset
        m_mesh->DrawSubset( i );
    }
}

void DisplayObject::CleanUp()
{
    if( m_meshMaterials != NULL )
	{
        delete[] m_meshMaterials;
		m_meshMaterials = NULL;
	}
    if( m_meshTextures )
    {
        for( DWORD i = 0; i < m_numMaterials; i++ )
        {
            if( m_meshTextures[i] )
			{
                m_meshTextures[i]->Release();
				m_meshTextures[i] = NULL;
			}
        }
        delete[] m_meshTextures;
		m_meshTextures = NULL;
    }
	UnLoadMesh();
}

//--------------------------------------------------------------------------------------
// Move object to absolute position and rotation
// Test purpose, setup manually position and rotation
// use MoveMesh() instead
//--------------------------------------------------------------------------------------
void DisplayObject::MoveMesh(D3DXVECTOR3 pos, D3DXQUATERNION rotation){
	//D3DXMATRIX matRotateX;
	//D3DXMATRIX matRotateY;
	D3DXMATRIX matRotate;
	D3DXMATRIX matTrans;
	D3DXMATRIX matFinal;

	D3DXQuaternionNormalize(&rotation, &rotation);
	D3DXMatrixRotationQuaternion(&matRotate, &rotation);


	//D3DXMatrixRotationX(&matRotateX, rotation.x);	
	//D3DXMatrixRotationY(&matRotateY, rotation.y);
	//D3DXMatrixRotationZ(&matRotateZ, rotation.z);
	D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);

	D3DXMatrixMultiply(&matFinal, &matRotate, &matTrans);
	//matFinal = matRotate * matTrans;
	//matRotate = matRotateZ * matRotateY * matRotateX * matTrans;
	m_world = matFinal;
	//DXUTGetD3D9Device()->SetTransform(m_world, &(matRotate));
}

void DisplayObject::MoveMesh(){
	//D3DXMATRIX matRotateX;
	//D3DXMATRIX matRotateY;
	D3DXMATRIX matRotate;
	D3DXMATRIX matTrans;
	D3DXMATRIX matFinal;
	
	D3DXQuaternionNormalize(&_m_rotation, &_m_rotation);
	D3DXMatrixRotationQuaternion(&matRotate, &_m_rotation);

	//D3DXMatrixRotationX(&matRotateX, rotation.x);	
	//D3DXMatrixRotationY(&matRotateY, rotation.y);
	//D3DXMatrixRotationZ(&matRotateZ, rotation.z);
	D3DXMatrixTranslation(&matTrans, _m_pos.x, _m_pos.y, _m_pos.z);

	D3DXMatrixMultiply(&matFinal, &matRotate, &matTrans);
	//matFinal = matRotate * matTrans;
	//matRotate = matRotateZ * matRotateY * matRotateX * matTrans;
	m_world = matFinal;
	//DXUTGetD3D9Device()->SetTransform(m_world, &(matRotate));
}