#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include <vector>
#include <boost/function.hpp>
#include <DXUT.h>

class InputController
{
public:
	typedef enum MouseEvent
	{
		MOUSE_LEFT,
		MOUSE_RIGHT,
		MOUSE_MIDDLE,
		MOUSE_SIDE1,
		MOUSE_SIDE2,
		MOUSE_WHEEL_UP,
		MOUSE_WHEEL_DOWN
	} MouseEvent;

	typedef unsigned int VirtualKey;
	typedef boost::function<void(bool, bool, void*)> CallbackFunctionKeyboard;
	typedef boost::function<void(bool, bool, bool, bool, bool, int, int, int, void*)> CallbackFunctionMouse;
    typedef std::map<VirtualKey, CallbackFunctionKeyboard> KeyMapping;
    typedef std::map<VirtualKey, CallbackFunctionKeyboard>::iterator KeyMappingIterator;
    typedef std::map<VirtualKey, CallbackFunctionKeyboard>::const_iterator KeyMappingConstIterator;
	typedef std::vector<CallbackFunctionMouse> MouseMapping;
    typedef std::vector<CallbackFunctionMouse>::iterator MouseMappingIterator;
    typedef std::vector<CallbackFunctionMouse>::const_iterator MouseMappingConstIterator;
	typedef boost::function<void(int, int, void*)> CallbackFunctionMouseClick;

private:
    KeyMapping keyMapping;
	MouseMapping mouseMapping;
	CallbackFunctionMouseClick leftClickCallback;
	CallbackFunctionMouseClick rightClickCallback;
	CallbackFunctionMouseClick middleClickCallback;
	CallbackFunctionMouseClick side1ClickCallback;
	CallbackFunctionMouseClick side2ClickCallback;

	bool isMouseLeftDown;
	bool isMouseRightDown;
	bool isMouseMiddleDown;
	bool isMouseSide1Down;
	bool isMouseSide2Down;
	int mouseWheelDelta;

public:
    InputController();
    virtual ~InputController() {}
    bool isKeyDown(VirtualKey key);
	bool isMouseButtonDown(unsigned char button);
    void registerKeyMapping(VirtualKey key, CallbackFunctionKeyboard);
	void registerMouseClickMapping(MouseEvent button, CallbackFunctionMouseClick);
	void registerMouseEvent(CallbackFunctionMouse callback);
	void clearMappings();
	void parseMouse(bool leftButtonDown, bool rightButtonDown, bool middleButtonDown, bool sideButton1Down,
		bool sideButton2Down, int mouseWheelDelta, int xPos, int yPos, void* context);
    void parseKeys(VirtualKey key, bool isDown, bool isAltDown, void*);
};
#endif //CONTROLLER_H
