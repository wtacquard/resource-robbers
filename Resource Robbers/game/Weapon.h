#ifndef WEAPON_H
#define WEAPON_H



class Weapon: public DisplayObject
{
public:
	Weapon();
	~Weapon();
	void setOffset(D3DXVECTOR3 offset);
private:
	D3DXVECTOR3	startPos;
	D3DXVECTOR3 rotation;
};

#endif VEHICLE_H