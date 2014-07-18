#include "ParticleSmoke.h"

ParticleSmoke::ParticleSmoke()
{
	
	size            = 1.01f;
	vbSize          = 2048;
	vbOffset        = 0;  
	vbBatchSize     = 512; 
	//attribute->position		= pos;
	//attribute->velocity = 0.01f;
}

void ParticleSmoke::resetParticle(Attribute* attribute)
{
	attribute->isAlive  = true;

	

	

	attribute->position		=  D3DXVECTOR3 (2,0,1);;
	attribute->velocity = D3DXVECTOR3 (((rand() % 10000) * 0.0001f)/3 - .2,((rand() % 10000) * 0.0001f)/3 - .2,0.5f);

	// green
	attribute->color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	attribute->age      = 0.0f; 
	attribute->lifeTime = 2.0f; 
}

void ParticleSmoke::update(float timeDelta)
{
	std::list<Attribute>::iterator i;

	for(i = particles.begin(); i != particles.end(); i++)
	{
		i->position += i->velocity * timeDelta;

		i->age += timeDelta;

		if(i->age > i->lifeTime) // kill 
			i->isAlive = false;
	}
	removeDeadParticles();
}