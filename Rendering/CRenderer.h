/***********************************************
* Filename:  		CRenderer.h
* Date:      		9/10/2014
* Mod. Date: 		09/11/14
* Mod. Initials:	JM
* Author:    		Jim Merrell
* Purpose:   		This is the Render Class that renders the entire game
************************************************/

#ifndef CRENDERER_H
#define CRENDERER_H

#include "RendererCommon.h"
#include "../VFX/Emitter.h"

class CGame;
class CRenderContext;
class Fuz_Effect;

class CRenderer
{
private:
	
	//unique_ptr<GamePad> gamePad;

	HINSTANCE						application;
	WNDPROC							appWndProc;
	HWND							window;

	DXGI_GAMMA_CONTROL				Gamma_Controller;
	CComPtr<IDXGIOutput>			Gamma_IDXGI_Output;
	DXGI_GAMMA_CONTROL_CAPABILITIES	Gamma_Control_Capabilities;
	float							m_fGamma_Level;

	CComPtr<ID3D11DeviceContext>	pImmediateContext;
	CComPtr<ID3D11Device>			pDevice;

	CComPtr<ID3D11RenderTargetView>	pRTV;
	CComPtr<ID3D11DepthStencilView>	pDSV;
	CComPtr<IDXGISwapChain>			pSwapChain;
	D3D11_VIEWPORT					ViewPort;

	CComPtr<ID3D11Texture2D>			pZBuffer;
	CComPtr<ID3D11BlendState>			pBlendState;
	CComPtr<ID3D11DepthStencilState>	pDSV_State_DepthEnabled;
	CComPtr<ID3D11DepthStencilState>	pDSV_State_DepthDisabled;
	CComPtr<ID3D11DepthStencilState>	pDSV_State_DepthCheckDisabled;

	CComPtr<ID3D11InputLayout>		InputLayout;

	CComPtr<ID3D11VertexShader>		VertShader;
	CComPtr<ID3D11PixelShader>		PShader;

	SCENE							scene;
	CComPtr<ID3D11Buffer>			scene_constBuffer;

	SCENE							m_CharacterSelectScene;
	CComPtr<ID3D11Buffer>			m_CharacterSelect_constBuffer;

	FUZ_POOL						fuz_Pool;
	CComPtr<ID3D11Buffer>			fuz_ConstBuffer;

	SCENE							debugCamera_scene;
	bool							bDebugCamera;
	float							m_fDebugCamera_height;
	float							m_fDebugCamera_distance;

	float							AspectRatio;
	float							m_fBackBufferWidth;
	float							m_fBackBufferHeight;
	float							m_fNearZ;
	float							m_fFarZ;

	int								m_nTesselationLevel;
	float							m_fTestXPos;
	float							m_fTestZPos;	

	bool							m_bScreenSpace_RC;
	bool							m_bParticles_RC;
	bool							m_bOpaque_RC;
	bool							m_bTransparentObj_RC;
	bool							m_bFuzEffect_RC;

	bool							m_bThermalRadarEffect;

	bool							m_bCharacterSelect;

	bool							m_bFullScreen;

	string							m_sDebugText;

	COLOR_RGBA						m_f4_currColor;
	CComPtr<ID3D11Buffer>			trans_constBuffer;

	CComPtr<ID3D11Resource>			pResource;

	CRenderContext* m_TransparentObj_RC;
	CRenderContext* m_ScreenSpace_RC;
	CRenderContext* m_OpaqueObj_RC;
	CRenderContext* m_FuzEffect_RC;
	CRenderContext* m_Particles_RC;
	CRenderContext* m_Player_RC;

	CModel sphere;
	vector<VERTEX_3D>		sphereMesh;
	CComPtr<ID3D11Buffer>	Sphere_Vertex_Buffer;
	CComPtr<ID3D11Buffer>	Sphere_Index_Buffer;	

	map<string, CComPtr<ID3D11ShaderResourceView>>	m_SRVs;
	CComPtr<ID3D11ShaderResourceView>				m_BasicWhiteSRV;


	//factory methods
	void Windows_Setup(HINSTANCE hinst, WNDPROC proc, float BackBuffer_Width, float BackBuffer_Height);
	void SwapChainAndDevice_Setup(float BackBuffer_Width, float BackBuffer_Height);
	void BackBuffer_Setup();
	void DepthBuffer_Setup(float BackBuffer_Width, float BackBuffer_Height);
	void ViewPort_Setup(float BackBuffer_Width, float BackBuffer_Height);
	void Scene_Setup(float BackBuffer_Width, float BackBuffer_Height, float Near_Z, float Far_Z);
	void SamplerState_Setup();
	void BlendState_Setup();
	void DepthStencilView_Setup();
	void GammaController_Setup();
	void CreateFuzPool();
	
	void LoadTextures();
	void SetupTestSphere();

//	void InitShaders_2D();


	void CreateQuad_2D(RECTF rect, ScreenSpaceObject& SSobj, XMFLOAT4 colorARGB, float fGradient = 0.0f);	
	
	CObjectManagerClient* m_pGameObjectManager;
	CGame* m_pGameInstance;

	CCamera* m_CurrCam;
	XMFLOAT4 m_F3_PostionVector;
	float m_xRotation;
	float m_yRotation;

	CPlayer* players[4];


	CComPtr<ID3D11SamplerState>	pSamplerState;
	
	void takeCameraInput();
	void updateAdvancedFuzPool(float dt);


public:
	CRenderer(){};
	~CRenderer();

	enum SCREEN_LAYER { SCREEN_FRONT, SCREEN_MIDDLE, SCREEN_BACK };

	// Added to get a model for CGame (Kenneth)
	//CModel getTestSphereModel(){return this->testSphere;}
	CObjectManagerClient* getObjectManager(){return this->m_pGameObjectManager;}
	void Initialize(HINSTANCE hinst, float BackBuffer_Width, float BackBuffer_Height, float Near_Z = 0.1f, float Far_Z = 1.0f,
		CObjectManagerClient* objectManager = nullptr, CGame* owner = nullptr);
	bool Run(float dt);
	void ShutDown();

	void AddRenderObject(Entity3D* obj);
	void RemoveRenderObject(Entity3D* obj);

	void AddEmitterObject(CEmitter* obj);
	void RemoveEmitterObject(CEmitter* obj);
	void RemoveAllEmitters(void);

	void AddFuzEffects(FUZ_EFFECT_POOL* fuzPool);
	void RemoveFuzEffects(FUZ_EFFECT_POOL* fuzPool);

	void SetCameraObject(Entity3D* camera);

	void SetGammaLevel(float gammaLevel);

	bool Resize(int nWidth, int nHeight, bool bWindowed);
	bool m_bWindowed;

	void TurnThermalEffect_ON();
	void TurnThermalEffect_OFF();
	void ToggleThermalRadarEffect();
	bool IsThermalActive() { return this->m_bThermalRadarEffect;}

	int ActivateAdvancedFuzEffect(Fuz_Effect fuzEffect);

	//// ***** EXAMPLE for using concatinated strings ******************************
	//// stringstream testSS;
	//// int i = 100;
	//// testSS << "Hello, " << i << " World.";
	//// string testCase = testSS.str();
	//// GetRenderer()->RenderText(testCase);
	void RenderText(string text = "test TEXT", int layer = SCREEN_BACK, int PosX = 0, int posY = 0, XMFLOAT4 colorRGBA = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), float scale = 1.0f, float rotation = 0.0f);

	bool LoadAddTexture(const char* DDSfilepath, const char* textureName);
	void DrawTexture(RECTF rect, const char* textureName, int layer = SCREEN_BACK, float R = 1.0f, float G = 1.0f, float B = 1.0f, float A = 1.0f, int useGradient_PS = 0, float fGradient = 0.0f);

	void DrawMultiTexture(RECTF rect, const char* textureName, int layer = SCREEN_BACK, XMFLOAT4 color_RGBA = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), float fGradient = 0.0f, const char* secondTextureName = "none", const char* thirdTextureName = "none");

	void DrawTexture(const char* textureName,  XMFLOAT2 position, XMFLOAT4 colorARGB, XMFLOAT2 sourceOrigin, float rotationDegrees, float scale, int layer = SCREEN_BACK);

	// RECTF values should be -1 to 1, cartesion quardinates
	//		left of the screen		= -1
	//		right of the screen		= 1
	//		top of the screen		= 1
	//		bottom of the screen	= -1
	void DrawRect(RECTF rect, int layer = SCREEN_BACK, XMFLOAT3 colorRGB = XMFLOAT3(1.0f, 1.0f, 1.0f), float alpha = 1.0f); //might need an #include to use XMFLOAT3	
	void DrawRect(RECTF rect, int layer = SCREEN_BACK, float R = 1.0f, float G = 1.0f, float B = 1.0f, float A = 1.0f);

	/******************************************************************************************
	* Code added by Alyssa to test with CHud for Cursor Pos
	******************************************************************************************/
	// Code Start
	float GetScreenWidth(void) { return m_fBackBufferWidth;}
	float GetScreenHeight(void) {return m_fBackBufferHeight;}
	XMFLOAT4X4 GetSceneViewMatrix(void) {return scene.viewMatrix;}

	//void SetCurrentCam(CCamera curCam) { 
	// Code End
	void CreateBoxFromAABB(Entity3D* ent);

	void SetCharacterSelect(bool bCharacterSelect);  // this will set the bool for cahracter select
													//	- if "true" the character model will render in front of the screenspace objects

	int FindStringLength(WCHAR const* string);

	void SetDebugText(string str);

	bool get_bFullScreen() const {return m_bFullScreen;}
};




#endif //CRENDERER_H
