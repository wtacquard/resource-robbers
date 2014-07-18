#include "graphics.h"

LPD3DXMESH          pMesh = NULL; // Our mesh object in sysmem
D3DMATERIAL9*       pMeshMaterials = NULL; // Materials for our mesh
LPDIRECT3DTEXTURE9* pMeshTextures = NULL; // Textures for our mesh
DWORD               NumMaterials = 0L;   // Number of mesh materials
LPDIRECT3DTEXTURE9* texture;    // a pointer to a texture


Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}



void Graphics::SetupMatrices()
{
	// Turn on the zbuffer
	DXUTGetD3D9Device()->SetRenderState( D3DRS_ZENABLE, TRUE );	
	//DXUTGetD3D9Device()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	// Turn on ambient lighting 
	//DXUTGetD3D9Device()->SetRenderState( D3DRS_AMBIENT, 0xffffffff );
	//DXUTGetD3D9Device()->SetRenderState(D3DRS_LIGHTING, TRUE);    // turn on the 3D lighting

	// SET UP THE TRANSFORMS

	D3DXMATRIX matView;    // the view transform matrix
	D3DXMatrixLookAtLH(&matView,
		&D3DXVECTOR3 (0.0f, 3.0f, 3.0f),    // the camera position
		&D3DXVECTOR3 (0.0f, 0.0f, 0.0f),    // the look-at position
		&D3DXVECTOR3 (0.0f, 1.0f, 0.0f));    // the up direction
	DXUTGetD3D9Device()->SetTransform(D3DTS_VIEW, &matView);    // set the view transform to matView 

	D3DXMATRIX matProjection;    // the projection transform matrix
	D3DXMatrixPerspectiveFovLH(&matProjection,
		D3DXToRadian(45),    // the horizontal field of view
		DXUTGetWindowWidth() / DXUTGetWindowHeight(),    // the aspect ratio
		1.0f,    // the near view-plane
		100.0f);    // the far view-plane
	DXUTGetD3D9Device()->SetTransform(D3DTS_PROJECTION, &matProjection);    // set the projection

	static float index = 0.0f; index+=0.03f;    // an ever-increasing float value
	//D3DXMATRIX matRotateY;    // a matrix to store the rotation for each triangle
	//D3DXMatrixRotationY(&matRotateY, index);    // the rotation matrix
	//DXUTGetD3D9Device()->SetTransform(D3DTS_WORLD, &(matRotateY));    // set the world transform
	PointCamera(D3DXVECTOR3(0.0f, 5.0f, 50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//MoveMesh(D3DXVECTOR3(1,0,0), D3DXVECTOR3(index,index,index));
}	


//--------------------------------------------------------------------------------------
// Turn on a Sun light
//--------------------------------------------------------------------------------------
void Graphics::SetLights(){

	D3DLIGHT9 light;
	ZeroMemory( &light, sizeof(D3DLIGHT9) );

	DXUTGetD3D9Device()->SetRenderState( D3DRS_LIGHTING, TRUE );
	DXUTGetD3D9Device()->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));
	
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Diffuse.a = 1.0f;
	light.Range = 1000.0f;
	D3DXVECTOR3 vDir(0.0f, -0.3f, 0.0f);
	
	light.Ambient.r = .1f;
	light.Ambient.g = .1f;
	light.Ambient.b = .1f;

	light.Specular.r = 1.0f;
	light.Specular.g = 1.0f;
	light.Specular.b = 1.0f;
	
	D3DXVec3Normalize ((D3DXVECTOR3*) &light.Direction, &vDir);
	
	DXUTGetD3D9Device()->SetLight(0, &light);
	DXUTGetD3D9Device()->LightEnable(0,TRUE);
}

//--------------------------------------------------------------------------------------
// Move camera to absolute postion and lookat point
//--------------------------------------------------------------------------------------
void Graphics::PointCamera(D3DXVECTOR3 cameraPos, D3DXVECTOR3 cameraLook){
	D3DXMATRIX matView; 
	D3DXMatrixLookAtLH(&matView, &cameraPos, &cameraLook, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	DXUTGetD3D9Device()->SetTransform(D3DTS_VIEW, &matView);
}
//--------------------------------------------------------------------------------------
// Move object to absolute position and rotation
//--------------------------------------------------------------------------------------
void Graphics::MoveMesh(D3DXVECTOR3 pos, D3DXVECTOR3 rotation){
	D3DXMATRIX matRotateX;
	D3DXMATRIX matRotateY;
	D3DXMATRIX matRotateZ;
	D3DXMATRIX matTrans;
	D3DXMATRIX matRotate;

	D3DXMatrixRotationX(&matRotateX, rotation.x);	
	D3DXMatrixRotationY(&matRotateY, rotation.y);
	D3DXMatrixRotationZ(&matRotateZ, rotation.z);
	D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);


	matRotate = matRotateZ * matRotateY * matRotateX * matTrans;

	DXUTGetD3D9Device()->SetTransform(D3DTS_WORLD, &(matRotate));
}