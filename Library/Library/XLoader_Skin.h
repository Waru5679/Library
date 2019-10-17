#pragma once

#include "Main.h"
#include "Singleton.h"
#include "Struct.h"

#include <stdio.h>

//頂点構造体
struct VERTEX
{
	D3DXVECTOR3 m_vPos;	//頂点位置
	D3DXVECTOR3 m_vNorm;//頂点法線
	D3DXVECTOR2 m_vTex;	//UV座標
};

//ポリゴン
struct FACE
{
	int m_FaceOfVer;	//構成する頂点の数
	int* m_pIndex;		//構成する頂点のインデックス
};

//マテリアル構造体
struct MATERIAL
{
	D3DXVECTOR4					m_vFaceColor;	//面の色
	D3DXVECTOR3					m_vKs;			//スペキュラー
	float						m_fPower;		//スペキュラーのパワー
	D3DXVECTOR3					m_vKe;			//エミッシブ
	char						m_TexName[100];	//ファイル名
	ID3D10ShaderResourceView*	m_pTexture;		//テクスチャポインタ	
	int							m_FaceNum;		//このマテリアルを使用する面の数
	int*						m_pVerNum;		//頂点数のリスト
	ID3D10Buffer**				m_ppIndexBuffer;//インデックスバッファ
};

//ボーン構造体
struct BONE
{
	char		m_Name[100];	//ボーン名
	int			m_index;		//自身のインデックス
	int			m_ChildNum;		//子の数
	int*		m_pChildIndex;	//自分の子のインデックスリスト
	D3DXMATRIX	m_matBindPose;	//初期ポーズ（ずっと変わらない）
	D3DXMATRIX	m_matNewPose;	//現在のポーズ（その都度変わる）
};

//どのボーンがどのボーンにどれだけの影響を与えるか
struct SKIN_WEIGHT
{
	char		m_BoneName[100];//ボーン名
	int			m_WeightNum;	//ウェイトの数
	int*		m_pIndex;		//影響を受ける頂点のインデックスリスト
	float*		m_pWeight;		//ウェイトリスト
	D3DXMATRIX	m_matOffset;	//オフセット行列
};

//アニメーションのキー
struct Key
{
	int		m_Time;		//コマ
	int		m_ValueNum;	//値の数
	float* m_pValue;	//値のリスト
};

//アニメーション構造体
struct ANIMATION
{
	char	m_Name[100];			//アニメーション名
	char	m_AffectBoneName[100];	//影響を受けるボーン名
	int		m_KeyType;				//キータイプ
	int		m_KeyNum;				//キーの数
	Key*	m_pKey;					//キーリスト
};

//メッシュ
struct MESH
{
	int				m_VerNum;		//頂点数
	int				m_UvNum;		//UVの数(頂点数と同じ)
	int				m_FaceNum;		//面(ポリゴン)数
	FACE*			m_pFace;		//面リスト
	int				m_MaterialNum;	//マテリアルの数
	MATERIAL*		m_pMaterial;	//マテリアルリスト
	ID3D10Buffer*	m_pVertexBuffer;//頂点バッファ
};

//スキンメッシュ
struct SKIN_MESH
{
	MESH		m_Mesh;			//メッシュ
	int			m_BoneNum;		//ボーン数
	BONE*		m_pBone;		//ボーンリスト	
	D3DXMATRIX	m_mFinalWorld;	//最終的なワールド行列（この姿勢でレンダリングする）
	int			m_WeightNum;	//ウェイト数
	SKIN_WEIGHT*m_pSkinWeight;	//スキンウェイト
	int			m_AnimeNum;		//アニメーション数
	ANIMATION*	m_pAnimation;	//アニメーション
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
	bool LoadSkinMesh(const char* FileName, SKIN_MESH* pSkinMesh);	//スキンメッシュの読み込み
	bool LoadMesh(FILE* fp, MESH* pMesh);	//メッシュ情報の読み込み
	
	bool LoadBone(FILE* fp, SKIN_MESH* pSkinMesh);	//ボーン読み込み
	BONE LoadBoneInfo(FILE* fp, int* pBoneIndex, SKIN_MESH* pSkinMesh);//ボーン情報の読み込み
	//メッシュ描画(テスト用)
	void DrawMesh(D3DMATRIX matWorld, SKIN_MESH* pSkinMesh, CColorData* pColor);
	
private:

	SKIN_MESH	m_SkinMesh;		//スキンメッシュ
};