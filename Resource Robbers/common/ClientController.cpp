#include <string>
#include <winsock2.h>
#include <exception>
#include "Exception.hpp"
#include "SocketException.hpp"
#include "Socket.hpp"
#include "SocketClient.hpp"

#include <map>
#include <vector>
#include "DXUT.h"
#include "common/typedef.h"
#include "common/DualBuffer.hh"
#include "server/Object.h"
#include "game/WorldObject.h"
#include "game/DisplayObject.h"
#include "game/Weapon.h"
#include "game/Vehicle.h"
#include "game/Merger.hpp"
#include "common/ClientController.h"
#include "game/Game.h"
#include "mainMenu/MainMenu.h"
#include <memory>

#define safeRelease(x) if((x) != NULL) { (x)->Release(); (x) = NULL; }

ClientController* ClientController::_singleton = NULL;

ClientController::ClientController()
 : _windowDelegator(WINDOW_COUNT), _inputController(),
 _d3dxFont(NULL), _d3dxSprite(NULL)//, _socket()
{
	_windowDelegator.put(new MainMenu(), NULL, WINDOW_MAIN_MENU);
	_windowDelegator.put(new Game(_socket), NULL, WINDOW_GAME);
}

ClientController::~ClientController()
{
	for (int i = 0; i < WINDOW_COUNT; ++i)
	{
		delete _windowDelegator.put(NULL, NULL, i);
	}
}

ClientController&
ClientController::instance()
{
	return ((_singleton) ? *_singleton : *(_singleton = new ClientController()));
}

void
ClientController::destroy()
{
	_singleton = NULL;
	delete this;
}

bool CALLBACK ClientController::IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
                                      bool bWindowed, void* pUserContext )
{
    // Typically want to skip back buffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                         AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
                                         D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    return true;
}

void CALLBACK ClientController::OnMouse(bool leftButtonDown, 
										bool rightButtonDown, 
										bool middleButtonDown,
										bool sideButton1Down,
										bool sideButton2Down,
										int mouseWheelDelta,
										int xPos,
										int yPos,
										void* context)
{
	ClientController::instance().getWindowDelegator().parseMouse(leftButtonDown, rightButtonDown, middleButtonDown, sideButton1Down,
		sideButton2Down, mouseWheelDelta, xPos, yPos);
}
void CALLBACK ClientController::OnKey(InputController::VirtualKey key, bool isKeyDown, bool isAltDown, void* context)
{
	ClientController::instance().getWindowDelegator().parseKeys(key, isKeyDown, isAltDown);
}

/*
**Here, all objects that have something linked with DXUT and D3D should be destroyed
**That's why we have some 'leaks' and non-zero count reference. We are deleting these
** objects to late in destructors called by ClientController one.
*/
void CALLBACK ClientController::OnD3D9DestroyDevice( void* pUserContext )
{
	LPD3DXSPRITE	s = ClientController::instance().getSprite();
	LPD3DXFONT		f = ClientController::instance().getFont();

//close connection to server to avoid new objects to be sent and created
	ClientController::instance().getSocket().disconnect(false);
	safeRelease(f);
	safeRelease(s);
	ClientController::instance().setSprite(NULL);
	ClientController::instance().setFont(NULL);
	ClientController::instance().getWindowDelegator().onDestroyDevice();
}
void CALLBACK ClientController::OnD3D9LostDevice( void* pUserContext )
{
}

LRESULT CALLBACK ClientController::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
										   bool* pbNoFurtherProcessing, void* pUserContext )
{
	return 0;
}

void CALLBACK ClientController::OnD3D9FrameRender(IDirect3DDevice9* pd3dDevice, double elapsedTime, 
												  float frameTime, void* pUserContext )
{
	ClientController::instance().getWindowDelegator().render(pd3dDevice, elapsedTime, frameTime);
}
void CALLBACK ClientController::OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	std::string bufferfromserver;
	ClientController& controller = ClientController::instance();
	controller._socket.exec();
	controller._socket.readAllData(bufferfromserver);
	if (!bufferfromserver.empty())
		controller.getWindowDelegator().onNetworkSignal(bufferfromserver);
	controller.getWindowDelegator().frameMove(fTime, fElapsedTime);
}
HRESULT CALLBACK ClientController::OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
													 void* pUserContext )
{
	return S_OK;
}
HRESULT CALLBACK ClientController::OnD3D9CreateDevice( IDirect3DDevice9* d3dDevice, 
													  const D3DSURFACE_DESC* pBackBufferSurfaceDesc, 
													  void* pUserContext )
{
	HRESULT			hr;
	LPD3DXFONT		font;
	LPD3DXSPRITE	sprite;

	V(d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE));
    V(d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
    V(d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA));

	V(D3DXCreateFont(d3dDevice,    // the D3D Device
		30,    // font height of 30
		0,    // default font width
		FW_NORMAL,    // font weight
		1,    // not using MipLevels
		true,    // italic font
		DEFAULT_CHARSET,    // default character set
		OUT_DEFAULT_PRECIS,    // default OutputPrecision,
		ANTIALIASED_QUALITY,    // default Quality
		DEFAULT_PITCH | FF_DONTCARE,    // default pitch and family
		L"Arial",    // use Facename Arial
		&font));    // the font object
	V(D3DXCreateSprite(d3dDevice, &sprite));
	ClientController::instance().setSprite(sprite);
	ClientController::instance().setFont(font);
	ClientController::instance().getWindowDelegator().onCreateDevice(d3dDevice, pBackBufferSurfaceDesc);
	//graphics.Load(testObject.getPath());

	return S_OK;
}
bool CALLBACK ClientController::ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    return true;
}

LPD3DXFONT ClientController::getFont()
{
	return (_d3dxFont);
}

LPD3DXSPRITE ClientController::getSprite()
{
	return (_d3dxSprite);
}

RR::SocketClient& ClientController::getSocket()
{
	return (_socket);
}

WindowDelegator& ClientController::getWindowDelegator()
{
	return (_windowDelegator);
}

bool
ClientController::connectTo(const std::string &addr, const std::string &port)
{
	return _socket.connect(addr, port);
}

void ClientController::getSocketData(std::string& buffer)
{
	//todo: finish
}