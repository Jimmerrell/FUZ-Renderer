/***********************************************
* Filename:  		Particle_RC.h
* Date:      		10/29/2014
* Mod. Date: 		10/29/2014
* Mod. Initials:	WR
* Author:    		Wesley Rasmusson
* Purpose:   		Holds rendering data specific to particles
************************************************/

#ifndef _PARTICLE_RC_H_
#define _PARTICLE_RC_H_

#include "rendercontext.h"

class CParticle;

class Particle_RC :
	public CRenderContext
{
private:
	vector<CEmitter*>	m_vRenderables;
	
	CComPtr<ID3D11GeometryShader> GeoShader;
	CComPtr<ID3D11BlendState> m_pBlendState;

public:
	Particle_RC(void);
	~Particle_RC(void);

	virtual void Initialize(ID3D11Device* device, ID3D11DeviceContext* immediateContext, void* texture_SRVs, SCENE* _scene);
	virtual void Run(void);
	
	virtual void AddRenderObject(CEmitter* ren_obj);
	virtual void RemoveRenderObject(CEmitter* obj);
	void RemoveAll(void);

private:
	virtual void SetupShadersAndInputLayout(void);
};

#endif