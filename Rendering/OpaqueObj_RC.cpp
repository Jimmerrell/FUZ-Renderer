/***********************************************
* Filename:  		CRenderContext.cpp
* Date:      		10/2/2014
* Mod. Date: 		10/2/14
* Mod. Initials:	JM
* Author:    		Jim Merrell
* Purpose:   		This is the RenderContext Class that holds common render data
************************************************/

#include "OpaqueObj_RC.h"

#include "CompiledShaders\FBX_VS.csh"
#include "CompiledShaders\FBX_PS.csh"
#include "CompiledShaders\Hand_PS.csh"
#include "CompiledShaders\FBX_PS_GreyScale.csh"
#include "CompiledShaders\Anim_VS.csh"
#include "CompiledShaders\CollisionBox_PS.csh"
#include "CompiledShaders\CollisionBox_VS.csh"
#include "CompiledShaders\Hull_Shader.csh"
#include "CompiledShaders\Domain_Shader.csh"
#include "CompiledShaders\Geometry_Shader.csh"


OpaqueObj_RC::~OpaqueObj_RC(void)
{
	for (int i = 0; i < (int)m_vRenderables.size(); i++)
	{
		delete m_vRenderables[i];
	}
	m_vRenderables.clear();
}

void OpaqueObj_RC::Initialize(ID3D11Device* device, ID3D11DeviceContext* immediateContext, void* texture_SRVs, SCENE* _scene)
{
	pDevice = device;
	pImmediateContext = immediateContext;

	m_SRVs = (map<string, CComPtr<ID3D11ShaderResourceView>>*)texture_SRVs;
	scene = _scene;

	m_f4_currColor.colorRGBA = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	SetupShadersAndInputLayout();
	SetupRasterizerStates();
	SetupConstBuffers();
}

void OpaqueObj_RC::OrthoNormalInverse(XMFLOAT4X4& r, const XMFLOAT4X4& m)
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

void OpaqueObj_RC::Run()
{
	pImmediateContext->VSSetShader(VertexShader, NULL, NULL);

	if(!m_bThermalRadarEffect)
		pImmediateContext->PSSetShader(PixelShader, NULL, NULL);
	else
		pImmediateContext->PSSetShader(Thermal_PS, NULL, NULL);


	// draw the objects into the world
	for (int i = 0; i < (int)m_vRenderables.size(); i++)
	{
		if(DeadProjectile(m_vRenderables[i]->object))
			continue;

		if(m_vRenderables[i]->object->getToRender() == false) // go to next object and don't bother drawing this one
			continue;

		// map the object color
		{
			//if(TESSELATION)
			//{
			//	m_f4_currColor.colorRGBA.w = (float)m_nTesselationLevel;
			//	m_f4_currColor.colorRGBA.x = m_fTestXPos;
			//	m_f4_currColor.colorRGBA.z = m_fTestZPos;
			//}
			//else
			m_f4_currColor.colorRGBA = m_vRenderables[i]->object->getColorRGBA();



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
		};

		string textureName = m_vRenderables[i]->object->getTextureKey();
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

		// map the matrix buffer
		{
			XMFLOAT4X4 worldMatrix;
			XMStoreFloat4x4(&worldMatrix, m_vRenderables[i]->object->getWorldMatrix());

			D3D11_MAPPED_SUBRESOURCE MappedResource;
			ZeroMemory(&MappedResource, sizeof(MappedResource));
			pImmediateContext->Map(m_vRenderables[i]->matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &MappedResource);
			memcpy(MappedResource.pData, &worldMatrix, sizeof(worldMatrix));
			pImmediateContext->Unmap(m_vRenderables[i]->matrixBuffer, 0);

			ID3D11Buffer* constbuffer[1] = { m_vRenderables[i]->matrixBuffer };
			pImmediateContext->VSSetConstantBuffers(0, 1, constbuffer);
			pImmediateContext->HSSetConstantBuffers( 0, 1, constbuffer );
			pImmediateContext->DSSetConstantBuffers( 0, 1, constbuffer );
		}

		// map the fuz data buffer
		{
			// for debugging
			//for(int j = 0; j < MAX_FUZ_PER_OBJECT; j++)
			//{
			//	if(m_vRenderables[i]->object->getFuzData().Fuz_Collision_Points[j] >= 0)
			//	{
			//		if(m_vRenderables[i]->object->getObjectType() != Entity3D::TERRAIN)
			//			int x = 0;
			//
			//
			//		if(m_vRenderables[i]->object->getObjectType() == Entity3D::GUN)
			//			int x = 0;
			//	}
			//
			//}



			D3D11_MAPPED_SUBRESOURCE MappedResource;
			ZeroMemory(&MappedResource, sizeof(MappedResource));
			pImmediateContext->Map(m_vRenderables[i]->fuzDataBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &MappedResource);
			memcpy(MappedResource.pData, &m_vRenderables[i]->object->getFuzData(), sizeof(FUZ_COLLISION_DATA));
			pImmediateContext->Unmap(m_vRenderables[i]->fuzDataBuffer, 0);

			ID3D11Buffer* constbuffer[1] = { m_vRenderables[i]->fuzDataBuffer };
			pImmediateContext->HSSetConstantBuffers(4, 1, constbuffer);
			pImmediateContext->GSSetConstantBuffers(4, 1, constbuffer);
		}

		// Map the UV Scrolling const buffer
		{
			XMFLOAT4 uvOffsets = m_vRenderables[i]->object->GetUVOffsets();
			D3D11_MAPPED_SUBRESOURCE MappedResource;
			ZeroMemory(&MappedResource, sizeof(MappedResource));
			pImmediateContext->Map(m_vRenderables[i]->scrollingUVBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &MappedResource);
			memcpy(MappedResource.pData, &uvOffsets, sizeof(XMFLOAT4));
			pImmediateContext->Unmap(m_vRenderables[i]->scrollingUVBuffer, 0);
			ID3D11Buffer* constbuffer[1] = { m_vRenderables[i]->scrollingUVBuffer };
			pImmediateContext->VSSetConstantBuffers(5, 1, constbuffer);
		}

		ID3D11Buffer* bufferarr[1] = { m_vRenderables[i]->vertexBuffer };
		UINT ArrStrides[1] = { sizeof(VERTEX_3D) };
		UINT ArrOffsets[1] = { 0 };
		pImmediateContext->IASetVertexBuffers(0, 1, bufferarr, ArrStrides, ArrOffsets);

		// ANIMATION VS
		//check if object is animated
		// if animated, use animated VS
		//		set up const buffers for that object
		// else use normal VS
		if (m_vRenderables[i]->object->isAnimating() &&
			m_vRenderables[i]->object->GetInterpolator()->GetInterpolatedJoints().size() > 0)
		{
			//if(RenObj->object->getModelKey() == "newScientistHand")
			pImmediateContext->PSSetShader(Hand_PShader, NULL, NULL);

			RenderAnimatedObject(m_vRenderables[i]);

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

			RenderStaticObject(m_vRenderables[i]);

			pImmediateContext->HSSetShader(NULL, NULL, NULL);
			pImmediateContext->DSSetShader(NULL, NULL, NULL);
			pImmediateContext->GSSetShader(NULL, NULL, NULL);
		}
	}

	pImmediateContext->RSSetState(m_pCullFront_RS);

	//DrawCollisionBoxes();
}

void OpaqueObj_RC::RenderAnimatedObject(Renderable* RenObj)
{
	unsigned int jointCount = RenObj->object->GetModel()->GetMeshes()[0].GetJoints().size();
	for (unsigned int i = 0; i < jointCount; i++)
	{
		XMFLOAT4X4 invBind;
		OrthoNormalInverse(invBind, RenObj->object->GetModel()->GetMeshes()[0].GetJoints()[i].world_bind_pose_transform);
		m_4x4bindPosJoints[i] = invBind;
		m_4x4interpolatedJoints[i] = RenObj->object->GetInterpolator()->GetInterpolatedJoints()[i];
	}
	/*jointCount = RenObj->object->GetModel()->GetMeshes()[0].GetJoints().size();
	for (unsigned int i = 0; i < jointCount; i++)
	{
	m_4x4bindPosJoints[i] = RenObj->object->GetModel()->GetMeshes()[0].GetJoints()[i].world_bind_pose_transform;
	m_4x4interpolatedJoints[i] = RenObj->object->GetInterpolator()->GetInterpolatedJoints()[i];
	}
	jointCount = RenObj->object->GetModel()->GetMeshes()[0].GetJoints().size();
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

	pImmediateContext->IASetInputLayout(InputLayout);

	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pImmediateContext->IASetIndexBuffer(RenObj->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// first draw on object
	//pImmediateContext->RSSetState(m_pCullBack_RS);
	//pImmediateContext->DrawIndexed(RenObj->object->getRenderIndex().size(), 0, 0);

	//second draw on object
	{
		if(!m_bWireFrame)
			pImmediateContext->RSSetState(m_pCullFront_RS);
		else
			pImmediateContext->RSSetState(m_pWireFrame_CullFront_RS);

		pImmediateContext->DrawIndexed(RenObj->object->getRenderIndex().size(), 0, 0);
	}

	pImmediateContext->VSSetShader(VertexShader, NULL, NULL);
}

void OpaqueObj_RC::RenderStaticObject(Renderable* RenObj)
{
	pImmediateContext->IASetInputLayout(InputLayout);

	if(!TESSELATION)
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	else
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	pImmediateContext->IASetIndexBuffer(RenObj->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//first draw on object
	if(RenObj->object->getObjectType() == Entity3D::GUN)
	{
		pImmediateContext->RSSetState(m_pCullBack_RS);
		pImmediateContext->DrawIndexed(RenObj->object->getRenderIndex().size(), 0, 0);
	}

	//second draw on object
	{
		if(!m_bWireFrame)
			pImmediateContext->RSSetState(m_pCullFront_RS);
		else
			pImmediateContext->RSSetState(m_pWireFrame_CullFront_RS);

		pImmediateContext->DrawIndexed(RenObj->object->getRenderIndex().size(), 0, 0);
	}
}

void OpaqueObj_RC::DrawCollisionBoxes()
{
	pImmediateContext->VSSetShader(CollisionBox_VShader, NULL, NULL);
	pImmediateContext->PSSetShader(CollisionBox_PShader, NULL, NULL);
	pImmediateContext->IASetInputLayout(CollisionBox_InputLayout);

	for (int i = 0; i < (int)m_vRenderables.size(); i++)
	{
		XMFLOAT4X4 worldMatrix;
		XMStoreFloat4x4(&worldMatrix, m_vRenderables[i]->object->getWorldMatrix());

		D3D11_MAPPED_SUBRESOURCE MappedResource;
		ZeroMemory(&MappedResource, sizeof(MappedResource));
		pImmediateContext->Map(m_vRenderables[i]->matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &MappedResource);
		memcpy(MappedResource.pData, &worldMatrix, sizeof(worldMatrix));
		pImmediateContext->Unmap(m_vRenderables[i]->matrixBuffer, 0);

		ID3D11Buffer* constbuffer[1] = { m_vRenderables[i]->matrixBuffer };
		pImmediateContext->VSSetConstantBuffers(0, 1, constbuffer);

		CreateBoxFromAABB(m_vRenderables[i]->object);

		ID3D11Buffer* buffarr[1] = { CollisionBox_VertexBuffer };
		UINT ArrStride[1] = { sizeof(BASIC_VERTEX) };
		UINT ArrOffset[1] = { 0 };
		pImmediateContext->IASetVertexBuffers(0, 1, buffarr, ArrStride, ArrOffset);

		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		pImmediateContext->IASetIndexBuffer(CollisionBox_Index_Buffer, DXGI_FORMAT_R16_UINT, 0);

		pImmediateContext->DrawIndexed(36, 0, 0);

	}
}

void OpaqueObj_RC::SetupShadersAndInputLayout()
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
	pDevice->CreatePixelShader(CollisionBox_PS, sizeof(CollisionBox_PS), NULL, &CollisionBox_PShader);

	D3D11_INPUT_ELEMENT_DESC vertLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	pDevice->CreateInputLayout(vertLayout, 2, &CollisionBox_VS, sizeof(CollisionBox_VS), &CollisionBox_InputLayout);

}

void OpaqueObj_RC::AddRenderObject(Renderable* ren_obj)
{
	m_vRenderables.push_back(ren_obj);
}

void OpaqueObj_RC::RemoveRenderObject(Entity3D* obj)
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

void OpaqueObj_RC::SetupConstBuffers()
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

void OpaqueObj_RC::CreateBoxFromAABB(Entity3D* ent)
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

bool OpaqueObj_RC::DeadProjectile(Entity3D* entity)
{
	if(entity->getObjectType() != Entity3D::PROJECTILE)
		return false;

	XMFLOAT3 p = entity->getPosition();
	XMFLOAT3 o = XMFLOAT3(0.0f, 0.0f, 0.0f);

	if(p.x == o.x && p.y == o.y && p.z == o.z )
		return true;
	else
		return false;
}
