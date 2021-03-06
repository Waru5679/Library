#pragma once

#include"Singleton.h"

//ヘッダー
struct VMD_HEAD
{
	char m_Signature[30];	//シグネチャ
	char m_ModelName[20];	//モデル名
};

//モーション
struct VMD_MOTION
{
	char	BoneName[15];	//ボーン名
	int		FrameNum;		//フレームNo
	float	Pos[3];			//位置
	float	Rot[4];			//回転
	unsigned char Data[64];	//補完データ
};

//スキン
struct VMD_SKIN
{
	char SkinName[15];	//スキン名
	int FlameNum;		//フレームNo
	float Weight;		//ウェイト
};

//VMDデータ
struct VMD_DATA
{
	VMD_DATA()
	{
		m_pMotion = nullptr;
		m_pSkin = nullptr;
	}
	~VMD_DATA()
	{
		//モーションリスト
		if (m_pMotion != nullptr)
		{
			delete[] m_pMotion;
			m_pMotion = nullptr;
		}
		//スキンリスト
		if (m_pSkin != nullptr)
		{
			delete[] m_pSkin;
			m_pSkin = nullptr;
		}
	}

	unsigned int	m_MotionRecordNum;	//モーション数
	unsigned int	m_SkinRecordNum;	//スキン数
	VMD_HEAD		m_Head;				//ヘッダー
	VMD_MOTION*		m_pMotion;			//モーションリスト
	VMD_SKIN*		m_pSkin;			//スキンリスト
};

//VMD読み込み
class CVmdLoader:public CSingleton<CVmdLoader>
{
private:
	//シングルトン
	friend CSingleton<CVmdLoader>;
	CVmdLoader() {};
	~CVmdLoader() {};
public:
	//読み込み
	bool Load(const char* FileName, VMD_DATA* pVmdData);
	//書き出し
	bool Write(const char* FileName, VMD_DATA* pVmdData);
private:
};
