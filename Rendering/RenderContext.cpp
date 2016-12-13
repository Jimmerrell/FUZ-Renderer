/***********************************************
* Filename:  		CRenderContext.cpp
* Date:      		10/2/2014
* Mod. Date: 		10/2/14
* Mod. Initials:	JM
* Author:    		Jim Merrell
* Purpose:   		This is the RenderContext Class that holds common render data
************************************************/

#include "RenderContext.h"


CRenderContext::CRenderContext()
{
}


CRenderContext::~CRenderContext(void)
{
}

void CRenderContext::SetupRasterizerStates()
{
	m_bWireFrame = false;
	m_bThermalRadarEffect = false;

	D3D11_RASTERIZER_DESC cullBack_desc;
	ZeroMemory(&cullBack_desc, sizeof(cullBack_desc));
	cullBack_desc.FillMode = D3D11_FILL_SOLID;
	cullBack_desc.CullMode = D3D11_CULL_BACK;
	pDevice->CreateRasterizerState(&cullBack_desc, &m_pCullBack_RS);

	D3D11_RASTERIZER_DESC cullFront_desc;
	ZeroMemory(&cullFront_desc, sizeof(cullFront_desc));
	cullFront_desc.FillMode = D3D11_FILL_SOLID;
	cullFront_desc.CullMode = D3D11_CULL_FRONT;
	pDevice->CreateRasterizerState(&cullFront_desc, &m_pCullFront_RS);

	D3D11_RASTERIZER_DESC wireframe_cullBack_desc;
	ZeroMemory(&wireframe_cullBack_desc, sizeof(wireframe_cullBack_desc));
	wireframe_cullBack_desc.FillMode = D3D11_FILL_WIREFRAME;
	wireframe_cullBack_desc.CullMode = D3D11_CULL_BACK;
	pDevice->CreateRasterizerState(&wireframe_cullBack_desc, &m_pWireFrame_CullBack_RS);

	D3D11_RASTERIZER_DESC wireframe_cullFront_desc;
	ZeroMemory(&wireframe_cullFront_desc, sizeof(wireframe_cullFront_desc));
	wireframe_cullFront_desc.FillMode = D3D11_FILL_WIREFRAME;
	wireframe_cullFront_desc.CullMode = D3D11_CULL_FRONT;
	pDevice->CreateRasterizerState(&wireframe_cullFront_desc, &m_pWireFrame_CullFront_RS);
}

void CRenderContext::TurnThermalEffect_ON()
{
	m_bThermalRadarEffect = true;
}

void CRenderContext::TurnThermalEffect_OFF()
{
	m_bThermalRadarEffect = false;
}

void CRenderContext::ToggleWireFrame()
{
	m_bWireFrame = !m_bWireFrame;
}

void CRenderContext::ToggleThermalRadarEffect()
{
	//m_bWireFrame = !m_bWireFrame;
	m_bThermalRadarEffect = !m_bThermalRadarEffect;
}
