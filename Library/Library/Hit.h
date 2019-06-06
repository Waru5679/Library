#pragma once
#include "Main.h"
#include <vector>
using namespace std;

//球データ
struct SphereData
{
	D3DXVECTOR3 m_vPos;	//位置
	float m_fRadius;	//半径
};

//当たり判定クラス
class CHit
{
public:
	SphereData Create(D3DXVECTOR3 vPos, float fRadius,D3DXVECTOR3 vScale);			//登録
	void Insert(SphereData* pData);
	bool SphereHit();
	void UpData(SphereData* pData, D3DXVECTOR3 vPos);
private:
	bool SphereAndSphre(SphereData* data1, SphereData* data2);//ヒット判定
	vector<SphereData*> m_data;	//球データ
};

extern CHit g_Hit;