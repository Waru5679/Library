#pragma once

#include <stdio.h>
#include <windows.h>
#include <d3dx10.h>

//必要なライブラリファイルのロード
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10.lib")

//マクロ
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

//オリジナル　マテリアル構造体
struct MY_MATERIAL
{
	CHAR szName[100];
	D3DXVECTOR4 Kd;//ディフューズ
	float Kp;//スペキュラーのパワー
	D3DXVECTOR3 Ks;//スペキュラー
	D3DXVECTOR3 Ke;//エミッシブ
	CHAR szTextureName[100];//テクスチャーファイル名
	ID3D10ShaderResourceView* pTexture;
	DWORD dwNumFace;//そのマテリアルであるポリゴン数
	MY_MATERIAL()
	{
		ZeroMemory(this, sizeof(MY_MATERIAL));
	}
	~MY_MATERIAL()
	{
		SAFE_RELEASE(pTexture);
	}
};

//オリジナルメッシュ
struct MY_MESH
{
	//開放
	void Release()
	{
		delete[] pMaterial;
		delete[] ppIndexBuffer;

		pVertexBuffer->Release();
	}

	DWORD dwNumVert;
	DWORD dwNumFace;
	ID3D10Buffer* pVertexBuffer;
	ID3D10Buffer** ppIndexBuffer;
	DWORD dwNumMaterial;


	MY_MATERIAL* pMaterial;
};

//頂点構造体
struct MY_VERTEX
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNorm;
	D3DXVECTOR2 vTex;
};

//X読み込みクラス
class CXLoader
{
public:
	//Xファイルを読み込む関数
	static HRESULT LoadMeshFromX(const char* FileName, MY_MESH* pMesh);
};