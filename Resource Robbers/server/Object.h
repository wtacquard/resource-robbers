#ifndef OBJECT_H_
# define OBJECT_H_

#include <string>
#include <sstream>
#include "common/Serializable.h"
#include "common/typedef.h"

class Object : public Serializable
{
private:
	ID _id;
	static ID _lastid;

protected:
	Object();
	Object(ID id);
	Object(const Object& copy);
	Object& operator=(const Object&);
	virtual ~Object(){};

public:
	static ID getLastAssignedId();
	virtual const ID& getID() const;
	virtual std::string& serialize(std::string& str);
	virtual Object* deserialize(const std::string&);

	bool operator<(const Object&);
	bool operator==(const Object&);
};

#endif //!OBJECT_H_
