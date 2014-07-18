#include <string>
#include <winsock2.h>
#include <exception>
#include "Exception.hpp"
#include "SocketException.hpp"
#include "Socket.hpp"
#include "SocketClient.hpp"

#include "MainMenu/MainMenu.h"
#include "common/ClientController.h"
#include <stdio.h>

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768
#define MAIN_MENU 0
#define START_MENU 1
#define OPTIONS_MENU 2
#define CREDITS_SCREEN 3
#define GAME 4
#define START_GAME_1 0
#define OPTIONS 1
#define EXIT 2
#define IP_ADDRESS 0
#define PORT 1
#define START_GAME_2 2
#define SM_BACK 3
#define ENTER_IP_ADD 4
#define ENTER_PORT 5
#define CREDITS 0
#define OM_BACK 1

MainMenu::MainMenu()
{
	screen = MAIN_MENU;
	selection = START_GAME_1;
	creditsPosition = SCREEN_HEIGHT;

	SetRect(&titleTextbox, (int)(SCREEN_WIDTH * .25), (int)(SCREEN_HEIGHT * .25), (int)(SCREEN_WIDTH * .75), (int)(SCREEN_HEIGHT * .5));
	SetRect(&startGameTextbox1, (int)(SCREEN_WIDTH * .25), (int)(SCREEN_HEIGHT * .5), (int)(SCREEN_WIDTH * .75), (int)(SCREEN_HEIGHT * (float)7/12));
	SetRect(&optionsTextbox, (int)(SCREEN_WIDTH * .25), (int)(SCREEN_HEIGHT * (float)7/12), (int)(SCREEN_WIDTH * .75), (int)(SCREEN_HEIGHT * (float)2/3));
	SetRect(&exitTextbox, (int)(SCREEN_WIDTH * .25), (int)(SCREEN_HEIGHT * (float)2/3), (int)(SCREEN_WIDTH * .75), (int)(SCREEN_HEIGHT * .75));

	SetRect(&IPAddressTextbox, (int)(SCREEN_WIDTH * .25), (int)(SCREEN_HEIGHT * .25), (int)(SCREEN_WIDTH * .385), (int)(SCREEN_HEIGHT * .375));
	SetRect(&enterIPAddTextbox, (int)(SCREEN_WIDTH * .385), (int)(SCREEN_HEIGHT * .25), (int)(SCREEN_WIDTH * .75), (int)(SCREEN_HEIGHT * .375));
	SetRect(&portTextbox, (int)(SCREEN_WIDTH * .25), (int)(SCREEN_HEIGHT * .375), (int)(SCREEN_WIDTH * .315), (int)(SCREEN_HEIGHT * .5));
	SetRect(&enterPortTextbox, (int)(SCREEN_WIDTH * .315), (int)(SCREEN_HEIGHT * .375), (int)(SCREEN_WIDTH * .75), (int)(SCREEN_HEIGHT * .5));
	SetRect(&startGameTextbox2, (int)(SCREEN_WIDTH * .25), (int)(SCREEN_HEIGHT * .5), (int)(SCREEN_WIDTH * .75), (int)(SCREEN_HEIGHT * .625));
	SetRect(&backTextbox1, (int)(SCREEN_WIDTH * .25), (int)(SCREEN_HEIGHT * .625), (int)(SCREEN_WIDTH * .75), (int)(SCREEN_HEIGHT * .75));

	SetRect(&creditsTextbox1, (int)(SCREEN_WIDTH * .25), (int)(SCREEN_HEIGHT * .375), (int)(SCREEN_WIDTH * .75), (int)(SCREEN_HEIGHT * .5));
	SetRect(&backTextbox2, (int)(SCREEN_WIDTH * .25), (int)(SCREEN_HEIGHT * .5), (int)(SCREEN_WIDTH * .75), (int)(SCREEN_HEIGHT * .625));

	SetRect(&creditsTextbox2, 0, creditsPosition, SCREEN_WIDTH, (int)(creditsPosition + SCREEN_HEIGHT * 1.8));

	previousLeftButtonDown = false;
}

void MainMenu::parseKeys( UINT vKey, bool keyDown, bool altDown )
{
	if(!keyDown)
		return;

	if (screen == START_MENU)
		switch(selection) {
			case ENTER_IP_ADD:
				if (vKey >= 0x30 && vKey <= 0x39)
					IPAddress += vKey;
				else if (vKey >= VK_NUMPAD0 && vKey <= VK_NUMPAD9)
					IPAddress += vKey - (VK_NUMPAD0 - 0x30);
				else if (vKey == VK_DECIMAL || vKey == 0xbe)
					IPAddress += '.';
				else if (vKey == VK_BACK && IPAddress.length() > 0)
					IPAddress.erase(IPAddress.length() - 1);
				break;
			case ENTER_PORT:
				if (vKey >= 0x30 && vKey <= 0x39)
					port += vKey;
				else if (vKey >= VK_NUMPAD0 && vKey <= VK_NUMPAD9)
					port += vKey - (VK_NUMPAD0 - 0x30);
				else if (vKey == VK_BACK && port.length() > 0)
					port.erase(port.length() - 1);
				break;
		}

	if (vKey == VK_UP) {
		if (screen == START_MENU && selection == ENTER_PORT)
			selection = IP_ADDRESS;
		else if (--selection < 0)
			switch(screen) {
				case MAIN_MENU:
					selection = 2;
					break;
				case START_MENU:
					selection = 3;
					break;
				case OPTIONS_MENU:
					selection = 1;
					break;
		}
	}
	if (vKey == VK_DOWN) {
		switch(screen) {
				case MAIN_MENU:
					selection = (selection + 1) % 3;
					break;
				case START_MENU:
					selection = (selection + 1) % 4;
					break;
				case OPTIONS_MENU:
					selection = (selection + 1) % 2;
					break;
		}
	}

	if (vKey == VK_RETURN) {
		switch(screen) {
				case MAIN_MENU:
					switch(selection) {
				case START_GAME_1:
					screen = START_MENU;
					selection = IP_ADDRESS;
					break;
				case OPTIONS:
					screen = OPTIONS_MENU;
					selection = CREDITS;
					break;
				case EXIT:
					DXUTShutdown(0);
					break;
					}
					break;
				case START_MENU:
					switch(selection) {
				case IP_ADDRESS:
					selection = ENTER_IP_ADD;
					break;
				case ENTER_IP_ADD:
					selection = PORT;
					break;
				case PORT:
					selection = ENTER_PORT;
					break;
				case ENTER_PORT:
					selection = START_GAME_2;
					break;
				case START_GAME_2:
					screen = GAME;
					break;
				case SM_BACK:
					screen = MAIN_MENU;
					selection = START_GAME_1;
					break;
					}
					break;
				case OPTIONS_MENU:
					switch(selection) {
				case CREDITS:
					screen = CREDITS_SCREEN;
					creditsPosition = SCREEN_HEIGHT;
					break;
				case OM_BACK:
					screen = MAIN_MENU;
					selection = OPTIONS;
					break;
					}
					break;
				case CREDITS_SCREEN:
					screen = OPTIONS_MENU;
					break;
		}
	}


	if (vKey == VK_SPACE)
		switch(screen) {
			case CREDITS_SCREEN:
				screen = OPTIONS_MENU;
				break;
	}

	return;
}

HRESULT MainMenu::render( IDirect3DDevice9 *d3dDevice, double totalTime,
						 float elapsedTime, void *data )
{
	HRESULT hr;
	V(d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
		0xff000000, 1.0f, 0));

	if(SUCCEEDED(d3dDevice->BeginScene()))
	{
		LPD3DXSPRITE sprite = ClientController::instance().getSprite();
		if(SUCCEEDED(sprite->Begin(0)))
		{
			LPD3DXFONT font = ClientController::instance().getFont();
			switch(screen) {
		case MAIN_MENU:
			// draw the Hello World text
			font->DrawText(sprite,
				L"Resource Robbers",
				16,
				&titleTextbox,
				DT_CENTER | DT_VCENTER,
				D3DCOLOR_ARGB(255, 255, 255, 255));

			// draw the Hello World text
			if (selection != START_GAME_1)
				font->DrawText(sprite,
				L"Start Game",
				10,
				&startGameTextbox1,
				DT_CENTER | DT_VCENTER,
				D3DCOLOR_ARGB(255, 255, 255, 255));
			else
				font->DrawText(sprite,
				L"Start Game",
				10,
				&startGameTextbox1,
				DT_CENTER | DT_VCENTER,
				D3DCOLOR_ARGB(255, 255, 255, 0));

			// draw the Hello World text
			if (selection != OPTIONS)
				font->DrawText(sprite,
				L"Options",
				7,
				&optionsTextbox,
				DT_CENTER | DT_VCENTER,
				D3DCOLOR_ARGB(255, 255, 255, 255));
			else
				font->DrawText(sprite,
				L"Options",
				7,
				&optionsTextbox,
				DT_CENTER | DT_VCENTER,
				D3DCOLOR_ARGB(255, 255, 255, 0));

			// draw the Hello World text
			if (selection != EXIT)
				font->DrawText(sprite,
				L"Exit",
				4,
				&exitTextbox,
				DT_CENTER | DT_VCENTER,
				D3DCOLOR_ARGB(255, 255, 255, 255));
			else
				font->DrawText(sprite,
				L"Exit",
				4,
				&exitTextbox,
				DT_CENTER | DT_VCENTER,
				D3DCOLOR_ARGB(255, 255, 255, 0));
			break;
		case START_MENU:
			// draw the Hello World text
			if (selection != IP_ADDRESS)
				font->DrawText(sprite,
					L"IP Address: ",
					12,
					&IPAddressTextbox,
					DT_VCENTER,
					D3DCOLOR_ARGB(255, 255, 255, 255));
			else
				font->DrawText(sprite,
					L"IP Address: ",
					12,
					&IPAddressTextbox,
					DT_VCENTER,
					D3DCOLOR_ARGB(255, 255, 255, 0));

			// draw the Hello World text
			font->DrawTextA(sprite,
				IPAddress.c_str(),
				IPAddress.length(),
				&enterIPAddTextbox,
				DT_VCENTER,
				D3DCOLOR_ARGB(255, 255, 255, 255));

			// draw the Hello World text
			if (selection != PORT)
				font->DrawText(sprite,
					L"Port: ",
					6,
					&portTextbox,
					DT_VCENTER,
					D3DCOLOR_ARGB(255, 255, 255, 255));
			else
				font->DrawText(sprite,
					L"Port: ",
					6,
					&portTextbox,
					DT_VCENTER,
					D3DCOLOR_ARGB(255, 255, 255, 0));

			// draw the Hello World text
			font->DrawTextA(sprite,
				port.c_str(),
				port.length(),
				&enterPortTextbox,
				DT_VCENTER,
				D3DCOLOR_ARGB(255, 255, 255, 255));

			// draw the Hello World text
			if (selection != START_GAME_2)
				font->DrawText(sprite,
				L"Start Game",
				10,
				&startGameTextbox2,
				DT_CENTER | DT_VCENTER,
				D3DCOLOR_ARGB(255, 255, 255, 255));
			else
				font->DrawText(sprite,
				L"Start Game",
				10,
				&startGameTextbox2,
				DT_CENTER | DT_VCENTER,
				D3DCOLOR_ARGB(255, 255, 255, 0));

			// draw the Hello World text
			if (selection != SM_BACK)
				font->DrawText(sprite,
				L"Back",
				4,
				&backTextbox1,
				DT_CENTER | DT_VCENTER,
				D3DCOLOR_ARGB(255, 255, 255, 255));
			else
				font->DrawText(sprite,
				L"Back",
				4,
				&backTextbox1,
				DT_CENTER | DT_VCENTER,
				D3DCOLOR_ARGB(255, 255, 255, 0));
			break;
		case OPTIONS_MENU:
			// draw the Hello World text
			if (selection != CREDITS)
				font->DrawText(sprite,
				L"Credits",
				7,
				&creditsTextbox1,
				DT_CENTER | DT_VCENTER,
				D3DCOLOR_ARGB(255, 255, 255, 255));
			else
				font->DrawText(sprite,
				L"Credits",
				7,
				&creditsTextbox1,
				DT_CENTER | DT_VCENTER,
				D3DCOLOR_ARGB(255, 255, 255, 0));

			// draw the Hello World text
			if (selection != OM_BACK)
				font->DrawText(sprite,
				L"Back",
				4,
				&backTextbox2,
				DT_CENTER | DT_VCENTER,
				D3DCOLOR_ARGB(255, 255, 255, 255));
			else
				font->DrawText(sprite,
				L"Back",
				4,
				&backTextbox2,
				DT_CENTER | DT_VCENTER,
				D3DCOLOR_ARGB(255, 255, 255, 0));
			break;
		case CREDITS_SCREEN:
			// create a RECT to contain the text
			SetRect(&creditsTextbox2, 0, creditsPosition, SCREEN_WIDTH, (int)(creditsPosition + SCREEN_HEIGHT * 1.8)); 

			// draw the Hello World text
			font->DrawText(sprite,
				L"3D Modeling\n\nKevin Groves\n\n\nAnimation\n\nKevin Groves\nSteven Nguyen\n\n\nGraphics\n\nKevin Groves\nSteven Nguyen\nEdgardo Rivera\n\n\nNetworking\n\nKevin Groves\nWilfried Tacquard\n\n\nGame Logic\n\nChris Louie\nSteven Nguyen\nEdgardo Rivera\n\n\nPhysics\n\nChris Louie\nWilfried Tacquard\n\n\nUser Input\n\nChris Louie\n\n\nSound\n\nEdgardo Rivera",
				310,
				&creditsTextbox2,
				DT_CENTER | DT_VCENTER,
				D3DCOLOR_ARGB(255, 255, 255, 255));

			creditsPosition -= 2;
			if (creditsPosition < -(SCREEN_HEIGHT * 1.8))
				screen = OPTIONS_MENU;
			break;
			}
			V(sprite->End());
		}
		V(d3dDevice->EndScene());
	}
	return S_OK;
}

void MainMenu::frameMove( double totalTime, float elapsedTime, 
						 void *data )
{
	if(screen == GAME)
	{
		if (!ClientController::instance().connectTo("192.168.1.41", "4242"))
		{
			MessageBox(DXUTGetHWND(), L"Can't connect to server", L"Connection error", MB_OK);
			screen = START_MENU;
		}
		else
			ClientController::instance().getWindowDelegator().setActive(ClientController::WINDOW_GAME);
	}
}
void MainMenu::parseMouse(bool leftButtonDown, bool rightButtonDown, bool middleButtonDown, bool sideButton1Down,
						  bool sideButton2Down, int mouseWheelDelta, int xPos, int yPos, void* context)
{
	static POINT mousePos;
	mousePos.x = xPos;
	mousePos.y = yPos;

	switch(screen)
	{
		case MAIN_MENU:
			if (PtInRect(&startGameTextbox1, mousePos))
				selection = START_GAME_1;
			else if (PtInRect(&optionsTextbox, mousePos))
				selection = OPTIONS;
			else if (PtInRect(&exitTextbox, mousePos))
				selection = EXIT;
			break;
		case START_MENU:
			if ((PtInRect(&IPAddressTextbox, mousePos) || PtInRect(&enterIPAddTextbox, mousePos)) && 
					!(!leftButtonDown && previousLeftButtonDown))
				selection = IP_ADDRESS;
			else if ((PtInRect(&portTextbox, mousePos) || PtInRect(&enterPortTextbox, mousePos)) && 
					!(!leftButtonDown && previousLeftButtonDown))
				selection = PORT;
			else if (PtInRect(&startGameTextbox2, mousePos))
				selection = START_GAME_2;
			else if (PtInRect(&backTextbox1, mousePos))
				selection = SM_BACK;
			break;
		case OPTIONS_MENU:
			if (PtInRect(&creditsTextbox1, mousePos))
				selection = CREDITS;
			else if (PtInRect(&backTextbox2, mousePos))
				selection = OM_BACK;
			break;
	}

	if (leftButtonDown)
		switch(screen) {
				case MAIN_MENU:
					switch(selection) {
				case START_GAME_1:
					screen = START_MENU;
					selection = START_GAME_2;
					break;
				case OPTIONS:
					screen = OPTIONS_MENU;
					selection = CREDITS;
					break;
				case EXIT:
					DXUTShutdown(0);
					break;
					}
					break;
				case START_MENU:
					switch(selection) {
				case IP_ADDRESS:
					selection = ENTER_IP_ADD;
					break;
				case PORT:
					selection = ENTER_PORT;
					break;
				case START_GAME_2:
					screen = GAME;
					break;
				case SM_BACK:
					screen = MAIN_MENU;
					selection = START_GAME_1;
					break;
					}
					break;
				case OPTIONS_MENU:
					switch(selection) {
				case CREDITS:
					screen = CREDITS_SCREEN;
					creditsPosition = SCREEN_HEIGHT;
					break;
				case OM_BACK:
					screen = MAIN_MENU;
					selection = OPTIONS;
					break;
					}
					break;
				case CREDITS_SCREEN:
					screen = OPTIONS_MENU;
					break;
		}

	previousLeftButtonDown = leftButtonDown;
}

void MainMenu::onCreateDevice(IDirect3DDevice9* d3dDevice, const D3DSURFACE_DESC* backBufferSurfaceDesc)
{
}

void MainMenu::onDestroyDevice()
{
}