#include <vector>
#include "DisplayObject.h"
#include "Weapon.h"
#include "Vehicle.h"


/****************************************************************************
* Constructor:
* Creates a new weapons list and sets starting location
*****************************************************************************/
Vehicle::Vehicle() 
: DisplayObject()
{
	//	SetNewWeapon(L"../assests/3dmodels/laserSingle-1.x"); //add a weapon for testing
}

Vehicle::Vehicle(ID id)
: DisplayObject(id)
{
}


Vehicle::Vehicle(const Vehicle& copy)
: DisplayObject(copy)
{
	setType(TANK);
}


void
Vehicle::setType(Type t)
{
	LoadMesh(getModelFromType(t));
}
/****************************************************************************
* Destructor:
*
*****************************************************************************/
Vehicle::~Vehicle()
{
	for (Weapons::iterator it = _weapons.begin(); it != _weapons.end(); ++it)
		delete(*it);
}



/****************************************************************************
* Set Type:
* Sets the vehicle type for the current instantiation
*****************************************************************************/
LPCTSTR Vehicle::getModelFromType(Type type) 
{
	switch(type)
	{
		case TANK:
			return (L"../assests/3dmodels/smalltank1-6.x");
			//defaults to tank
		default:
			return (L"../assests/3dmodels/smalltank1-6.x");
	}
}

/****************************************************************************
* Render:
* Renders all subsets of the vehicle as well as all it's weapons
*****************************************************************************/
void Vehicle::Render()
{					
    //Meshes are divided into subsets, one for each material. Render them in
    //a loop
	DXUTGetD3D9Device()->SetTransform(D3DTS_WORLD, &(GetWorldMatrix()));	//Set world matrix
    for( DWORD i = 0; i < GetNumMaterials(); i++ )
    {
        // Set the material and texture for this subset
        DXUTGetD3D9Device()->SetMaterial( &GetMeshMaterials()[i] );
        DXUTGetD3D9Device()->SetTexture( 0, GetMeshTextures()[i] );

        // Draw the mesh subset
        GetMesh()->DrawSubset( i );
    }

	//render all weapons as well
	//for (Weapons::iterator it = _weapons.begin(); it != _weapons.end(); ++it)
	//	(*it)->Render();
}



/****************************************************************************
* Weapon Set Accessor:
* Adds a new weapon to the weapons list
*****************************************************************************/
void Vehicle::SetNewWeapon(LPCTSTR file, int wepSlot, TransformGroup * vehicleTrafo, vector<TransformGroup *> weaponTrafos, vector<Shape3D *> weapons)
{
	Weapon *w = new Weapon;
	w->LoadMesh(file);
	w->setOffset(m_gunPos[wepSlot]);
	_weapons.push_back(w);
	weaponTrafos.push_back(new TransformGroup());
	vehicleTrafo->addChild(weaponTrafos.back());
	weapons.push_back(new Shape3D(w));
	weaponTrafos.back()->addChild(weapons.back());
}


/****************************************************************************
* Weapon Get Accessor:
* Returns the weapons list
*****************************************************************************/
const Vehicle::Weapons&
Vehicle::GetWeapons() const
{
	return (_weapons);
}



/****************************************************************************
* Get NumWeapons:
* Returns the number of weapons on this vehicle
*****************************************************************************/
unsigned int
Vehicle::GetNumWeapons() 
{
	return (_weapons.size());
}