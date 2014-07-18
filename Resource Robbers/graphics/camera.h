#ifndef CAMERA_H
#define CAMERA_H

class Camera{

public:
	enum CameraType { THIRDPERSON, LANDOBJECT, AIRCRAFT };

	Camera();
	Camera(CameraType cameraType);
	~Camera();
	void zoom(int units);		//Soom camera in and out
	void strafe(float units); // left/right
	void fly(float units);    // up/down
	void walk(float units);   // forward/backward
	void walk(D3DXVECTOR3* npos);   // forward/backward
	
	void orbitHorizontal(int direction);
	void orbitVertical(int direction);
	void setMouse(int x, int y);

	void pitch(float angle); // rotate on right vector
	void yaw(float angle);   // rotate on up vector
	void roll(float angle);  // rotate on look vector

	void getViewMatrix(D3DXMATRIX* V); 
	void setCameraType(CameraType cameraType); 
	void getPosition(D3DXVECTOR3* pos); 
	void setPosition(D3DXVECTOR3* pos); 
	void setLook(D3DXVECTOR3* look); 

	void getRight(D3DXVECTOR3* right);
	void getUp(D3DXVECTOR3* up);
	void getLook(D3DXVECTOR3* look);
private:
	CameraType  _cameraType;
	D3DXVECTOR3 _right;
	D3DXVECTOR3 _up;
	D3DXVECTOR3 _look;
	D3DXVECTOR3 _pos;
	D3DXVECTOR3 _posOff;
	float		_offset;
	float		_angleVert;
	float		_minZoom;
	int			_mouseXpos;
	int			_mouseYpos;
};

#endif CAMERA_H