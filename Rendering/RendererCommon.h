/***********************************************
* Filename:  		RendererCommon.h
* Date:      		10/2/2014
* Mod. Date: 		10/2/2014
* Mod. Initials:	JM
* Author:    		Jim Merrell
* Purpose:   		RenderCommon will have all of the common include needed for the rendering classes
************************************************/

#ifndef RENDERCOMMON_H
#define RENDERCOMMON_H

#include <iostream>
#include <ctime>
#include <vector>
#include <list>
#include <map>
#include <atlbase.h>
#include <memory>
#include <string>

using namespace std;

#include <d3d11.h>
#pragma comment( lib, "d3d11.lib" )

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

using namespace DirectX;

#include "../Animation/Model.h"

#include "../Object Management/CObjectManagerClient.h"
#include "../Object Management/Entity3D.h"
#include "../VFX/Emitter.h"
#include "../VFX/VFXManager.h"

//#include "../../DirectXTK/Inc/GamePad.h"

#include "ShaderShared.h"
//// if you cahnge this also change it in FBX_VS.hlsl
//#define TESSELATION true // READ BELOW
//// Setting the above to true will use the tesselation pipeline and 
////	render Fuz at the center of the map
////    - You can move the example Fuz with Numpad keys(8, 6, 2, 4)

//#define FUZ_COUNT 10

struct SCENE
{
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;
};

// RECTF is a structure that work like RECT but uses floats
struct RECTF
{
	float top;
	float bottom;
	float left;
	float right;
};

struct Renderable
{
	Entity3D* object;
	CComPtr<ID3D11Buffer>	vertexBuffer;
	CComPtr<ID3D11Buffer>	indexBuffer;
	CComPtr<ID3D11Buffer>	matrixBuffer;
	CComPtr<ID3D11Buffer>	fuzDataBuffer;

	CComPtr<ID3D11Buffer>	bindPosBuffer;
	CComPtr<ID3D11Buffer>	interpolatedBuffer;
	CComPtr<ID3D11Buffer>	scrollingUVBuffer;
};

struct Fuz
{
	FUZ_EFFECT_POOL*		FuzPool;
	CComPtr<ID3D11Buffer>	vertexBuffer;
};

struct ADVANCED_FUZ
{
	XMFLOAT4 worldPos;
	XMFLOAT4 color;
	XMFLOAT4 r_l_p_p; // radius, lifetime, padding, padding
};

struct FUZ_POOL
{
	ADVANCED_FUZ fuzzes[MAX_FUZ_COUNT];
};

struct COLOR_RGBA
{
	XMFLOAT4 colorRGBA;
};

struct BASIC_VERTEX
{
	XMFLOAT3 coordinate;
	XMFLOAT4 color;
};

struct RENDER_TEXT
{
	float		scale;
	float		rotation;
	XMFLOAT4	color;
	XMFLOAT2	origin;
	XMFLOAT2	position;
	string		text;
};

struct RENDER_TEXTURE
{
	RECT		sourceRect;
	float		scale;
	float		rotation;
	XMFLOAT4	color;
	XMFLOAT2	origin;
	XMFLOAT2	position;
	string		textureName;			
};

struct VERTEX_2D
{
	XMFLOAT2 Coordinate;
	XMFLOAT4 ColorRGBA;
	XMFLOAT2 UVcoord;
	float Gradient;
};

struct ScreenSpaceObject
{
	vector<VERTEX_2D>		mesh;
	CComPtr<ID3D11Buffer>	Vertex_Buffer;
	CComPtr<ID3D11Buffer>	Index_Buffer;
	string					textureName;
	string					secondTexture;
	string					thirdTexture;
	int						useGradient_PS;
};

struct DebugVolume
{
	CModel					model;
	vector<BASIC_VERTEX>	mesh;
	vector<unsigned short>	indices;
	XMFLOAT4X4				worldMatrix;
	CComPtr<ID3D11Buffer>	vertex_Buffer;
	CComPtr<ID3D11Buffer>	index_Buffer;
	CComPtr<ID3D11Buffer>	matrixBuffer;
};

char const validChars[] = "abcdefghijklmnopqrstuvwxyz"
                          "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                          "0123456789"
						  "./_()%!@#$*\":;,.-+<&"
                          " ";  // add whatever other characters are considered valid





#endif