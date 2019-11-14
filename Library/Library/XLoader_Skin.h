#pragma once

#include "Main.h"
#include "Singleton.h"
#include "Struct.h"
#include "LibraryHeader.h"
#include <stdio.h>

constexpr int TRIANGLE_POLYGON{ 3 };	//三角ポリゴン
constexpr int QUAD_POLYGON{ 4 };		//四角ポリゴン
constexpr int READ_ARRAY_SIZE{ 200 };	//読み込み用キー配列のサイズ
constexpr int NAME_ARRAY_SIZE{ 100 };	//名前配列のサイズ

constexpr int MAX_VER_WEIGH{ 4 };		//頂点の最大ウェイト

//頂点構造体
struct VERTEX
{
	VERTEX()
	{
		for (int i = 0; i < MAX_VER_WEIGH; i++)
		{
			m_BoneIndex[i] = -1;
			m_fWeight[i] = 0.0f;
		}
		m_WeightNum = 0;
	}

	D3DXVECTOR3 m_vPos;	//頂点位置
	D3DXVECTOR3 m_vNorm;//頂点法線
	D3DXVECTOR2 m_vTex;	//UV座標

	int m_BoneIndex[MAX_VER_WEIGH];	//影響を受けるボーンのインデックス
	float m_fWeight[MAX_VER_WEIGH];	//ウェイト
	int m_WeightNum;				//ウェイト数
};

//ポリゴン
struct FACE
{
	FACE()
	{
		m_FaceOfVer = -1;
		m_pIndex = nullptr;
		m_UseMaterial = -1;
	}
	int m_FaceOfVer;	//構成する頂点の数
	int* m_pIndex;		//構成する頂点のインデックス
	int m_UseMaterial;	//使用するマテリアル
};

//マテリアル構造体
struct MATERIAL
{
	MATERIAL()
	{
		ZeroMemory(this, sizeof(MATERIAL));
		m_pTexture = nullptr;
		m_pFaceIndex = nullptr;
		m_ppIndexBuffer = nullptr;
	}

	char						m_TexName[NAME_ARRAY_SIZE];	//ファイル名
	D3DXVECTOR4					m_vFaceColor;				//面の色
	D3DXVECTOR3					m_vKs;						//スペキュラー
	float						m_fPower;					//スペキュラーのパワー
	D3DXVECTOR3					m_vKe;						//エミッシブ
	ID3D10ShaderResourceView* m_pTexture;					//テクスチャポインタ	
	int							m_FaceNum;					//このマテリアルを使用する面の数
	int* m_pFaceIndex;				//このマテリアルで使用する面のインデックスリスト
	ID3D10Buffer** m_ppIndexBuffer;			//インデックスバッファ
};

//ボーン構造体
struct BONE
{
	BONE()
	{
		m_pChildIndex = nullptr;
		D3DXMatrixIdentity(&m_matBindPose);
		D3DXMatrixIdentity(&m_matNewPose);
		D3DXMatrixIdentity(&m_matOffset);
	}
	char		m_Name[NAME_ARRAY_SIZE];	//ボーン名
	int			m_index;					//自身のインデックス
	int			m_ChildNum;					//子の数
	int* m_pChildIndex;				//自分の子のインデックスリスト
	D3DXMATRIX	m_matBindPose;				//初期ポーズ（ずっと変わらない）
	D3DXMATRIX	m_matNewPose;				//現在のポーズ（その都度変わる）
	D3DXMATRIX  m_matOffset;				//オフセット行列
};

//どのボーンが度の頂点にどれだけの影響を与えるか
struct SKIN_WEIGHT
{
	SKIN_WEIGHT()
	{
		m_WeightNum = -1;
		m_pIndex = nullptr;
		m_pWeight = nullptr;
		D3DXMatrixIdentity(&m_matOffset);
	}

	char		m_BoneName[NAME_ARRAY_SIZE];//ボーン名
	int			m_WeightNum;				//ウェイトの数
	int* m_pIndex;					//影響を受ける頂点のインデックスリスト
	float* m_pWeight;					//ウェイトリスト
	D3DXMATRIX	m_matOffset;				//オフセット行列
};

//アニメーションのキー
struct KEY
{
	KEY()
	{
		m_Time = -1;
		m_ValueNum = -1;
		m_pfValue = nullptr;
	}

	int		m_Time;		//コマ
	int		m_ValueNum;	//値の数
	float* m_pfValue;	//値のリスト
};

//ボーンのキー
struct BONE_KEY
{
	BONE_KEY()
	{
		m_KeyNum = -1;
		m_KeyType = -1;
		m_pKey = nullptr;
	}

	char	m_AffectBoneName[NAME_ARRAY_SIZE];	//影響を受けるボーン名
	int		m_KeyType;							//キータイプ
	int		m_KeyNum;							//キーの数
	KEY* m_pKey;								//キーリスト
};

//アニメーション構造体
struct ANIMATION
{
	ANIMATION()
	{
		m_BoneKeyNum = -1;
		m_pBoneKey = nullptr;
	}

	char		m_Name[NAME_ARRAY_SIZE];//アニメーション名
	int			m_BoneKeyNum;			//対応ボーンの数
	BONE_KEY* m_pBoneKey;				//ボーンのキーリスト
};

//メッシュ
struct MESH
{
	MESH()
	{
		m_VerNum = -1;
		m_FaceNum = -1;
		m_MaterialNum = -1;
		m_pFace = nullptr;
		m_pMaterial = nullptr;
		m_pVertexBuffer = nullptr;
		m_pVertex = nullptr;
	}
	int				m_VerNum;		//頂点数
	int				m_FaceNum;		//面(ポリゴン)数
	FACE* m_pFace;		//面リスト
	int				m_MaterialNum;	//マテリアルの数
	MATERIAL* m_pMaterial;	//マテリアルリスト
	ID3D10Buffer* m_pVertexBuffer;//頂点バッファ
	VERTEX* m_pVertex;		//頂点情報のリスト
};

//スキンメッシュヘッダー
struct SKIN_MESH_HEADER
{
	SKIN_MESH_HEADER()
	{
		m_MaxVertex = -1;
		m_MaxFace = -1;
		m_BoneNum = -1;
	}
	int	m_MaxVertex;//頂点の最大ウェイト数
	int m_MaxFace;	//面の最大ウェイト数
	int m_BoneNum;	//ボーン数
};

//スキンメッシュ
struct SKIN_MESH
{
	SKIN_MESH()
	{
		m_Mesh = MESH();
		m_BoneNum = -1;
		m_pBone = nullptr;
		m_SkinHeader = SKIN_MESH_HEADER();
		m_WeightNum = -1;
		m_pWeight = nullptr;
		m_AnimeNum = -1;
		m_pAnimation = nullptr;
		m_pRoot = nullptr;
	}

	MESH				m_Mesh;			//メッシュ
	int					m_BoneNum;		//ボーン数
	BONE* m_pBone;		//ボーンリスト
	SKIN_MESH_HEADER	m_SkinHeader;	//スキンメッシュヘッダー
	int					m_WeightNum;	//ウェイト数
	SKIN_WEIGHT* m_pWeight;		//スキンウェイト
	int					m_AnimeNum;		//アニメーション数
	ANIMATION* m_pAnimation;	//アニメーション
	BONE* m_pRoot;		//ルートボーン
};


//Xファイル関連のクラス
class CX_Skin : public CSingleton<CX_Skin>
{
private:
	//シングルトン
	friend CSingleton<CX_Skin>;
	CX_Skin() {};	//コンストラクタ
	~CX_Skin() {};	//デストラクタ
public:
	void Release();//開放

	//スキンメッシュの読み込み
	bool LoadSkinMesh(const char* FileName, SKIN_MESH* pSkinMesh);

	//アニメーション
	void Animation(int AnimeId, int NowFrame, SKIN_MESH* pSkinMesh);

	//メッシュ描画(テスト用)
	void DrawMesh(D3DMATRIX matWorld, SKIN_MESH* pSkinMesh, CColorData* pColor);
private:
	
	//ボーンの更新
	void BoneUpdate(SKIN_MESH* pSkin, int AnimeId, int NowFrame);

	//ポーズを取得する
	D3DXMATRIX GetPose(bool* bFind, SKIN_MESH* pSkin, BONE* pBone, ANIMATION Anime, int NowFrame, int BoneID);

	//ウェイトが大きい順にソートする
	void WeightSort(SKIN_MESH* pSkin);

	//フレーム補完
	KEY FrameComplement(int NowFrame, BONE_KEY BoneKey);
	SKIN_MESH m_SkinMesh;	//スキンメッシュ
};