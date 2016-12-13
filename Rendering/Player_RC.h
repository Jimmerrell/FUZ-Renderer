/***********************************************
* Filename:  		StaticObj_RC.cpp
* Date:      		10/6/2014
* Mod. Date: 		10/6/14
* Mod. Initials:	JM
* Author:    		Jim Merrell
* Purpose:   		This is the RenderContext Class for player objects
************************************************/

#ifndef PLAYER_RC_H
#define PLAYER_RC_H

#include "RenderContext.h"

class Player_RC : public CRenderContext
{
public:
	Player_RC() {}
	~Player_RC();

	
	virtual void Initialize(ID3D11Device* device, ID3D11DeviceContext* immediateContext, void* texture_SRVs, SCENE* _scene);
	virtual void Run();
	
	virtual void AddRenderObject(Renderable* ren_obj);
	virtual void RemoveRenderObject(Entity3D* obj);


private:
	CComPtr<ID3D11VertexShader>		CollisionBox_VShader;
	CComPtr<ID3D11GeometryShader>	CollisionBox_GShader;
	CComPtr<ID3D11PixelShader>		CollisionBox_PShader;
	CComPtr<ID3D11InputLayout>		CollisionBox_InputLayout;

//Factory Methods
	virtual void SetupShadersAndInputLayout();

	void SetupConstBuffers();
	void SetupPlayerCapsul();
	void SetupSphere(DebugVolume& sphere, XMFLOAT4 color);

	vector<Renderable*>		m_vRenderables;
	list<Renderable*>		m_lDrawList;

	void SortDrawList();
	bool IsCloser(Entity3D* listObj, Entity3D* vecObj);
	void CreateBoxFromAABB(Entity3D* ent);

	COLOR_RGBA				m_f4_currColor;
	CComPtr<ID3D11Buffer>	m_D3Dcolor_constBuffer;

	CComPtr<ID3D11DepthStencilState>	pDSV_State_DepthEnabled;
	CComPtr<ID3D11DepthStencilState>	pDSV_State_DepthDisabled;

	XMFLOAT4X4				m_4x4bindPosJoints[50];
	XMFLOAT4X4				m_4x4interpolatedJoints[50];
	CComPtr<ID3D11Buffer>	m_D3DBindPos_constbuffer;
	CComPtr<ID3D11Buffer>	m_D3DInterpolated_constbuffer;
	CComPtr<ID3D11Buffer>	m_D3DScrollingUV_constbuffer;

	XMFLOAT4				m_f4_jointFuzData[50];
	CComPtr<ID3D11Buffer>	jointFuzData_CBuffer;


	XMFLOAT4X4				CollisionBox_WorldMatrix;
	CComPtr<ID3D11Buffer>	CollisionBox_Index_Buffer;
	CComPtr<ID3D11Buffer>	CollisionBox_VertexBuffer;
	vector<BASIC_VERTEX>	CollisionBox_box;

	CComPtr<ID3D11Buffer>	CollisionTriangleBuffer;
	vector<BASIC_VERTEX>	CollisionTriangleVerts;

	void DrawPlayerCapsul(XMFLOAT4X4 player_worldMatrix);
	void DrawSphere(DebugVolume* sphere, XMFLOAT3 worldPosition);
	void DrawCollisionBoxes();
	//void DrawCollisionTriangles(Entity3D* pPlayer, Renderable* pTerrain);
	void RenderAnimatedObject(Renderable* RenObj);
	
	void OrthoNormalInverse(XMFLOAT4X4& r, const XMFLOAT4X4& m);

	
	CModel player_capsul;
	vector<BASIC_VERTEX>	capsul_Mesh;
	vector<unsigned short>	capsul_indices;
	XMFLOAT4X4				capsul_worldMatrix;
	CComPtr<ID3D11Buffer>	capsul_vertexBuffer;
	CComPtr<ID3D11Buffer>	capsul_indexBuffer;
	CComPtr<ID3D11Buffer>	capsul_matrixBuffer;

	DebugVolume	playerOrigin;
	DebugVolume closestPointOnTri;
	DebugVolume closestPointOnCap;


};

#endif