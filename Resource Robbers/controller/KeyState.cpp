#include <tinyxml.h>
#include <sstream>
#include <log.hpp>
#include "KeyState.h"

using std::string;
using RR::Log;
using std::istringstream;

KeyState::KeyState()
: down(false), fire(false), left(false), right(false), up(false)
{
}

KeyState::KeyState(bool down, bool fire, bool left, bool right, bool up)
: down(down), fire(fire), left(left), right(right), up(up)
{
}

KeyState::KeyState(const KeyState& that)
: down(that.down), fire(that.fire), left(that.left), right(that.right), up(that.up)
{
}

void KeyState::deserialize(const string& xml)
{
	string name;
	bool isDown;
	bool isAltDown;
	Log& log = Log::instance();
	istringstream iss(xml);
	TiXmlDocument doc;
	iss >> doc;
	TiXmlHandle docHandle(&doc);
	TiXmlHandle root(NULL);
	TiXmlElement* element;
	element = docHandle.FirstChildElement().Element();
	if(!element)
	{
		log << "KeyState::parseXml(const string&) could not find XML command element\n";
		return;
	}
	root = TiXmlHandle(element);
	element = root.FirstChildElement("name").Element();
	if(!element)
	{
		log << "KeyState::parseXml(const string&) could not find XML name element\n";
		return;
	}
	name = element->GetText();
	element = root.FirstChildElement("isDown").Element();
	if(!element)
	{
		log << "KeyState::parseXml(const string&) could not find XML isDown element\n";
		return;
	}
	isDown = string(element->GetText()).compare("true") == 0;
	element = root.FirstChildElement("isAltDown").Element();
	if(!element)
	{
		log << "KeyState::parseXml(const string&) could not find XML isAltDown element\n";
		return;
	}
	isAltDown = string(element->GetText()).compare("true") == 0;
	if(name.compare("moveForward") == 0)
		up = isDown;
	else if(name.compare("moveBackward") == 0)
		down = isDown;
	else if(name.compare("moveLeft") == 0)
		left = isDown;
	else if(name.compare("moveRight") == 0)
		right = isDown;
	else if(name.compare("fire") == 0)
		fire = isDown;
}