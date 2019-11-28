#pragma once
#include "Singleton.h"

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

	unsigned char WeightType;//ウェイト変形方式
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
	PMX_VERTEX* m_pVertex;
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
	
};
