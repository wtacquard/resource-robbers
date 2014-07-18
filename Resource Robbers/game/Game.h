#ifndef GAME_H_
# define GAME_H_

#include <DXUT.h>

#include "WindowDelegator/WindowDelegator.h"
#include "controller/InputController.h"
#include "controller/commands.h"
#include "graphics/camera.h"
#include "graphics/Graphics.h"
#include "Vehicle.h"
#include "Weapon.h"
#include "WorldObject.h"
#include "common/DualBuffer.hh"
#include "socket.hpp"
#include "gParticleSystem.h"
#include "ParticleSmoke.h"
#include "CSVreader.h"
#include "cTerrainObject.h"
#include "TransformGroup.h"
#include "Shape3D.h"

class Game : public Window
{
private:
	//CModelViewerCamera          g_Camera;   
	InputController inputController;
	Graphics graphics;
	Commands commands;
	ParticleSmoke *particleTest;
	cTerrain * terrainTest;
	Merger::Buffers wObjects;
	Merger _merger;
	Camera camera1;//Camera::LANDOBJECT cameraType);

	CSVreader configs;
	TransformGroup * world;
	std::vector<TransformGroup *> vehicleTrafos;
	std::vector<Shape3D *> vehicles;
	std::vector<std::vector<TransformGroup *>> weaponTrafos;
	std::vector<std::vector<Shape3D *>> weapons;
public:
	Game(RR::Socket&);
	~Game();

	Graphics* getGraphics();
	virtual void frameMove( double totalTime, float elapsedTime, void *data );
	void onCreateDevice(IDirect3DDevice9* d3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc);
	void onDestroyDevice();
	virtual void parseKeys( UINT vKey, bool keyDown, bool altDown );
	virtual void parseMouse(bool leftButtonDown, bool rightButtonDown, bool middleButtonDown, bool sideButton1Down,
		bool sideButton2Down, int mouseWheelDelta, int xPos, int yPos, void* context);
	virtual HRESULT render( IDirect3DDevice9 *d3dDevice, double totalTime,
		float elapsedTime, void *data );
	virtual void onNetworkSignal(std::string &message);
};

#endif //!GAME_H_
