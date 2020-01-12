#pragma once
#include "Main.h"
#include "Singleton.h"
#include "Struct.h"
#include <stdio.h>

//頂点構造体
struct VERTEX
{
	VERTEX()
	{
		/*m_pBoneId = nullptr;
		m_pfWeight = nullptr;*/
	}
	~VERTEX()
	{
		////ボーンID
		//if (m_pBoneId != nullptr)
		//{
		//	delete[] m_pBoneId;
		//	m_pBoneId = nullptr;
		//}
		////ウェイト
		//if (m_pfWeight != nullptr)
		//{
		//	delete[] m_pfWeight;
		//	m_pfWeight = nullptr;
		//}
	}

	D3DXVECTOR3 vPos;	//位置
	D3DXVECTOR3 vNorm;	//法線
	D3DXVECTOR2 vUv;	//UV

	//int		WeightNum;	//ウェイト数
	//int*	m_pBoneId;	//影響を受けるボーンID
	//float*	m_pfWeight;	//ウェイト
};

//ポリゴン
struct FACE
{
	FACE()
	{
		m_pIndexBuffer = nullptr;
	}
	~FACE()
	{
		//インデックスバッファ
		if (m_pIndexBuffer != nullptr)
		{
			m_pIndexBuffer->Release();
		}
	}
	int m_VerId[3];						//構成する頂点ID
	int m_UseMateId;					//使用するマテリアルID
	ID3D10Buffer* m_pIndexBuffer;		//インデックスバッファー
};

//マテリアル
struct MATERIAL
{
	char m_Name[100];//マテリアル名
	
	D3DXVECTOR4 m_vColor;	//面の色
	D3DXVECTOR4 m_vDiffuse;	//ディフューズ
	D3DXVECTOR4 m_vSpecular;//スペキュラー
	D3DXVECTOR4 m_vAmbient;	//アンビエント
	float		m_fSpePower;//スペキュラーパワー
	int			m_TexId;	//テクスチャID
};

//モデルデータ
struct MODEL_DATA
{
	MODEL_DATA()
	{
		m_pVertex = nullptr;
		m_pFace = nullptr;
		m_pMaterial = nullptr;
		m_pVertexBuffer = nullptr;
		m_ppTexture = nullptr;
	}
	~MODEL_DATA()
	{
		//頂点
		if (m_pVertex != nullptr)
		{
			delete[] m_pVertex;
			m_pVertex = nullptr;
		}
		//面
		if (m_pFace != nullptr)
		{
			delete[] m_pFace;
			m_pFace = nullptr;
		}
		//マテリアル
		if (m_pMaterial != nullptr)
		{
			delete[] m_pMaterial;
			m_pMaterial = nullptr;
		}
		//バーテックスバッファ
		if (m_pVertexBuffer!= nullptr)
		{
			m_pVertexBuffer->Release();
		}
		//テクスチャ
		for (int i = 0; i < m_TexNum; i++)
		{
			m_ppTexture[i]->Release();
		}
	}

	char						m_Name[20];		//モデル名
	int							m_VerNum;		//頂点数
	int							m_FaceNum;		//面(ポリゴン)数
	int							m_TexNum;		//テクスチャ数
	int							m_MateNum;		//マテリアルの数
	VERTEX*						m_pVertex;		//頂点情報のリスト
	FACE*						m_pFace;		//面リスト
	MATERIAL*					m_pMaterial;	//マテリアルリスト
	ID3D10Buffer*				m_pVertexBuffer;//バーテックスバッファ
	ID3D10ShaderResourceView**	m_ppTexture;	//テクスチャ
};

//モデル読み込み
class CModelLoader:public CSingleton<CModelLoader>
{
private:
	//シングルトン
	friend CSingleton<CModelLoader>;
	CModelLoader() {};	//コンストラクタ
	~CModelLoader() {};	//デストラクタ
public:
	void Release();	//解放

	//モデル読みこみ
	bool Load(const char* FileName, MODEL_DATA* pModel);
	//モデル描画
	void Draw(D3DMATRIX matWorld, MODEL_DATA* pModel, CColorData* pColor);

private:
	//頂点読み込み
	bool VertexLoad(FILE* fp, MODEL_DATA* pModel);
	//面読み込み
	bool FaceLoad(FILE* fp, MODEL_DATA* pModel);
	//テクスチャ読み込み
	bool TexLoad(FILE* fp, MODEL_DATA* pModel);
	//マテリアル読み込み
	bool MateLoad(FILE* fp, MODEL_DATA* pModel);
};