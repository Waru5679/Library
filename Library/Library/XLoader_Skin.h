#pragma once

#include "Main.h"
#include "Singleton.h"
#include "Struct.h"
#include <stdio.h>

constexpr int TRIANGLE_POLYGON{ 3 };	//三角ポリゴン
constexpr int QUAD_POLYGON{ 4 };		//四角ポリゴン
constexpr int READ_ARRAY_SIZE{ 200 };	//読み込み用キー配列のサイズ
constexpr int NAME_ARRAY_SIZE{ 100 };	//名前配列のサイズ

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
	char						m_TexName[NAME_ARRAY_SIZE];	//ファイル名
	D3DXVECTOR4					m_vFaceColor;				//面の色
	D3DXVECTOR3					m_vKs;						//スペキュラー
	float						m_fPower;					//スペキュラーのパワー
	D3DXVECTOR3					m_vKe;						//エミッシブ
	ID3D10ShaderResourceView*	m_pTexture;					//テクスチャポインタ	
	int							m_FaceNum;					//このマテリアルを使用する面の数
	int*						m_pVerNum;					//頂点数のリスト
	ID3D10Buffer**				m_ppIndexBuffer;			//インデックスバッファ
};

//ボーン構造体
struct BONE
{
	char		m_Name[NAME_ARRAY_SIZE];	//ボーン名
	int			m_index;					//自身のインデックス
	int			m_ChildNum;					//子の数
	int*		m_pChildIndex;				//自分の子のインデックスリスト
	D3DXMATRIX	m_matBindPose;				//初期ポーズ（ずっと変わらない）
	D3DXMATRIX	m_matNewPose;				//現在のポーズ（その都度変わる）
};

//どのボーンが度の頂点にどれだけの影響を与えるか
struct SKIN_WEIGHT
{
	char		m_BoneName[NAME_ARRAY_SIZE];//ボーン名
	int			m_WeightNum;				//ウェイトの数
	int*		m_pIndex;					//影響を受ける頂点のインデックスリスト
	float*		m_pWeight;					//ウェイトリスト
	D3DXMATRIX	m_matOffset;				//オフセット行列
};

////スキンウェイト情報を持ったボーン
//struct SKIN_BONE
//{
//	char		m_Name[NAME_ARRAY_SIZE];//ボーン名
//	int			m_index;				//自身のインデックス
//	int			m_ChildNum;				//子の数
//	int*		m_pChildIndex;			//自分の子のインデックスリスト
//	int			m_WeightNum;			//ウェイトの数
//	int*		m_pIndex;				//影響を与える頂点インデックスリスト
//	float*		m_pWeight;				//ウェイトリスト
//	D3DXMATRIX	m_matOffset;			//オフセット行列
//	D3DXMATRIX	m_matBindPose;			//初期ポーズ（ずっと変わらない）
//	D3DXMATRIX	m_matNewPose;			//現在のポーズ（その都度変わる）
//};

//アニメーションのキー
struct KEY
{
	int		m_Time;		//コマ
	int		m_ValueNum;	//値の数
	float*	m_pfValue;	//値のリスト
};

//ボーンのキー
struct BONE_KEY
{
	char	m_AffectBoneName[NAME_ARRAY_SIZE];	//影響を受けるボーン名
	int		m_KeyType;							//キータイプ
	int		m_KeyNum;							//キーの数
	KEY* m_pKey;								//キーリスト
};

//アニメーション構造体
struct ANIMATION
{
	char		m_Name[NAME_ARRAY_SIZE];//アニメーション名
	int			m_BoneKeyNum;			//対応ボーンの数
	BONE_KEY*	m_pBoneKey;				//ボーンのキーリスト
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
	VERTEX*			m_pVertex;		//頂点情報のリスト
};

//スキンメッシュ
struct SKIN_MESH
{
	MESH		m_Mesh;			//メッシュ
	int			m_BoneNum;		//ボーン数
	BONE*		m_pBone;		//ボーンリスト
	int			m_WeightNum;	//ウェイト数
	SKIN_WEIGHT*m_pWeight;		//スキンウェイト
	int			m_AnimeNum;		//アニメーション数
	ANIMATION*	m_pAnimation;	//アニメーション
	D3DXMATRIX	m_mFinalWorld;	//最終的なワールド行列（この姿勢でレンダリングする）


	//SKIN_BONE* m_pSkinBone;		//スキン情報を持たせたボーン
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
	int GetBoneNum(FILE* fp, long lStartPos);		//ボーン数の取得
	int GetSkinWeightNum(FILE* fp, long lStartPos);	//スキンウェイトの数を取得
	int GetAnimeNum(FILE* fp, long lStartPos);		//アニメーションの数を取得

	//templateを飛ばした読み込み開始位置を取得する
	long GetTemplateSkipStartPos(FILE* fp);

	//メッシュ情報の読み込み
	bool LoadMesh(FILE* fp, MESH* pMesh, long lStartPos);
	
	//ボーン読み込み
	bool LoadBone(FILE* fp, BONE* pBone, long lStartPos);

	//ボーン情報の読み込み(再起関数))
	BONE LoadBoneInfo(FILE* fp, int* pBoneIndex, BONE* pBone);
	
	//スキンウェイトの読み込み
	bool LoadSkinWeight(FILE* fp, SKIN_WEIGHT* pSkinWeight, long lStartPos);

	//アニメーション読み込み
	bool LoadAnimation(FILE* fp, ANIMATION* pAnime, long lStartPos);

	//指定文字を文字列から消す
	void ErasCharFromString(char* pSource, int Size, char Erace);

	////ボーンとスキン情報をまとめる
	//void BoneWithSkin(BONE* pBone, int BoneNum, SKIN_WEIGHT* pSkinWeight, int SkinWeightNum, SKIN_BONE* pSkinBone);

	////スキンメッシュにまとめる
	//void SkinMeshPutTogether(MESH Mesh, SKIN_BONE* pSkinBone, int BoneNum, ANIMATION* pAnimation, int AnimeNum, SKIN_MESH* pSkinMesh);

	//スキンメッシュにまとめる
	void SkinMeshPutTogether(MESH Mesh, BONE* pBone, int BoneNum,SKIN_WEIGHT* pSkinWeight,int WeightNum,ANIMATION* pAnimation, int AnimeNum, SKIN_MESH* pSkinMesh);

	//ボーン毎のキー情報読み込み
	BONE_KEY LoadBoneKey(FILE* fp);

	//フレーム補完
	//KEY FrameComplement(int NowFrame, ANIMATION anime);
	
	SKIN_MESH	m_SkinMesh;		//スキンメッシュ
};