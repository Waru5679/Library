#pragma once
#include "Main.h"
#include "Singleton.h"

//頂点構造体
struct VERTEX
{
	VERTEX()
	{
		m_pBoneId = nullptr;
		m_pfWeight = nullptr;
	}
	~VERTEX()
	{
		//ボーンID
		if (m_pBoneId != nullptr)
		{
			delete[] m_pBoneId;
			m_pBoneId = nullptr;
		}
		//ウェイト
		if (m_pfWeight != nullptr)
		{
			delete[] m_pfWeight;
			m_pfWeight = nullptr;
		}
	}

	D3DXVECTOR3 vPos;	//位置
	D3DXVECTOR3 vNorm;	//法線
	D3DXVECTOR2 vUv;	//UV

	int		WeightNum;	//ウェイト数
	int*	m_pBoneId;	//影響を受けるボーンID
	float*	m_pfWeight;	//ウェイト
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
	int m_VerIn[3];		//構成する頂点ID
	int m_UseMateId;					//使用するマテリアルID
	ID3D10Buffer* m_pIndexBuffer;		//インデックスバッファー
};

//マテリアル
struct MATERIAL
{
	char m_Name[20];//マテリアル名
	
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
	}
	int				m_VerNum;		//頂点数
	int				m_FaceNum;		//面(ポリゴン)数
	int				m_MateNum;		//マテリアルの数
	VERTEX*			m_pVertex;		//頂点情報のリスト
	FACE*			m_pFace;		//面リスト
	MATERIAL*		m_pMaterial;	//マテリアルリスト
	ID3D10Buffer* m_pVertexBuffer;	//頂点バッファ	
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

	bool Load(const char* FileName, MODEL_DATA* pModel);
};