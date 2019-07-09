#pragma once

#include <d3dx10.h>
#include <vector>
#include "Release.h"

using namespace std;

//RECT構造体
struct RECT_F
{
	//コンストラクタ
	RECT_F() {}
	RECT_F(float Top, float Left, float Width, float Height)
	{
		m_Top = Top;
		m_Left = Left;
		m_Right = Left + Width;
		m_Bottom = Top + Height;
	}

	float m_Top;	//上
	float m_Left;	//左
	float m_Right;	//右
	float m_Bottom;	//下
};

//色データ
struct ColorData
{
	//コンストラクタ
	ColorData() {}
	ColorData(float R, float G, float B, float A)
	{
		m_Red = R;
		m_Green = G;
		m_Blue = B;
		m_Alpha = A;
	}
	
	float m_Red;	//赤
	float m_Green;	//緑
	float m_Blue;	//青
	float m_Alpha;	//透過
};

//頂点構造体
struct MY_VERTEX
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNorm;
	D3DXVECTOR2 vTex;
};

//面情報
struct FACE_INFO
{
	//デストラクタ
	~FACE_INFO()
	{
		VectorRelease(Vertex);
	}
	vector<MY_VERTEX> Vertex;	//頂点情報
};

//マテリアル構造体
struct MY_MATERIAL
{

	//デストラクタ
	~MY_MATERIAL()
	{
		VectorRelease(FaceInfo);
		VectorRelease(pIndex);
		VectorRelease(pVertex);

		if (pTexture != nullptr)
		{
			pTexture = nullptr;
		}
	}

	D3DXVECTOR3 Ka;//アンビエント(環境光)
	D3DXVECTOR3 Kd;//ディフューズ(拡散光)
	D3DXVECTOR3 Ks;//スペキュラー(鏡面反射光）

	//面情報
	vector<FACE_INFO> FaceInfo;

	//テクスチャ
	ID3D10ShaderResourceView* pTexture;

	//バッファー
	vector<ID3D10Buffer*> pIndex;
	vector<ID3D10Buffer*> pVertex;
};

//オリジナルメッシュ
struct MY_MESH
{
	//デストラクタ
	~MY_MESH()
	{
		VectorRelease(Material);
	}
	vector<MY_MATERIAL> Material;	//マテリアル

	D3DXVECTOR3 vMin;	//頂点の最小座標
	D3DXVECTOR3 vMax;	//頂点の最大座標

	int Id;//管理用のID
};
