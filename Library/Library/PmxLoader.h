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

//pmx頂点データ
struct PMX_VERTEX
{
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

//pmxデータ
struct PMX_DATA
{
	~PMX_DATA()
	{
		delete m_pVertex;
		m_pVertex = nullptr;
	}

	PMX_HEADER m_Head;//ヘッダー
	int VerNum;//頂点数
	PMX_VERTEX* m_pVertex;//頂点データ
	int FaceNum;//面の数
	PMX_FACE* m_pFace;//面のデータ

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
private:

	//頂点読み込み
	void VertexLoad(FILE* fp, PMX_DATA* pPmxData);
	
	//面読み込み
	void FaceLoad(FILE* fp, PMX_DATA* pPmxData);
	
};
