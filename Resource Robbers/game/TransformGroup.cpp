#include "TransformGroup.h"

void TransformGroup::setTranslations(D3DXVECTOR3 pos, D3DXQUATERNION rotation) {
	for (std::list<Node *>::const_iterator iter = childNodes.begin(); iter != childNodes.end(); iter++)
		(*iter)->setTranslations(pos, rotation);
}

void TransformGroup::MoveMesh() {
	for (std::list<Node *>::const_iterator iter = childNodes.begin(); iter != childNodes.end(); iter++)
		(*iter)->MoveMesh();
}

void TransformGroup::MoveMesh(D3DXVECTOR3 pos, D3DXQUATERNION rotation) {
	for (std::list<Node *>::const_iterator iter = childNodes.begin(); iter != childNodes.end(); iter++)
		(*iter)->MoveMesh(pos, rotation);
}

void TransformGroup::Render() {
	for (std::list<Node *>::const_iterator iter = childNodes.begin(); iter != childNodes.end(); iter++)
		(*iter)->Render();
}

void TransformGroup::Update() {
	for (std::list<Node *>::const_iterator iter = childNodes.begin(); iter != childNodes.end(); iter++)
		(*iter)->Update();
}