#ifndef SHAPE_3D_H
#define SHAPE_3D_H

#include "Leaf.h"
#include "DisplayObject.h"

class Shape3D : public Leaf {
public:
	Shape3D(DisplayObject * object);
	void setTranslations(D3DXVECTOR3 pos, D3DXQUATERNION rotation);
	void MoveMesh();
	void MoveMesh(D3DXVECTOR3 pos, D3DXQUATERNION rotation);
	void Render();
	DisplayObject * getObject();
	void Update();
	void setObject(DisplayObject * newObject);

private:
	DisplayObject * object;
};

#endif