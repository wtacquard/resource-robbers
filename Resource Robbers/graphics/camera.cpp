//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: camera.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Defines a camera's position and orientation.
//         
//////////////////////////////////////////////////////////////////////////////////////////////////
#include <DXUT.h>
#include "camera.h"

#define MOUSE_ROTATE_SENSITIVITY 100.0
Camera::Camera()
{
	_cameraType = THIRDPERSON;

	//_pos   = D3DXVECTOR3(0.0f, 6.0f, -5.0f);
	_posOff   = D3DXVECTOR3(0.0f, 0.0f, -5.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look  = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_pos = _look + _posOff;
	//_angleVert = D3DXVec3Dot(&_posOff, &_right);
	
	_offset = 1;
	_angleVert	= 0;
	_minZoom = 3;


}

Camera::Camera(CameraType cameraType)
{
	_cameraType = cameraType;

	_pos   = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look  = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

Camera::~Camera()
{

}

void Camera::getPosition(D3DXVECTOR3* pos)
{
	*pos = _pos;
}

void Camera::setPosition(D3DXVECTOR3* pos)
{
	_pos = *pos;
}
void Camera::setLook(D3DXVECTOR3* look)
{
	_look = *look;
}
void Camera::getRight(D3DXVECTOR3* right)
{
	*right = _right;
}

void Camera::getUp(D3DXVECTOR3* up)
{
	*up = _up;
}

void Camera::getLook(D3DXVECTOR3* look)
{
	*look = _look;
}
void Camera::zoom(int units)
{
	D3DXVECTOR3 tmp;
	if(_cameraType == THIRDPERSON)
	{
		D3DXVec3Normalize(&tmp, &_posOff);
		tmp*= 2;
		if(units > 0){
			
			D3DXVec3Add(&_posOff, &_posOff,&tmp);
		}
		else if(units < 0 && !(_posOff.x < _minZoom && _posOff.y < _minZoom && _posOff.z < _minZoom)){
			D3DXVec3Subtract(&_posOff, &_posOff,&tmp);
		}
		
	}
}
void Camera::walk(float units)
{
	// move only on xz plane for land object
	if( _cameraType == LANDOBJECT )
		_pos += D3DXVECTOR3(_look.x, 0.0f, _look.z) * units;

	if( _cameraType == AIRCRAFT )
		_pos += _look * units;
}
void Camera::walk(D3DXVECTOR3* npos)
{
	if( _cameraType == THIRDPERSON ){
		setLook(npos);
		_pos = _look + _posOff;// 
		
	}
}
void Camera::orbitHorizontal(int pos)
{
	D3DXMATRIX matYrot;
	float angle = (_mouseXpos - pos)/MOUSE_ROTATE_SENSITIVITY;
	
	_mouseXpos = pos;

	D3DXMatrixRotationY(&matYrot, angle);
	D3DXVECTOR4 tpos;
	D3DXVec3Transform(&tpos, &_posOff, &matYrot);
	//tpos3/=tpos3.w;
	_posOff.x = tpos.x;
	_posOff.y = tpos.y;
	_posOff.z = tpos.z;
	_pos = _look + _posOff;
}
void Camera::orbitVertical(int pos)
{
	D3DXMATRIX matArot;
	float angle = (_mouseYpos - pos)/MOUSE_ROTATE_SENSITIVITY;
	float tempAngle = _angleVert + angle;
	if( tempAngle < 0 || tempAngle > 3.1/2)
		angle = 0;
	else
		_angleVert = tempAngle;
	_mouseYpos = pos;
	D3DXVECTOR3 xzAxis(_posOff.x, 0, _posOff.z);
	D3DXVECTOR3 yAxis(0, 1, 0);
	D3DXVec3Normalize(&xzAxis, &xzAxis);
	D3DXVec3Cross(&xzAxis, &xzAxis, &yAxis);
	D3DXMatrixRotationAxis(&matArot, &xzAxis, angle);

	D3DXVECTOR4 tpos;
	D3DXVec3Transform(&tpos, &_posOff, &matArot);
	//tpos3/=tpos3.w;
	_posOff.x = tpos.x;
	_posOff.y = tpos.y;
	_posOff.z = tpos.z;
	_pos = _look + _posOff;
}
void Camera::setMouse(int x, int y)
{
	_mouseXpos = x;
	_mouseYpos = y;
}

void Camera::strafe(float units)
{
	// move only on xz plane for land object
	if( _cameraType == LANDOBJECT )
		_pos += D3DXVECTOR3(_right.x, 0.0f, _right.z) * units;

	if( _cameraType == AIRCRAFT )
		_pos += _right * units;
}

void Camera::fly(float units)
{
	// move only on y-axis for land object
	if( _cameraType == LANDOBJECT )
		_pos.y += units;

	if( _cameraType == AIRCRAFT )
		_pos += _up * units;
}

void Camera::pitch(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &_right,	angle);

	// rotate _up and _look around _right vector
	D3DXVec3TransformCoord(&_up,&_up, &T);
	D3DXVec3TransformCoord(&_look,&_look, &T);
}

void Camera::yaw(float angle)
{
	D3DXMATRIX T;

	// rotate around world y (0, 1, 0) always for land object
	if( _cameraType == LANDOBJECT )
		D3DXMatrixRotationY(&T, angle);

	// rotate around own up vector for aircraft
	if( _cameraType == AIRCRAFT )
		D3DXMatrixRotationAxis(&T, &_up, angle);

	// rotate _right and _look around _up or y-axis
	D3DXVec3TransformCoord(&_right,&_right, &T);
	D3DXVec3TransformCoord(&_look,&_look, &T);
}

void Camera::roll(float angle)
{
	// only roll for aircraft type
	if( _cameraType == AIRCRAFT )
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &_look,	angle);

		// rotate _up and _right around _look vector
		D3DXVec3TransformCoord(&_right,&_right, &T);
		D3DXVec3TransformCoord(&_up,&_up, &T);
	}
}

void Camera::getViewMatrix(D3DXMATRIX* V)
{
	// Keep camera's axes orthogonal to eachother
	//D3DXVec3Normalize(&_look, &_look);

	D3DXVec3Cross(&_up, &_look, &_right);
	D3DXVec3Normalize(&_up, &_up);

	D3DXVec3Cross(&_right, &_up, &_look);
	D3DXVec3Normalize(&_right, &_right);
	if(_cameraType == THIRDPERSON){
		//D3DXMATRIX matView;
		D3DXMatrixLookAtLH(V, &_pos, &_look, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		//DXUTGetD3D9Device()->SetTransform(D3DTS_VIEW, &matView);
	}
	else{
		// Build the view matrix:
		float x = -D3DXVec3Dot(&_right, &_pos);
		float y = -D3DXVec3Dot(&_up, &_pos);
		float z = -D3DXVec3Dot(&_look, &_pos);

		(*V)(0,0) = _right.x; (*V)(0, 1) = _up.x; (*V)(0, 2) = _look.x; (*V)(0, 3) = 0.0f;
		(*V)(1,0) = _right.y; (*V)(1, 1) = _up.y; (*V)(1, 2) = _look.y; (*V)(1, 3) = 0.0f;
		(*V)(2,0) = _right.z; (*V)(2, 1) = _up.z; (*V)(2, 2) = _look.z; (*V)(2, 3) = 0.0f;
		(*V)(3,0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
	}
}

void Camera::setCameraType(CameraType cameraType)
{
	_cameraType = cameraType;
}
