#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "DXUT.h"
#include <string>
#include "game/DisplayObject.h"

class Graphics
{
	size_t numObjs;
	DisplayObject* disObjects;

public:
	Graphics();
	~Graphics();
	void SetupMatrices();
	void SetLights();
	void PointCamera(D3DXVECTOR3 cameraPos, D3DXVECTOR3 cameraLook);
	void MoveMesh(D3DXVECTOR3 pos, D3DXVECTOR3 rotation);
	void OnCreateDevice(IDirect3DDevice9* d3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc);
	//void CreateDirectionalLight();
};


#endif //GRAPHICS_H