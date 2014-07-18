#ifndef CLIENT_CONTROLLER_H
# define CLIENT_CONTROLLER_H

#include "controller/InputController.h"
#include "graphics/graphics.h"
#include "physics/PhysicsEngine.h"

#include "WindowDelegator/WindowDelegator.h"

class ClientController
{
private:
	static ClientController *_singleton;

	WindowDelegator		_windowDelegator;
	InputController		_inputController;
	LPD3DXFONT			_d3dxFont;
	LPD3DXSPRITE		_d3dxSprite;
	RR::SocketClient	_socket;

private:
	ClientController();
	ClientController(const ClientController&) : _windowDelegator(0) {};
	ClientController& operator=(const ClientController&) {return *this;};
	~ClientController();

public:
	typedef enum {
		WINDOW_MAIN_MENU = 0,
		WINDOW_GAME,
		WINDOW_COUNT
	} Windows;

	static ClientController& instance();
	void destroy();

	static bool CALLBACK IsD3D9DeviceAcceptable(D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
		bool bWindowed, void* pUserContext );
	static void CALLBACK OnMouse(bool leftButtonDown, bool rightButtonDown, bool middleButtonDown, bool sideButton1Down,
		bool sideButton2Down, int mouseWheelDelta, int xPos, int yPos, void* context);
	static void CALLBACK OnKey(InputController::VirtualKey key, bool isKeyDown, bool isAltDown, void* context);
	static void CALLBACK OnD3D9DestroyDevice( void* pUserContext );
	static void CALLBACK OnD3D9LostDevice( void* pUserContext );
	static LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
		bool* pbNoFurtherProcessing, void* pUserContext );
	static void CALLBACK OnD3D9FrameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, 
		void* pUserContext );
	static void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
	static HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, 
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	static HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* d3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext );
	static bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );

	LPD3DXFONT			getFont();
	void				setFont(LPD3DXFONT f) {_d3dxFont = f;};
	LPD3DXSPRITE		getSprite();
	void				setSprite(LPD3DXSPRITE s) {_d3dxSprite = s;};
	RR::SocketClient&	getSocket();
	WindowDelegator&	getWindowDelegator();
	void				getSocketData(std::string& buffer);

	bool	connectTo(const std::string& addr, const std::string& port);

};

#endif //CLIENT_CONTROLLER_H