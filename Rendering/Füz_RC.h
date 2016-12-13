/***********************************************
* Filename:  		CRenderContext.h
* Date:      		10/2/2014
* Mod. Date: 		10/2/2014
* Mod. Initials:	JM
* Author:    		Jim Merrell
* Purpose:   		This is the RenderContext Class that holds common render data
************************************************/

#ifndef FUZ_RC_H
#define FUZ_RC_H

#include "RenderContext.h"
#include "../Object Management/CFüz.h"

class Fuz_RC : public CRenderContext
{
public:
	Fuz_RC(void){}
	~Fuz_RC(void);

	
	virtual void Initialize(ID3D11Device* device, ID3D11DeviceContext* immediateContext, void* texture_SRVs, SCENE* _scene);
	virtual void Run();
	
	virtual void AddRenderObject(Fuz* ren_obj);
	virtual void RemoveRenderObject(FUZ_EFFECT_POOL* ren_obj);

	CRenderer* m_pRenderer;

private:

//Factory Methods
	virtual void SetupShadersAndInputLayout();

	vector<Fuz*>	m_vRenderables;
	
	CComPtr<ID3D11GeometryShader> Fuz_GeoShader;
};

#endif