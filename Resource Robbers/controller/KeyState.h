#ifndef KEY_STATE_H
#define KEY_STATE_H

#include <string>

class KeyState
{
public:
	KeyState();
	KeyState(bool down, bool fire, bool left, bool right, bool up);
	KeyState(const KeyState& that);
	bool up;
	bool down;
	bool left;
	bool right;
	bool fire;
	void deserialize(const std::string&);
};
#endif //KEY_STATE_H