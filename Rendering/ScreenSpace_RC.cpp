/***********************************************
* Filename:  		CRenderContext.cpp
* Date:      		10/2/2014
* Mod. Date: 		10/2/14
* Mod. Initials:	JM
* Author:    		Jim Merrell
* Purpose:   		This is the RenderContext Class that holds common render data
************************************************/

#include "ScreenSpace_RC.h"
#include "CompiledShaders\VertShader_2D.csh"
#include "CompiledShaders\PixShader_2D.csh"
#include "CompiledShaders\Gradient_PS.csh"
#include "CompiledShaders\MultiTexture_2D_PS.csh"

void ScreenSpace_RC::Initialize(ID3D11Device* device, ID3D11DeviceContext* immediateContext, void* texture_SRVs, SCENE* _scene)
{
	pDevice = device;
	pImmediateContext = immediateContext;

	m_SRVs = (map<string, CComPtr<ID3D11ShaderResourceView>>*)texture_SRVs;

	SetupShadersAndInputLayout();
	BlendState_Setup();

	spriteBatch.reset(new SpriteBatch(pImmediateContext));
	spriteFont.reset(new SpriteFont(pDevice, L"../../Assets/Textures/Arial.SpriteFont"));
}

void ScreenSpace_RC::Run()
{
	for (int layer = 2; layer > -1; layer--)
	{
		// draw the Screen Space objects to the screen
		for (int i = 0; i < (int)m_vScreenSpaceObjects[layer].size(); i++)
		{
			if (m_vScreenSpaceObjects[layer][i].textureName.size() > 0)
			{
				ID3D11ShaderResourceView* SRVs[1] = { (*m_SRVs)[m_vScreenSpaceObjects[layer][i].textureName] };
				pImmediateContext->PSSetShaderResources(0, 1, SRVs);
			}
			else
			{
				ID3D11ShaderResourceView* SRVs[1] = { (*m_SRVs)["white"] };
				pImmediateContext->PSSetShaderResources(0, 1, SRVs);
			}

			ID3D11Buffer* bufferarr[1] = { m_vScreenSpaceObjects[layer][i].Vertex_Buffer };
			UINT ArrStrides[1] = { sizeof(VERTEX_2D) };
			UINT ArrOffsets[1] = { 0 };
			pImmediateContext->IASetVertexBuffers(0, 1, bufferarr, ArrStrides, ArrOffsets);
			pImmediateContext->VSSetShader(VertexShader, NULL, NULL);

			if(m_vScreenSpaceObjects[layer][i].useGradient_PS == 0)
				pImmediateContext->PSSetShader(PixelShader, NULL, NULL);
			else if(m_vScreenSpaceObjects[layer][i].useGradient_PS == 1)
				pImmediateContext->PSSetShader(Gradient_PixelShader, NULL, NULL);
			else
			{
				if (m_vScreenSpaceObjects[layer][i].secondTexture.size() > 0)
				{
					ID3D11ShaderResourceView* SRVs[1] = { (*m_SRVs)[m_vScreenSpaceObjects[layer][i].secondTexture] };
					pImmediateContext->PSSetShaderResources(1, 1, SRVs);
				}
				else
				{
					ID3D11ShaderResourceView* SRVs[1] = { (*m_SRVs)["white"] };
					pImmediateContext->PSSetShaderResources(1, 1, SRVs);
				}
				
				if (m_vScreenSpaceObjects[layer][i].thirdTexture.size() > 0)
				{
					ID3D11ShaderResourceView* SRVs[1] = { (*m_SRVs)[m_vScreenSpaceObjects[layer][i].thirdTexture] };
					pImmediateContext->PSSetShaderResources(2, 1, SRVs);
				}
				else
				{
					ID3D11ShaderResourceView* SRVs[1] = { (*m_SRVs)["white"] };
					pImmediateContext->PSSetShaderResources(2, 1, SRVs);
				}

				pImmediateContext->PSSetShader(MultiTexture_PixelShader, NULL, NULL);
			}

			pImmediateContext->IASetInputLayout(InputLayout);

			pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			pImmediateContext->IASetIndexBuffer(m_vScreenSpaceObjects[layer][i].Index_Buffer, DXGI_FORMAT_R16_UINT, 0);

			pImmediateContext->DrawIndexed(6, 0, 0);
		}

		// begin sprite rendering
		{
			spriteBatch->Begin(SpriteSortMode_Deferred, pBlendState);

			for(int i = 0; i < (int)m_vRenderTextures[layer].size(); i++)
			{
				RENDER_TEXTURE RT = m_vRenderTextures[layer][i];

				if(RT.textureName.size() < 1)
					RT.textureName = "white";

				XMVECTOR color = XMLoadFloat4(&RT.color);

				spriteBatch->Draw((*m_SRVs)[RT.textureName], RT.position, NULL, color, XMConvertToRadians(RT.rotation), RT.origin, RT.scale);
			}

			for (int i = 0; i < (int)m_vRenderTexts[layer].size(); i++)
			{
				RENDER_TEXT t = m_vRenderTexts[layer][i];

				bool validString = isValidString(t.text.c_str());
				if(validString == false)
				{
					t.text = "String passed to RenderText() NOT valid.";
					//MessageBox(NULL, L"String passed to RenderText() NOT valid.", NULL, NULL);
					//continue; // don't write out this string
				}

				std::wstring Text_Out = std::wstring(t.text.begin(), t.text.end());

				const wchar_t* widecstr = Text_Out.c_str();

				XMVECTOR color = { t.color.x, t.color.y, t.color.z, t.color.w };
				XMVECTOR position = { t.position.x, t.position.y };
				XMVECTOR origin = { t.origin.x, t.origin.y };

				spriteFont->DrawString(spriteBatch.get(), widecstr, position, color, t.rotation, origin, t.scale);
			}

			spriteBatch->End();
		}

	}

	// clear the screen space objects / screen space objects are added
	//		to this list every frame by the DrawRect and DrawTextureFunctions
	for (int i = 0; i < 3; i++)
	{
		m_vScreenSpaceObjects[i].clear();
		m_vRenderTexts[i].clear();
		m_vRenderTextures[i].clear();
	}
}

void ScreenSpace_RC::SetupShadersAndInputLayout()
{
	pDevice->CreateVertexShader(VertShader_2D, sizeof(VertShader_2D), NULL, &VertexShader);
	pDevice->CreatePixelShader(PixShader_2D, sizeof(PixShader_2D), NULL, &PixelShader);
	pDevice->CreatePixelShader(Gradient_PS, sizeof(Gradient_PS), NULL, &Gradient_PixelShader);
	pDevice->CreatePixelShader(MultiTexture_2D_PS, sizeof(MultiTexture_2D_PS), NULL, &MultiTexture_PixelShader);

	D3D11_INPUT_ELEMENT_DESC vLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	pDevice->CreateInputLayout(vLayout, 4, &VertShader_2D, sizeof(VertShader_2D), &InputLayout);
}

void ScreenSpace_RC::SetBlendState(void* blendstate)
{
	pBlendState = (ID3D11BlendState*)blendstate;
}

void ScreenSpace_RC::AddRenderText(RENDER_TEXT renText, int layer)
{
	m_vRenderTexts[layer].push_back(renText);
}

void ScreenSpace_RC::AddRenderRECT(ScreenSpaceObject SSobj, int layer)
{
	m_vScreenSpaceObjects[layer].push_back(SSobj);
}

void ScreenSpace_RC::AddRenderTexture(RENDER_TEXTURE RT, int layer)
{
	m_vRenderTextures[layer].push_back(RT);
}

void ScreenSpace_RC::BlendState_Setup()
{
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
	pDevice->CreateBlendState(&bDesc, &pBlendState);
}

bool ScreenSpace_RC::isValidString(char const* s) 
{
	return strspn(s, validChars) == strlen(s);
}

int ScreenSpace_RC::FindStringLength(WCHAR const* string) 
{
	XMFLOAT3 Vec;
	XMStoreFloat3(&Vec, spriteFont->MeasureString(string));
	return (int)(Vec.x+0.5f);
}