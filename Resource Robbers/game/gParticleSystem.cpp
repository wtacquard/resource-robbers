#include "gParticleSystem.h"


using namespace psys;
inline DWORD FtoDW(float f) {
 return *((DWORD*)&f);
}
const DWORD gParticle::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
GparticleSystem::GparticleSystem(){
	vb     = 0;
	tex    = 0;
}
GparticleSystem::~GparticleSystem(){
	tex->Release();
	vb->Release();
}
bool GparticleSystem::init(){
	DXUTGetD3D9Device()->CreateVertexBuffer(
							vbSize * sizeof(gParticle),
							D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
							gParticle::FVF,
							D3DPOOL_DEFAULT,
							& vb,
							0);
	HRESULT hr = 0;
	const CHAR* strPrefix = "../assests/particles/smoke.png";
                CHAR strTexture[MAX_PATH];
                strcpy_s( strTexture, MAX_PATH, strPrefix );
	hr = D3DXCreateTextureFromFileA(
							DXUTGetD3D9Device(),
							strTexture,
							&tex);
	if(FAILED(hr))
	{
		::MessageBox(0,L"D3DXCreateTextureFromFile() - FAILED",L"PSystem", 0);
		//return false;
	}

	return true;
}
void GparticleSystem::reset(){
	std::list<Attribute>::iterator i;
	for(i = particles.begin(); i != particles.end(); i++){
		resetParticle(&(*i));
	}
}
void GparticleSystem::addParticle(){
	Attribute attribute;
	resetParticle(&attribute);
	particles.push_back(attribute);
}
void GparticleSystem::render(){

	if( !particles.empty() )
		{
	//pre render
	DXUTGetD3D9Device()->SetRenderState(D3DRS_LIGHTING, false);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_POINTSIZE, FtoDW(size));
	DXUTGetD3D9Device()->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(0.0f));
	//DXUTGetD3D9Device()->SetRenderState(D3DRS_POINTSIZE_MAX, (DWORD)50.0f);

	DXUTGetD3D9Device()->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(0.0f));
	DXUTGetD3D9Device()->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(0.0f));
	DXUTGetD3D9Device()->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(1.0f));

	DXUTGetD3D9Device()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	DXUTGetD3D9Device()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	DXUTGetD3D9Device()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	DXUTGetD3D9Device()->SetRenderState(D3DRS_ZWRITEENABLE, false);
	//render
	
		//
		// Remarks:  The render method works by filling a section of the vertex buffer with data,
		//           then we render that section.  While that section is rendering we lock a new
		//           section and begin to fill that section.  Once that sections filled we render it.
		//           This process continues until all the particles have been drawn.  The benifit
		//           of this method is that we keep the video card and the CPU busy.  

		
			//
			// set render states
			//

			//preRender();
			
			DXUTGetD3D9Device()->SetTexture(0, tex);
			DXUTGetD3D9Device()->SetFVF(gParticle::FVF);
			DXUTGetD3D9Device()->SetStreamSource(0, vb, 0, sizeof(gParticle));

			//
			// render batches one by one
			//

			// start at beginning if we're at the end of the vb
			if(vbOffset >= vbSize)
				vbOffset = 0;

			gParticle* v = 0;

			vb->Lock(
				vbOffset    * sizeof( gParticle ),
				vbBatchSize * sizeof( gParticle ),
				(void**)&v,
				vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

			DWORD numParticlesInBatch = 0;

			//
			// Until all particles have been rendered.
			//
			std::list<Attribute>::iterator i;
			for(i = particles.begin(); i != particles.end(); i++)
			{
				if( i->isAlive )
				{
					//
					// Copy a batch of the living particles to the
					// next vertex buffer segment
					//
					v->position = i->position;
					v->color    = (D3DCOLOR)i->color;
					v++; // next element;

					numParticlesInBatch++; //increase batch counter

					// if this batch full?
					if(numParticlesInBatch == vbBatchSize) 
					{
						//
						// Draw the last batch of particles that was
						// copied to the vertex buffer. 
						//
						vb->Unlock();

						DXUTGetD3D9Device()->DrawPrimitive(
							D3DPT_POINTLIST,
							vbOffset,
							vbBatchSize);

						//
						// While that batch is drawing, start filling the
						// next batch with particles.
						//

						// move the offset to the start of the next batch
						vbOffset += vbBatchSize; 

						// don't offset into memory thats outside the vb's range.
						// If we're at the end, start at the beginning.
						if(vbOffset >= vbSize) 
							vbOffset = 0;       

						vb->Lock(
							vbOffset    * sizeof( gParticle ),
							vbBatchSize * sizeof( gParticle ),
							(void**)&v,
							vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

						numParticlesInBatch = 0; // reset for new batch
					}	
				}
			}

			vb->Unlock();

			// its possible that the LAST batch being filled never 
			// got rendered because the condition 
			// (numParticlesInBatch == _vbBatchSize) would not have
			// been satisfied.  We draw the last partially filled batch now.
			
			if( numParticlesInBatch )
			{
				DXUTGetD3D9Device()->DrawPrimitive(
					D3DPT_POINTLIST,
					vbOffset,
					numParticlesInBatch);
			}

			// next block
			vbOffset += vbBatchSize; 

			//
			// reset render states
			//

			//postRender();
		
	


	//post render
	DXUTGetD3D9Device()->SetRenderState(D3DRS_LIGHTING, true);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_ZWRITEENABLE, true);
	}

}
void GparticleSystem::removeDeadParticles()
{
	std::list<Attribute>::iterator i;

	i = particles.begin();

	while( i != particles.end() )
	{
		if( i->isAlive == false )
		{
			// erase returns the next iterator, so no need to
		    // incrememnt to the next one ourselves.
			i = particles.erase(i); 
		}
		else
		{
			i++; // next in list
		}
	}
}