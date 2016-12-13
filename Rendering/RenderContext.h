/***********************************************
* Filename:  		CRenderContext.h
* Date:      		10/2/2014
* Mod. Date: 		10/2/2014
* Mod. Initials:	JM
* Author:    		Jim Merrell
* Purpose:   		This is the RenderContext Class that holds common render data
************************************************/

#ifndef CRENDERCONTEXT_H
#define CRENDERCONTEXT_H

#include "RendererCommon.h"

class CRenderContext
{
public:
	CRenderContext();
	~CRenderContext(void);

	
	virtual void Initialize(ID3D11Device* device, ID3D11DeviceContext* immediateContext, void* texture_SRVs, SCENE* _scene) = 0;
	virtual void Run() = 0;
	
	
	virtual void AddRenderObject(Renderable* ren_obj) {}
	virtual void RemoveRenderObject(Entity3D* obj) {}

	int								m_nTesselationLevel;
	float							m_fTestXPos;
	float							m_fTestZPos;

	void ToggleWireFrame();

	void TurnThermalEffect_ON();
	void TurnThermalEffect_OFF();
	void ToggleThermalRadarEffect();

protected:
	CComPtr<ID3D11DeviceContext>	pImmediateContext;
	CComPtr<ID3D11Device>			pDevice;

	CComPtr<ID3D11VertexShader>		VertexShader;
	CComPtr<ID3D11HullShader>		HullShader;
	CComPtr<ID3D11DomainShader>		DomainShader;
	CComPtr<ID3D11GeometryShader>	GeometryShader;
	CComPtr<ID3D11PixelShader>		PixelShader;
	CComPtr<ID3D11InputLayout>		InputLayout;

	CComPtr<ID3D11PixelShader>		Thermal_PS;

	map<string, CComPtr<ID3D11ShaderResourceView>>*	m_SRVs;
	SCENE*							scene;

	CComPtr<ID3D11RasterizerState> m_pCullBack_RS;
	CComPtr<ID3D11RasterizerState> m_pCullFront_RS;

	CComPtr<ID3D11RasterizerState> m_pWireFrame_CullBack_RS;
	CComPtr<ID3D11RasterizerState> m_pWireFrame_CullFront_RS;

	bool m_bWireFrame;
	bool m_bThermalRadarEffect;

//Factory Methods
	virtual void SetupShadersAndInputLayout() = 0;
	void SetupRasterizerStates();
};

#endif