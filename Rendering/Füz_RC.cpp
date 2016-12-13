/***********************************************
* Filename:  		CRenderContext.cpp
* Date:      		10/2/2014
* Mod. Date: 		10/2/14
* Mod. Initials:	JM
* Author:    		Jim Merrell
* Purpose:   		This is the RenderContext Class that holds common render data
************************************************/

#include "Füz_RC.h"

#include "CompiledShaders\Füz_VS.csh"
#include "CompiledShaders\Füz_GS.csh"
#include "CompiledShaders\Füz_PS.csh"

#include "CRenderer.h"



Fuz_RC::~Fuz_RC(void)
{
	for (int i = 0; i < (int)m_vRenderables.size(); i++)
	{
		delete m_vRenderables[i];
	}
	m_vRenderables.clear();
}

void Fuz_RC::Initialize(ID3D11Device* device, ID3D11DeviceContext* immediateContext, void* texture_SRVs, SCENE* _scene)
{
	pDevice = device;
	pImmediateContext = immediateContext;

	scene = _scene;

	SetupShadersAndInputLayout();

	m_pRenderer = nullptr;
}

void Fuz_RC::Run()
{
	int FuzCount = (int)m_vRenderables.size();

	//stringstream debugfeed;
	//debugfeed << "Number of FuzFX: " << FuzCount;
	//string debugString = debugfeed.str();
	//m_pRenderer->RenderText(debugString, CRenderer::SCREEN_FRONT, 0, 350);

	if(FuzCount > 0)
	{
		pImmediateContext->VSSetShader(VertexShader, NULL, NULL);
		pImmediateContext->GSSetShader(Fuz_GeoShader, NULL, NULL);
		pImmediateContext->PSSetShader(PixelShader, NULL, NULL);


		// draw the objects into the world
		for (int i = 0; i < FuzCount; i++)
		{
			//XMFLOAT4X4 worldMatrix;		
			if(m_vRenderables[i]->FuzPool)
			{
				//XMStoreFloat4x4(&worldMatrix, m_vRenderables[i]->object->getWorldMatrix());
		
				//D3D11_MAPPED_SUBRESOURCE MappedResource;
				//ZeroMemory(&MappedResource, sizeof(MappedResource));
				//pImmediateContext->Map(m_vRenderables[i]->matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &MappedResource);
				//memcpy(MappedResource.pData, &worldMatrix, sizeof(worldMatrix));
				//pImmediateContext->Unmap(m_vRenderables[i]->matrixBuffer, 0);
		
				//ID3D11Buffer* constbuffer[1] = { m_vRenderables[i]->matrixBuffer };
				//pImmediateContext->VSSetConstantBuffers(0, 1, constbuffer);

				// Vertex buffer
				D3D11_MAPPED_SUBRESOURCE MappedResource;
				ZeroMemory(&MappedResource, sizeof(MappedResource));
				pImmediateContext->Map(m_vRenderables[i]->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &MappedResource);
				memcpy(MappedResource.pData, &(m_vRenderables[i]->FuzPool[0]), MAX_FUZ_FX * sizeof(Fuz_Effect));
				pImmediateContext->Unmap(m_vRenderables[i]->vertexBuffer, 0);

				ID3D11Buffer* bufferarr[1] = { m_vRenderables[i]->vertexBuffer };
				UINT ArrStrides[1] = { sizeof(Fuz_Effect) };
				UINT ArrOffsets[1] = { 0 };
				pImmediateContext->IASetVertexBuffers(0, 1, bufferarr, ArrStrides, ArrOffsets);
		
				pImmediateContext->IASetInputLayout(InputLayout);
		
				pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		
				pImmediateContext->Draw(MAX_FUZ_FX, 0);
			}		
		}

		pImmediateContext->GSSetShader(NULL, NULL, NULL);
	}
}

void Fuz_RC::SetupShadersAndInputLayout()
{
	pDevice->CreateVertexShader(Füz_VS, sizeof(Füz_VS), NULL, &VertexShader);
	pDevice->CreateGeometryShader(Füz_GS, sizeof(Füz_GS), NULL, &Fuz_GeoShader);
	pDevice->CreatePixelShader(Füz_PS, sizeof(Füz_PS), NULL, &PixelShader);
	
	// Fuz_Effect Layout
	// XMFLOAT3 f3_WorldPosition;
	// XMFLOAT3 f3_ColorRGB;
	// XMFLOAT3 f3_Normal;
	// float m_fRadius;
	// float m_fLifetime;

	D3D11_INPUT_ELEMENT_DESC vLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "RADIUS", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "LIFETIME", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	pDevice->CreateInputLayout(vLayout, 5, &Füz_VS, sizeof(Füz_VS), &InputLayout);
}

void Fuz_RC::AddRenderObject(Fuz* ren_obj)
{
	m_vRenderables.push_back(ren_obj);
}

void Fuz_RC::RemoveRenderObject(FUZ_EFFECT_POOL* obj)
{
	vector<Fuz*>::iterator itr;
	for (itr = m_vRenderables.begin(); itr != m_vRenderables.end();)
	{
		Fuz* ren = *itr;
		if (ren->FuzPool == obj)
		{
			delete ren;
			m_vRenderables.erase(itr);
			break;
		}
		else
		{
			itr++;
		}
	}
}

