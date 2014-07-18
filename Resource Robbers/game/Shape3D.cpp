#include "Shape3D.h"

Shape3D::Shape3D(DisplayObject * object) {
	this->object = object;
}

void Shape3D::setTranslations(D3DXVECTOR3 pos, D3DXQUATERNION rotation) {
	object->setTranslations(pos, rotation);
}

void Shape3D::MoveMesh() {
	object->MoveMesh();
}

void Shape3D::MoveMesh(D3DXVECTOR3 pos, D3DXQUATERNION rotation) {
	object->MoveMesh(pos, rotation);
}

void Shape3D::Render() {
	object->Render();
}

DisplayObject * Shape3D::getObject() {
	return object;
}

void Shape3D::Update() {
	object->Update();
}

void Shape3D::setObject(DisplayObject * newObject) {
	object = newObject;
}