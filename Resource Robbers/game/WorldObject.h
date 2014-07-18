#ifndef WORLDOBJECT_H
# define WORLDOBJECT_H

class WorldObject : public Object
{
public:
	WorldObject();
	WorldObject(ID);
	WorldObject(const WorldObject&);
	virtual ~WorldObject();

	WorldObject& operator=(const WorldObject&);

	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void CleanUp() = 0;
	D3DXVECTOR3 m_pos() const;
	void m_pos(D3DXVECTOR3 pos);
	D3DXQUATERNION m_rotation() const;
	void m_rotation(D3DXQUATERNION rot);

protected:
	D3DXVECTOR3 _m_pos;							//Updated by server
	D3DXQUATERNION _m_rotation;					//Updated by server

};

#endif
