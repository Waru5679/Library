#pragma once
#include "Singleton.h"
#include <stdio.h>

struct	BDEF1
{
	BDEF1()
	{
		m_BoneID = -1;
	}
	int m_BoneID;
};

struct	BDEF2
{
	BDEF2()
	{
		m_BoneID[0] = -1;
		m_BoneID[1] = -1;
	}
	int m_BoneID[2];
	float m_Weight;
};

struct	BDEF4
{
	BDEF4()
	{
		m_BoneID[0] = -1;
		m_BoneID[1] = -1;
		m_BoneID[2] = -1;
		m_BoneID[3] = -1;
	}

	int m_BoneID[4];
	float m_Weight[4];
};

struct SDEF
{
	SDEF()
	{
		m_BoneID[0] = -1;
		m_BoneID[1] = -1;
	}
	int m_BoneID[2];
	float m_Weight;
	float m_Matrix[3][3];
};

//ウェイトデータ
struct PMX_WEIGHT_DATA
{
	unsigned char m_WeightType;//ウェイト変形方式
	BDEF1	m_Bdef1;	//BDEF1形式のデータ
	BDEF2	m_Bdef2;	//BDEF2形式のデータ
	BDEF4	m_Bdef4;	//BDEF4形式のデータ
	SDEF	m_Sdef;		//SDEF形式のデータ
};

//pmxヘッダー
struct PMX_HEADER
{
	~PMX_HEADER()
	{
		delete[] m_pData;
		m_pData = nullptr;
	}
	char	m_FileType[4];	//ファイル種類"PMX_"固定
	float	m_Ver;			//バージョン
	char	m_Size;
	unsigned char*	m_pData;
};

//pmxモデル情報
struct PMX_MODEL_INFO
{
	PMX_MODEL_INFO()
	{
		m_CommentEngSize = -1;
		m_CommentJpnSize = -1;
		m_NameEngSize = -1;
		m_NameJpnSize = -1;
		m_pNameJap = nullptr;
		m_pNameEng = nullptr;
		m_pCommentJap = nullptr;
		m_pCommentEng = nullptr;
	}
	~PMX_MODEL_INFO()
	{
		delete[] m_pNameJap;
		m_pNameJap = nullptr;
		delete[] m_pNameEng;
		m_pNameEng = nullptr;
		delete[] m_pCommentJap;
		m_pCommentJap = nullptr;
		delete[] m_pCommentEng;
		m_pCommentEng = nullptr;
	}
	int m_NameJpnSize;		//モデル名(日)サイズ
	int m_NameEngSize;		//モデル名(英)サイズ
	int m_CommentJpnSize;	//コメント(日)サイズ
	int m_CommentEngSize;	//コメント(英)サイズ
	
	unsigned char* m_pNameJap;	//モデル名(日)
	unsigned char* m_pNameEng;	//モデル名(英)
	unsigned char* m_pCommentJap;//コメント(日)
	unsigned char* m_pCommentEng;//コメント(英)
};

//pmx頂点データ
struct PMX_VERTEX
{
	PMX_VERTEX()
	{
		m_pfAddUv = nullptr;
	}
	~PMX_VERTEX()
	{
		delete[] m_pfAddUv;
		m_pfAddUv = nullptr;
	}

	float m_fPos[3];
	float m_fNorm[3];
	float m_fUv[2];
	float* m_pfAddUv;	//追加UV
	PMX_WEIGHT_DATA m_WeightData;//ウェイトデータ
	float m_EdgeMagn;//エッジ倍率
};

//pmx面データ
struct PMX_FACE
{
	int m_VerIndex[3];//頂点
};

//pmxテクスチャ
struct PMX_TEXTURE
{
	PMX_TEXTURE()
	{
		m_pPass = nullptr;
	}
	~PMX_TEXTURE()
	{
		delete[] m_pPass;
		m_pPass = nullptr;
	}
	unsigned char* m_pPass;
};

//pmxデータ
struct PMX_DATA
{
	~PMX_DATA()
	{
		delete[] m_pVertex;
		m_pVertex = nullptr;
		delete[] m_pFace;
		m_pFace = nullptr;
		delete[] m_pTex;
		m_pTex = nullptr;
	}

	PMX_HEADER	m_Head;		//ヘッダー
	PMX_MODEL_INFO m_ModelInfo;//モデルデータ
	int			m_VerNum;	//頂点数
	PMX_VERTEX* m_pVertex;	//頂点データ
	int			m_FaceNum;	//面の数
	PMX_FACE*	m_pFace;	//面のデータ
	int			m_TexNum;	//テクスチャ数
	PMX_TEXTURE* m_pTex;	//テクスチャデータ

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
	//書き出し
	bool Write(const char* FileName, PMX_DATA* pPmxData);
private:
	//モデル情報読み込み
	void ModelInfoLoad(FILE* fp, PMX_DATA* pPmxData);

	//頂点読み込み
	void VertexLoad(FILE* fp, PMX_DATA* pPmxData);
	
	//面読み込み
	void FaceLoad(FILE* fp, PMX_DATA* pPmxData);
	
	//テクスチャ読み込み
	void TexLoad(FILE* fp, PMX_DATA* pPmxData);
};
