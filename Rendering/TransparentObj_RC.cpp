/***********************************************
* Filename:  		TransparentObj_RC.cpp
* Date:      		10/6/2014
* Mod. Date: 		10/6/14
* Mod. Initials:	JM
* Author:    		Jim Merrell
* Purpose:   		This is the RenderContext Class for transparent game objects
************************************************/

#include "TransparentObj_RC.h"

#include "CompiledShaders\FBX_VS.csh"
#include "CompiledShaders\FBX_PS.csh"
#include "CompiledShaders\Hand_PS.csh"
#include "CompiledShaders\FBX_PS_GreyScale.csh"
#include "CompiledShaders\Anim_VS.csh"
#include "CompiledShaders\CollisionBox_PS.csh"
#include "CompiledShaders\CollisionBox_VS.csh"
#include "CompiledShaders\CollisionBox_GS.csh"
#include "CompiledShaders\Hull_Shader.csh"
#include "CompiledShaders\Domain_Shader.csh"
#include "CompiledShaders\Geometry_Shader.csh"

TransparentObj_RC::~TransparentObj_RC(void)
{
	for (int i = 0; i < (int)m_vRenderables.size(); i++)
	{
		delete m_vRenderables[i];
	}
	m_vRenderables.clear();
}

void TransparentObj_RC::Initialize(ID3D11Device* device, ID3D11DeviceContext* immediateContext, void* texture_SRVs, SCENE* _scene)
{
	pDevice = device;
	pImmediateContext = immediateContext;

	m_SRVs = (map<string, CComPtr<ID3D11ShaderResourceView>>*)texture_SRVs;
	scene = _scene;

	m_f4_currColor.colorRGBA = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	SetupShadersAndInputLayout();
	SetupRasterizerStates();
	SetupConstBuffers();
	SetupPlayerCapsul();
	m_bThermalRadarEffect = false;

	SetupSphere(playerOrigin, XMFLOAT4(1.0f, 0.35f, 0.0f, 1.0f));
	SetupSphere(closestPointOnTri, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	SetupSphere(closestPointOnCap, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

	CD3D11_DEPTH_STENCIL_DESC DSV_State_Desc(D3D11_DEFAULT);
	DSV_State_Desc.DepthEnable = true;
	DSV_State_Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DSV_State_Desc.DepthFunc = D3D11_COMPARISON_LESS;
	pDevice->CreateDepthStencilState(&DSV_State_Desc, &pDSV_State_DepthEnabled);

	DSV_State_Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	pDevice->CreateDepthStencilState(&DSV_State_Desc, &pDSV_State_DepthDisabled);
}

void TransparentObj_RC::OrthoNormalInverse(XMFLOAT4X4& r, const XMFLOAT4X4& m)
{
	//r.axis_x = vec3f(m.xx, m.yx, m.zx);
	r._11 = m._11;
	r._12 = m._21;
	r._13 = m._31;
	r._14 = 0.0f;
	//r.axis_y = vec3f(m.xy, m.yy, m.zy);
	r._21 = m._12;
	r._22 = m._22;
	r._23 = m._32;
	r._24 = 0.0f;
	//r.axis_z = vec3f(m.xz, m.yz, m.zz);
	r._31 = m._13;
	r._32 = m._23;
	r._33 = m._33;
	r._34 = 0.0f;

	r._41 = -XMVectorGetX(XMVector3Dot(XMLoadFloat3(&XMFLOAT3(r._11, r._12, r._13)), XMLoadFloat3(&XMFLOAT3(m._41, m._42, m._43))));		//dot_product(m.axis_x, m.axis_w);
	r._42 = -XMVectorGetX(XMVector3Dot(XMLoadFloat3(&XMFLOAT3(r._21, r._22, r._23)), XMLoadFloat3(&XMFLOAT3(m._41, m._42, m._43))));		//dot_product(m.axis_y, m.axis_w);
	r._43 = -XMVectorGetX(XMVector3Dot(XMLoadFloat3(&XMFLOAT3(r._31, r._32, r._33)), XMLoadFloat3(&XMFLOAT3(m._41, m._42, m._43))));		//dot_product(m.axis_z, m.axis_w);
	r._44 = 1.0f;
}

void TransparentObj_RC::Run()
{
	SortDrawList();

	pImmediateContext->VSSetShader(VertexShader, NULL, NULL);

	if(!m_bThermalRadarEffect)
		pImmediateContext->PSSetShader(PixelShader, NULL, NULL);
	else
		pImmediateContext->PSSetShader(Thermal_PS, NULL, NULL);

	pImmediateContext->IASetInputLayout(InputLayout);


	// draw the objects into the world
	list<Renderable*>::iterator itr;
	for (itr = m_lDrawList.begin(); itr != m_lDrawList.end(); itr++)
	{
		Renderable* RenObj = *itr;

		if(RenObj->object->getToRender() == false) // go to next object and don't bother drawing this one
			continue;



#if 0 // _DEBUG
		// this section will only work properly with one terrain object in the world
		if (RenObj->object->getObjectType() == Entity3D::PLAYER)
		{
			// draw player capsule
			XMFLOAT4X4 playerWorldMat;
			XMStoreFloat4x4(&playerWorldMat, RenObj->object->getWorldMatrix());
			DrawPlayerCapsul(playerWorldMat);
			XMFLOAT3 playerPosition = RenObj->object->getPosition();
			DrawSphere(&playerOrigin, playerPosition);

			XMVECTOR pPos = XMLoadFloat3(&playerPosition);

			XMFLOAT3 pointOnTri = reinterpret_cast<CPlayer*>(RenObj->object)->GetPointOnTri();
			XMVECTOR pOnT = XMLoadFloat3(&pointOnTri);
			pOnT = pPos + pOnT;
			XMStoreFloat3(&pointOnTri, pOnT);
			DrawSphere(&closestPointOnTri, pointOnTri);


			XMFLOAT3 pointOnCap = reinterpret_cast<CPlayer*>(RenObj->object)->GetPointOnCap();


			DrawSphere(&closestPointOnCap, pointOnCap);

			// find the first terrain obj
			Renderable* pTerrain = nullptr;
			for (int i = 0; i < (int)m_vRenderables.size(); i++)
			{
				if (m_vRenderables[i]->object->getObjectType() == Entity3D::TERRAIN)
				{
					pTerrain = m_vRenderables[i];
				}
			}

			//if(pTerrain != nullptr)
			//DrawCollisionTriangles(RenObj->object, pTerrain);
		}
#endif

		// map the object color
		{
			//if(TESSELATION)
			//{
			//	m_f4_currColor.colorRGBA.w = (float)m_nTesselationLevel;
			//	m_f4_currColor.colorRGBA.x = m_fTestXPos;
			//	m_f4_currColor.colorRGBA.z = m_fTestZPos;
			//}
			//else
			m_f4_currColor.colorRGBA = RenObj->object->getColorRGBA();

			if(m_bThermalRadarEffect)
				if(RenObj->object->getObjectType() == Entity3D::PLAYER)
					m_f4_currColor.colorRGBA.w = 1.0f;



			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(mappedResource));
			pImmediateContext->Map(m_D3Dcolor_constBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource);

			memcpy(mappedResource.pData, &m_f4_currColor, sizeof(m_f4_currColor));

			pImmediateContext->Unmap(m_D3Dcolor_constBuffer, 0);

			ID3D11Buffer* trans_Buffer[1] = { m_D3Dcolor_constBuffer };
			pImmediateContext->VSSetConstantBuffers(2, 1, trans_Buffer);
			pImmediateContext->HSSetConstantBuffers( 2, 1, trans_Buffer );
			pImmediateContext->DSSetConstantBuffers( 2, 1, trans_Buffer );
			pImmediateContext->GSSetConstantBuffers( 2, 1, trans_Buffer );

		}

		string textureName = RenObj->object->getTextureKey();
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

		XMFLOAT4X4 worldMatrix;
		XMStoreFloat4x4(&worldMatrix, RenObj->object->getWorldMatrix());

		// map the matrix buffer
		{
			D3D11_MAPPED_SUBRESOURCE MappedResource;
			ZeroMemory(&MappedResource, sizeof(MappedResource));
			pImmediateContext->Map(RenObj->matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &MappedResource);
			memcpy(MappedResource.pData, &worldMatrix, sizeof(worldMatrix));
			pImmediateContext->Unmap(RenObj->matrixBuffer, 0);

			ID3D11Buffer* constbuffer[1] = { RenObj->matrixBuffer };
			pImmediateContext->VSSetConstantBuffers(0, 1, constbuffer);
			pImmediateContext->HSSetConstantBuffers( 0, 1, constbuffer );
			pImmediateContext->DSSetConstantBuffers( 0, 1, constbuffer );
		}

		// map the fuz data buffer
		{
			D3D11_MAPPED_SUBRESOURCE MappedResource;
			ZeroMemory(&MappedResource, sizeof(MappedResource));
			pImmediateContext->Map(RenObj->fuzDataBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &MappedResource);
			memcpy(MappedResource.pData, &RenObj->object->getFuzData(), sizeof(FUZ_COLLISION_DATA));
			pImmediateContext->Unmap(RenObj->fuzDataBuffer, 0);

			ID3D11Buffer* constbuffer[1] = { RenObj->fuzDataBuffer };
			pImmediateContext->HSSetConstantBuffers(4, 1, constbuffer);
			pImmediateContext->GSSetConstantBuffers(4, 1, constbuffer);
		}

		// Map the UV Scrolling const buffer
		{
			XMFLOAT4 uvOffsets = RenObj->object->GetUVOffsets();
			D3D11_MAPPED_SUBRESOURCE MappedResource;
			ZeroMemory(&MappedResource, sizeof(MappedResource));
			pImmediateContext->Map(RenObj->scrollingUVBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &MappedResource);
			memcpy(MappedResource.pData, &uvOffsets, sizeof(XMFLOAT4));
			pImmediateContext->Unmap(RenObj->scrollingUVBuffer, 0);

			ID3D11Buffer* constbuffer[1] = { RenObj->scrollingUVBuffer };
			pImmediateContext->VSSetConstantBuffers(5, 1, constbuffer);
		}

		ID3D11Buffer* bufferarr[1] = { RenObj->vertexBuffer };
		UINT ArrStrides[1] = { sizeof(VERTEX_3D) };
		UINT ArrOffsets[1] = { 0 };
		pImmediateContext->IASetVertexBuffers(0, 1, bufferarr, ArrStrides, ArrOffsets);

		// ANIMATION VS
		//check if objet is animated
		// if animated, use animated VS
		//		set up const buffers for that object
		// else use normal VS
		if (RenObj->object->isAnimating() &&
			RenObj->object->GetInterpolator()->GetInterpolatedJoints().size() > 0)
		{
			//if(RenObj->object->getModelKey() == "newScientistHand")
			pImmediateContext->PSSetShader(Hand_PShader, NULL, NULL);

			RenderAnimatedObject(RenObj);
			
			if(!m_bThermalRadarEffect)
				pImmediateContext->PSSetShader(PixelShader, NULL, NULL);
			else
				pImmediateContext->PSSetShader(Thermal_PS, NULL, NULL);
		}
		else
		{			
			if(TESSELATION)
			{
				pImmediateContext->HSSetShader(HullShader, NULL, NULL);
				pImmediateContext->DSSetShader(DomainShader, NULL, NULL);
				pImmediateContext->GSSetShader(GeometryShader, NULL, NULL);
			}

			if(RenObj->object->getObjectType() == Entity3D::VFX_MESH)
			{
				pImmediateContext->OMSetDepthStencilState(pDSV_State_DepthDisabled, NULL);	
				RenderStaticObject(RenObj);
				pImmediateContext->OMSetDepthStencilState(pDSV_State_DepthEnabled, NULL);
			}
			else
				RenderStaticObject(RenObj);

			pImmediateContext->HSSetShader(NULL, NULL, NULL);
			pImmediateContext->DSSetShader(NULL, NULL, NULL);
			pImmediateContext->GSSetShader(NULL, NULL, NULL);
		}

	}
	pImmediateContext->RSSetState(m_pCullBack_RS);

	//pImmediateContext->VSSetShader(NULL, NULL, NULL);
	//pImmediateContext->HSSetShader(NULL, NULL, NULL);
	//pImmediateContext->DSSetShader(NULL, NULL, NULL);
	//pImmediateContext->GSSetShader(NULL, NULL, NULL);
	//pImmediateContext->PSSetShader(NULL, NULL, NULL);

	//DrawCollisionBoxes();
}

void TransparentObj_RC::RenderAnimatedObject(Renderable* RenObj)
{
	unsigned int jointCount = RenObj->object->GetModel()->GetMeshes()[0].GetJoints().size();
	for (unsigned int i = 0; i < jointCount; i++)
	{
		XMFLOAT4X4 invBind;
		OrthoNormalInverse(invBind, RenObj->object->GetModel()->GetMeshes()[0].GetJoints()[i].world_bind_pose_transform);
		m_4x4bindPosJoints[i] = invBind;
		if (RenObj->object->GetBlendTimer() > 0.0f)
		{
			m_4x4interpolatedJoints[i] = RenObj->object->GetBlendedJoints()[i];
		}
		else
		{
			m_4x4interpolatedJoints[i] = RenObj->object->GetInterpolator()->GetInterpolatedJoints()[i];
		}
	}
	//jointCount = RenObj->object->GetModel()->GetMeshes()[0].GetJoints().size();
	//for (unsigned int i = 0; i < jointCount; i++)
	//{
	//	m_4x4bindPosJoints[i] = RenObj->object->GetModel()->GetMeshes()[0].GetJoints()[i].world_bind_pose_transform;
	//	m_4x4interpolatedJoints[i] = RenObj->object->GetInterpolator()->GetInterpolatedJoints()[i];
	//}
	/*jointCount = RenObj->object->GetModel()->GetMeshes()[0].GetJoints().size();
	for (unsigned int i = 0; i < jointCount; i++)
	{
	XMFLOAT4X4 invBind;
	OrthoNormalInverse(invBind, RenObj->object->GetModel()->GetMeshes()[0].GetJoints()[i].world_bind_pose_transform);
	m_4x4bindPosJoints[i] = invBind;
	m_4x4interpolatedJoints[i] = RenObj->object->GetInterpolator()->GetInterpolatedJoints()[i];
	}*/

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(mappedResource));
	pImmediateContext->Map(m_D3DBindPos_constbuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource);

	memcpy(mappedResource.pData, &m_4x4bindPosJoints[0], sizeof(m_4x4bindPosJoints[0]) * 50);

	pImmediateContext->Unmap(m_D3DBindPos_constbuffer, 0);

	ID3D11Buffer* Buffer[1] = { m_D3DBindPos_constbuffer };
	pImmediateContext->VSSetConstantBuffers(3, 1, Buffer);
	//pImmediateContext->HSSetConstantBuffers( 3, 1, Buffer );
	//pImmediateContext->DSSetConstantBuffers( 3, 1, Buffer );

	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(subResource));
	pImmediateContext->Map(m_D3DInterpolated_constbuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &subResource);

	memcpy(subResource.pData, &m_4x4interpolatedJoints[0], sizeof(m_4x4interpolatedJoints[0]) * 50);

	pImmediateContext->Unmap(m_D3DInterpolated_constbuffer, 0);

	ID3D11Buffer* buff[1] = { m_D3DInterpolated_constbuffer };
	pImmediateContext->VSSetConstantBuffers(4, 1, buff);
	//pImmediateContext->HSSetConstantBuffers( 4, 1, buff );
	//pImmediateContext->DSSetConstantBuffers( 4, 1, buff );

	pImmediateContext->VSSetShader(Anim_VShader, NULL, NULL);

	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	pImmediateContext->IASetIndexBuffer(RenObj->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// first draw on object
	{
		if(!m_bWireFrame)
			pImmediateContext->RSSetState(m_pCullBack_RS);
		else
			pImmediateContext->RSSetState(m_pWireFrame_CullBack_RS);

		pImmediateContext->DrawIndexed(RenObj->object->getRenderIndex().size(), 0, 0);
	}

	//second draw on object
	{
		if(!m_bWireFrame || (m_bThermalRadarEffect && RenObj->object->getObjectType() == Entity3D::PLAYER))
			pImmediateContext->RSSetState(m_pCullFront_RS);
		else
			pImmediateContext->RSSetState(m_pWireFrame_CullFront_RS);

		pImmediateContext->DrawIndexed(RenObj->object->getRenderIndex().size(), 0, 0);
	}

	pImmediateContext->VSSetShader(VertexShader, NULL, NULL);
}

void TransparentObj_RC::RenderStaticObject(Renderable* RenObj)
{
	pImmediateContext->IASetInputLayout(InputLayout);

	if(!TESSELATION)
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	else
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	pImmediateContext->IASetIndexBuffer(RenObj->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// first draw on object
	{
		if(!m_bWireFrame)
			pImmediateContext->RSSetState(m_pCullBack_RS);
		else
			pImmediateContext->RSSetState(m_pWireFrame_CullBack_RS);

		pImmediateContext->DrawIndexed(RenObj->object->getRenderIndex().size(), 0, 0);
	}

	//second draw on object
	{
		if(!m_bWireFrame || (m_bThermalRadarEffect && RenObj->object->getObjectType() == Entity3D::PLAYER))
			pImmediateContext->RSSetState(m_pCullFront_RS);
		else
			pImmediateContext->RSSetState(m_pWireFrame_CullFront_RS);

		pImmediateContext->DrawIndexed(RenObj->object->getRenderIndex().size(), 0, 0);
	}
}

void TransparentObj_RC::DrawPlayerCapsul(XMFLOAT4X4 player_worldMatrix)
{
	// set shaders to collision shaders
	pImmediateContext->VSSetShader(CollisionBox_VShader, NULL, NULL);
	pImmediateContext->PSSetShader(CollisionBox_PShader, NULL, NULL);
	pImmediateContext->IASetInputLayout(CollisionBox_InputLayout);

	// set up and Map the world matrix
	XMMATRIX worldMat = XMMatrixIdentity();
	XMMATRIX playerWorldMat = XMLoadFloat4x4(&player_worldMatrix);
	XMVECTOR worldPos = playerWorldMat.r[3];
	XMMATRIX translate = XMMatrixTranslationFromVector(worldPos);
	worldMat = translate * worldMat;

	XMStoreFloat4x4(&capsul_worldMatrix, worldMat);

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	ZeroMemory(&MappedResource, sizeof(MappedResource));
	pImmediateContext->Map(capsul_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &MappedResource);
	memcpy(MappedResource.pData, &capsul_worldMatrix, sizeof(capsul_worldMatrix));
	pImmediateContext->Unmap(capsul_matrixBuffer, 0);

	ID3D11Buffer* constbuffer[1] = { capsul_matrixBuffer };
	pImmediateContext->VSSetConstantBuffers(0, 1, constbuffer);

	// setup vertex buffer
	ID3D11Buffer* bufferarr[1] = { capsul_vertexBuffer };
	UINT ArrStrides[1] = { sizeof(BASIC_VERTEX) };
	UINT ArrOffsets[1] = { 0 };
	pImmediateContext->IASetVertexBuffers(0, 1, bufferarr, ArrStrides, ArrOffsets);

	// setup index buffer
	pImmediateContext->IASetIndexBuffer(capsul_indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// make draw call
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	pImmediateContext->RSSetState(m_pCullBack_RS);
	pImmediateContext->DrawIndexed(capsul_indices.size(), 0, 0);

	// set shaders back to normal shaders
	pImmediateContext->VSSetShader(VertexShader, NULL, NULL);
	pImmediateContext->PSSetShader(PixelShader, NULL, NULL);
	pImmediateContext->IASetInputLayout(InputLayout);
}

void TransparentObj_RC::DrawSphere(DebugVolume* sphere, XMFLOAT3 worldPosition)
{
	// set shaders to collision shaders
	pImmediateContext->VSSetShader(CollisionBox_VShader, NULL, NULL);
	pImmediateContext->PSSetShader(CollisionBox_PShader, NULL, NULL);
	pImmediateContext->IASetInputLayout(CollisionBox_InputLayout);

	// setup, map and pass the world matrix
	XMMATRIX worldMat = XMMatrixIdentity();
	XMVECTOR worldPos = XMLoadFloat3(&worldPosition);
	XMMATRIX translate = XMMatrixTranslationFromVector(worldPos);
	worldMat = translate * worldMat;
	XMStoreFloat4x4(&sphere->worldMatrix, worldMat);

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	ZeroMemory(&MappedResource, sizeof(MappedResource));
	pImmediateContext->Map(sphere->matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &MappedResource);
	memcpy(MappedResource.pData, &sphere->worldMatrix, sizeof(sphere->worldMatrix));
	pImmediateContext->Unmap(sphere->matrixBuffer, 0);

	ID3D11Buffer* constbuffer[1] = { sphere->matrixBuffer };
	pImmediateContext->VSSetConstantBuffers(0, 1, constbuffer);

	// pass the vertex buffer
	ID3D11Buffer* bufferarr[1] = { sphere->vertex_Buffer };
	UINT ArrStrides[1] = { sizeof(BASIC_VERTEX) };
	UINT ArrOffsets[1] = { 0 };
	pImmediateContext->IASetVertexBuffers(0, 1, bufferarr, ArrStrides, ArrOffsets);

	// pass the index buffer
	pImmediateContext->IASetIndexBuffer(capsul_indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// make draw call
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pImmediateContext->DrawIndexed(sphere->indices.size(), 0, 0);

	// set shaders back to normal shaders
	pImmediateContext->VSSetShader(VertexShader, NULL, NULL);
	pImmediateContext->PSSetShader(PixelShader, NULL, NULL);
	pImmediateContext->IASetInputLayout(InputLayout);
}

void TransparentObj_RC::DrawCollisionBoxes()
{
	pImmediateContext->VSSetShader(CollisionBox_VShader, NULL, NULL);
	pImmediateContext->PSSetShader(CollisionBox_PShader, NULL, NULL);
	pImmediateContext->IASetInputLayout(CollisionBox_InputLayout);

	list<Renderable*>::iterator itr;
	for (itr = m_lDrawList.begin(); itr != m_lDrawList.end(); itr++)
	{
		Renderable* RenObj = *itr;
		// draw a box based on the aabb

		XMFLOAT4X4 worldMatrix;
		XMStoreFloat4x4(&worldMatrix, RenObj->object->getWorldMatrix());

		D3D11_MAPPED_SUBRESOURCE MappedResource;
		ZeroMemory(&MappedResource, sizeof(MappedResource));
		pImmediateContext->Map(RenObj->matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &MappedResource);
		memcpy(MappedResource.pData, &worldMatrix, sizeof(worldMatrix));
		pImmediateContext->Unmap(RenObj->matrixBuffer, 0);

		ID3D11Buffer* constbuffer[1] = { RenObj->matrixBuffer };
		pImmediateContext->VSSetConstantBuffers(0, 1, constbuffer);

		CreateBoxFromAABB(RenObj->object);

		ID3D11Buffer* buffarr[1] = { CollisionBox_VertexBuffer };
		UINT ArrStride[1] = { sizeof(BASIC_VERTEX) };
		UINT ArrOffset[1] = { 0 };
		pImmediateContext->IASetVertexBuffers(0, 1, buffarr, ArrStride, ArrOffset);

		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		pImmediateContext->IASetIndexBuffer(CollisionBox_Index_Buffer, DXGI_FORMAT_R16_UINT, 0);

		pImmediateContext->DrawIndexed(36, 0, 0);

	}
}

/*
void TransparentObj_RC::DrawCollisionTriangles(Entity3D* pPlayer, Renderable* pTerrain)
{
// setup vertex buffer


int triangle_count = reinterpret_cast<CPlayer*>(pPlayer)->getCollisionInfoVec().size();

for (int triIndex = 0; triIndex < triangle_count; triIndex++)
{
int collTriIndex = reinterpret_cast<CPlayer*>(pPlayer)->getCollisionInfoVec()[triIndex].nTriIndex;

for (int i = 0; i < 3; i++)
{
BASIC_VERTEX tempVert;

tempVert.coordinate = pTerrain->object->GetModel()->GetMeshes()[0].GetPolygons()[collTriIndex].verts[i];
tempVert.color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

CollisionTriangleVerts.push_back(tempVert);
}
}

if (CollisionTriangleVerts.size() > 0)
{
CComPtr<ID3D11Buffer> tempVertexBuffer;

D3D11_BUFFER_DESC Vertex_Buffer_Desc;
ZeroMemory(&Vertex_Buffer_Desc, sizeof(Vertex_Buffer_Desc));
Vertex_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;
Vertex_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
Vertex_Buffer_Desc.CPUAccessFlags = NULL;
Vertex_Buffer_Desc.ByteWidth = sizeof(BASIC_VERTEX) * CollisionTriangleVerts.size();

D3D11_SUBRESOURCE_DATA objData;
ZeroMemory(&objData, sizeof(objData));
objData.pSysMem = &CollisionTriangleVerts[0];
pDevice->CreateBuffer(&Vertex_Buffer_Desc, &objData, &tempVertexBuffer);

CollisionTriangleBuffer = tempVertexBuffer;

// set shaders to collision shaders
pImmediateContext->VSSetShader(CollisionBox_VShader, NULL, NULL);
pImmediateContext->GSSetShader(CollisionBox_GShader, NULL, NULL);
pImmediateContext->PSSetShader(CollisionBox_PShader, NULL, NULL);
pImmediateContext->IASetInputLayout(CollisionBox_InputLayout);

//set the matrix constant buffer
XMFLOAT4X4 worldMatrix;
XMStoreFloat4x4(&worldMatrix, pTerrain->object->getWorldMatrix());

D3D11_MAPPED_SUBRESOURCE MappedResource;
ZeroMemory(&MappedResource, sizeof(MappedResource));
pImmediateContext->Map(pTerrain->matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &MappedResource);
memcpy(MappedResource.pData, &worldMatrix, sizeof(worldMatrix));
pImmediateContext->Unmap(pTerrain->matrixBuffer, 0);

ID3D11Buffer* constbuffer[1] = { pTerrain->matrixBuffer };
pImmediateContext->VSSetConstantBuffers(0, 1, constbuffer);

// make draw call
ID3D11Buffer* buffarr[1] = { CollisionTriangleBuffer };
UINT ArrStride[1] = { sizeof(BASIC_VERTEX) };
UINT ArrOffset[1] = { 0 };
pImmediateContext->IASetVertexBuffers(0, 1, buffarr, ArrStride, ArrOffset);

pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

pImmediateContext->RSSetState(m_pCullFront_RS);
pImmediateContext->Draw(CollisionTriangleVerts.size(), 0);

pImmediateContext->RSSetState(m_pCullBack_RS);
pImmediateContext->Draw(CollisionTriangleVerts.size(), 0);

// set shaders back to normal shaders
pImmediateContext->VSSetShader(VertexShader, NULL, NULL);
pImmediateContext->GSSetShader(NULL, NULL, NULL);
pImmediateContext->PSSetShader(PixelShader, NULL, NULL);
pImmediateContext->IASetInputLayout(InputLayout);

CollisionTriangleVerts.clear();
}



}
*/

void TransparentObj_RC::SetupShadersAndInputLayout()
{
	pDevice->CreateVertexShader(FBX_VS, sizeof(FBX_VS), NULL, &VertexShader);
	pDevice->CreateVertexShader(Anim_VS, sizeof(Anim_VS), NULL, &Anim_VShader);

	pDevice->CreateHullShader(Hull_Shader, sizeof(Hull_Shader), NULL, &HullShader);
	pDevice->CreateDomainShader(Domain_Shader, sizeof(Domain_Shader), NULL, &DomainShader);
	pDevice->CreateGeometryShader(Geometry_Shader, sizeof(Geometry_Shader), NULL, &GeometryShader);

	pDevice->CreatePixelShader(FBX_PS, sizeof(FBX_PS), NULL, &PixelShader);
	pDevice->CreatePixelShader(FBX_PS_GreyScale, sizeof(FBX_PS_GreyScale), NULL, &Thermal_PS);
	pDevice->CreatePixelShader(Hand_PS, sizeof(Hand_PS), NULL, &Hand_PShader);

	D3D11_INPUT_ELEMENT_DESC vLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	pDevice->CreateInputLayout(vLayout, 5, &FBX_VS, sizeof(FBX_VS), &InputLayout);


	pDevice->CreateVertexShader(CollisionBox_VS, sizeof(CollisionBox_VS), NULL, &CollisionBox_VShader);
	pDevice->CreateGeometryShader(CollisionBox_GS, sizeof(CollisionBox_GS), NULL, &CollisionBox_GShader);
	pDevice->CreatePixelShader(CollisionBox_PS, sizeof(CollisionBox_PS), NULL, &CollisionBox_PShader);

	D3D11_INPUT_ELEMENT_DESC vertLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	pDevice->CreateInputLayout(vertLayout, 2, &CollisionBox_VS, sizeof(CollisionBox_VS), &CollisionBox_InputLayout);

}

void TransparentObj_RC::AddRenderObject(Renderable* ren_obj)
{
	m_vRenderables.push_back(ren_obj);
}

void TransparentObj_RC::RemoveRenderObject(Entity3D* obj)
{
	// remove the object pointer

	vector<Renderable*>::iterator itr;
	for (itr = m_vRenderables.begin(); itr != m_vRenderables.end();)
	{
		Renderable* ren = *itr;
		if (ren->object == obj)
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

void TransparentObj_RC::SortDrawList()
{
	// sort from largest distance from camera to smallest
	// depth = viewDirection dotted with obj.pos
	m_lDrawList.clear();

	for (int i = 0; i < (int)m_vRenderables.size(); i++)
	{
		if (m_lDrawList.size() == 0)
		{
			// Add the first object to the drawList
			m_lDrawList.push_back(m_vRenderables[i]);
			continue;
		}

		if (m_vRenderables[i]->object->getObjectType() == Entity3D::TERRAIN)
		{
			m_lDrawList.insert(m_lDrawList.begin(), m_vRenderables[i]);
		}
		else
		{
			// Do an insertion sort for the remaining objects
			// go through the list and find which two objs it belongs between or place it at the end if it is closest
			list<Renderable*>::iterator itr;
			itr = m_lDrawList.begin();
			while (true)
			{
				if (itr == m_lDrawList.end())
				{
					m_lDrawList.push_back(m_vRenderables[i]);
					break;
				}

				Renderable* ren = *itr;

				if (IsCloser(ren->object, m_vRenderables[i]->object)
					&& ren->object->getObjectType() != Entity3D::TERRAIN)
				{
					m_lDrawList.insert(itr, m_vRenderables[i]);
					break;
				}
				else
				{
					itr++;
					continue;
				}
			}
		}
	}
}

bool TransparentObj_RC::IsCloser(Entity3D* listObj, Entity3D* vecObj)
{
	// depth = viewDirection dotted with obj.pos
	float depth_One = 10000.0f;
	float depth_Two = 10000.0f;

	XMMATRIX viewMat = XMLoadFloat4x4(&scene->viewMatrix);
	XMMATRIX inverse = XMMatrixInverse(NULL, viewMat);

	XMVECTOR viewDir = inverse.r[2];

	XMVECTOR objPos = XMLoadFloat3(&listObj->getPosition());
	//objPos = XMVector3Transform(objPos, viewMat);

	XMVECTOR camPos = inverse.r[3];

	XMVECTOR toObj = objPos - camPos;
	//toObj = XMVector3Normalize(toObj);

	XMVECTOR vecObj_Depth = XMVector3Dot(viewDir, toObj);
	XMStoreFloat(&depth_One, vecObj_Depth);

	XMVECTOR objPos2 = XMLoadFloat3(&vecObj->getPosition());
	//objPos2 = XMVector3Transform(objPos2, viewMat);

	XMVECTOR toObj2 = objPos2 - camPos;
	//toObj2 = XMVector3Normalize(toObj2);

	XMVECTOR listObj_Depth = XMVector3Dot(viewDir, toObj2);
	XMStoreFloat(&depth_Two, listObj_Depth);

	//if(objPos.m128_f32[2] < objPos2.m128_f32[2])
	if (abs(depth_One) < abs(depth_Two))
		return true;
	else
		return false;
}

void TransparentObj_RC::SetupConstBuffers()
{

	D3D11_BUFFER_DESC trans_Buffer_Desc;
	ZeroMemory(&trans_Buffer_Desc, sizeof(trans_Buffer_Desc));
	trans_Buffer_Desc.ByteWidth = sizeof(XMFLOAT4);
	trans_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;
	trans_Buffer_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	trans_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA trans_Data;
	trans_Data.pSysMem = &m_f4_currColor;
	trans_Data.SysMemPitch = 0;
	trans_Data.SysMemSlicePitch = 0;

	pDevice->CreateBuffer(&trans_Buffer_Desc, &trans_Data, &m_D3Dcolor_constBuffer);

	// ANIMATION STUFF
	D3D11_BUFFER_DESC bindpos_Buffer_Desc;
	ZeroMemory(&bindpos_Buffer_Desc, sizeof(bindpos_Buffer_Desc));
	bindpos_Buffer_Desc.ByteWidth = sizeof(XMFLOAT4X4) * 50;
	bindpos_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;
	bindpos_Buffer_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bindpos_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA bindpos_trans_Data;
	bindpos_trans_Data.pSysMem = &m_4x4bindPosJoints[0];
	bindpos_trans_Data.SysMemPitch = 0;
	bindpos_trans_Data.SysMemSlicePitch = 0;

	pDevice->CreateBuffer(&bindpos_Buffer_Desc, &bindpos_trans_Data, &m_D3DBindPos_constbuffer);

	D3D11_BUFFER_DESC interpolated_Buffer_Desc;
	ZeroMemory(&interpolated_Buffer_Desc, sizeof(interpolated_Buffer_Desc));
	interpolated_Buffer_Desc.ByteWidth = sizeof(XMFLOAT4X4) * 50;
	interpolated_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;
	interpolated_Buffer_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	interpolated_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA interpolated_trans_Data;
	interpolated_trans_Data.pSysMem = &m_4x4interpolatedJoints[0];
	interpolated_trans_Data.SysMemPitch = 0;
	interpolated_trans_Data.SysMemSlicePitch = 0;

	pDevice->CreateBuffer(&interpolated_Buffer_Desc, &interpolated_trans_Data, &m_D3DInterpolated_constbuffer);

}

void TransparentObj_RC::CreateBoxFromAABB(Entity3D* ent)
{
	CollisionBox_box.clear();
	AABB aabb = ent->GetAABB();
	//build the vertlist
	BASIC_VERTEX tempVert;
	for (unsigned int i = 0; i < 8; i++)
	{
		tempVert.color.x = 0.0f; //(rand() % 100) * 0.01f;
		tempVert.color.y = 1.0f; //(rand() % 100) * 0.01f;
		tempVert.color.z = 0.0f; //(rand() % 100) * 0.01f;
		tempVert.color.w = 1.0f;

		switch (i)
		{
		case 0: // top left
			tempVert.coordinate.x = aabb.f3_LowerLeft.x;//-.5;
			tempVert.coordinate.y = aabb.f3_UpperRight.y;//.5;
			tempVert.coordinate.z = aabb.f3_LowerLeft.z;//-.5;
			break;
		case 1: // top right
			tempVert.coordinate.x = aabb.f3_UpperRight.x;//.5;
			tempVert.coordinate.y = aabb.f3_UpperRight.y;//.5;
			tempVert.coordinate.z = aabb.f3_LowerLeft.z;//-.5;
			break;
		case 2: // bottom right
			tempVert.coordinate.x = aabb.f3_UpperRight.x;//.5;
			tempVert.coordinate.y = aabb.f3_LowerLeft.y;//-.5;
			tempVert.coordinate.z = aabb.f3_LowerLeft.z;//-.5;
			break;
		case 3: // bottom left
			tempVert.coordinate.x = aabb.f3_LowerLeft.x;//-.5;
			tempVert.coordinate.y = aabb.f3_LowerLeft.y;//-.5;
			tempVert.coordinate.z = aabb.f3_LowerLeft.z;//-.5;
			break;
		case 4: // rear top left
			tempVert.coordinate.x = aabb.f3_LowerLeft.x;//-.5;
			tempVert.coordinate.y = aabb.f3_UpperRight.y;//.5;
			tempVert.coordinate.z = aabb.f3_UpperRight.z;//.5;
			break;
		case 5: // rear top right
			tempVert.coordinate.x = aabb.f3_UpperRight.x;//.5;
			tempVert.coordinate.y = aabb.f3_UpperRight.y;//.5;
			tempVert.coordinate.z = aabb.f3_UpperRight.z;//.5;
			break;
		case 6: // rear bottom right
			tempVert.coordinate.x = aabb.f3_UpperRight.x;//.5;
			tempVert.coordinate.y = aabb.f3_LowerLeft.y;//-.5;
			tempVert.coordinate.z = aabb.f3_UpperRight.z;//.5;
			break;
		case 7: // rear bottom left
			tempVert.coordinate.x = aabb.f3_LowerLeft.x;//-.5;
			tempVert.coordinate.y = aabb.f3_LowerLeft.y;//-.5;
			tempVert.coordinate.z = aabb.f3_UpperRight.z;//.5;
			break;
		}

		CollisionBox_box.push_back(tempVert);
	}

	//build the index list
	unsigned short numIndices = 36;
	unsigned short Indices[36] = {
		4, 5, 7,
		5, 6, 7,

		7, 6, 3,
		6, 2, 3,

		6, 5, 2,
		2, 5, 1,

		4, 7, 0,
		0, 7, 3,

		3, 2, 0,
		0, 2, 1,

		0, 1, 4,
		1, 5, 4
	};


	CComPtr<ID3D11Buffer> tempVertexBuffer;

	D3D11_BUFFER_DESC Vertex_Buffer_Desc;
	ZeroMemory(&Vertex_Buffer_Desc, sizeof(Vertex_Buffer_Desc));
	Vertex_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;
	Vertex_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	Vertex_Buffer_Desc.CPUAccessFlags = NULL;
	Vertex_Buffer_Desc.ByteWidth = sizeof(BASIC_VERTEX) * CollisionBox_box.size();

	D3D11_SUBRESOURCE_DATA objData;
	ZeroMemory(&objData, sizeof(objData));
	objData.pSysMem = &CollisionBox_box[0];
	pDevice->CreateBuffer(&Vertex_Buffer_Desc, &objData, &tempVertexBuffer);

	CollisionBox_VertexBuffer = tempVertexBuffer;

	// build and add the index buffer
	CComPtr<ID3D11Buffer> tempIndexBuffer;

	D3D11_BUFFER_DESC Index_Buffer_Desc;
	ZeroMemory(&Index_Buffer_Desc, sizeof(Index_Buffer_Desc));
	Index_Buffer_Desc.Usage = D3D11_USAGE_IMMUTABLE;
	Index_Buffer_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	Index_Buffer_Desc.CPUAccessFlags = NULL;
	Index_Buffer_Desc.ByteWidth = sizeof(unsigned short) * 36;

	D3D11_SUBRESOURCE_DATA IndexData;
	ZeroMemory(&IndexData, sizeof(IndexData));
	IndexData.pSysMem = &Indices[0];
	pDevice->CreateBuffer(&Index_Buffer_Desc, &IndexData, &tempIndexBuffer);

	CollisionBox_Index_Buffer = tempIndexBuffer;
}

void TransparentObj_RC::SetupPlayerCapsul()
{
	player_capsul.LoadMeshesFromFBX("../../Assets/Models/Player_Capsul.fbx");

	for (int i = 0; i < (int)player_capsul.GetMeshes()[0].GetVertices().size(); i++)
	{
		BASIC_VERTEX tempVert;
		tempVert.coordinate = player_capsul.GetMeshes()[0].GetVertices()[i].position;
		tempVert.color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		capsul_Mesh.push_back(tempVert);
	}

	CComPtr<ID3D11Buffer> tempVertexBuffer;

	D3D11_BUFFER_DESC Vertex_Buffer_Desc;
	ZeroMemory(&Vertex_Buffer_Desc, sizeof(Vertex_Buffer_Desc));
	Vertex_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;
	Vertex_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	Vertex_Buffer_Desc.CPUAccessFlags = NULL;
	Vertex_Buffer_Desc.ByteWidth = sizeof(BASIC_VERTEX) * capsul_Mesh.size();

	D3D11_SUBRESOURCE_DATA objData;
	ZeroMemory(&objData, sizeof(objData));
	objData.pSysMem = &capsul_Mesh[0];
	pDevice->CreateBuffer(&Vertex_Buffer_Desc, &objData, &tempVertexBuffer);

	capsul_vertexBuffer = tempVertexBuffer;

	// set up indices
	for (int i = 0; i < (int)player_capsul.GetMeshes()[0].GetIndices().size(); i++)
	{
		capsul_indices.push_back(player_capsul.GetMeshes()[0].GetIndices()[i]);
	}

	// build and add the index buffer
	CComPtr<ID3D11Buffer> tempIndexBuffer;

	D3D11_BUFFER_DESC Index_Buffer_Desc;
	ZeroMemory(&Index_Buffer_Desc, sizeof(Index_Buffer_Desc));
	Index_Buffer_Desc.Usage = D3D11_USAGE_IMMUTABLE;
	Index_Buffer_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	Index_Buffer_Desc.CPUAccessFlags = NULL;
	Index_Buffer_Desc.ByteWidth = sizeof(unsigned short)* capsul_indices.size();

	D3D11_SUBRESOURCE_DATA IndexData;
	ZeroMemory(&IndexData, sizeof(IndexData));
	IndexData.pSysMem = &capsul_indices[0];
	pDevice->CreateBuffer(&Index_Buffer_Desc, &IndexData, &tempIndexBuffer);

	capsul_indexBuffer = tempIndexBuffer;

	// build and add the local const buffer for the matrix
	D3D11_BUFFER_DESC obj_Buffer_Desc;
	ZeroMemory(&obj_Buffer_Desc, sizeof(obj_Buffer_Desc));
	obj_Buffer_Desc.ByteWidth = sizeof(XMMATRIX);
	obj_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;
	obj_Buffer_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	obj_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA Object_Data;
	Object_Data.pSysMem = &capsul_worldMatrix;
	Object_Data.SysMemPitch = 0;
	Object_Data.SysMemSlicePitch = 0;

	CComPtr<ID3D11Buffer> tempMatrixBuffer;
	pDevice->CreateBuffer(&obj_Buffer_Desc, &Object_Data, &tempMatrixBuffer);

	capsul_matrixBuffer = tempMatrixBuffer;
}

void TransparentObj_RC::SetupSphere(DebugVolume& sphere, XMFLOAT4 color)
{
	sphere.model.LoadMeshesFromFBX("../../Assets/Models/FuzBallSphere.fbx");

	for (int i = 0; i < (int)sphere.model.GetMeshes()[0].GetVertices().size(); i++)
	{
		BASIC_VERTEX tempVert;
		tempVert.coordinate = sphere.model.GetMeshes()[0].GetVertices()[i].position;
		tempVert.color = color;
		sphere.mesh.push_back(tempVert);
	}

	CComPtr<ID3D11Buffer> tempVertexBuffer;

	D3D11_BUFFER_DESC Vertex_Buffer_Desc;
	ZeroMemory(&Vertex_Buffer_Desc, sizeof(Vertex_Buffer_Desc));
	Vertex_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;
	Vertex_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	Vertex_Buffer_Desc.CPUAccessFlags = NULL;
	Vertex_Buffer_Desc.ByteWidth = sizeof(BASIC_VERTEX) * sphere.mesh.size();

	D3D11_SUBRESOURCE_DATA objData;
	ZeroMemory(&objData, sizeof(objData));
	objData.pSysMem = &sphere.mesh[0];
	pDevice->CreateBuffer(&Vertex_Buffer_Desc, &objData, &tempVertexBuffer);

	sphere.vertex_Buffer = tempVertexBuffer;

	// set up indices
	for (int i = 0; i < (int)sphere.model.GetMeshes()[0].GetIndices().size(); i++)
	{
		sphere.indices.push_back(sphere.model.GetMeshes()[0].GetIndices()[i]);
	}

	// build and add the index buffer
	CComPtr<ID3D11Buffer> tempIndexBuffer;

	D3D11_BUFFER_DESC Index_Buffer_Desc;
	ZeroMemory(&Index_Buffer_Desc, sizeof(Index_Buffer_Desc));
	Index_Buffer_Desc.Usage = D3D11_USAGE_IMMUTABLE;
	Index_Buffer_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	Index_Buffer_Desc.CPUAccessFlags = NULL;
	Index_Buffer_Desc.ByteWidth = sizeof(unsigned short)* sphere.indices.size();

	D3D11_SUBRESOURCE_DATA IndexData;
	ZeroMemory(&IndexData, sizeof(IndexData));
	IndexData.pSysMem = &sphere.indices[0];
	pDevice->CreateBuffer(&Index_Buffer_Desc, &IndexData, &tempIndexBuffer);

	sphere.index_Buffer = tempIndexBuffer;

	// build and add the local const buffer for the matrix
	D3D11_BUFFER_DESC obj_Buffer_Desc;
	ZeroMemory(&obj_Buffer_Desc, sizeof(obj_Buffer_Desc));
	obj_Buffer_Desc.ByteWidth = sizeof(XMMATRIX);
	obj_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;
	obj_Buffer_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	obj_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA Object_Data;
	Object_Data.pSysMem = &sphere.worldMatrix;
	Object_Data.SysMemPitch = 0;
	Object_Data.SysMemSlicePitch = 0;

	CComPtr<ID3D11Buffer> tempMatrixBuffer;
	pDevice->CreateBuffer(&obj_Buffer_Desc, &Object_Data, &tempMatrixBuffer);

	sphere.matrixBuffer = tempMatrixBuffer;
}