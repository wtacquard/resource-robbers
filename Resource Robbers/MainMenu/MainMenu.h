#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <DXUT.h>
#include "WindowDelegator/WindowDelegator.h"
#include "Menu.h"

class MainMenu : public Window
{
	static const RECT MENU_LOCATION;

	Menu menu;

	int screen, selection;
	int creditsPosition;
	RECT titleTextbox, startGameTextbox1, optionsTextbox, exitTextbox;
	RECT IPAddressTextbox, enterIPAddTextbox, portTextbox, enterPortTextbox, startGameTextbox2, backTextbox1;
	RECT creditsTextbox1, backTextbox2;
	RECT creditsTextbox2;
	std::string IPAddress, port;
	bool previousLeftButtonDown;

public:
	MainMenu();
	virtual void parseKeys(UINT vKey, bool keyDown, bool altDown);
	virtual HRESULT render(IDirect3DDevice9 *d3dDevice, double totalTime, float elapsedTime, void *data);
	virtual void frameMove(double totalTime, float elapsedTime, void *data);
	virtual void parseMouse(bool leftButtonDown, bool rightButtonDown, bool middleButtonDown, bool sideButton1Down,
		bool sideButton2Down, int mouseWheelDelta, int xPos, int yPos, void* context);
	virtual void onCreateDevice(IDirect3DDevice9* d3dDevice, const D3DSURFACE_DESC* backBufferSurfaceDesc);
	virtual void onDestroyDevice();
	virtual void onNetworkSignal(std::string &message){};
};
#endif //MAIN_MENU_H