#ifndef TRANSFORM_GROUP_H
#define TRANSFORM_GROUP_H

#include "Group.h"

class TransformGroup : public Group {
public:
	void setTranslations(D3DXVECTOR3 pos, D3DXQUATERNION rotation);
	void MoveMesh();
	void MoveMesh(D3DXVECTOR3 pos, D3DXQUATERNION rotation);
	void Render();
	void Update();
};

#endif