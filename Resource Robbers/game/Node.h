#ifndef NODE_H
#define NODE_H

#include <d3dx9math.h>

class Node {
public:
	virtual void setTranslations(D3DXVECTOR3 pos, D3DXQUATERNION rotation) = 0;
	virtual void MoveMesh() = 0;
	virtual void MoveMesh(D3DXVECTOR3 pos, D3DXQUATERNION rotation) = 0;
	virtual void Render() = 0;
	virtual void Update() = 0;
};

#endif