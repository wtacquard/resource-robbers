#ifndef MERGER_HPP_
# define MERGER_HPP_

class Merger : public Serializable
{
public:
	enum ObjectType
	{
		DEFAULT,
		HOVERCRAFT
	};
	typedef std::map<ID, WorldObject*> Buffer;
	typedef RR::DualBuffer<Buffer> Buffers;
	typedef WorldObject *(Merger::*pfct)(std::string&);
	typedef void (Merger::*pfct_2)(std::string&, WorldObject*);

private:
	static bool _init;
	static std::map<std::string, pfct> _fct;
	static std::map<std::string, pfct_2> _fctPhysics;
	static std::map<std::string, pfct_2> _fctGame;

	Buffers& _buf;
	Buffer* _read_buf;
	Buffer* _write_buf;
	std::string _carry;

private:
	static ObjectType stringTypeToEnumType(const std::string&);
	WorldObject* parsePhysic(std::string&);
	void parsePosition(std::string&, WorldObject*);
	void parseOrientation(std::string&, WorldObject*);

	WorldObject* parseGame(std::string&);

	bool readMark(const std::string& input, std::string& markvalue);
	bool markContent(std::string& input, const std::string& markname, std::string& content);

	Merger(const Merger &);
	Merger& operator=(const Merger&);
	static void init();

	WorldObject* copyObject(WorldObject* wo);
	WorldObject* synchronizeObject(ID id, bool create, const ObjectType& type);
	WorldObject* createNewObject(ID id, const ObjectType& type);

protected:
	std::string& serialize(std::string&);
	Object* deserialize(const std::string&);
public:
	Merger(Buffers&);
	~Merger();

	void update(const std::string&);
	void clean();
};

//istream& operator>>(istream&, Merger::ObjectType&);

#endif //!MERGER_HPP_
