#ifndef CSVREADER_H
#define CSVREADER_H

using namespace std;

class CSVreader {
public:
	void loadVehicles(string fileName);
	Vehicle* getVehicle(Vehicle::Type typ);
	void loadWeapons(string fileName);
	vector<Vehicle*> vehicles;
	
};


#endif 
