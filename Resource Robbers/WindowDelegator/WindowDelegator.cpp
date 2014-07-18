#include "WindowDelegator.h"

WindowDelegator::WindowDelegator(const unsigned int size) : activeWindow(0),
    maxSize(size)
{
    windows = new Window*[size];
    windowContexts = new void*[size];
}

Window* WindowDelegator::getWindow(unsigned int index)
{
	return windows[index];
}

void WindowDelegator::parseKeys( UINT vKey, bool keyDown, bool altDown )
{
    windows[activeWindow]->parseKeys( vKey, keyDown, altDown );
}

void WindowDelegator::parseMouse(bool leftButtonDown, bool rightButtonDown, bool middleButtonDown, bool sideButton1Down,
		bool sideButton2Down, int mouseWheelDelta, int xPos, int yPos)
{
	windows[activeWindow]->parseMouse(leftButtonDown, rightButtonDown, middleButtonDown, sideButton1Down,
		sideButton2Down, mouseWheelDelta, xPos, yPos, windowContexts[activeWindow]);
}

void WindowDelegator::frameMove( double totalTime, float elapsedTime )
{
    windows[activeWindow]->frameMove( totalTime, elapsedTime,
        windowContexts[activeWindow] );
}

void WindowDelegator::onCreateDevice(IDirect3DDevice9* d3dDevice, const D3DSURFACE_DESC* backBufferSurfaceDesc)
{
	for(size_t i = 0; i < maxSize; i++)
	{
		if(windows[i] != NULL)
			windows[i]->onCreateDevice(d3dDevice, backBufferSurfaceDesc);
	}
}

void WindowDelegator::onDestroyDevice()
{
	size_t i;

	for(i = 0; i < maxSize; i++)
	{
		if(windows[i] != NULL)
			windows[i]->onDestroyDevice();
	}
}

Window* WindowDelegator::put( Window *window, void* windowContext,
    unsigned int index )
{
	HRESULT hr;
    if( index > maxSize )
        V(D3DERR_INVALIDCALL);

    Window *old = windows[index];
    windows[index] = window;
    windowContexts[index] = windowContext;
    return old;
}

HRESULT WindowDelegator::render( IDirect3DDevice9 *pd3dDevice, 
    double totalTime, float elapsedTime )
{
    return windows[activeWindow]->render( pd3dDevice, totalTime, elapsedTime, 
        windowContexts[activeWindow] );
}

void WindowDelegator::setActive( const unsigned int index )
{
	HRESULT hr;
    if( index > maxSize )
        V( D3DERR_INVALIDCALL );

    activeWindow = index;
}

void
WindowDelegator::onNetworkSignal(std::string &message)
{
	windows[activeWindow]->onNetworkSignal(message);
}

WindowDelegator::~WindowDelegator()
{
    delete[] windows;
	windows = NULL;
	delete[] windowContexts;
	windowContexts = NULL;
}
