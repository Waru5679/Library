#pragma once

#include "CObj.h"
#include "Main.h"
#include <vector>
using namespace std;

//球データ
struct SphereData
{
	D3DXVECTOR3 m_vPos;	//位置
	float m_fRadius;	//半径
};

//当たり判定
struct CollisionData
{
	CObj3DBase* m_pObj;	//オブジェクトのポインタ
	MY_MESH* m_pMesh;	//メッシュポインタ
	int m_Id;			//オブジェクトID

	SphereData m_SphereData;	//球データ
};

//当たり判定クラス
class CHit
{
public:
	bool SphereHit();
	void UpData(CollisionData* pData, D3DXVECTOR3 vPos);

	//当たり判定データ登録
	CollisionData CollisionCreate(CObj3DBase* pCobj);
	void Insert(CollisionData* pData);
private:
	//球と級のヒット判定
	bool SphereAndSphre(SphereData* data1, SphereData* data2);
	
	vector<CollisionData* > m_Collision;	//当たり判定データ
};

extern CHit g_Hit;