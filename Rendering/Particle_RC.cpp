/***********************************************
* Filename:  		Particle_RC.cpp
* Date:      		10/29/2014
* Mod. Date: 		10/29/2014
* Mod. Initials:	WR
* Author:    		Wesley Rasmusson
* Purpose:   		Holds rendering data specific to particles
************************************************/

#include "Precompiled.h"
#include "Particle_RC.h"
#include "../VFX/Particle.h"

// Compiled shaders
#include "CompiledShaders\Particle_VS.csh"
#include "CompiledShaders\Particle_GS.csh"
#include "CompiledShaders\Particle_PS.csh"

Particle_RC::Particle_RC(void)
{

}

Particle_RC::~Particle_RC(void)
{
	m_vRenderables.clear();
}

void Particle_RC::Initialize(ID3D11Device* device, ID3D11DeviceContext* immediateContext, void* texture_SRVs, SCENE* _scene)
{
	// Set members from passed in parameters
	pDevice = device;
	pImmediateContext = immediateContext;
	m_SRVs = (map<string, CComPtr<ID3D11ShaderResourceView>>*)texture_SRVs;
	scene = _scene;

	// Set up blend state to make particles blend
	D3D11_BLEND_DESC bDesc;
	ZeroMemory(&bDesc, sizeof(bDesc));
	bDesc.AlphaToCoverageEnable = false;
	bDesc.IndependentBlendEnable = false;
	bDesc.RenderTarget[0].BlendEnable = true;
	bDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	bDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	bDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	pDevice->CreateBlendState(&bDesc, &m_pBlendState);

	// Helper to set up shader and input layout
	SetupShadersAndInputLayout();
}

void Particle_RC::Run(void)
{
	unsigned int size = m_vRenderables.size();
	if(!size)
		return;

	pImmediateContext->OMSetBlendState(m_pBlendState, NULL, UINT_MAX);

	// Set shaders
	pImmediateContext->VSSetShader(VertexShader, NULL, NULL);
	pImmediateContext->GSSetShader(GeoShader, NULL, NULL);
	pImmediateContext->PSSetShader(PixelShader, NULL, NULL);

	// For each emitter, set up matrix and vertex buffer
	for(int i = 0; i < (int)m_vRenderables.size(); i++)
	{
		// If not set to render, skip thie emitter
		if(m_vRenderables[i]->GetToRender() == false)
			continue;

		// Set texture
		if(m_vRenderables[i]->GetParticleRenderVertInfo().size() <= 0)
		{
			RemoveRenderObject(m_vRenderables[i]);
			i--;
			continue;
		}
		string textureName = m_vRenderables[i]->GetTexture();
		if(textureName.size() <= 0)
			textureName = "none";
		if (textureName != "none")
		{
			ID3D11ShaderResourceView* SRVs[1] = { (*m_SRVs)[textureName] };
			pImmediateContext->PSSetShaderResources(0, 1, SRVs);
		}
		else
		{
			ID3D11ShaderResourceView* SRVs[1] = { (*m_SRVs)["white"] };
			pImmediateContext->PSSetShaderResources(0, 1, SRVs);
		}

		UINT numParticles = m_vRenderables[i]->GetParticleRenderVertInfo().size();
		XMFLOAT4X4 worldMatrix = m_vRenderables[i]->GetWorldMatrix();
		// Matrix buffer
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		ZeroMemory(&MappedResource, sizeof(MappedResource));
		pImmediateContext->Map(m_vRenderables[i]->matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &MappedResource);
		memcpy(MappedResource.pData, &worldMatrix, sizeof(worldMatrix));
		pImmediateContext->Unmap(m_vRenderables[i]->matrixBuffer, 0);

		ID3D11Buffer* constbuffer[1] = { m_vRenderables[i]->matrixBuffer };
		pImmediateContext->VSSetConstantBuffers(0, 1, constbuffer);

		// Vertex buffer
		ZeroMemory(&MappedResource, sizeof(MappedResource));
		pImmediateContext->Map(m_vRenderables[i]->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &MappedResource);
		memcpy(MappedResource.pData, &(m_vRenderables[i]->GetParticleRenderVertInfo()[0]), numParticles * sizeof(PARTICLE_VERTEX));
		pImmediateContext->Unmap(m_vRenderables[i]->vertexBuffer, 0);

		ID3D11Buffer* bufferarr[1] = { m_vRenderables[i]->vertexBuffer };
		UINT ArrStrides[1] = { sizeof(PARTICLE_VERTEX) };
		UINT ArrOffsets[1] = { 0 };
		pImmediateContext->IASetVertexBuffers(0, 1, bufferarr, ArrStrides, ArrOffsets);

		// Input layout
		pImmediateContext->IASetInputLayout(InputLayout);

		// Primitive topology
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		// Draw
		pImmediateContext->Draw(numParticles, 0);
	}

	// Set Geometry shader to NULL
	pImmediateContext->GSSetShader(NULL, NULL, NULL);
}

void Particle_RC::AddRenderObject(CEmitter* ren_obj)
{
	m_vRenderables.push_back(ren_obj);
}

void Particle_RC::RemoveRenderObject(CEmitter* obj)
{
	for(auto iter = m_vRenderables.begin(); iter != m_vRenderables.end(); ++iter)
	{
		if(obj == (*iter))
		{
			m_vRenderables.erase(iter);
			return;
		}
	}
}

void Particle_RC::RemoveAll(void)
{
	m_vRenderables.clear();
}

void Particle_RC::SetupShadersAndInputLayout(void)
{
	pDevice->CreateVertexShader(Particle_VS, sizeof(Particle_VS), NULL, &VertexShader);
	pDevice->CreateGeometryShader(Particle_GS, sizeof(Particle_GS), NULL, &GeoShader);
	pDevice->CreatePixelShader(Particle_PS, sizeof(Particle_PS), NULL, &PixelShader);

	D3D11_INPUT_ELEMENT_DESC vLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "PSIZE",    0, DXGI_FORMAT_R32_FLOAT,			 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	pDevice->CreateInputLayout(vLayout, 3, &Particle_VS, sizeof(Particle_VS), &InputLayout);
}