#include "gParticleSystem.h"

using namespace psys;
class ParticleSmoke : public GparticleSystem
{
public:
	ParticleSmoke();
	void resetParticle(Attribute* attribute);
	void update(float timeDelta);

	
};