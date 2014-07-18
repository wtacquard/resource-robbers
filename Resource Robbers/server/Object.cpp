#include "server/Object.h"

unsigned long long Object::_lastid = 1;

Object::Object()
 : _id(_lastid)
{
	++_lastid;
}

Object::Object(ID id)
 :_id(id)
{
	if (id > _lastid)
		_lastid = id;
};

Object::Object(const Object& copy)
: _id(copy._id)
{

}

Object&
Object::operator=(const Object& copy)
{
	_id = copy._id;
	return (*this);
}

bool
Object::operator<(const Object& o)
{
	return (_id < o._id);
}

bool
Object::operator==(const Object& o)
{
	return (_id == o._id);
}

std::string&
Object::serialize(std::string& str)
{
	std::ostringstream os;
	os<<"<object_id>"<<_id<<"</object_id>";
	str += os.str();
	return (str);
}

Object*
Object::deserialize(const std::string&)
{
	return NULL;
}

ID
Object::getLastAssignedId()
{
	return (_lastid - 1);
}

const ID&
Object::getID() const
{
	return (_id);
}
