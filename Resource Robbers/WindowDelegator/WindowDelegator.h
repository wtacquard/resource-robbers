#ifndef WINDOWDELEGATOR_H
#define WINDOWDELEGATOR_H

#include "DXUT.h"
#include "Window.h"

class WindowDelegator
{
    unsigned int activeWindow;
    const unsigned int maxSize;
    Window** windows;
    void** windowContexts;

	WindowDelegator(const WindowDelegator&);  //don't use
	WindowDelegator& operator =(const WindowDelegator&);  //don't use

    public:
    WindowDelegator(const unsigned int size);
    ~WindowDelegator();
	void onCreateDevice(IDirect3DDevice9* d3dDevice, const D3DSURFACE_DESC* backBufferSurfaceDesc);
	void onDestroyDevice();
    void parseKeys( UINT vKey, bool keyDown, bool altDown );
    void frameMove( double totalTime, float elapsedTime );
    Window* put( Window *window, void* windowContext, unsigned int index );
    HRESULT render( IDirect3DDevice9 *pd3dDevice, double totalTime,
        float elapsedTime );
	void parseMouse(bool leftButtonDown, bool rightButtonDown, bool middleButtonDown, bool sideButton1Down,
		bool sideButton2Down, int mouseWheelDelta, int xPos, int yPos);
	void onNetworkSignal(std::string &message);
    void setActive( const unsigned int index );
	Window* getWindow(unsigned int index);
};

#endif //WINDOWDELEGATOR_H
