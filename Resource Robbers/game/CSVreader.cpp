#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <DXUT.h>
#include "common/dualBuffer.hh"
#include "common/Typedef.h"
#include "server/Object.h"
#include "Game/WorldObject.h"
#include "Game/DisplayObject.h"
#include "Game/weapon.h"
#include "Game/Vehicle.h"
#include "CSVreader.h"

void CSVreader::loadVehicles(string fileName){
	string line;
	Vehicle *tempVehicle;

	ifstream myfile (fileName.c_str());
	if (myfile.is_open())
	{
		while (! myfile.eof() )
		{
			getline (myfile,line);
			if(line.length() == 0)continue;
			tempVehicle = new Vehicle();
			
			//name
			tempVehicle->setName(line.substr(0,line.find(',')));
			line = line.substr(line.find(',')+1);
			//size/class
			tempVehicle->setSizeClass(atoi(line.substr(0,line.find(',')).c_str()));
			line = line.substr(line.find(',')+1);
			//cost
			tempVehicle->setCost(atoi(line.substr(0,line.find(',')).c_str()));
			line = line.substr(line.find(',')+1);
			//maxspeed
			tempVehicle->setMaxSpeed(atoi(line.substr(0,line.find(',')).c_str()));
			line = line.substr(line.find(',')+1);
			//maxhp
			tempVehicle->setMaxHP(atoi(line.substr(0,line.find(',')).c_str()));
			line = line.substr(line.find(',')+1);
			//carryCapcity
			tempVehicle->setCarryCapcity(atoi(line.substr(0,line.find(',')).c_str()));
			line = line.substr(line.find(',')+1);
			//max gas
			tempVehicle->setMaxGas(atoi(line.substr(0,line.find(',')).c_str()));
			line = line.substr(line.find(',')+1);
			//mass
			tempVehicle->setMass(atof(line.substr(0,line.find(',')).c_str()));
			line = line.substr(line.find(',')+1);
			//max acceleration
			tempVehicle->setMaxAcceleration(atof(line.substr(0,line.find(',')).c_str()));
			line = line.substr(line.find(',')+1);
			//base armor
			tempVehicle->setBaseArmor(atoi(line.substr(0,line.find(',')).c_str()));
			line = line.substr(line.find(',')+1);
			//gun/drop sizes
			tempVehicle->setGunsDrops(line.substr(0,line.find(',')));
			line = line.substr(line.find(',')+1);
			//path
			tempVehicle->setPath(line.substr(0,line.find(',')));
			line = line.substr(line.find(',')+1);
			//gunpos
			vector<float> tempPos;
			string tempposstr = "";
			for(int p=0; p<24; p+=3){
				tempposstr = line.substr(0,line.find(','));
				if(tempposstr.length() == 0){
					continue;
				}
				else{
					float t1 = atof(tempposstr.substr(0,tempposstr.find(' ')).c_str());
					tempposstr = tempposstr.substr(tempposstr.find(' ')+1);
					float t2 = atof(tempposstr.substr(0,tempposstr.find(' ')).c_str());
					tempposstr = tempposstr.substr(tempposstr.find(' ')+1);
					float t3 = atof(tempposstr.substr(0,tempposstr.find(' ')).c_str());
					tempVehicle->setGunPos(D3DXVECTOR3 (t1, t2, t3));
				}
				line = line.substr(line.find(',')+1);
			}
			//tempVehicle->setGunPos(tempPos);
		


		

			vehicles.push_back(tempVehicle);
		}
		myfile.close();
	} 

	//return 0;
}
Vehicle* CSVreader::getVehicle(Vehicle::Type typ){
	return new Vehicle(*vehicles[typ]);
}