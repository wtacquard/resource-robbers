//--------------------------------------------------------------------------------------
// File: Main.cpp
//
// Main driver for the game
//--------------------------------------------------------------------------------------


#include <string>
#include <iostream>
#include <sstream>
#include <winsock2.h>
#include <exception>
#include "Exception.hpp"
#include "SocketException.hpp"
#include "Socket.hpp"
#include "SocketClient.hpp"

#include "Log.hpp"

#include "DXUT.h"
#include "controller/InputController.h"
#include "graphics/graphics.h"
#include "globals.h"
#include "server/Object.h"
#include "game/WorldObject.h"
#include "game/DisplayObject.h"
#include "WindowDelegator/WindowDelegator.h"
#include "common/ClientController.h"


//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
	RR::Log& log = RR::Log::instance();
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	if (::AllocConsole())
	{
		::SetConsoleTitle(L"Ressource Robbers - Debug/Log - by Wil ^-^");
		FILE *stream;
		freopen_s(&stream, "CONIN$", "r", stdin); 
		freopen_s(&stream, "CONOUT$", "w", stdout); 
		freopen_s(&stream, "CONOUT$", "w", stderr);
		std::ios_base::sync_with_stdio();
		log<<"Debugging Console Initialized\n";
	}
#endif //!DEBUG

//	_crtBreakAlloc = 536;


    // TODO: Perform any application-level initialization here
	ClientController& cc = ClientController::instance();

	// Initialize DXUT and create the desired Win32 window and Direct3D device for the application
	DXUTSetCallbackD3D9DeviceAcceptable(&ClientController::IsD3D9DeviceAcceptable);
	DXUTSetCallbackD3D9DeviceCreated(&ClientController::OnD3D9CreateDevice);
    DXUTSetCallbackD3D9DeviceReset(&ClientController::OnD3D9ResetDevice);
    DXUTSetCallbackD3D9FrameRender(&ClientController::OnD3D9FrameRender);
    DXUTSetCallbackD3D9DeviceLost(&ClientController::OnD3D9LostDevice);
    DXUTSetCallbackD3D9DeviceDestroyed(&ClientController::OnD3D9DestroyDevice);
    DXUTSetCallbackDeviceChanging(&ClientController::ModifyDeviceSettings);
    DXUTSetCallbackMsgProc(&ClientController::MsgProc);
    DXUTSetCallbackFrameMove(&ClientController::OnFrameMove);
	DXUTSetCallbackKeyboard(&ClientController::OnKey);
	DXUTSetCallbackMouse(&ClientController::OnMouse, true);
	DXUTInit(true, true); // Parse the command line and show msgboxes
    DXUTSetHotkeyHandling(true, true, true);  // handle the default hotkeys
    DXUTSetCursorSettings(true, true); // Show the cursor and clip it when in full screen
    DXUTCreateWindow(L"Resource Robbers" );
    DXUTCreateDevice(true, 1024, 768);

    // Start the render loop
    DXUTMainLoop();

    // TODO: Perform any application-level cleanup here
	int ret;
	DXUTShutdown(ret = DXUTGetExitCode());
	cc.destroy();
#if defined(DEBUG) | defined(_DEBUG)
	::FreeConsole();
#endif //!DEBUG
	log.destroy();
	return (ret);
}


