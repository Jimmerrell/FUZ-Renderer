/***********************************************
* Filename:  		CRenderContext.h
* Date:      		10/2/2014
* Mod. Date: 		10/2/2014
* Mod. Initials:	JM
* Author:    		Jim Merrell
* Purpose:   		This is the RenderContext Class that holds common render data
************************************************/

#ifndef SCREENSPACE_RC_H
#define SCREENSPACE_RC_H

#include "RenderContext.h"

#include "../../DirectXTK/Inc/SpriteBatch.h"
#include "../../DirectXTK/Inc/SpriteFont.h"

class ScreenSpace_RC : public CRenderContext
{
public:
	ScreenSpace_RC(void){}
	~ScreenSpace_RC(void){}


	virtual void Initialize(ID3D11Device* device, ID3D11DeviceContext* immediateContext, void* texture_SRVs, SCENE* _scene);
	virtual void Run();

	void SetBlendState(void* blendstate);
	void AddRenderText(RENDER_TEXT renText, int layer);
	void AddRenderRECT(ScreenSpaceObject SSobj, int layer);
	void AddRenderTexture(RENDER_TEXTURE RT, int layer);
	int FindStringLength(WCHAR const* string);
private:


	vector<ScreenSpaceObject>		m_vScreenSpaceObjects[3];
	vector<RENDER_TEXT>				m_vRenderTexts[3];
	vector<RENDER_TEXTURE>			m_vRenderTextures[3];

	unique_ptr<SpriteBatch> spriteBatch;
	unique_ptr<SpriteFont>	spriteFont;

	CComPtr<ID3D11BlendState>		pBlendState;
	CComPtr<ID3D11PixelShader>		Gradient_PixelShader;
	CComPtr<ID3D11PixelShader>		MultiTexture_PixelShader;


	//Factory Methods
	virtual void SetupShadersAndInputLayout();
	void BlendState_Setup();

	// Helper Functions
	bool isValidString(char const* s); 

};

#endif