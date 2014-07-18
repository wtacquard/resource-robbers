#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

class Object;

class Serializable
{
public:
	Serializable(){}
	~Serializable(){};

	virtual std::string& serialize(std::string&) = 0;
	virtual Object* deserialize(const std::string&) = 0;
};

#endif //!SERIALIZABLE_H_