#include "DXUT.h"
#include <typeinfo>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <windows.h>
#include "Log.hpp"
#include "common/Serializable.h"
#include "server/Object.h"
#include "game/WorldObject.h"
#include "game/DisplayObject.h"
#include "game/Weapon.h"
#include "game/Vehicle.h"
#include "common/DualBuffer.hh"
#include "common/typedef.h"
#include "game/Merger.hpp"

bool Merger::_init = false;
std::map<std::string, Merger::pfct> Merger::_fct;
std::map<std::string, Merger::pfct_2> Merger::_fctPhysics;
std::map<std::string, Merger::pfct_2> Merger::_fctGame;

void
Merger::init()
{
	Merger::_fct.insert(std::make_pair<std::string, Merger::pfct>("physics", &Merger::parsePhysic));
	Merger::_fct.insert(std::make_pair<std::string, Merger::pfct>("game", &Merger::parseGame));

	_fctPhysics.insert(std::make_pair<std::string, pfct_2>("position", &Merger::parsePosition));
	_fctPhysics.insert(std::make_pair<std::string, pfct_2>("orientation", &Merger::parseOrientation));
}

Merger::Merger(Buffers& buf)
: _buf(buf), _read_buf(NULL), _write_buf(NULL), _carry()
{
	if (!_init)
	{
		Merger::init();
		_init = true;
	}
}

Merger::Merger(const Merger&)
: _buf(*((Buffers*)NULL)), _read_buf(NULL), _write_buf(NULL), _carry()
{

}

Merger::~Merger()
{
	clean();
}

void
Merger::clean()
{
	Buffer *buf;

	buf = _buf.getWriteAccess();
	for (Buffer::iterator it = buf->begin(); it != buf->end(); ++it)
		delete it->second;
	buf->clear();
	_buf.releaseWriteAccess(&buf);
	buf = _buf.getReadAccess();
	for (Buffer::iterator it = buf->begin(); it != buf->end(); ++it)
		delete it->second;
	buf->clear();
	_buf.releaseReadAccess(&buf);
}

Merger&
Merger::operator=(const Merger&)
{
	return (*this);
}

bool
Merger::readMark(const std::string& input, std::string& mark_value)
{
	std::string::size_type start, end;

	if ((start = input.find_first_of('<')) != 0)
		return (false);
	if ((end = input.find_first_of('>')) == std::string::npos)
		return (false);
	mark_value = input.substr(start + 1, end - start - 1);
	return (true);
}

void
Merger::update(const std::string &str)
{
	_write_buf = _buf.getWriteAccess();
	_read_buf = _buf.getReadAccess();
	deserialize(str);
	_buf.releaseReadAccess(&_read_buf);
	_buf.releaseWriteAccess(&_write_buf);
}

std::string&
Merger::serialize(std::string &)
{
	return (*(std::string *)NULL);
}

bool
Merger::markContent(std::string& input, const std::string &mark_name, std::string &content)
{
	std::string::size_type start;
	std::string::size_type end;
	std::string mark;

	mark = "<" + mark_name + ">";
	if ((start = input.find(mark)) == std::string::npos)
		return (false);
	mark.insert(1, 1, '/');
	if ((end = input.find(mark)) == std::string::npos)
		return (false);
	content = input.substr(start + mark.length() - 1, end - mark.length() + 1);
	input.erase(start, end - start + mark.length());
	return (true);
}


WorldObject*
Merger::copyObject(WorldObject* wo)
{
	WorldObject *copy;
	const type_info &t = typeid(*wo);

	if (t == typeid(Vehicle))
		copy = new Vehicle(*(static_cast<Vehicle *>(wo)));
	else if (t == typeid(DisplayObject))
		copy = new DisplayObject(*(static_cast<DisplayObject *>(wo)));
	else
		copy = NULL;
	_write_buf->insert(std::make_pair<ID, WorldObject*>(copy->getID(), copy));
	return (copy);
}

WorldObject*
Merger::createNewObject(ID id, const ObjectType& type)
{
	std::pair<Buffer::iterator, bool> res;
	Vehicle* o;

	switch (type)
	{
	case HOVERCRAFT:
		o = new Vehicle(id);
		o->setType(Vehicle::TANK);
		res  = _write_buf->insert(std::make_pair<ID, WorldObject*>(id, o));
		break;
	default:
		res = _write_buf->insert(std::make_pair<ID, WorldObject*>(id, new DisplayObject(id)));
	}
	return (res.first->second);
}

WorldObject*
Merger::synchronizeObject(ID id, bool create, const ObjectType& type)
{
	Buffer::iterator it_r, it_w;
	WorldObject* o;

	it_r = _read_buf->find(id);
	if (it_r == _read_buf->end()) //new object, just need to create one if ask
	{
		if (!create)
			return (NULL);
		return (createNewObject(id, type));
	}
//synchronisation
	if ((it_w = _write_buf->find(id)) == _write_buf->end())
	{
		o = copyObject(it_r->second);
	}
	else
		o = it_w->second;
	o->m_pos(it_r->second->m_pos());
	o->m_rotation(it_r->second->m_rotation());
	return (o);
}

Object*
Merger::deserialize(const std::string &str)
{
	std::string markvalue;
	std::string content;
	std::map<std::string, pfct>::iterator it;
	Object *last_object = NULL;

	_carry += str;
	do
	{
		if (!readMark(_carry, markvalue))
			return (last_object);
		if ((it = _fct.find(markvalue)) == _fct.end())
			return (last_object);
		if (!markContent(_carry, markvalue, content))
			return (last_object);
		last_object = (this->*(it->second))(content);
	} while (/* last_object && */!_carry.empty());
	return (last_object);
}

WorldObject*
Merger::parsePhysic(std::string &physic_stuff)
{
	std::string content;
	std::string markvalue;
	std::istringstream iss;
	std::map<std::string, pfct_2>::iterator it;
	RR::Log& log = RR::Log::instance();
	ID id = 0;
	WorldObject *o = NULL;

	markContent(physic_stuff, "object_id", content);
	iss.str(content);
	iss >> id;
	if (id == 0)
		return (o);
	if ((o = synchronizeObject(id, false, Merger::DEFAULT)) == NULL)
		return (o);
	while (readMark(physic_stuff, markvalue))
	{
		if (!markContent(physic_stuff, markvalue, content))
			continue ;
		if ((it = _fctPhysics.find(markvalue)) == _fctPhysics.end())
			continue ;
		log << id << "@";
		(this->*(it->second))(content, o);
		log << "\n";
	}
	return (o);
}

WorldObject*
Merger::parseGame(std::string &game_stuff)
{
	std::string content;
	std::string markvalue;
	std::istringstream iss;
	std::map<std::string, pfct_2>::iterator it;
	ID id = 0;
	WorldObject *o = NULL;

	markContent(game_stuff, "object_id", content);
	iss.str(content);
	iss >> id;
	if (id == 0)
		return (o);
	markContent(game_stuff, "type", content);
	if ((o = synchronizeObject(id, true, stringTypeToEnumType(content))) == NULL)
		return (o);
	while (readMark(game_stuff, markvalue))
	{
		if (!markContent(game_stuff, markvalue, content))
			continue ;
		if ((it = _fctPhysics.find(markvalue)) == _fctPhysics.end())
			continue ;
		(this->*(it->second))(content, o);
	}
	return (o);
}


void
Merger::parsePosition(std::string& position, WorldObject *o)
{
	std::string field;
	std::string content;
	D3DXVECTOR3 pos;
	float i;
	std::istringstream iss;
	RR::Log& log = RR::Log::instance();

	field = "x";
	markContent(position, field, content);
	iss.clear();
	iss.str(content);
	iss >> i;
	pos.x = i;
	field = "y";
	markContent(position, field, content);
	iss.clear();
	iss.str(content);
	iss >> i;
	pos.y = i;
	field = "z";
	markContent(position, field, content);
	iss.clear();
	iss.str(content);
	iss >> i;
	pos.z = i;
	log << "{" << (int)pos.x << ";" << (int)pos.y << ";" << (int)pos.z <<"}";
	o->m_pos(pos);
}


//TODO change rot.w
void
Merger::parseOrientation(std::string& orientation, WorldObject* o)
{
	std::string field;
	std::string content;
	D3DXQUATERNION rot;
	float i;
	std::istringstream iss;

	field = "w";
	markContent(orientation, field, content);
	iss.str(content);
	iss >> i;
	rot.w = 1;
	field = "x";
	markContent(orientation, field, content);
	iss.str(content);
	iss >> i;
	rot.x = i;
	field = "y";
	markContent(orientation, field, content);
	iss.str(content);
	iss >> i;
	rot.y = i;
	field = "z";
	markContent(orientation, field, content);
	iss.str(content);
	iss >> i;
	rot.z = i;
	o->m_rotation(rot);
}

Merger::ObjectType
Merger::stringTypeToEnumType(const std::string& type)
{
	if (type == "hovercraft")
		return (Merger::HOVERCRAFT);
	return (Merger::DEFAULT);
}