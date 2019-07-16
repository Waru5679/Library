#pragma once

#include <d3dx10.h>
#include <vector>
#include "Release.h"
#include "Singleton.h"
using namespace std;


//頂点構造体
class CVertex
{
public:
	//開放
	void Release() {};

	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNorm;
	D3DXVECTOR2 vTex;
};

//面データ
class CFaceData
{
public:
	//開放
	void Release()
	{
		for (unsigned int i = 0; i < m_Vertex.size(); i++)
		{
			m_Vertex[i].Release();
		}
		VectorRelease(m_Vertex);
	}

	vector<CVertex> m_Vertex;	//頂点情報
};

//マテリアルデータ
class CMaterialData
{
public:
	//開放
	void Release()
	{
		//面情報
		for (unsigned int i = 0; i < m_Face.size(); i++)
		{
			m_Face[i].Release();
		}
		VectorRelease(m_Face);

		//インデックスバッファー
		for (unsigned int i = 0; i < m_pIndex.size(); i++)
		{
			m_pIndex[i]->Release();
		}
		VectorRelease(m_pIndex);

		//バーテックスバッファー
		for (unsigned int i = 0; i < m_pVertex.size(); i++)
		{
			m_pVertex[i]->Release();
		}
		VectorRelease(m_pVertex);

		//テクスチャ
		if (m_pTexture != nullptr)
		{
			m_pTexture->Release();
			m_pTexture = nullptr;
		}
	}

	D3DXVECTOR3 Ka;//アンビエント(環境光)
	D3DXVECTOR3 Kd;//ディフューズ(拡散光)
	D3DXVECTOR3 Ks;//スペキュラー(鏡面反射光）

	//面情報
	vector<CFaceData> m_Face;

	//テクスチャ
	ID3D10ShaderResourceView* m_pTexture;

	//バッファー
	vector<ID3D10Buffer*> m_pIndex;
	vector<ID3D10Buffer*> m_pVertex;
};

//モデルデータクラス
class CModelData
{
public:
	//開放
	void Release()
	{
		for (unsigned int i = 0; i < m_Material.size(); i++)
			m_Material[i].Release();
		
		VectorRelease(m_Material);
	}

	vector<CMaterialData> m_Material;	//マテリアル

	D3DXVECTOR3 m_vMin;	//頂点の最小座標
	D3DXVECTOR3 m_vMax;	//頂点の最大座標

	int m_Id;//管理用のID
};

//オブジェクト管理クラス
class CModelManager :public CSingleton<CModelManager>
{
private:
	//シングルトン
	friend CSingleton<CModelManager>;
	CModelManager() {};	//コンストラクタ
	~CModelManager() {};//デストラクタ
public:
	//モデル取得
	CModelData* GetModelData(int id);
private:
	list<CModelData> m_List;//モデルリスト
};