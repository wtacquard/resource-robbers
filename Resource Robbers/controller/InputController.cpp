#include <set>
#include "InputController.h"
#include "DXUT.h"
#include "Command.h"


void testHandler(bool, bool, void*)
{
	MessageBox(DXUTGetHWND(), L"You pressed a button to trigger the test handler", L"test handler", MB_OK);
}


void mouseHandler(bool isLeftDown, bool isRightDown, bool isMiddleDown, bool isSide1Down, bool isSide2Down,
				  int mouseDelta, int x, int y, void*)
{
	wchar_t buffer[BUFSIZ];
	_snwprintf_s(buffer, BUFSIZ, L"isLeftDown: %d\nisRightDown: %d\nisMiddleDown %d\nisSide1Down: %d\nisSide2Down: %d\nmouseDelta: %d\n x: %d\n y: %d\n",
		isLeftDown, isRightDown, isMiddleDown, isSide1Down, isSide2Down, mouseDelta, x, y);
	MessageBox(DXUTGetHWND(), buffer, L"mouse test", MB_OK);
}

InputController::InputController()
: keyMapping(),
mouseMapping(),
leftClickCallback(NULL),
rightClickCallback(NULL),
middleClickCallback(NULL),
side1ClickCallback(NULL),
side2ClickCallback(NULL),
isMouseLeftDown(false),
isMouseRightDown(false),
isMouseMiddleDown(false),
isMouseSide1Down(false),
isMouseSide2Down(false),
mouseWheelDelta(0)
{
	//registerKeyMapping(VK_RETURN, testHandler);
	//registerMouseEvent(mouseHandler);
}

void InputController::clearMappings()
{
	keyMapping.clear();
	mouseMapping.clear();
}


void InputController::parseKeys(VirtualKey key, bool isDown, bool isAltDown, 
    void* context)
{
    KeyMappingIterator f = keyMapping.find(key);
    if(f == keyMapping.end())
        return;

	(f->second)(isDown, isAltDown, context);
}

void InputController::parseMouse(bool leftButtonDown, bool rightButtonDown, bool middleButtonDown,
		bool sideButton1Down, bool sideButton2Down, int mouseWheelDelta, int xPos, int yPos, void* context)
{
	if(isMouseLeftDown && !leftButtonDown && leftClickCallback)
		(leftClickCallback)(xPos, yPos, context);
	if(isMouseRightDown && !rightButtonDown && rightClickCallback)
		(rightClickCallback)(xPos, yPos, context);
	if(isMouseMiddleDown && !rightButtonDown && middleClickCallback)
		(middleClickCallback)(xPos, yPos, context);
	if(isMouseSide1Down && !sideButton1Down && side1ClickCallback)
		(side1ClickCallback)(xPos, yPos, context);
	if(isMouseSide2Down && !sideButton2Down && side2ClickCallback)
		(side2ClickCallback)(xPos, yPos, context);

	this->isMouseLeftDown = leftButtonDown;
	this->isMouseRightDown = rightButtonDown;
	this->isMouseMiddleDown = middleButtonDown;
	this->isMouseSide1Down = sideButton1Down;
	this->isMouseSide2Down = sideButton2Down;
	this->mouseWheelDelta = mouseWheelDelta;

	for(MouseMappingIterator it = mouseMapping.begin(), end = mouseMapping.end(); it != end; it++)
		(*it)(leftButtonDown, rightButtonDown, middleButtonDown, sideButton1Down, sideButton2Down,
			mouseWheelDelta, xPos, yPos, context);
}

//valid values are VK_LBUTTON, VK_RBUTTON, VK_MBUTTON, VK_XBUTTON1, VK_XBUTTON2
bool InputController::isMouseButtonDown(unsigned char button)
{
	return DXUTIsMouseButtonDown(button);
}

bool InputController::isKeyDown(VirtualKey key)
{
	return DXUTIsKeyDown((unsigned char)key);
}

void InputController::registerMouseClickMapping(MouseEvent button, CallbackFunctionMouseClick callback)
{
	switch(button) {
	case MOUSE_LEFT:
		leftClickCallback = callback;
		break;
	case MOUSE_RIGHT:
		rightClickCallback = callback;
		break;
	case MOUSE_MIDDLE:
		middleClickCallback = callback;
		break;
	case MOUSE_SIDE1:
		side1ClickCallback = callback;
		break;
	case MOUSE_SIDE2:
		side2ClickCallback = callback;
		break;
	}
}

void InputController::registerMouseEvent(CallbackFunctionMouse mouseHandler)
{
	mouseMapping.push_back(mouseHandler);
}

void InputController::registerKeyMapping(VirtualKey key, CallbackFunctionKeyboard keyHandlerFunction)
{
    keyMapping[key] = keyHandlerFunction;
}
