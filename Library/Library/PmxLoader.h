#pragma once
#include "Singleton.h"
#include "PmxStruct.h"
#include <stdio.h>

//pmxデータ
struct PMX_DATA
{
	PMX_DATA()
	{	
		m_pVertex		= nullptr;	//頂点データ
		m_pFace			= nullptr;	//面のデータ
		m_pTex			= nullptr;	//テクスチャデータ
		m_pMaterial		= nullptr;	//マテリアルデータ
		m_pBone			= nullptr;	//ボーンデータ
		m_pMorph		= nullptr;	//モーフデータ
		m_pDisplay		= nullptr;	//表示枠
		m_pRigidBody	= nullptr;	//剛体
		m_pJoint		= nullptr;	//Joint
	}
	~PMX_DATA()
	{
		//頂点データ
		if (m_pVertex != nullptr)
		{
			delete[] m_pVertex;
			m_pVertex = nullptr;
		}
		//面のデータ
		if (m_pFace != nullptr)
		{
			delete[] m_pFace;
			m_pFace = nullptr;
		}
		//テクスチャデータ
		if (m_pTex != nullptr)
		{
			delete[] m_pTex;
			m_pTex = nullptr;
		}
		//マテリアルデータ
		if (m_pMaterial != nullptr)
		{
			delete[] m_pMaterial;
			m_pMaterial = nullptr;
		}
		//ボーンデータ
		if (m_pBone != nullptr)
		{
			delete[] m_pBone;
			m_pBone = nullptr;
		}
		//モーフデータ
		if (m_pMorph != nullptr)
		{
			delete[] m_pMorph;
			m_pMorph = nullptr;
		}
		//表示枠
		if (m_pDisplay != nullptr)
		{
			delete[] m_pDisplay;
			m_pDisplay = nullptr;
		}
		//剛体
		if (m_pRigidBody != nullptr)
		{
			delete[] m_pRigidBody;
			m_pRigidBody = nullptr;
		}
		//Joint
		if (m_pJoint != nullptr)
		{
			delete[] m_pJoint;
			m_pJoint = nullptr;
		}
	}

	int				m_VerNum;		//頂点数
	int				m_FaceNum;		//面の数
	int				m_TexNum;		//テクスチャ数
	int				m_MaterialNum;	//マテリアル数
	int				m_BoneNum;		//ボーン数
	int				m_MorphNum;		//モーフ数
	int				m_DisplayNum;	//表示枠数
	int				m_RigidNum;		//剛体数
	int				m_JointNum;		//Joint数
	PMX_HEADER		m_Head;			//ヘッダー
	PMX_MODEL_INFO	m_ModelInfo;	//モデルデータ
	PMX_VERTEX*		m_pVertex;		//頂点データ
	PMX_FACE*		m_pFace;		//面のデータ
	PMX_TEXTURE*	m_pTex;			//テクスチャデータ
	PMX_MATERIAL*	m_pMaterial;	//マテリアルデータ
	PMX_BONE*		m_pBone;		//ボーンデータ
	PMX_MORPH*		m_pMorph;		//モーフデータ	
	PMX_DISPLAY*	m_pDisplay;		//表示枠データ
	PMX_RIGIT_BODY* m_pRigidBody;	//剛体データ
	PMX_JOINT*		m_pJoint;		//Jointデータ

};

//PMX読み込み
class CPmxLoader :public CSingleton<CPmxLoader>
{
private:
	//シングルトン
	friend CSingleton<CPmxLoader>;
	CPmxLoader() {};
	~CPmxLoader() {};
public:

	//読み込み
	bool Load(const char* FileName, PMX_DATA* pPmxData);
	
	//データをまとめる
	void DataPut(PMX_DATA* pPmxData);
	
	//書き出し
	bool Write(const char* FileName, PMX_DATA* pPmxData);
private:
	//ヘッダ情報読みこみ
	void HeadLoad(FILE* fp, PMX_DATA* pPmxData);

	//モデル情報読み込み
	void ModelInfoLoad(FILE* fp, PMX_DATA* pPmxData);

	//頂点読み込み
	bool VertexLoad(FILE* fp, PMX_DATA* pPmxData);
	
	//面読み込み
	bool FaceLoad(FILE* fp, PMX_DATA* pPmxData);
	
	//テクスチャ読み込み
	bool TextureLoad(FILE* fp, PMX_DATA* pPmxData);

	//マテリアル読み込み
	bool MaterialLoad(FILE* fp, PMX_DATA* pPmxData);

	//ボーン読み込み
	bool BoneLoad(FILE* fp, PMX_DATA* pPmxData);

	//モーフ読み込み
	bool MorphLoad(FILE* fp, PMX_DATA* pPmxData);

	//表示枠読み込み
	bool DisplayFrameLoad(FILE* fp, PMX_DATA* pPmxData);

	//剛体読み込み
	bool RigidBodyLoad(FILE* fp, PMX_DATA* pPmxData);

	//Joint読み込み
	bool JointLoad(FILE* fp, PMX_DATA* pPmxData);
};
