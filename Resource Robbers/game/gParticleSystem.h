#ifndef GPARTICLESYSTEM_H
#define GPARTICLESYSTEM_H

#include <DXUT.h>
#include <list>

namespace psys
{
	struct gParticle
	{
		D3DXVECTOR3 position;
		D3DCOLOR    color;
		static const DWORD FVF;
	};
	
	struct Attribute
	{
		Attribute()
		{
			lifeTime = 0.0f;
			age      = 0.0f;
			isAlive  = true;
		}

		D3DXVECTOR3 position;     
		D3DXVECTOR3 velocity;     
		D3DXVECTOR3 acceleration; 
		float       lifeTime;     // how long the particle lives for before dying  
		float       age;          // current age of the particle  
		D3DXCOLOR   color;        // current color of the particle   
		D3DXCOLOR   colorFade;    // how the color fades with respect to time
		bool        isAlive;    
	};
	/*DWORD FtoDw(float f)
	{
	return *((DWORD*)&f);
	}*/
	class GparticleSystem{
	public:
		GparticleSystem();
		virtual ~GparticleSystem() =0;
		virtual bool init();
		virtual void addParticle();
		virtual void resetParticle(Attribute* attribute) = 0;
		virtual void reset();
		
		virtual void update(float timeDelta) = 0;
		virtual void render();

	protected:
		virtual void removeDeadParticles();

		D3DXVECTOR3				origin;
		float					emitRate;
		IDirect3DTexture9*		tex;
		IDirect3DVertexBuffer9*	vb;
		std::list<Attribute>		particles;
		int						maxParticles;
		float                   size; 
		DWORD vbSize;      // size of vb
		DWORD vbOffset;    // offset in vb to lock   
		DWORD vbBatchSize; // number of vertices to lock starting at _vbOffset

	};
}

#endif
