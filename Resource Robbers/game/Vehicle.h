#ifndef VEHICLE_H_
#define VEHICLE_H_

//#include <D3dx9math.h>
#include "TransformGroup.h"
#include "Shape3D.h"

class Vehicle: public DisplayObject
{
	typedef std::vector<Weapon *> Weapons;

public:
	enum Type
	{
		TANK = 0,
		NB_TYPE_MAX
	};

	Vehicle();
	Vehicle(ID);
	Vehicle(const Vehicle& copy);

	~Vehicle();
	friend class CSVreader;
	void Render(); //renders the vehicle and any associated weapons
//	void LoadMeshes();  //loads the vehicle mesh and any associated weapon meshes
	void setType(Type);
	//Get and Set methods for weapon list
	unsigned int GetNumWeapons(); //returns the current number of weapons for this vehicle
	const Weapons& GetWeapons() const; //returns weapon list
	void SetNewWeapon(LPCTSTR file, int gunSlot, TransformGroup * vehicleTrafo, vector<TransformGroup *> weaponTrafos, vector<Shape3D *> weapons); //adds a new weapon to the vehicle
	
	string	getName() {return m_name;	};
	void	setName(const string& name) {m_name = name;};
	int		getSizeClass() {return m_sizeClass;};
	void	setSizeClass(int class1) {m_sizeClass = class1;};
	int		getCost() {return m_cost;};
	void	setCost(int cost) {m_cost = cost;};
	int		getMaxSpeed() {return m_maxSpeed;};
	void	setMaxSpeed(int speed) {m_maxSpeed = speed;};
	int		getMaxHP() {return m_maxHP;};
	void	setMaxHP(int maxhp) {m_maxHP = maxhp;};
	int		getCarryCapcity() {return m_carryCapcity;};
	void	setCarryCapcity(int capcity) {m_carryCapcity = capcity;};
	int		getMaxGas() {return m_maxGas;	};
	void	setMaxGas(int gas) {m_maxGas = gas;};
	float	getMass() {return m_mass;};
	void	setMass(float mass) {m_mass = mass;};
	float	getMaxAcceleration() {return m_maxAcceleration;};
	void	setMaxAcceleration(float maxAcceleration) {m_maxAcceleration = maxAcceleration;};
	int		getBaseArmor() {return m_baseArmor;};
	void	setBaseArmor(int armor) {m_baseArmor = armor;};
	string	getPath() {return m_path;};
	void	setPath(const string& path) {m_path = path;};
	void	setGunPos(const D3DXVECTOR3& gps){ m_gunPos.push_back(gps);};
	void	setGunsDrops(const string& arr){m_gunsDrops = arr;};
	
private:
	static LPCTSTR getModelFromType(Type type); //sets the type of vehicle to instantiate by setting filepath to corresponding mesh
	D3DXVECTOR3 position;
	D3DXQUATERNION rotation;
	Weapons _weapons;  //weapons list array
	static const int maxWeapons = 1;  //weapon capacity for vehicle class - should be implemented depending on type
	string m_name;
	int m_sizeClass;
	int m_cost;
	int m_maxSpeed;
	int m_maxHP;
	int m_carryCapcity;
	int m_maxGas;
	float m_mass;
	float m_maxAcceleration;
	int m_baseArmor;
	string m_gunsDrops;
	string m_path;
	vector<D3DXVECTOR3> m_gunPos;

};

#endif //!VEHICLE_H_