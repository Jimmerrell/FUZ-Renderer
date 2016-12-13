/***********************************************
* Filename:  		CRenderer.cpp
* Date:      		9/10/2014
* Mod. Date: 		09/11/14
* Mod. Initials:	JM
* Author:    		Jim Merrell
* Purpose:   		This is the Render Class that renders the entire game
************************************************/

#include "CRenderer.h"
#include "../Globals/CGame.h"
#include "DDSTextureLoader.h"
#include "../Object Management/Entity3D.h"

#include "../Object Management/CTerrain.h"
#include "../Object Management/CPlayer.h"
#include "../../DirectXTK/Inc/SimpleMath.h"
#include "../Object Management/Entity3D.h"

#include "RenderContext.h"
#include "TransparentObj_RC.h"
#include "ScreenSpace_RC.h"
#include "OpaqueObj_RC.h"
#include "Füz_RC.h"
#include "Particle_RC.h"
#include "Player_RC.h"
#include "../VFX/Fuz_Effect.h"
#include "../VFX/VFXManager.h"



//*********************************************************
// Public Functions
//*********************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
void CRenderer::Initialize(HINSTANCE hinst, float BackBuffer_Width, float BackBuffer_Height, float Near_Z, float Far_Z,
						   CObjectManagerClient* objectManager, CGame* owner)
{
	m_fBackBufferWidth = BackBuffer_Width;
	m_fBackBufferHeight = BackBuffer_Height;
	m_fNearZ = Near_Z;
	m_fFarZ = Far_Z;
	m_bWindowed = true;
	m_bFullScreen = true;

	m_fDebugCamera_height = 0.0f;
	m_fDebugCamera_distance = 20.0f;

	m_pGameObjectManager = objectManager;
	m_pGameInstance = owner;

	m_nTesselationLevel = 4;
	m_fTestXPos = 0.0f;
	m_fTestZPos = 0.0f;

	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(231);

	Windows_Setup(hinst, (WNDPROC)WndProc, BackBuffer_Width, BackBuffer_Height);
	SwapChainAndDevice_Setup(BackBuffer_Width, BackBuffer_Height);
	BackBuffer_Setup();
	DepthBuffer_Setup(BackBuffer_Width, BackBuffer_Height);
	ViewPort_Setup(BackBuffer_Width, BackBuffer_Height);
	Scene_Setup(BackBuffer_Width, BackBuffer_Height, Near_Z, Far_Z);
	SamplerState_Setup();
	BlendState_Setup();
	DepthStencilView_Setup();
	GammaController_Setup();
	CreateFuzPool();

	LoadTextures();
	SetupTestSphere();

	m_BasicWhiteSRV = m_SRVs["white"];
	m_f4_currColor.colorRGBA = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	m_TransparentObj_RC = new TransparentObj_RC();
	m_TransparentObj_RC->Initialize(pDevice, pImmediateContext, &m_SRVs, &scene);

	m_ScreenSpace_RC = new ScreenSpace_RC();
	m_ScreenSpace_RC->Initialize(pDevice, pImmediateContext, &m_SRVs, &scene);

	m_OpaqueObj_RC = new OpaqueObj_RC();
	m_OpaqueObj_RC->Initialize(pDevice, pImmediateContext, &m_SRVs, &scene);

	m_FuzEffect_RC = new Fuz_RC();
	m_FuzEffect_RC->Initialize(pDevice, pImmediateContext, &m_SRVs, &scene);
	reinterpret_cast<Fuz_RC*>(m_FuzEffect_RC)->m_pRenderer = this;

	m_Particles_RC = new Particle_RC();
	m_Particles_RC->Initialize(pDevice, pImmediateContext, &m_SRVs, &scene);

	m_Player_RC = new Player_RC();
	m_Player_RC->Initialize(pDevice, pImmediateContext, &m_SRVs, &scene);

	m_bScreenSpace_RC =	true;
	m_bParticles_RC = true;
	m_bOpaque_RC = true;
	m_bTransparentObj_RC = true;
	m_bFuzEffect_RC = true;

	m_bThermalRadarEffect = false;

	m_bCharacterSelect = false;


	ZeroMemory(&m_CharacterSelectScene, sizeof(m_CharacterSelectScene));
	XMVECTOR eyePos = { 0.0f, 10.0f, -16.0f };
	XMVECTOR focusPos = { -1.5f, 3.0f, 0.0f };
	XMVECTOR upDir = { 0.0f, 1.0f, 0.0f };
	XMStoreFloat4x4(&m_CharacterSelectScene.viewMatrix, XMMatrixLookAtLH(eyePos, focusPos, upDir));
	XMStoreFloat4x4(&m_CharacterSelectScene.projectionMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(45), AspectRatio, 0.1f, 100.0f));


	D3D11_BUFFER_DESC scene_Buffer_Desc;
	ZeroMemory(&scene_Buffer_Desc, sizeof(scene_Buffer_Desc));
	scene_Buffer_Desc.ByteWidth = sizeof(SCENE);
	scene_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;
	scene_Buffer_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	scene_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA Scene_Data;
	Scene_Data.pSysMem = &m_CharacterSelectScene;
	Scene_Data.SysMemPitch = 0;
	Scene_Data.SysMemSlicePitch = 0;

	pDevice->CreateBuffer(&scene_Buffer_Desc, &Scene_Data, &m_CharacterSelect_constBuffer);
}

CRenderer::~CRenderer()
{
	ShutDown();
}

bool CRenderer::Run(float dt)
{
	if (this->m_pGameInstance->getPlayerIndex() != 4)
	{
		takeCameraInput();
	}

	updateAdvancedFuzPool(dt);

	RenderText(m_sDebugText, SCREEN_MIDDLE, 200, 200);

	if (GetAsyncKeyState('G')
		&& GetAsyncKeyState(VK_UP))
	{
		SetGammaLevel(m_fGamma_Level + 0.01f);
	}
	if (GetAsyncKeyState('G') 
		&& GetAsyncKeyState(VK_DOWN))
	{
		SetGammaLevel(m_fGamma_Level - 0.01f);
	}

	//stringstream testGamma;
	//string str;
	//testGamma << "Gamma Level: " << m_fGamma_Level;
	//str = testGamma.str();
	//RenderText(str);

	//RECTF testRectF;
	//testRectF.bottom = -0.5f;
	//testRectF.top = 0.5f;
	//testRectF.left = -0.1f;
	//testRectF.right = 0.1f;
	//
	//DrawMultiTexture(testRectF, "white", SCREEN_FRONT);


	ID3D11RenderTargetView* rtvs[1] = { pRTV };
	pImmediateContext->OMSetRenderTargets(1, rtvs, pDSV);
	UINT pNumViewports = 1;
	D3D11_VIEWPORT tempVP;
	ZeroMemory(&tempVP, sizeof(tempVP));

	tempVP.Height = m_fBackBufferHeight;
	tempVP.Width = m_fBackBufferWidth;
	tempVP.MinDepth = 0;
	tempVP.MaxDepth = 1;
	pImmediateContext->RSSetViewports(pNumViewports, &tempVP);
	pImmediateContext->ClearRenderTargetView(pRTV, Colors::Black);
	pImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1, 0);

	ID3D11SamplerState* sampleStates[1] = { pSamplerState };
	pImmediateContext->PSSetSamplers(0, 1, sampleStates);

	pImmediateContext->OMSetBlendState(pBlendState, NULL, UINT_MAX);
	pImmediateContext->OMSetDepthStencilState(pDSV_State_DepthEnabled, NULL);

	// map the scene
	{
		D3D11_MAPPED_SUBRESOURCE SceneMappedResource;
		ZeroMemory(&SceneMappedResource, sizeof(SceneMappedResource));
		ID3D11Buffer* scene_Buffer[1];

		if (!m_bCharacterSelect)
		{
			pImmediateContext->Map(scene_constBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &SceneMappedResource);
			memcpy(SceneMappedResource.pData, &scene, sizeof(scene));
			pImmediateContext->Unmap(scene_constBuffer, 0);
			scene_Buffer[0] = scene_constBuffer;
		}
		else
		{
			pImmediateContext->Map(m_CharacterSelect_constBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &SceneMappedResource);
			memcpy(SceneMappedResource.pData, &m_CharacterSelectScene, sizeof(m_CharacterSelectScene));
			pImmediateContext->Unmap(m_CharacterSelect_constBuffer, 0);
			scene_Buffer[0] = m_CharacterSelect_constBuffer;
		}

		pImmediateContext->VSSetConstantBuffers(1, 1, scene_Buffer);
		pImmediateContext->HSSetConstantBuffers( 1, 1, scene_Buffer );
		pImmediateContext->DSSetConstantBuffers( 1, 1, scene_Buffer );
		pImmediateContext->GSSetConstantBuffers(1, 1, scene_Buffer);
	}
	// map the fuz_Pool
	{
		D3D11_MAPPED_SUBRESOURCE fuzMappedResource;
		ZeroMemory(&fuzMappedResource, sizeof(fuzMappedResource));
		pImmediateContext->Map(fuz_ConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &fuzMappedResource);

		memcpy(fuzMappedResource.pData, &fuz_Pool, sizeof(fuz_Pool));

		pImmediateContext->Unmap(fuz_ConstBuffer, 0);

		ID3D11Buffer* fuz_Buffer[1] = { fuz_ConstBuffer };
		pImmediateContext->HSSetConstantBuffers(3, 1, fuz_Buffer);
		pImmediateContext->GSSetConstantBuffers(3, 1, fuz_Buffer); 
	}

	//if(TESSELATION)
	//{
	//	m_OpaqueObj_RC->m_nTesselationLevel = m_nTesselationLevel;
	//	m_TransparentObj_RC->m_nTesselationLevel = m_nTesselationLevel;
	//	m_OpaqueObj_RC->m_fTestXPos = m_fTestXPos;
	//	m_TransparentObj_RC->m_fTestXPos = m_fTestXPos;	
	//	m_OpaqueObj_RC->m_fTestZPos = m_fTestZPos;
	//	m_TransparentObj_RC->m_fTestZPos = m_fTestZPos;	
	//}


	if (!m_bCharacterSelect)
	{
		if (m_bOpaque_RC)
			m_OpaqueObj_RC->Run();
		if (m_bFuzEffect_RC)
			m_FuzEffect_RC->Run();
		if (m_bTransparentObj_RC)
		{
			m_TransparentObj_RC->Run();

			if (m_bThermalRadarEffect)
				pImmediateContext->OMSetDepthStencilState(pDSV_State_DepthCheckDisabled, NULL);

			m_Player_RC->Run();
		}

		if (m_bParticles_RC)
		{
			pImmediateContext->OMSetDepthStencilState(pDSV_State_DepthDisabled, NULL);
			m_Particles_RC->Run();
			pImmediateContext->OMSetDepthStencilState(pDSV_State_DepthEnabled, NULL);
		}
	}

	if(m_bScreenSpace_RC)
	{
		m_ScreenSpace_RC->Run();
	}

	if(m_bCharacterSelect)
	{
		pImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1, 0);
		pImmediateContext->OMSetDepthStencilState(pDSV_State_DepthEnabled, NULL);
		m_Player_RC->Run();
		m_OpaqueObj_RC->Run();
		m_TransparentObj_RC->Run();

		m_pGameInstance->getStateManager()->GetTop()->GetHud()->m_bDrawCursor = true;
		m_pGameInstance->getStateManager()->GetTop()->GetHud()->DrawHud_Cursor(m_pGameInstance);
		m_pGameInstance->getStateManager()->GetTop()->GetHud()->DrawHud_Retcile(m_pGameInstance);
		m_ScreenSpace_RC->Run();
		m_pGameInstance->getStateManager()->GetTop()->GetHud()->m_bDrawCursor = false;
	}

	pSwapChain->Present(1, 0);

	BOOL bFullscreen = false;
	pSwapChain->GetFullscreenState(&bFullscreen, NULL); 
	if(bFullscreen)
		m_bFullScreen = true;
	else
		m_bFullScreen = false;
	//string fullscreenStatus = "";
	//if(bFullscreen)
	//	fullscreenStatus = "swapchain in fullscreen";
	//else
	//	fullscreenStatus = "swapchain NOT in fullscreen";

	//SetDebugText(fullscreenStatus);

	return true;
}

void CRenderer::ShutDown()
{
	delete m_TransparentObj_RC;
	delete m_ScreenSpace_RC;
	delete m_OpaqueObj_RC;
	delete m_FuzEffect_RC;
	delete m_Particles_RC;	
	delete m_Player_RC;

	m_pGameObjectManager = nullptr;
	if (pImmediateContext)
	{
		pImmediateContext->ClearState();
	}

	UnregisterClass(L"DirectXApplication", application);
}

void CRenderer::AddRenderObject(Entity3D* obj)
{
	Renderable* tempRenderObject = new Renderable();

	// add the object to the array
	tempRenderObject->object = obj;

	// build and add a vertex buffer for the object
	CComPtr<ID3D11Buffer> tempVertexBuffer;

	D3D11_BUFFER_DESC Vertex_Buffer_Desc;
	ZeroMemory(&Vertex_Buffer_Desc, sizeof(Vertex_Buffer_Desc));
	Vertex_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;
	Vertex_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	Vertex_Buffer_Desc.CPUAccessFlags = NULL;
	Vertex_Buffer_Desc.ByteWidth = sizeof(VERTEX_3D) * obj->getRenderVerts().size();

	D3D11_SUBRESOURCE_DATA objData;
	ZeroMemory(&objData, sizeof(objData));
	objData.pSysMem = &obj->getRenderVerts()[0];
	pDevice->CreateBuffer(&Vertex_Buffer_Desc, &objData, &tempVertexBuffer);

	tempRenderObject->vertexBuffer = tempVertexBuffer;

	// build and add the index buffer
	CComPtr<ID3D11Buffer> tempIndexBuffer;

	D3D11_BUFFER_DESC Index_Buffer_Desc;
	ZeroMemory(&Index_Buffer_Desc, sizeof(Index_Buffer_Desc));
	Index_Buffer_Desc.Usage = D3D11_USAGE_IMMUTABLE;
	Index_Buffer_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	Index_Buffer_Desc.CPUAccessFlags = NULL;
	Index_Buffer_Desc.ByteWidth = sizeof(unsigned short)* obj->getRenderIndex().size();

	D3D11_SUBRESOURCE_DATA IndexData;
	ZeroMemory(&IndexData, sizeof(IndexData));
	IndexData.pSysMem = &obj->getRenderIndex()[0];
	pDevice->CreateBuffer(&Index_Buffer_Desc, &IndexData, &tempIndexBuffer);

	tempRenderObject->indexBuffer = tempIndexBuffer;

	// build and add the local const buffer for the matrix
	D3D11_BUFFER_DESC obj_Buffer_Desc;
	ZeroMemory(&obj_Buffer_Desc, sizeof(obj_Buffer_Desc));
	obj_Buffer_Desc.ByteWidth = sizeof(XMMATRIX);
	obj_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;
	obj_Buffer_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	obj_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA Object_Data;
	Object_Data.pSysMem = &obj->getWorldMatrix();
	Object_Data.SysMemPitch = 0;
	Object_Data.SysMemSlicePitch = 0;

	CComPtr<ID3D11Buffer> tempMatrixBuffer;
	pDevice->CreateBuffer(&obj_Buffer_Desc, &Object_Data, &tempMatrixBuffer);

	tempRenderObject->matrixBuffer = tempMatrixBuffer;

	// build and add the const buffer for the Fuz Collision Points
	D3D11_BUFFER_DESC Fuz_Buffer_Desc;
	ZeroMemory(&Fuz_Buffer_Desc, sizeof(Fuz_Buffer_Desc));
	Fuz_Buffer_Desc.ByteWidth = sizeof(FUZ_COLLISION_DATA);
	Fuz_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;
	Fuz_Buffer_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Fuz_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA Fuz_Data;
	Fuz_Data.pSysMem = &obj->getFuzData();
	Fuz_Data.SysMemPitch = 0;
	Fuz_Data.SysMemSlicePitch = 0;

	CComPtr<ID3D11Buffer> tempFuzBuffer;
	pDevice->CreateBuffer(&Fuz_Buffer_Desc, &Fuz_Data, &tempFuzBuffer);

	tempRenderObject->fuzDataBuffer = tempFuzBuffer;

	// Set up const buffer for the scrolling uv 
	D3D11_BUFFER_DESC uv_Buffer_Desc;
	ZeroMemory(&uv_Buffer_Desc, sizeof(uv_Buffer_Desc));
	uv_Buffer_Desc.ByteWidth = sizeof(XMFLOAT4);
	uv_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;
	uv_Buffer_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	uv_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA uv_Data;
	uv_Data.pSysMem = &obj->GetUVOffsets();
	uv_Data.SysMemPitch = 0;
	uv_Data.SysMemSlicePitch = 0;

	CComPtr<ID3D11Buffer> tempUVBuffer;
	pDevice->CreateBuffer(&uv_Buffer_Desc, &uv_Data, &tempUVBuffer);

	tempRenderObject->scrollingUVBuffer = tempUVBuffer;

	if(tempRenderObject->object->getObjectType() == Entity3D::PLAYER)
		m_Player_RC->AddRenderObject(tempRenderObject);
	else if(tempRenderObject->object->isTransparent())
		m_TransparentObj_RC->AddRenderObject(tempRenderObject);
	else
		m_OpaqueObj_RC->AddRenderObject(tempRenderObject);

} 

void CRenderer::RemoveRenderObject(Entity3D* obj)
{
	if(obj != nullptr)
	{
		if(obj->getObjectType() == Entity3D::PLAYER)
			m_Player_RC->RemoveRenderObject(obj);
		else if (obj->isTransparent())
			m_TransparentObj_RC->RemoveRenderObject(obj);
		else
			m_OpaqueObj_RC->RemoveRenderObject(obj);
	}
}

void CRenderer::SetCameraObject(Entity3D* camera)
{
	CCamera* pCurrentCamera = this->m_pGameObjectManager->getPlayer(this->m_pGameInstance->getPlayerIndex())->getPlayerCam();
	XMStoreFloat4x4(&scene.viewMatrix, pCurrentCamera->getViewMat());
	XMStoreFloat4x4(&scene.projectionMatrix, pCurrentCamera->getProjMat());

	pCurrentCamera = nullptr;
}

bool CRenderer::Resize(int nWidth, int nHeight, bool bWindowed)
{
	m_bWindowed = bWindowed;
	int viewport_Width = nWidth;
	int viewport_Height = nHeight;
	float newAspectRatio = (float)nWidth / (float)nHeight;
	if (newAspectRatio < 1.32 || newAspectRatio > 1.34)
		viewport_Width = (int)((float)nHeight + ((float)nHeight * 0.333f));

	// only allow this operation if required objects are present
	if (!pDevice || !pSwapChain || !pImmediateContext) return false;

	// make sure our device context is not connected to the swapChain
	pImmediateContext->ClearState();

	// release the current RT view if it exists and resize the buffers to match the new window
	pRTV.Release();

	// resize our swapChain to the requested size ( 0,0 means figure it out yourself )
	static bool initial = true; // remove this when they ($MS) fix the bug
	if (!initial) // we don't resize the buffers the first time around because it breaks the Graphics Debugger
		pSwapChain->ResizeBuffers(0, nWidth, nHeight, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	initial = false; // should always be called from here on out

	//************ SET THE SWAPCHAIN BACKBUFFER AS OUR RENDERTARGET ************/

	CComPtr<ID3D11Texture2D> pBackBuffer; // temp handle to our backbuffer
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	// Create a render-target view using our device
	pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRTV);

	//************ DEFINE THE VIEWPORT COVERING THE ENTIRE RENDERTARGET ********/

	DXGI_SWAP_CHAIN_DESC curr_size;
	pSwapChain->GetDesc(&curr_size);
	// Setup the viewport covering the entire rendertarget
	ViewPort.Width = float(viewport_Width/*curr_size.BufferDesc.Width*/);
	ViewPort.Height = float(viewport_Height/*curr_size.BufferDesc.Height*/);
	ViewPort.MinDepth = FLT_EPSILON; // first part of the Z buffer is reserved for the weapon
	ViewPort.MaxDepth = 1.0f;
	ViewPort.TopLeftX = 30;
	ViewPort.TopLeftY = 0;
	pImmediateContext->RSSetViewports(1, &ViewPort);

	// re-build projection matrix (by placing this here our aspect ratio will always be correct)
	float oldAspectRatio = ViewPort.Width / ViewPort.Height;
	XMStoreFloat4x4(&scene.projectionMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(65), oldAspectRatio, m_fNearZ, m_fFarZ));

	if (m_pGameObjectManager != nullptr)
	{
		CPlayer* myPlayer = m_pGameObjectManager->getPlayer(m_pGameObjectManager->getMyPlayerIndex());

		if (myPlayer != nullptr)
			myPlayer->getPlayerCam()->InitializeCamera(myPlayer, (float)viewport_Width, (float)nHeight, m_fNearZ, m_fFarZ, 65.0f);
	}

	//************ CREATE A 2D DEPTH TEXTURE TO MATCH OUR BACKBUFFER FOR 3D RENDERING ************/

	// delete the previous depth texture and render target view
	pDSV.Release();
	pZBuffer.Release();
	// create the new depth texture
	D3D11_TEXTURE2D_DESC zbuff;
	ZeroMemory(&zbuff, sizeof(D3D11_TEXTURE2D_DESC));
	zbuff.ArraySize = 1;
	zbuff.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	zbuff.CPUAccessFlags = 0;
	zbuff.Format = DXGI_FORMAT_D32_FLOAT; // a format with stencil is also available
	zbuff.Width = nWidth/*curr_size.BufferDesc.Width*/;
	zbuff.Height = nHeight/*curr_size.BufferDesc.Height*/;
	zbuff.MipLevels = 1;
	zbuff.MiscFlags = 0;
	zbuff.Usage = D3D11_USAGE_DEFAULT;
	zbuff.SampleDesc = curr_size.SampleDesc; // copy MSAA settings from BackBuffer
	// create the 2D texture
	pDevice->CreateTexture2D(&zbuff, 0, &pZBuffer);
	// bind the new depth texture to the rendering pipeline
	pDevice->CreateDepthStencilView(pZBuffer, 0, &pDSV);

	//************ BIND THE BACKBUFFER & THE DEPTH TEXTURE TO THE PIPELINE ************/

	// Bind the views to the immediate context (use .p when you want to interpret a com ptr as an array)
	pImmediateContext->OMSetRenderTargets(1, &pRTV.p, pDSV);

	pSwapChain->SetFullscreenState(bWindowed, NULL);

	// all done here
	return true;
}

void CRenderer::RenderText(string text, int layer, int posX, int posY, XMFLOAT4 colorRGBA, float scale, float rotation)
{
	RENDER_TEXT renderText; //	= new RENDER_TEXT();
	renderText.text = text;
	XMFLOAT2 position = XMFLOAT2((float)posX, (float)posY);
	renderText.position = position;
	XMFLOAT4 fontColor = XMFLOAT4(colorRGBA.x, colorRGBA.y, colorRGBA.z, colorRGBA.w);
	renderText.color = fontColor;
	XMFLOAT2 origin = XMFLOAT2(0.0f, 0.0f);
	renderText.origin = origin;
	renderText.scale = scale;
	renderText.rotation = XMConvertToRadians(rotation); //0.0f;

	reinterpret_cast<ScreenSpace_RC*>(m_ScreenSpace_RC)->AddRenderText(renderText, layer);

}

bool CRenderer::LoadAddTexture(const char* DDSfilepath, const char* textureName)
{
	CComPtr<ID3D11ShaderResourceView> tempSRV;
	string key = textureName;
	string filename = DDSfilepath;
	wstring ImgFlName(filename.begin(), filename.end());
	ID3D11Resource* resource[1] = { pResource };
	CreateDDSTextureFromFile(pDevice, ImgFlName.c_str(), resource, &tempSRV);
	m_SRVs.emplace(key, tempSRV);
	if (resource[0] != nullptr)
		resource[0]->Release();
	return true;
}

void CRenderer::DrawTexture(RECTF rect, const char* textureName, int layer, float R, float G, float B, float A, int useGradient_PS, float fGradient)
{
	XMFLOAT4 colorRGBA = XMFLOAT4(R, G, B, A);
	ScreenSpaceObject SSobj;
	CreateQuad_2D(rect, SSobj, colorRGBA, fGradient);

	SSobj.textureName = textureName;
	SSobj.secondTexture = "";
	SSobj.thirdTexture = "";
	SSobj.useGradient_PS = useGradient_PS;

	reinterpret_cast<ScreenSpace_RC*>(m_ScreenSpace_RC)->AddRenderRECT(SSobj, layer);
}

void CRenderer::DrawMultiTexture(RECTF rect, const char* textureName, int layer, XMFLOAT4 color_RGBA, float fGradient, const char* secondTextureName, const char* thirdTextureName)
{
	XMFLOAT4 colorRGBA = color_RGBA;
	ScreenSpaceObject SSobj;
	CreateQuad_2D(rect, SSobj, colorRGBA, fGradient);

	SSobj.textureName = textureName;
	SSobj.secondTexture = secondTextureName;
	SSobj.thirdTexture = secondTextureName;

	SSobj.useGradient_PS = 2;

	reinterpret_cast<ScreenSpace_RC*>(m_ScreenSpace_RC)->AddRenderRECT(SSobj, layer);
}

void CRenderer::DrawTexture(const char* textureName,  XMFLOAT2 position, XMFLOAT4 colorARGB, XMFLOAT2 sourceOrigin, float rotationDegrees, float scale, int layer)
{
	RENDER_TEXTURE RT;
	RT.textureName = textureName;
	RT.position = position;
	RT.color = colorARGB;
	RT.origin = sourceOrigin;
	RT.rotation = rotationDegrees;
	RT.scale = scale;

	reinterpret_cast<ScreenSpace_RC*>(m_ScreenSpace_RC)->AddRenderTexture(RT, layer);
}

void CRenderer::DrawRect(RECTF rect, int layer, XMFLOAT3 colorRGB, float alpha)
{
	XMFLOAT4 colorRGBA = XMFLOAT4(colorRGB.x, colorRGB.y, colorRGB.z, alpha);
	ScreenSpaceObject SSobj;
	CreateQuad_2D(rect, SSobj, colorRGBA);

	reinterpret_cast<ScreenSpace_RC*>(m_ScreenSpace_RC)->AddRenderRECT(SSobj, layer);
}

void CRenderer::DrawRect(RECTF rect, int layer, float R, float G, float B, float A)
{
	XMFLOAT4 colorRGBA = XMFLOAT4(R, G, B, A);
	ScreenSpaceObject SSobj;
	CreateQuad_2D(rect, SSobj, colorRGBA);

	reinterpret_cast<ScreenSpace_RC*>(m_ScreenSpace_RC)->AddRenderRECT(SSobj, layer);
}

void CRenderer::Windows_Setup(HINSTANCE hinst, WNDPROC proc, float BackBuffer_Width, float BackBuffer_Height)
{
	application = hinst;
	appWndProc = proc;

	WNDCLASSEX  wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = appWndProc;
	wndClass.lpszClassName = L"DirectXApplication";
	wndClass.hInstance = application;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
	//wndClass.hIcon			= LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FSICON));
	RegisterClassEx(&wndClass);

	RECT window_size = { (LONG)0, (LONG)0, (LONG)BackBuffer_Width, (LONG)BackBuffer_Height };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, false);

	window = CreateWindow(L"DirectXApplication", L"Füz", WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME/* | WS_MAXIMIZEBOX*/),
		CW_USEDEFAULT, CW_USEDEFAULT, window_size.right - window_size.left, window_size.bottom - window_size.top,
		NULL, NULL, application, NULL);

	ShowWindow(window, SW_SHOW);
}

void CRenderer::SwapChainAndDevice_Setup(float BackBuffer_Width, float BackBuffer_Height)
{
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.BufferDesc.Width = (UINT)BackBuffer_Width;
	SwapChainDesc.BufferDesc.Height = (UINT)BackBuffer_Height;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.OutputWindow = window;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
#if _DEBUG
	SwapChainDesc.Windowed = TRUE;
#else
	SwapChainDesc.Windowed = FALSE;
#endif

	//SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	UINT createDeviceFlags = 0;

	//#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	//#endif

	D3D_FEATURE_LEVEL	FeatureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
	UINT				numLevelRequested = 1;
	D3D_FEATURE_LEVEL	FeatureLevelsSupported;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		createDeviceFlags, NULL, NULL,
		D3D11_SDK_VERSION, &SwapChainDesc,
		&pSwapChain, &pDevice, &FeatureLevelsSupported, &pImmediateContext);

	//**********************************************************************************************************************
	//// The below code is to disable alt-enter /////////////////////////////////

	//IDXGIDevice * pDXGIDevice;
	//hr = pDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
	//
	//IDXGIAdapter * pDXGIAdapter;
	//hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);
	//
	//IDXGIFactory * pIDXGIFactory;
	//pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pIDXGIFactory);

	//pIDXGIFactory->MakeWindowAssociation(window, DXGI_MWA_NO_ALT_ENTER);

	//**********************************************************************************************************************
}

void CRenderer::BackBuffer_Setup()
{
	CComPtr<ID3D11Texture2D> pBackBuffer = 0;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(void**)&pBackBuffer);
	pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRTV);
}

void CRenderer::DepthBuffer_Setup(float BackBuffer_Width, float BackBuffer_Height)
{
	D3D11_TEXTURE2D_DESC ZBufferDesc;
	ZeroMemory(&ZBufferDesc, sizeof(ZBufferDesc));
	ZBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	ZBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	ZBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	ZBufferDesc.Width = (UINT)BackBuffer_Width;
	ZBufferDesc.Height = (UINT)BackBuffer_Height;
	ZBufferDesc.ArraySize = 1;
	ZBufferDesc.MipLevels = 1;
	ZBufferDesc.SampleDesc.Count = 1;

	pDevice->CreateTexture2D(&ZBufferDesc, NULL, &pZBuffer);

	pDevice->CreateDepthStencilView(pZBuffer, NULL, &pDSV);
}

void CRenderer::ViewPort_Setup(float BackBuffer_Width, float BackBuffer_Height)
{
	ViewPort.Height = BackBuffer_Height;
	ViewPort.MaxDepth = 1;
	ViewPort.MinDepth = 0;
	ViewPort.TopLeftX = 0;
	ViewPort.TopLeftY = 0;
	ViewPort.Width = BackBuffer_Width;
}

void CRenderer::Scene_Setup(float BackBuffer_Width, float BackBuffer_Height, float Near_Z, float Far_Z)
{
	AspectRatio = BackBuffer_Width / BackBuffer_Height;

	ZeroMemory(&scene, sizeof(scene));

	if (this->m_pGameObjectManager->getPlayer(this->m_pGameInstance->getPlayerIndex()) != nullptr)
	{
		CCamera* pCurrentCamera = this->m_pGameObjectManager->getPlayer(this->m_pGameInstance->getPlayerIndex())->getPlayerCam();
		XMStoreFloat4x4(&scene.viewMatrix, pCurrentCamera->getViewMat());
		XMStoreFloat4x4(&scene.projectionMatrix, pCurrentCamera->getProjMat());

		pCurrentCamera = nullptr;
	}

	D3D11_BUFFER_DESC scene_Buffer_Desc;
	ZeroMemory(&scene_Buffer_Desc, sizeof(scene_Buffer_Desc));
	scene_Buffer_Desc.ByteWidth = sizeof(SCENE);
	scene_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;
	scene_Buffer_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	scene_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA Scene_Data;
	Scene_Data.pSysMem = &scene;
	Scene_Data.SysMemPitch = 0;
	Scene_Data.SysMemSlicePitch = 0;

	pDevice->CreateBuffer(&scene_Buffer_Desc, &Scene_Data, &scene_constBuffer);


	// setupUp a scene for using as the debug camera
	//ZeroMemory(&debugCamera_scene, sizeof(debugCamera_scene));
	//XMVECTOR eyePos = { -40, 75, -25 };
	//XMVECTOR focusPos = { 0, 30, 0 };
	//XMVECTOR upDir = { 0, 1, 0 };
	//XMStoreFloat4x4(&debugCamera_scene.viewMatrix, XMMatrixLookAtLH(eyePos, focusPos, upDir));
	//
	//XMStoreFloat4x4(&debugCamera_scene.projectionMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(65), AspectRatio, 0.1f, 100.0f));
}

void CRenderer::CreateFuzPool()
{
	for(int i = 0; i < MAX_FUZ_COUNT; i++)
	{
		float posX = (float)(rand() % 300 - 150);
		float posZ = (float)(rand() % 300 - 150);
		fuz_Pool.fuzzes[i].worldPos = XMFLOAT4(posX, 0, posZ, 1);

		float r = (float)(rand() % 100) * 0.01f;
		float g = (float)(rand() % 100) * 0.01f;
		float b = (float)(rand() % 100) * 0.01f;
		fuz_Pool.fuzzes[i].color = XMFLOAT4(r, g, b, 1);		
		fuz_Pool.fuzzes[i].r_l_p_p = XMFLOAT4(5, 10, 0, 0);
	}

	D3D11_BUFFER_DESC fuz_bufferDesc;
	ZeroMemory(&fuz_bufferDesc, sizeof(fuz_bufferDesc));
	fuz_bufferDesc.ByteWidth = sizeof(FUZ_POOL);
	fuz_bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	fuz_bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	fuz_bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA fuz_data;
	fuz_data.pSysMem = &fuz_Pool;
	fuz_data.SysMemPitch = 0;
	fuz_data.SysMemSlicePitch = 0;

	pDevice->CreateBuffer(&fuz_bufferDesc, &fuz_data, &fuz_ConstBuffer);
}

void CRenderer::takeCameraInput()
{
	CPlayer* currentPlayer = this->m_pGameObjectManager->getPlayer(this->m_pGameInstance->getPlayerIndex());

	if (currentPlayer == nullptr)
		return;

	CCamera* currentCam = currentPlayer->getPlayerCam();

	XMVECTOR CameraPos = currentCam->getWorldMatrix().r[3];
	XMMATRIX translation = XMMatrixTranslationFromVector(CameraPos);
	XMMATRIX newMatrix = XMMatrixIdentity();
	newMatrix = translation * newMatrix;
	//	XMStoreFloat4x4(&SkyBox_LocalSpace.worldMatrix, newMatrix);
#if _DEBUG
	if (GetAsyncKeyState('P') & 0x1)
	{
		bDebugCamera = !bDebugCamera;
	}
	if (GetAsyncKeyState('O') & 0x1)
	{
		cout << (int)currentPlayer->getPosition().y << endl;
	}

	if (bDebugCamera)
	{
		if (GetAsyncKeyState(VK_UP))
		{
			m_fDebugCamera_height -= 0.1f;
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			m_fDebugCamera_height += 0.1f;
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			m_fDebugCamera_distance += 0.1f;
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			m_fDebugCamera_distance -= 0.1f;
		}

		XMMATRIX cameraView = currentCam->getViewMat();

		XMFLOAT3 f3_DebugOffset = XMFLOAT3(0.0f, m_fDebugCamera_height, m_fDebugCamera_distance);
		XMVECTOR Vec_DebugOffset = XMLoadFloat3(&f3_DebugOffset);
		XMMATRIX debugTranslate = XMMatrixTranslationFromVector(Vec_DebugOffset);
		newMatrix = cameraView * debugTranslate;

		XMStoreFloat4x4(&scene.viewMatrix, newMatrix);
		XMStoreFloat4x4(&scene.projectionMatrix, currentCam->getProjMat());
		//scene = debugCamera_scene;
		return;
	}
#endif


	//if (GetAsyncKeyState('1') & 0x1)
	//{
	//	m_bScreenSpace_RC =	!m_bScreenSpace_RC;
	//}
	//if (GetAsyncKeyState('2') & 0x1)
	//{
	//	m_bParticles_RC = !m_bParticles_RC;
	//}
	//if (GetAsyncKeyState('3') & 0x1)
	//{
	//	m_bOpaque_RC = !m_bOpaque_RC;
	//}
	//if (GetAsyncKeyState('4') & 0x1)
	//{
	//	m_bTransparentObj_RC = !m_bTransparentObj_RC;
	//}
	//if (GetAsyncKeyState('5') & 0x1)
	//{
	//	m_bFuzEffect_RC = !m_bFuzEffect_RC;
	//}
	//if (GetAsyncKeyState('J') & 0x1 && GetAsyncKeyState('I') & 0x1 && GetAsyncKeyState('M') & 0x1)
	//{
	//	ToggleThermalRadarEffect();
	//}

	if(TESSELATION)
	{
		//if (GetAsyncKeyState('L') & 0x1)
		//{
		//	m_nTesselationLevel++;
		//}
		//if (GetAsyncKeyState('K') & 0x1)
		//{
		//	m_nTesselationLevel--;
		//}
		//if(m_nTesselationLevel > 32)
		//	m_nTesselationLevel = 32;
		//else if(m_nTesselationLevel < 1)
		//	m_nTesselationLevel = 1;
		//
		//if (GetAsyncKeyState(VK_NUMPAD4) & 0x1)
		//{
		//	fuz_Pool.fuzzes[0].worldPos.x -= 1.0f;
		//}
		//if (GetAsyncKeyState(VK_NUMPAD6) & 0x1)
		//{
		//	fuz_Pool.fuzzes[0].worldPos.x += 1.0f;
		//}
		//if (GetAsyncKeyState(VK_NUMPAD2) & 0x1)
		//{
		//	fuz_Pool.fuzzes[0].worldPos.z -= 1.0f;
		//}
		//if (GetAsyncKeyState(VK_NUMPAD8) & 0x1)
		//{
		//	fuz_Pool.fuzzes[0].worldPos.z += 1.0f;
		//}



		//auto state = gamePad->GetState(0);
		//
		//if( state.IsConnected())
		//{
		//	if ( state.IsAPressed() ) // Do action for button A being down
		//	{
		//
		//	}				
		//
		//	if ( state.buttons.y )// Do action for button Y being down
		//	{
		//
		//	}
		//
		//	if ( state.IsDPadLeftPressed() )// Do action for DPAD Left being 
		//	{
		//		fuz_Pool.fuzzes[0].worldPos.x -= 1.0f;
		//	}
		//
		//	if ( state.dpad.up || state.dpad.down || state.dpad.left || state.dpad.right )	// Do action based on any DPAD change
		//	{
		//
		//	}
		//
		//	float posx = state.thumbSticks.leftX;
		//
		//	float posy = state.thumbSticks.leftY;
		//	// These values are normalized to -1 to 1
		//
		//	float throttle = state.triggers.right;
		//	// This value is normalized 0 -> 1
		//
		//	if ( state.IsLeftTriggerPressed() )	// Do action based on a left trigger pressed more than 
		//	{
		//
		//	}
		//
		//	//if ( state.IsViewPressed() )
		//	// This is an alias for the Xbox 360 'Back' button
		//	// which is called 'View' on the Xbox One controller.
		//}


	}


	XMStoreFloat4x4(&scene.viewMatrix, currentCam->getViewMat());
	XMStoreFloat4x4(&scene.projectionMatrix, currentCam->getProjMat());




}

void CRenderer::LoadTextures()
{
	//////////////////////////////
	///MENUS
	//Character Customization
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Alert_Window.dds", "Menu_Alert_Window");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_Color_BGWindow_Light.dds", "Menu_Custom_Color_BGWindow");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_Color_HoverOver.dds", "Menu_Custom_Color_HoverOver");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_Color_Selectable.dds", "Menu_Custom_Color_Selectable");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_Color_Unselectable.dds", "Menu_Custom_Color_Unselectable");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_Color_Border.dds", "Menu_Custom_Color_Border");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_Color_RandomIcon.dds", "Menu_Custom_Color_RandomIcon");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_Model_BGWindow_Color.dds", "Menu_Custom_Model_BGWindow");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_Model_HoverOver.dds", "Menu_Custom_Model_HoverOver");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_Model_PreviewWindow.dds", "Menu_Custom_Model_PreviewWindow");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_Model_Preview_TempBunny.dds", "Menu_Custom_Model_Preview_TempBunny");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_Model_Selectable_Vert.dds", "Menu_Custom_Model_Selectable");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_Model_Unselectable.dds", "Menu_Custom_Model_Unselectable");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/BunnySelect_Cartoon_White.dds", "Menu_Custom_Model_BunnyCartoon");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/BunnySelect_LongEars_White.dds", "Menu_Custom_Model_BunnyLongEars");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/BunnySelect_Normal_White.dds", "Menu_Custom_Model_BunnyNormal");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/BunnySelect_Cute_White.dds", "Menu_Custom_Model_BunnyCute");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_NameEntryBox_Color.dds", "Menu_Custom_Name_EntryBox");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_Name_Cancel.dds", "Menu_Custom_Name_Cancel");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_Name_Confirm.dds", "Menu_Custom_Name_Confirm");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_RotateArrow_Left.dds", "Menu_Custom_RotateArrow_Left");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_RotateArrow_Right.dds", "Menu_Custom_RotateArrow_Right");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_WindowBG.dds", "Menu_Custom_WindowBG");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Customize_Icon_New.dds", "Menu_Custom_Icon_Settings");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_P1.dds", "Menu_Custom_P1");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_P2.dds", "Menu_Custom_P2");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_P3.dds", "Menu_Custom_P3");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Custom_P4.dds", "Menu_Custom_P4");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Name_Warning_CapitalLimits.dds", "Menu_Name_Warning_CapitalLimits");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Name_Warning_Maximum.dds", "Menu_Name_Warning_Maximum");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Name_Warning_Minimum.dds", "Menu_Name_Warning_Minimum");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/Menu_Name_Warning_NoSpecialChars.dds", "Menu_Name_Warning_NoSpecialChars");
	///LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/CharacterCustomization/.dds", ""); <---Template

	//Menu_Pieces
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Menu_Pieces/Menu_Pause_BG.dds", "Menu_Pause_BG");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Menu_Icons/Menu_Help_Button_Down.dds", "Menu_Help_Button_Down");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Menu_Icons/Menu_Help_Button_Up.dds", "Menu_Help_Button_Up");
	///LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Menu_Pieces/.dds", ""); <---Template


	//Lobby
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Lobby/Lobby_PlayerInfo_BG.dds", "Lobby_PlayerInfo_BG");
	//LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Lobby/.dds", "");

	//////////////////////////////
	///HUD
	//AlertsMessages
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/AlertsMessages/HUD_Killfeed_BG.dds", "HUD_Killfeed_BG");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/AlertsMessages/HUD_Message_Assist.dds", "HUD_Message_Assist");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/AlertsMessages/HUD_Message_Dominating.dds", "HUD_Message_Dominating");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/AlertsMessages/HUD_Message_FuzFromGrave.dds", "HUD_Message_FuzFromGrave");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/AlertsMessages/HUD_Message_Revenge.dds", "HUD_Message_Revenge");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/AlertsMessages/HUD_Message_Fuzguardian.dds", "HUD_Message_Fuzguardian");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/AlertsMessages/HUD_Message_FuzzingSpree.dds", "HUD_Message_FuzzingSpree");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/AlertsMessages/HUD_Message_Legendary.dds", "HUD_Message_Legendary");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/AlertsMessages/HUD_Message_Survivor.dds", "HUD_Message_Survivor");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/AlertsMessages/HUD_Message_Unstoppable.dds", "HUD_Message_Unstoppable");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/AlertsMessages/HUD_Message_Comeback.dds", "HUD_Message_Comeback");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/AlertsMessages/HUD_Message_FirstFuz.dds", "HUD_Message_FirstFuz");
	///LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/AlertsMessages/.dds", ""); <---Template

	//Health_Sprint
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Health_Sprint/HUD_Health_Fill_GrayScale.dds", "HUD_Health_Fill_GrayScale");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Health_Sprint/HUD_Health_Fill_Normal.dds", "HUD_Health_Fill_Normal");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Health_Sprint/HUD_Health_Frame.dds", "HUD_Health_Frame");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Health_Sprint/HUD_Health_Gradient.dds", "HUD_Health_Gradient");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Health_Sprint/HUD_Sprint_Fill_Grayscale.dds", "HUD_Sprint_Fill_Grayscale");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Health_Sprint/HUD_Sprint_Fill_Normal.dds", "HUD_Sprint_Fill_Normal");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Health_Sprint/HUD_Sprint_Gradient.dds", "HUD_Sprint_Gradient");
	///LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Health_Sprint/.dds", ""); <---Template

	//Match_Countdown
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Match_Countdown/Match_Start_1.dds", "Match_Start_1");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Match_Countdown/Match_Start_2.dds", "Match_Start_2");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Match_Countdown/Match_Start_3.dds", "Match_Start_3");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Match_Countdown/Match_Start_BG.dds", "Match_Start_BG");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Match_Countdown/Match_Start_ClockHand.dds", "Match_Start_ClockHand");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Match_Countdown/Match_Start_Fuz.dds", "Match_Start_Fuz");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Match_Countdown/Match_Start_GlowRing.dds", "Match_Start_GlowRing");
	///LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Match_Countdown/.dds", ""); <---Template

	//Overheat_PwrUp
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Overheat_PwrUp/HUD_Overheat_Circular_BG.dds", "HUD_Overheat_Circular_BG");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Overheat_PwrUp/HUD_Overheat_Circular_Gauge.dds", "HUD_Overheat_Circular_Gauge");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Overheat_PwrUp/HUD_Overheat_Circular_Gradient.dds", "HUD_Overheat_Circular_Gradient");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Overheat_PwrUp/HUD_PwrUp_Icon_DoubleDamage.dds", "HUD_PwrUp_Icon_DoubleDamage");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Overheat_PwrUp/HUD_PwrUp_Icon_DoubleSpeed.dds", "HUD_PwrUp_Icon_DoubleSpeed");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Overheat_PwrUp/HUD_PwrUp_Icon_Freeze.dds", "HUD_PwrUp_Icon_Freeze");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Overheat_PwrUp/HUD_PwrUp_Icon_Fuz-B-Gone.dds", "HUD_PwrUp_Icon_Fuz-B-Gone");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Overheat_PwrUp/HUD_PwrUp_Icon_Shield.dds", "HUD_PwrUp_Icon_Shield");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Overheat_PwrUp/HUD_PwrUp_Icon_Stallone.dds", "HUD_PwrUp_Icon_Stallone");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Overheat_PwrUp/HUD_PwrUp_Icon_Sticky.dds", "HUD_PwrUp_Icon_Sticky");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Overheat_PwrUp/HUD_PwrUp_Icon_Thermal.dds", "HUD_PwrUp_Icon_Thermal");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Overheat_PwrUp/HUD_PwrUp_Icon_Uber.dds", "HUD_PwrUp_Icon_Uber");
	///LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Overheat_PwrUp/.dds", ""); <---Template

	//Radar
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Radar/HUD_Radar_Background.dds", "HUD_Radar_Background");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Radar/HUD_Radar_Frame_GS.dds", "HUD_Radar_Frame"); //gray scaled version
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Radar/Radar_Icon_Bunny_Black_2.dds", "Radar_Icon_Bunny_Black");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Radar/Radar_Icon_Bunny_Black_Circle_2.dds", "Radar_Icon_Bunny_Black_Circle");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Radar/Radar_Icon_Bunny_White_2.dds", "Radar_Icon_Bunny_White");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Radar/Radar_Icon_Bunny_White_Circle_2.dds", "Radar_Icon_Bunny_White_Circle");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Radar/Radar_Icon_PwrUp_2.dds", "Radar_Icon_PwrUp");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Radar/Radar_Icon_PwrUp_Circle_2.dds", "Radar_Icon_PwrUp_Circle");
	///LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Radar/.dds", ""); <---Template

	//Reticle_Related
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Reticle_Related/HUD_Reticle_PwrUp_Shield_Fill.dds", "HUD_Reticle_PwrUp_Shield_Fill"); 
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Reticle_Related/HUD_Reticle_PwrUp_Shield_Frame.dds", "HUD_Reticle_PwrUp_Shield_Frame"); 
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Reticle_Related/HUD_Reticle_PwrUp_Shield_Gradient.dds", "HUD_Reticle_PwrUp_Shield_Gradient"); 
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Reticle_Related/HUD_Reticle_PwrUp_Timer_Fill.dds", "HUD_Reticle_PwrUp_Timer_Fill"); 
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Reticle_Related/HUD_Reticle_PwrUp_Timer_Frame.dds", "HUD_Reticle_PwrUp_Timer_Frame"); 
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Reticle_Related/HUD_Reticle_PwrUp_Timer_Gradient.dds", "HUD_Reticle_PwrUp_Timer_Gradient"); 
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Reticle_Related/HUD_WarningTray_BG.dds", "HUD_WarningTray_BG");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Reticle_Related/HUD_WarningTray_Frame.dds", "HUD_WarningTray_Frame");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Reticle_Related/HUD_WarningTray_Frame_GS.dds", "HUD_WarningTray_Frame_GS");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Reticle_Related/HUD_WarningTray_Icon_Ex.dds", "HUD_WarningTray_Icon_Ex");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Reticle_Related/HUD_WarningTray_Icon_Fuz.dds", "HUD_WarningTray_Icon_Fuz");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Reticle_Related/HUD_WarningTray_Icon_Hp.dds", "HUD_WarningTray_Icon_Hp");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Reticle_Related/HUD_WarningTray_Icon_OH.dds", "HUD_WarningTray_Icon_OH");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Reticle_Related/HUD_WarningTray_Msg_Fuz.dds", "HUD_WarningTray_Msg_Fuz");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Reticle_Related/HUD_WarningTray_Msg_Hp.dds", "HUD_WarningTray_Msg_Hp");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Reticle_Related/HUD_WarningTray_Msg_OH.dds", "HUD_WarningTray_Msg_OH");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Reticle_Related/HUD_Feedback_DamageIndicator.dds", "HUD_Feedback_DamageIndicator");
	///LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Reticle_Related/.dds", ""); <---Template

	//Scoring
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Scoring/HUD_Score_BG_Dark.dds", "HUD_Score_BG");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Scoring/HUD_Score_Juggernaut_Dark.dds", "HUD_Score_Juggernaut");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Scoring/HUD_Score_Star.dds", "HUD_Score_Star");
	///LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/NewHUD/Scoring/.dds", ""); <---Template

	//Lifetime Stats
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/LifetimeStats.dds","LifetimeStats");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/LifetimeStatsNameBlock.dds","LifetimeStatsNameBlock");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/lifeStatsBlock.dds","lifeStatsBlock");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/LifetimeStatsNameBlockHighlighted.dds","LifetimeStatsNameBlockHighlighted");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/LifetimeStatsNameBlockSelected.dds","LifetimeStatsNameBlockSelected");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/AwardGhost.dds","AwardGhost");




	////////////////////////////
	LoadAddTexture("../../Assets/Textures/TestTexturesForCore/white10x10.dds", "white");
	LoadAddTexture("../../Assets/Textures/TestTexturesForCore/LabDesk.dds", "desk");
	LoadAddTexture("../../Assets/Textures/TestTexturesForCore/TempGlass.dds", "wall");
	LoadAddTexture("../../Assets/Textures/TestTexturesForCore/CageTop.dds", "top");
	LoadAddTexture("../../Assets/Textures/TestTexturesForCore/boxTexture.dds", "box");
	LoadAddTexture("../../Assets/Textures/TestTexturesForCore/Woodchips_01.dds", "wood");
	LoadAddTexture("../../Assets/Textures/TestTexturesForCore/soccerball.dds", "ball");
	LoadAddTexture("../../Assets/Textures/TestTexturesForCore/LegoTeddy.dds", "lego");
	LoadAddTexture("../../Assets/Textures/TestTexturesForCore/Nimbus_3D_Teddy_D.dds", "tedd");
	LoadAddTexture("../../Assets/Textures/TestTexturesForCore/TestCube.dds", "tcub");
	LoadAddTexture("../../Assets/Textures/TestTexturesForCore/FuzBallFur_01.dds", "Fuzz");
	/*LoadAddTexture("../../Assets/Textures/PowerUps/icon_Uber_fuz.dds", "Uber");
	LoadAddTexture("../../Assets/Textures/PowerUps/icon_beOff.dds", "bOff");
	LoadAddTexture("../../Assets/Textures/PowerUps/icon_Freeze.dds", "freeze");
	LoadAddTexture("../../Assets/Textures/PowerUps/Icon_Stallone.dds", "Stallone");
	LoadAddTexture("../../Assets/Textures/PowerUps/Icon_Sticky_Bomb.dds", "sticky");*/
	LoadAddTexture("../../Assets/Textures/Environment/Cage_Floor_D.dds", "Cage_Floor_D");
	LoadAddTexture("../../Assets/Textures/Environment/Cage_Wall_D.dds", "Cage_Wall_D");
	LoadAddTexture("../../Assets/Textures/Environment/Towers_D.dds", "Towers_D");
	LoadAddTexture("../../Assets/Textures/PowerUps/Env_PowerUp_BaseForPowerUp_Diffuse.dds", "PowerUpBase_D");
	LoadAddTexture("../../Assets/Textures/PowerUps/Env_PowerUp_DoubleFuz_Diffuse.dds", "DoubleFuz_D");
	LoadAddTexture("../../Assets/Textures/PowerUps/Env_PowerUp_FuzShield_Diffuse.dds", "FuzShield_D");
	LoadAddTexture("../../Assets/Textures/PowerUps/Env_PowerUp_HoseFuz_Diffuse.dds", "HoseFuz_D");
	LoadAddTexture("../../Assets/Textures/PowerUps/Env_PowerUp_ThermalRadar_Diffuse.dds", "ThermalRadar_D");
	LoadAddTexture("../../Assets/Textures/PowerUps/Env_PowerUp_FuzBGone.dds", "FuzBGone_D");
	LoadAddTexture("../../Assets/Textures/PowerUps/Env_PowerUp_Double_Speed.dds", "DoubleSpeed_D");
	LoadAddTexture("../../Assets/Textures/PowerUps/Env_PowerUp_Uber_Fuz.dds", "UberFuz_D");
	LoadAddTexture("../../Assets/Textures/PowerUps/Env_PowerUp_Freeze_Fuz.dds", "FreezeFuz_D");
	LoadAddTexture("../../Assets/Textures/PowerUps/Power_Up_Icon_Stallone_Diffuze.dds", "StalloneFuz_D");
	LoadAddTexture("../../Assets/Textures/PowerUps/Reticle_Uber_Fuz.dds", "UberFuzReticle");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/PowerUps/PowerUp_HUD_Double_Damage.dds", "DoubleDamage_HUD");
	LoadAddTexture("../../Assets/Textures/PowerUps/Reticle_Double_Fuz.dds", "DoubleFuzReticle");
	LoadAddTexture("../../Assets/Textures/Environment/ch_Scientist_Hand_Diffuse.dds","ScientistHand_D");
	LoadAddTexture("../../Assets/Textures/Environment/GPStudios_TableTop.dds", "tabletop");
	LoadAddTexture("../../Assets/Textures/Environment/GPStudios_Ceiling.dds", "ceiling");
	//LoadAddTexture("../../Assets/Textures/Environment/GPStudios_NorthView.dds", "northview");
	LoadAddTexture("../../Assets/Textures/Environment/GPStudios_Wall.dds", "GP_Wall");
	LoadAddTexture("../../Assets/Textures/Environment/Habitat_Object_MushroomHut_D.dds","MushroomHut_D");
	LoadAddTexture("../../Assets/Textures/Environment/Habitat_Object_Arch_Hut_D.dds","ArchHut_D");
	LoadAddTexture("../../Assets/Textures/Environment/Habitat_Object_WaterBottle_D.dds","WaterBottle_D");
	LoadAddTexture("../../Assets/Textures/Environment/Habitat_Object_House_Hut_D.dds","House_Hut_D");
	LoadAddTexture("../../Assets/Textures/Environment/Habitat_Object_TwigBundle_D.dds", "TwigBundle_D");
	LoadAddTexture("../../Assets/Textures/Environment/Habitat_Object_WheatBall_D.dds", "WheatBall_D");
	LoadAddTexture("../../Assets/Textures/Environment/Env_Habitat_Object_DomeFace_D.dds","DomeFace_D");
	LoadAddTexture("../../Assets/Textures/Environment/Habitat_Object_Straight_Tube_D.dds", "Straight_Ramp_D");
	LoadAddTexture("../../Assets/Textures/Environment/Habitat_Object_Spiral_Tube_D.dds", "Spiral_Ramp_D");
	LoadAddTexture("../../Assets/Textures/Environment/Habitat_Object_Corner_Tube_D.dds", "Corner_Ramp_D");
	LoadAddTexture("../../Assets/Textures/Environment/TX_WoodChipFloor_D.dds","WoodChipFloor_D");

	//TeamCheckStep #1 : Add Texture
	//Team Check Texture (TeamCheck)
	LoadAddTexture("../../Assets/Models/BunnyExperiment/Experimental_Rabbit.fbm/Experimental_Rabbit_D.dds","BunnyExperiment");
	LoadAddTexture("../../Assets/Models/Powerups/Sticky_Shot/StickyBall_AO128.dds","ProjectileStickyFuz_D"); 
	LoadAddTexture("../../Assets/Models/Powerups/Freeze_Shot/Freeze_D128.dds","ProjectileFreezeFuz_D"); 
	LoadAddTexture("../../Assets/Models/Powerups/Freeze_FX/Frozen_Cube_D.dds","FreezeCubeFX_D");
	LoadAddTexture("../../Assets/Models/Environment/LabDesk_Floor/LabDesk_Level_DeskFloor.fbm/floor_D.dds", "DeskFloor_D");
	LoadAddTexture("../../Assets/Models/Environment/LabDesk_Funnel/Funnel_Diffuse.dds", "Funnel_D");
	LoadAddTexture("../../Assets/Models/Environment/LabDesk_Microscope/Microscope.dds","Microscope_D");
	LoadAddTexture("../../Assets/Models/Environment/LabDesk_GlassStirrer/LabDesk_Object_GlassRodStirrer.fbm/GlassRod_Diffuse.dds","GlassStirrer_D");
	LoadAddTexture("../../Assets/Models/Environment/LabDesk_Clipboard/LabDesk_Object_ClipBoard_Diffuse.dds", "Clipboard_D");
	LoadAddTexture("../../Assets/Models/Environment/LabDesk_SupportStand/StandDif.dds", "SupportStand_D");
	LoadAddTexture("../../Assets/Models/Environment/LabDesk_TripodStand/LabDesk_Object_TripodStand.dds", "TripodStand_D");
	LoadAddTexture("../../Assets/Models/Environment/LabDesk_Beaker/Glass_Beaker_Filled.dds","BeakerFull_D");
	LoadAddTexture("../../Assets/Models/Environment/LabDesk_Beaker/Glass_Beaker_Empty.dds","BeakerEmpty_D");
	LoadAddTexture("../../Assets/Textures/Player/fuz_gun_D.dds","Gun_D"); //diffuse
	LoadAddTexture("../../Assets/Textures/Player/fuz_gun_S.dds","Gun_S"); //specular
	LoadAddTexture("../../Assets/Textures/Player/ColoredGunTextures/FuzGun_Color_01_1.dds","FuzGun_Color_01"); //coral
	LoadAddTexture("../../Assets/Textures/Player/ColoredGunTextures/FuzGun_Color_02_1.dds","FuzGun_Color_02"); //red
	LoadAddTexture("../../Assets/Textures/Player/ColoredGunTextures/FuzGun_Color_03_1.dds","FuzGun_Color_03"); //soft pink
	LoadAddTexture("../../Assets/Textures/Player/ColoredGunTextures/FuzGun_Color_04_1.dds","FuzGun_Color_04"); //hot pink
	LoadAddTexture("../../Assets/Textures/Player/ColoredGunTextures/FuzGun_Color_05_1.dds","FuzGun_Color_05"); //orange
	LoadAddTexture("../../Assets/Textures/Player/ColoredGunTextures/FuzGun_Color_06_1.dds","FuzGun_Color_06"); //gold
	LoadAddTexture("../../Assets/Textures/Player/ColoredGunTextures/FuzGun_Color_07_1.dds","FuzGun_Color_07"); //lavender
	LoadAddTexture("../../Assets/Textures/Player/ColoredGunTextures/FuzGun_Color_08_1.dds","FuzGun_Color_08"); //purple
	LoadAddTexture("../../Assets/Textures/Player/ColoredGunTextures/FuzGun_Color_09_1.dds","FuzGun_Color_09"); //yellow
	LoadAddTexture("../../Assets/Textures/Player/ColoredGunTextures/FuzGun_Color_10_1.dds","FuzGun_Color_10"); //seafoam
	LoadAddTexture("../../Assets/Textures/Player/ColoredGunTextures/FuzGun_Color_11_1.dds","FuzGun_Color_11"); //green
	LoadAddTexture("../../Assets/Textures/Player/ColoredGunTextures/FuzGun_Color_12_1.dds","FuzGun_Color_12"); //cyan
	LoadAddTexture("../../Assets/Textures/Player/ColoredGunTextures/FuzGun_Color_13_1.dds","FuzGun_Color_13"); //blue
	LoadAddTexture("../../Assets/Textures/Player/ColoredGunTextures/FuzGun_Color_14_1.dds","FuzGun_Color_14"); //lime
	LoadAddTexture("../../Assets/Textures/Player/ColoredGunTextures/FuzGun_Color_15_1.dds","FuzGun_Color_15"); //gray

	//Bunnies
	LoadAddTexture("../../Assets/Models/Characters/BigEars/Experimental_Bunny_BigEars_D.dds","BigEars_D");
	LoadAddTexture("../../Assets/Models/Characters/Cute/Bunny_Cute_D.dds", "Cute_D");
	LoadAddTexture("../../Assets/Models/Characters/Cartoon/Experimental_Rabbit_D.dds","Cartoon_D");
	//LoadAddTexture("../../Assets/Models/Characters/Mangy/Mangled_Rabbit_AO.dds","Mangy_D");
	LoadAddTexture("../../Assets/Models/Characters/Regular/Experimental_Rabbit_D.dds","Regular_D");
	//LoadAddTexture("../../Assets/Models/Characters/Stuffed/Experimental_Bunny_Stuffed_D.dds","Stuffed_D");





	LoadAddTexture("../../Assets/Models/Environment/LabDesk_TestTube/LabDesk_Object_TestTubeRack.dds","TestTubeRack_D");
	LoadAddTexture("../../Assets/Models/Environment/LabDesk_TestTube/TestTubesEmpty_D.dds","TestTubeEmpty_D");
	LoadAddTexture("../../Assets/Models/Environment/LabDesk_TestTube/TestTubesFilled_D.dds","TestTubeFill_D");
	LoadAddTexture("../../Assets/Models/Environment/LabDesk_FlorenceFlask/T_FlorenceFlaskFill_D.dds","FlorenceFlaskFill_D");
	LoadAddTexture("../../Assets/Models/Environment/LabDesk_FlorenceFlask/T_FlorenceFlaskEmpty_D.dds","FlorenceFlaskEmpty_D");

	LoadAddTexture("../../Assets/Models/Environment/LabDesk_Syringe/Env_LabDesk_Object_SyringeEmpty_Diffuse.dds","SyringeEmpty_D");

	//LoadAddTexture("../../Assets/Models///.dds","");  (<---QuickAdd)


	//Science Lab
	LoadAddTexture("../../Assets/Textures/Environment/Science_Lab/Env_Level_Model_Chair.dds","Env_Level_Model_Chair");
	LoadAddTexture("../../Assets/Textures/Environment/Science_Lab/Env_Level_Model_ChalkBoard_Diffuse.dds","Env_Level_Model_ChalkBoard_Diffuse");
	LoadAddTexture("../../Assets/Textures/Environment/Science_Lab/Env_Level_Model_Door_Diffuse.dds","Env_Level_Model_Door_Diffuse");
	LoadAddTexture("../../Assets/Textures/Environment/Science_Lab/Env_Level_Model_Floor_Diffuse.dds","Env_Level_Model_Floor_Diffuse");
	LoadAddTexture("../../Assets/Textures/Environment/Science_Lab/Env_Level_Model_Floor_Normal.dds","Env_Level_Model_Floor_Normal");
	LoadAddTexture("../../Assets/Textures/Environment/Science_Lab/Env_Level_Model_Pod_Diffuse.dds","Env_Level_Model_Pod_Diffuse");
	LoadAddTexture("../../Assets/Textures/Environment/Science_Lab/Env_Level_Model_Pod_Emmisive.dds","Env_Level_Model_Pod_Emmisive");
	LoadAddTexture("../../Assets/Textures/Environment/Science_Lab/Env_Level_Model_Pod_Normal.dds","Env_Level_Model_Pod_Normal");
	LoadAddTexture("../../Assets/Textures/Environment/Science_Lab/Env_Level_Model_Table_Diffuse.dds","Env_Level_Model_Table_Diffuse");
	LoadAddTexture("../../Assets/Textures/Environment/Science_Lab/Env_Level_Model_Table_Emmisive.dds","Env_Level_Model_Table_Emmisive");
	LoadAddTexture("../../Assets/Textures/Environment/Science_Lab/Env_Level_Model_Wall_Diffuse.dds","Env_Level_Model_Wall_Diffuse");
	LoadAddTexture("../../Assets/Textures/Environment/Science_Lab/Env_Level_Model_Wall_Normal.dds","Env_Level_Model_Wall_Normal");
	LoadAddTexture("../../Assets/Textures/Environment/Science_Lab/bunsen_D.dds","bunsen_D");
	LoadAddTexture("../../Assets/Textures/Environment/Science_Lab/ClipBoard_D.dds","ClipBoard_D");
	LoadAddTexture("../../Assets/Textures/Environment/Science_Lab/Beaker_Filled.dds","Beaker_Filled");




	//Liz Level
	LoadAddTexture("../../Assets/Textures/Level/Level_Blocks.dds","Level_Blocks");
	LoadAddTexture("../../Assets/Textures/Level/Level_Cardboard.dds","Level_Cardboard");
	LoadAddTexture("../../Assets/Textures/Level/Level_Tube.dds","Level_Tube");
	LoadAddTexture("../../Assets/Textures/Environment/wheel_diffuse.dds","wheel_diffuse");



	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/EndGameStats_NoCut.dds","EndGameStats");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/SpecialStatTemplate.dds","StatTemplate");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/TopStatBackground.dds","TopStatBackground");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/DetailStatsBackground.dds","DetailStatsBackground");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/IndividualStatBlock.dds","IndividualStatBlock");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/ArrowButton.dds","ArrowButton");



	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/Menu_Award_ClayPigeon.dds","Menu_Award_ClayPigeon");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/Menu_Award_Exterminator.dds","Menu_Award_Exterminator");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/Menu_Award_Ghost.dds","Menu_Award_Ghost");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/Menu_Award_GrandTheft.dds","Menu_Award_GrandTheft");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/Menu_Award_HipHop.dds","Menu_Award_HipHop");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/Menu_Award_Rambo.dds","Menu_Award_Rambo");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/Menu_Award_Sharpshooter.dds","Menu_Award_Sharpshooter");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/Menu_Award_Stormtrooper.dds","Menu_Award_Stormtrooper");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/Menu_Award_Wingman.dds","Menu_Award_Wingman");

	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/Menu_Icon_ClayPigeon.dds","Menu_Icon_ClayPigeon");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/Menu_Icon_Exterminator.dds","Menu_Icon_Exterminator");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/Menu_Icon_GrandTheft.dds","Menu_Icon_GrandTheft");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/Menu_Icon_HipHop.dds","Menu_Icon_HipHop");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/Menu_Icon_Rambo.dds","Menu_Icon_Rambo");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/Menu_Icon_Sharpshooter.dds","Menu_Icon_Sharpshooter");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/Menu_Icon_Stormtrooper.dds","Menu_Icon_Stormtrooper");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/Menu_Icon_Wingman.dds","Menu_Icon_Wingman");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Awards/Winner_Banner.dds","Winner_Banner");


	LoadAddTexture("../../Assets/Textures/HUD_Menu/HUD/HUD_Spectator_Control.dds","Spectator_Control");

	// Credits Team Logos
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Credits/Logo_Team_Bo.dds","BoBunny");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Credits/Logo_Team_Wes.dds","WesBunny");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Credits/Logo_Team_Alyssa.dds","AlyssaBunny");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Credits/Logo_Team_Jim.dds","JimBunny");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Credits/Logo_Team_Kenneth.dds","KennethBunny");
	LoadAddTexture("../../Assets/Textures/HUD_Menu/Menu/Credits/Logo_Team_Brent.dds","BrentBunny");

}

void CRenderer::SetupTestSphere()
{
	sphere = *m_pGameObjectManager->getModel("sphere");
	for (unsigned int i = 0; i < sphere.GetMeshes()[0].GetVertices().size(); i++)
	{
		VERTEX_3D tempVert;
		tempVert.Coordinate = sphere.GetMeshes()[0].GetVertices()[i].position;
		tempVert.Normal = sphere.GetMeshes()[0].GetVertices()[i].normal;
		tempVert.UVs.x = sphere.GetMeshes()[0].GetVertices()[i].tex_coord.u;
		tempVert.UVs.y = sphere.GetMeshes()[0].GetVertices()[i].tex_coord.v;
		sphereMesh.push_back(tempVert);
	}

	CComPtr<ID3D11Buffer> tempVertexBuffer;

	D3D11_BUFFER_DESC Vertex_Buffer_Desc;
	ZeroMemory(&Vertex_Buffer_Desc, sizeof(Vertex_Buffer_Desc));
	Vertex_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;
	Vertex_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	Vertex_Buffer_Desc.CPUAccessFlags = NULL;
	Vertex_Buffer_Desc.ByteWidth = sizeof(VERTEX_3D) * sphereMesh.size();

	D3D11_SUBRESOURCE_DATA objData;
	ZeroMemory(&objData, sizeof(objData));
	objData.pSysMem = &sphereMesh[0];
	pDevice->CreateBuffer(&Vertex_Buffer_Desc, &objData, &tempVertexBuffer);

	Sphere_Vertex_Buffer = tempVertexBuffer;

	// build and add the index buffer
	CComPtr<ID3D11Buffer> tempIndexBuffer;

	D3D11_BUFFER_DESC Index_Buffer_Desc;
	ZeroMemory(&Index_Buffer_Desc, sizeof(Index_Buffer_Desc));
	Index_Buffer_Desc.Usage = D3D11_USAGE_IMMUTABLE;
	Index_Buffer_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	Index_Buffer_Desc.CPUAccessFlags = NULL;
	Index_Buffer_Desc.ByteWidth = sizeof(unsigned int)* sphere.GetMeshes()[0].GetIndices().size();

	D3D11_SUBRESOURCE_DATA IndexData;
	ZeroMemory(&IndexData, sizeof(IndexData));
	IndexData.pSysMem = &sphere.GetMeshes()[0].GetIndices()[0];
	pDevice->CreateBuffer(&Index_Buffer_Desc, &IndexData, &tempIndexBuffer);

	Sphere_Index_Buffer = tempIndexBuffer;
}

void CRenderer::SamplerState_Setup()
{
	D3D11_SAMPLER_DESC Sampler_Desc;
	ZeroMemory(&Sampler_Desc, sizeof(Sampler_Desc));
	Sampler_Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	Sampler_Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	Sampler_Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	Sampler_Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	Sampler_Desc.MinLOD = (-FLT_MAX);
	Sampler_Desc.MaxLOD = (FLT_MAX);
	Sampler_Desc.MipLODBias = 0.0f;
	Sampler_Desc.MaxAnisotropy = 1;
	Sampler_Desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	pDevice->CreateSamplerState(&Sampler_Desc, &pSamplerState);
}

void CRenderer::BlendState_Setup()
{
	D3D11_BLEND_DESC bDesc;
	ZeroMemory(&bDesc, sizeof(bDesc));
	bDesc.AlphaToCoverageEnable = false;
	bDesc.IndependentBlendEnable = true;
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

void CRenderer::DepthStencilView_Setup()
{
	CD3D11_DEPTH_STENCIL_DESC DSV_State_Desc(D3D11_DEFAULT);
	DSV_State_Desc.DepthEnable = true;
	DSV_State_Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DSV_State_Desc.DepthFunc = D3D11_COMPARISON_LESS;
	pDevice->CreateDepthStencilState(&DSV_State_Desc, &pDSV_State_DepthEnabled);

	DSV_State_Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	pDevice->CreateDepthStencilState(&DSV_State_Desc, &pDSV_State_DepthDisabled);

	DSV_State_Desc.DepthEnable = false;
	DSV_State_Desc.StencilEnable = true;
	DSV_State_Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DSV_State_Desc.DepthFunc = D3D11_COMPARISON_LESS;
	pDevice->CreateDepthStencilState(&DSV_State_Desc, &pDSV_State_DepthCheckDisabled);
}

void CRenderer::GammaController_Setup()
{
	m_fGamma_Level = 1.0f;

	HRESULT hr = pSwapChain->GetContainingOutput(&Gamma_IDXGI_Output);
	//if(hr != S_OK)
	//	MessageBox(NULL, L"GetContainingOutput() ERROR!", NULL, NULL);

	//	hr = Gamma_IDXGI_Output->GetGammaControl(&Gamma_Controller);
	//	if(hr != S_OK)
	//		MessageBox(NULL, L"GetGammaControl() ERROR!", NULL, NULL);

	hr = Gamma_IDXGI_Output->GetGammaControlCapabilities(&Gamma_Control_Capabilities);
	//if(hr != S_OK)
	//	MessageBox(NULL, L"GetGammaControlCapabilities() ERROR!", NULL, NULL);

	Gamma_Controller.Scale.Red = 1.0f;
	Gamma_Controller.Scale.Green = 1.0f;
	Gamma_Controller.Scale.Blue = 1.0f;
	Gamma_Controller.Offset.Red = 0.0f;
	Gamma_Controller.Offset.Green = 0.0f;
	Gamma_Controller.Offset.Blue = 0.0f;

	int numGammaControlPoints = (int)Gamma_Control_Capabilities.NumGammaControlPoints;
	for(int i = 0; i < numGammaControlPoints; i++)
	{
		Gamma_Controller.GammaCurve[i].Red = min(1.0f, (float)i * (m_fGamma_Level) / (float)(numGammaControlPoints - 1.0f));
		Gamma_Controller.GammaCurve[i].Green = min(1.0f, (float)i * (m_fGamma_Level) / (float)(numGammaControlPoints - 1.0f));
		Gamma_Controller.GammaCurve[i].Blue = min(1.0f, (float)i * (m_fGamma_Level) / (float)(numGammaControlPoints - 1.0f));
	}

	hr = Gamma_IDXGI_Output->SetGammaControl(&Gamma_Controller);
	//if(hr != S_OK)
	//	MessageBox(NULL, L"SetGammaControl() ERROR!", NULL, NULL);

}

void CRenderer::SetGammaLevel(float gammaLevel)
{
	m_fGamma_Level = gammaLevel;

	if(m_fGamma_Level > 2.0f)
		m_fGamma_Level = 2.0f;
	if(m_fGamma_Level < 0.25f)
		m_fGamma_Level = 0.25f;

	int numGammaControlPoints = (int)Gamma_Control_Capabilities.NumGammaControlPoints;
	for(int i = 0; i < numGammaControlPoints; i++)
	{
		Gamma_Controller.GammaCurve[i].Red = min(1.0f, (float)i * (m_fGamma_Level) / (float)(numGammaControlPoints - 1.0f));
		Gamma_Controller.GammaCurve[i].Green = min(1.0f, (float)i * (m_fGamma_Level) / (float)(numGammaControlPoints - 1.0f));
		Gamma_Controller.GammaCurve[i].Blue = min(1.0f, (float)i * (m_fGamma_Level) / (float)(numGammaControlPoints - 1.0f));
	}

	Gamma_IDXGI_Output->SetGammaControl(&Gamma_Controller);
}

void CRenderer::CreateQuad_2D(RECTF rect, ScreenSpaceObject& SSobj, XMFLOAT4 colorRGBA, float fGradient)
{
	// set up verts
	{
		VERTEX_2D tempVert;
		tempVert.ColorRGBA = colorRGBA;

		// vertex top left
		tempVert.UVcoord.x = 0;
		tempVert.UVcoord.y = 0;
		tempVert.Gradient = fGradient;

		tempVert.Coordinate = XMFLOAT2(rect.left, rect.top);

		SSobj.mesh.push_back(tempVert);

		// vertex top right
		tempVert.UVcoord.x = 1;
		tempVert.UVcoord.y = 0;
		tempVert.Gradient = fGradient;

		tempVert.Coordinate = XMFLOAT2(rect.right, rect.top);
		SSobj.mesh.push_back(tempVert);

		// bottom left
		tempVert.UVcoord.x = 0;
		tempVert.UVcoord.y = 1;
		tempVert.Gradient = fGradient;

		tempVert.Coordinate = XMFLOAT2(rect.left, rect.bottom);
		SSobj.mesh.push_back(tempVert);

		// bottom right
		tempVert.UVcoord.x = 1;
		tempVert.UVcoord.y = 1;
		tempVert.Gradient = fGradient;

		tempVert.Coordinate = XMFLOAT2(rect.right, rect.bottom);;
		SSobj.mesh.push_back(tempVert);
	}

	// set up the vertex buffer
	{
		D3D11_BUFFER_DESC Vertex_Buffer_Desc;
		ZeroMemory(&Vertex_Buffer_Desc, sizeof(Vertex_Buffer_Desc));
		Vertex_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;
		Vertex_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		Vertex_Buffer_Desc.CPUAccessFlags = NULL;
		Vertex_Buffer_Desc.ByteWidth = sizeof(VERTEX_2D)* SSobj.mesh.size();

		D3D11_SUBRESOURCE_DATA SubResourceData;
		ZeroMemory(&SubResourceData, sizeof(SubResourceData));
		SubResourceData.pSysMem = &SSobj.mesh[0];
		pDevice->CreateBuffer(&Vertex_Buffer_Desc, &SubResourceData, &SSobj.Vertex_Buffer);
	}

	// set up index buffer
	{
		unsigned short numIndices = 6;
		unsigned short Indices[6] = {
			0, 1, 2,
			2, 1, 3
		};

		D3D11_BUFFER_DESC Index_Buffer_Desc;
		ZeroMemory(&Index_Buffer_Desc, sizeof(Index_Buffer_Desc));
		Index_Buffer_Desc.Usage = D3D11_USAGE_IMMUTABLE;
		Index_Buffer_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		Index_Buffer_Desc.CPUAccessFlags = NULL;
		Index_Buffer_Desc.ByteWidth = sizeof(unsigned short)* numIndices;

		D3D11_SUBRESOURCE_DATA IndexData;
		ZeroMemory(&IndexData, sizeof(IndexData));
		IndexData.pSysMem = Indices;
		pDevice->CreateBuffer(&Index_Buffer_Desc, &IndexData, &SSobj.Index_Buffer);
	}
}

void CRenderer::AddEmitterObject(CEmitter* obj)
{
	// build and add a vertex buffer for the object
	CComPtr<ID3D11Buffer> tempVertexBuffer;

	D3D11_BUFFER_DESC Vertex_Buffer_Desc;
	ZeroMemory(&Vertex_Buffer_Desc, sizeof(Vertex_Buffer_Desc));
	Vertex_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;
	Vertex_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	Vertex_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Vertex_Buffer_Desc.ByteWidth = sizeof(PARTICLE_VERTEX) * obj->GetParticleRenderVertInfo().size();

	D3D11_SUBRESOURCE_DATA objData;
	ZeroMemory(&objData, sizeof(objData));
	objData.pSysMem = &(obj->GetParticleRenderVertInfo()[0]);
	pDevice->CreateBuffer(&Vertex_Buffer_Desc, &objData, &tempVertexBuffer);

	obj->vertexBuffer = tempVertexBuffer;

	/*CComPtr<ID3D11Buffer> tempVertexBuffer;

	D3D11_BUFFER_DESC Vertex_Buffer_Desc;
	ZeroMemory(&Vertex_Buffer_Desc, sizeof(Vertex_Buffer_Desc));
	Vertex_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;
	Vertex_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	Vertex_Buffer_Desc.CPUAccessFlags = NULL;
	Vertex_Buffer_Desc.ByteWidth = sizeof(PARTICLE_VERTEX) * obj->GetParticleRenderVertInfo().size();

	D3D11_SUBRESOURCE_DATA objData;
	ZeroMemory(&objData, sizeof(objData));
	objData.pSysMem = &tempemitter->object->GetParticleRenderVertInfo()[0];
	pDevice->CreateBuffer(&Vertex_Buffer_Desc, &objData, &tempVertexBuffer);

	tempemitter->vertexBuffer = tempVertexBuffer;*/

	// build and add the local const buffer for the matrix
	D3D11_BUFFER_DESC obj_Buffer_Desc;
	ZeroMemory(&obj_Buffer_Desc, sizeof(obj_Buffer_Desc));
	obj_Buffer_Desc.ByteWidth = sizeof(XMMATRIX);
	obj_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;
	obj_Buffer_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	obj_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA Object_Data;
	Object_Data.pSysMem = &obj->GetWorldMatrix();
	Object_Data.SysMemPitch = 0;
	Object_Data.SysMemSlicePitch = 0;

	CComPtr<ID3D11Buffer> tempMatrixBuffer;
	pDevice->CreateBuffer(&obj_Buffer_Desc, &Object_Data, &tempMatrixBuffer);

	obj->matrixBuffer = tempMatrixBuffer;


	reinterpret_cast<Particle_RC*>(m_Particles_RC)->AddRenderObject(obj);
}

void CRenderer::RemoveEmitterObject(CEmitter* obj)
{
	reinterpret_cast<Particle_RC*>(m_Particles_RC)->RemoveRenderObject(obj);
}

void CRenderer::RemoveAllEmitters(void)
{
	reinterpret_cast<Particle_RC*>(m_Particles_RC)->RemoveAll();
}

void CRenderer::AddFuzEffects(FUZ_EFFECT_POOL* fuzPool)
{
	Fuz* tempFuz = new Fuz();
	tempFuz->FuzPool = fuzPool;

	// build and add a vertex buffer for the object
	CComPtr<ID3D11Buffer> tempVertexBuffer;

	D3D11_BUFFER_DESC Vertex_Buffer_Desc;
	ZeroMemory(&Vertex_Buffer_Desc, sizeof(Vertex_Buffer_Desc));
	Vertex_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;
	Vertex_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	Vertex_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Vertex_Buffer_Desc.ByteWidth = sizeof(Fuz_Effect) * MAX_FUZ_FX;

	D3D11_SUBRESOURCE_DATA objData;
	ZeroMemory(&objData, sizeof(objData));
	objData.pSysMem = &(fuzPool->m_Fuz_Effects[0]);
	pDevice->CreateBuffer(&Vertex_Buffer_Desc, &objData, &tempVertexBuffer);

	tempFuz->vertexBuffer = tempVertexBuffer;

	reinterpret_cast<Fuz_RC*>(m_FuzEffect_RC)->AddRenderObject(tempFuz);
}

void CRenderer::RemoveFuzEffects(FUZ_EFFECT_POOL* fuzPool)
{
	reinterpret_cast<Fuz_RC*>(m_FuzEffect_RC)->RemoveRenderObject(fuzPool);
}

void CRenderer::TurnThermalEffect_ON()
{
	m_bThermalRadarEffect = true;

	m_OpaqueObj_RC->TurnThermalEffect_ON();
	m_TransparentObj_RC->TurnThermalEffect_ON();
	m_Player_RC->TurnThermalEffect_ON();
}

void CRenderer::TurnThermalEffect_OFF()
{
	m_bThermalRadarEffect = false;

	m_OpaqueObj_RC->TurnThermalEffect_OFF();
	m_TransparentObj_RC->TurnThermalEffect_OFF();
	m_Player_RC->TurnThermalEffect_OFF();
}

void CRenderer::ToggleThermalRadarEffect()
{
	m_bThermalRadarEffect = !m_bThermalRadarEffect;

	m_OpaqueObj_RC->ToggleThermalRadarEffect();
	m_TransparentObj_RC->ToggleThermalRadarEffect();
	m_Player_RC->ToggleThermalRadarEffect();

	//m_OpaqueObj_RC->ToggleWireFrame();
	//m_TransparentObj_RC->ToggleWireFrame();
}

int CRenderer::ActivateAdvancedFuzEffect(Fuz_Effect fuzEffect)
{
	for(int i = 0; i < MAX_FUZ_COUNT; i++)
	{
		if(fuz_Pool.fuzzes[i].r_l_p_p.y <= 0)
		{
			fuz_Pool.fuzzes[i].r_l_p_p.z = fuzEffect.m_fRadius; // eventual radius
			fuz_Pool.fuzzes[i].r_l_p_p.x = 0.0f; // initial radius

			fuz_Pool.fuzzes[i].r_l_p_p.y = fuzEffect.m_fLifetime;
			float r = fuzEffect.f3_ColorRGB.x * 2.0f;
			float g = fuzEffect.f3_ColorRGB.y * 2.0f;
			float b = fuzEffect.f3_ColorRGB.z * 2.0f;
			fuz_Pool.fuzzes[i].color = XMFLOAT4(r, g, b, 1.0f);
			fuz_Pool.fuzzes[i].worldPos = XMFLOAT4(fuzEffect.f3_WorldPosition.x, fuzEffect.f3_WorldPosition.y, fuzEffect.f3_WorldPosition.z, 1.0f);

			return i;
		}
	}

	return -1;
}

void CRenderer::updateAdvancedFuzPool(float dt)
{
	for(int i = 0; i < MAX_FUZ_COUNT; i++)
	{
		// radius/lifetime/final radius/padding
		if(fuz_Pool.fuzzes[i].r_l_p_p.y >= 0)
		{
			fuz_Pool.fuzzes[i].r_l_p_p.y -= dt;

			// if lifetime is less than 1 begin shrinking the radius
			if(fuz_Pool.fuzzes[i].r_l_p_p.y < 1.0f)
			{
				fuz_Pool.fuzzes[i].r_l_p_p.x -= dt * fuz_Pool.fuzzes[i].r_l_p_p.z;
				if(fuz_Pool.fuzzes[i].r_l_p_p.x > fuz_Pool.fuzzes[i].r_l_p_p.z)
					fuz_Pool.fuzzes[i].r_l_p_p.x = fuz_Pool.fuzzes[i].r_l_p_p.z;
			}
			// increase from initial radius to final radius
			else if(fuz_Pool.fuzzes[i].r_l_p_p.x < fuz_Pool.fuzzes[i].r_l_p_p.z)
				fuz_Pool.fuzzes[i].r_l_p_p.x += dt * fuz_Pool.fuzzes[i].r_l_p_p.z;
		}
	}
}

void CRenderer::SetCharacterSelect(bool bCharacterSelect)
{
	m_bCharacterSelect = bCharacterSelect;
	if(m_bCharacterSelect == false)
		m_pGameInstance->getStateManager()->GetTop()->GetHud()->m_bDrawCursor = true;
}

int CRenderer::FindStringLength(WCHAR const* string)
{
	return ((ScreenSpace_RC*)m_ScreenSpace_RC)->FindStringLength(string);
}

void CRenderer::SetDebugText(string str)
{
	m_sDebugText = str;
}
