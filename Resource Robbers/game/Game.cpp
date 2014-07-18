#include <string>
#include <iostream>
#include <sstream>
#include <winsock2.h>
#include <exception>
#include "Exception.hpp"
#include "SocketException.hpp"
#include "Log.hpp"
#include "Socket.hpp"
#include "SocketClient.hpp"

#include <map>
#include <vector>
#include "DXUT.h"


#include "common/typedef.h"
#include "common/DualBuffer.hh"
#include "server/Object.h"
#include "game/WorldObject.h"
#include "game/DisplayObject.h"
#include "game/Merger.hpp"
#include "common/ClientController.h"
#include "sound/SoundController.h"
#include "controller/Commands.h"
#include "game/Weapon.h"
#include "game/Vehicle.h"
#include "common/globals.h"
#include "game/Game.h"
#include <sstream>

using boost::ptr_set;
using RR::Socket;



Game::Game(Socket& socket)
: inputController(), graphics(), commands(), particleTest(NULL), wObjects(), _merger(wObjects)
{
	
	configs.loadVehicles("../assests/assets1test.csv");
	world = new TransformGroup();
	commands.setupForClient(socket);
	ptr_set<Command>::iterator it;
	ptr_set<Command>::iterator end;
	for(it = commands.getCommands().begin(), end = commands.getCommands().end(); it != end; it++)
		inputController.registerKeyMapping(it->getKey(), it->getCallback());
}

/* graphics stuffs must be deleted in onDestroyDevice */
Game::~Game()
{
	delete world;
}

void
Game::onNetworkSignal(std::string &message)
{
	_merger.update(message);
	Merger::Buffer * read_buf = wObjects.getReadAccess();
	unsigned int i = 0;
	for (Merger::Buffer::iterator it = read_buf->begin(); it != read_buf->end(); ++it, ++i)
	{
		if (i < vehicleTrafos.size() && it->second != vehicles[i]->getObject())
		{
			vehicles[i]->setObject((Vehicle *)it->second);
			vehicleTrafos[i]->getChildNodes().clear();
			vehicleTrafos[i]->addChild(vehicles[i]);
			weaponTrafos[i].clear();
			weapons[i].clear();
			for (unsigned int j = 0; j < ((Vehicle *)it->second)->GetNumWeapons(); ++j)
			{
				weaponTrafos[i].push_back(new TransformGroup());
				vehicleTrafos[i]->addChild(weaponTrafos[i].back());
				weapons[i].push_back(new Shape3D(((Vehicle *)it->second)->GetWeapons()[j]));
				weaponTrafos[i].back()->addChild(weapons[i].back());
			}
		}
		else if (i >= vehicleTrafos.size())
		{
			vehicleTrafos.push_back(new TransformGroup());
			world->addChild(vehicleTrafos.back());
			vehicles.push_back(new Shape3D((Vehicle *)it->second));
			vehicleTrafos.back()->addChild(vehicles.back());
			std::vector<TransformGroup *> vector1;
			weaponTrafos.push_back(vector1);
			std::vector<Shape3D *> vector2;
			weapons.push_back(vector2);
			for (unsigned int j = 0; j < ((Vehicle *)it->second)->GetNumWeapons(); ++j)
			{
				weaponTrafos[i].push_back(new TransformGroup());
				vehicleTrafos.back()->addChild(weaponTrafos[i].back());
				weapons[i].push_back(new Shape3D(((Vehicle *)it->second)->GetWeapons()[j]));
				weaponTrafos[i].back()->addChild(weapons[i].back());
			}
		}
	}
	wObjects.releaseReadAccess(&read_buf);
}

void
Game::frameMove(double totalTime, float elapsedTime, void *data)
{
	Merger::Buffer* read_buf;

	// Setup the world, view, and projection matrices
	//graphics.SetupMatrices();
	// Setup the lights
	graphics.SetLights();

/*
/*
	//set vehicle positions
	tempWBuff = wObjects.getWriteAccess();

	float i[4] = {0,1,1,-1};
	float j[4] = {0,1,1,-1};

	int k = 0;
	for(Merger::Buffer::iterator it = tempWBuff->begin(); it != tempWBuff->end(); ++it, ++k)
	{
		vehicleTrafos[k]->MoveMesh(D3DXVECTOR3(i[k],0,j[k]), D3DXQUATERNION(0, 0, 0, 1)); 
		if(((Vehicle*)it->second)->GetNumWeapons() == 1)
			((Vehicle*)it->second)->GetWeapons()[0]->MoveMesh(((Vehicle*)it->second)->GetWeapons()[0]->m_pos(), D3DXQUATERNION(0, 0, 0, 1));
	}

//		((Vehicle*)tempWBuff->at(1))->MoveMesh(D3DXVECTOR3(-1,0,0), vrotation); 
//		((Vehicle*)tempWBuff->at(2))->MoveMesh(D3DXVECTOR3(0,0,1), vrotation); 
//		((Vehicle*)tempWBuff->at(3))->MoveMesh(D3DXVECTOR3(0,0,0), vrotation); 

	//set vehicle weapon positions
	
	((Vehicle*)tempWBuff->at(0))->GetWeapon()[0].MoveMesh(D3DXVECTOR3(.75, .5, 0), wrotation); //set vehicle weapon position
	((Vehicle*)tempWBuff->at(1))->GetWeapon()[0].MoveMesh(D3DXVECTOR3(-1.25, .5, 0), wrotation); //set vehicle weapon position
	((Vehicle*)tempWBuff->at(2))->GetWeapon()[0].MoveMesh(D3DXVECTOR3(-.25, .5, -1), wrotation); //set vehicle weapon position
	((Vehicle*)tempWBuff->at(3))->GetWeapon()[0].MoveMesh(D3DXVECTOR3(0, .5, -.25), wrotation); //set vehicle weapon position

	wObjects.releaseWriteAccess(&tempWBuff);
*/

	//this will update all objects automaticly every frame.  Disabled right now since
	//the server is not changing the states.
	read_buf = wObjects.getReadAccess();
	unsigned int i = 0;
	D3DXVECTOR3 * vehiclePos = new D3DXVECTOR3(0,0,0);
	for(Merger::Buffer::iterator it = read_buf->begin(); it != read_buf->end(); ++it, ++i)
	{
		if(i == 0 && it->second->m_pos() != NULL){
			vehiclePos->x = it->second->m_pos().x;
			vehiclePos->y = it->second->m_pos().y;
			vehiclePos->z = it->second->m_pos().z;
		}
		//it->second->Update();
		if (i < vehicleTrafos.size())
			vehicleTrafos[i]->Update();
	}
	
	//Merger::Buffer::iterator it = read_buf->begin();
	//vehiclePos =  new D3DXVECTOR3(it->second->m_pos());
	wObjects.releaseReadAccess(&read_buf);

	//Particle tests
	static float index = 0.0f;
	static float index2 = 0.0f;
	static float rot = 0.1f;
	static float rot2 = 0.1f;
	if(index < 0)
		rot = 0.05f;
	else if(index > 1.14)
	{
		rot = -0.05f;
		//particleTest->addParticle();
	}
	if(index2 < 0)
		rot2 = 0.05f;
	else if(index2 > 0.15)
	{
		rot2 = -0.05f;
		particleTest->addParticle();
	}
	index += rot;
	index2 += rot2;
	particleTest->update(.01f);
	//graphics.SetupMatrices();
	D3DXMATRIX V;
	//D3DXVECTOR3 * campos;
	//campos = new D3DXVECTOR3(vehiclePos->x,vehiclePos->y + 2, vehiclePos->z -2);
	camera1.walk(vehiclePos);
	//camera1.setLook(vehiclePos);
	//camera1.setPosition(vehiclePos);
	//camera1.setPosition(campos);
	camera1.getViewMatrix(&V);

	DXUTGetD3D9Device()->SetRenderState( D3DRS_ZENABLE, TRUE );	
	DXUTGetD3D9Device()->SetTransform(D3DTS_VIEW, &V);


	D3DXMATRIX matProjection; 
	D3DXMatrixPerspectiveFovLH(&matProjection,
		D3DXToRadian(45),    // the horizontal field of view
		DXUTGetWindowWidth() / DXUTGetWindowHeight(),    // the aspect ratio
		1.0f,    // the near view-plane
		100.0f);    // the far view-plane
	DXUTGetD3D9Device()->SetTransform(D3DTS_PROJECTION, &matProjection); 	
}

Graphics* Game::getGraphics()
{
	return &graphics;
}

void Game::parseKeys( UINT vKey, bool keyDown, bool altDown )
{
	inputController.parseKeys(vKey, keyDown, altDown, NULL);
}

void Game::onCreateDevice(IDirect3DDevice9* d3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc)
{
	RR::Log& log = RR::Log::instance();

	// Initialize sound and get wavebank/soundbank, then start bgmusic
	sound.Init();
	//sound.PlayBGMusic();
	log << "Sound initialized\n";
	terrainTest = new cTerrain();
	terrainTest->LoadMesh(L"../assests/3dmodels/terrain1.x");
	log << "Terrain initialized\n";
	particleTest = new ParticleSmoke();
	particleTest->init();
	particleTest->addParticle();
}

void Game::onDestroyDevice() 
{
	RR::Log& log = RR::Log::instance();

	_merger.clean();
	log << "World Objects clean\n";
	terrainTest->CleanUp();
	log << "Terrain clean\n";
	delete particleTest;
	log << "Particles clean\n";
	// Clean up sound objects
	sound.CleanUp();
	log << "Sound stopped and clean\n";
}

void Game::parseMouse(bool leftButtonDown, bool rightButtonDown, bool middleButtonDown, bool sideButton1Down,
					  bool sideButton2Down, int mouseWheelDelta, int xPos, int yPos, void* context)
{
	inputController.parseMouse(leftButtonDown, rightButtonDown, middleButtonDown, sideButton1Down, sideButton2Down,
		mouseWheelDelta, xPos, yPos, context);
	static bool middleDown = false;

	if(mouseWheelDelta != 0)
		camera1.zoom(-mouseWheelDelta);
	
	if(middleButtonDown){
		if(middleDown){
			camera1.orbitHorizontal(xPos);
			camera1.orbitVertical(yPos);
		}
		else{
			camera1.setMouse(xPos, yPos);
			middleDown = true;
		}
	}
	if(!middleButtonDown){
		middleDown = false;
	}
	/*
	if(xPos > DXUTGetWindowWidth()-50 && middleButtonDown)
		camera1.orbitHorizontal(-1);
	if(xPos < 50 && middleButtonDown)
		camera1.orbitHorizontal(1);
	if(yPos > DXUTGetWindowHeight()-50 && middleButtonDown)
		camera1.orbitVertical(-1);
	if(yPos < 50 && middleButtonDown)
		camera1.orbitVertical(1);
		*/
		
}



HRESULT Game::render( IDirect3DDevice9 *d3dDevice, double totalTime,
					 float elapsedTime, void *data )
{
	HRESULT hr;

	V(d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0));
	if(SUCCEEDED(d3dDevice->BeginScene()))
	{
		LPD3DXSPRITE sprite = ClientController::instance().getSprite();
		if(SUCCEEDED(sprite->Begin(0)))
			V(sprite->End());
		wObjects.swap();
		world->Render();
		terrainTest->Render();
		particleTest->render();
		V(d3dDevice->EndScene());
		d3dDevice->Present(NULL, NULL, NULL, NULL);
		// Update Sound
		sound.DoWork();
	}
	return S_OK;
}

