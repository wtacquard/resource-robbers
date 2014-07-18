#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include "DXUT.H"

using namespace std;

class Window
{
public:
	virtual void parseKeys( UINT vKey, bool keyDown, bool altDown ) = 0;
	virtual HRESULT render( IDirect3DDevice9 *d3dDevice, double totalTime,
		float elapsedTime, void *data ) = 0;
	virtual void frameMove( double totalTime, float elapsedTime, 
		void *data ) = 0;
	virtual void parseMouse(bool leftButtonDown, bool rightButtonDown, bool middleButtonDown, bool sideButton1Down,
		bool sideButton2Down, int mouseWheelDelta, int xPos, int yPos, void* context) = 0;
	virtual void onCreateDevice(IDirect3DDevice9* d3dDevice, const D3DSURFACE_DESC* backBufferSurfaceDesc) = 0;
	virtual void onDestroyDevice() = 0;
	virtual void onNetworkSignal(std::string &message) = 0;
};

#endif //WINDOW_H
