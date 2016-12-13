/***********************************************
* Filename:  		CRenderContext.h
* Date:      		10/2/2014
* Mod. Date: 		10/2/2014
* Mod. Initials:	JM
* Author:    		Jim Merrell
* Purpose:   		This is the RenderContext Class that holds common render data
************************************************/

#ifndef OPAQUEOBJ_RC_H
#define OPAQUEOBJ_RC_H

#include "RenderContext.h"

class OpaqueObj_RC : public CRenderContext
{
public:
	OpaqueObj_RC(void){}
	~OpaqueObj_RC(void);

	
	virtual void Initialize(ID3D11Device* device, ID3D11DeviceContext* immediateContext, void* texture_SRVs, SCENE* _scene);
	virtual void Run();
	
	virtual void AddRenderObject(Renderable* ren_obj);
	virtual void RemoveRenderObject(Entity3D* obj);

private:
	CComPtr<ID3D11VertexShader>		Anim_VShader;
	CComPtr<ID3D11VertexShader>		CollisionBox_VShader;
	CComPtr<ID3D11PixelShader>		CollisionBox_PShader;
	CComPtr<ID3D11InputLayout>		CollisionBox_InputLayout;
	
	CComPtr<ID3D11PixelShader>		Hand_PShader;

//Factory Methods
	virtual void SetupShadersAndInputLayout();

	void SetupConstBuffers();

	vector<Renderable*>		m_vRenderables;

	void CreateBoxFromAABB(Entity3D* ent);

	COLOR_RGBA				m_f4_currColor;
	CComPtr<ID3D11Buffer>	m_D3Dcolor_constBuffer;

	XMFLOAT4X4				m_4x4bindPosJoints[50];
	XMFLOAT4X4				m_4x4interpolatedJoints[50];
	CComPtr<ID3D11Buffer>	m_D3DBindPos_constbuffer;
	CComPtr<ID3D11Buffer>	m_D3DInterpolated_constbuffer;
	CComPtr<ID3D11Buffer>	m_D3DScrollingUV_constbuffer;



	XMFLOAT4X4				CollisionBox_WorldMatrix;
	CComPtr<ID3D11Buffer>	CollisionBox_Index_Buffer;
	CComPtr<ID3D11Buffer>	CollisionBox_VertexBuffer;
	vector<BASIC_VERTEX>	CollisionBox_box;

	void DrawCollisionBoxes();
	void RenderAnimatedObject(Renderable* RenObj);
	void RenderStaticObject(Renderable* RenObj);
	
	void OrthoNormalInverse(XMFLOAT4X4& r, const XMFLOAT4X4& m);

	bool DeadProjectile(Entity3D* entity); 
};

#endif