#include "Hit.h"
#include "Math.h"

 CHit g_Hit;
 
//当たり判定データ登録
CollisionData CHit::CollisionCreate(CObj3DBase* pCobj)
{
	//当たり判定情報
	CollisionData coll_data;
	coll_data.m_pObj = pCobj;
	coll_data.m_pMesh = pCobj->GetMesh();
	coll_data.m_Id = pCobj->GetId();
	
	//半径求める
	D3DXVECTOR3 vMax, vMin;
	vMax = coll_data.m_pMesh->vMax;
	vMin = coll_data.m_pMesh->vMin;

	//一番長い距離から半径を設定
	float diameter = MostLongComponent(vMax - vMin);	//直径
	float radius = fabsf(diameter) / 2.0f;				//半径

	//球データを格納
	coll_data.m_SphereData.m_fRadius = radius;
	coll_data.m_SphereData.m_vPos = pCobj->GetPos();
	
	return coll_data;
}

void CHit::Insert(CollisionData* pData)
{
	// 登録
	m_Collision.push_back(pData);
}

//更新
void CHit::UpData(CollisionData* pData,D3DXVECTOR3 vPos)
{
	pData->m_SphereData.m_vPos = vPos;
}

//球衝突判定
bool CHit::SphereHit()
{
	return SphereAndSphre( &m_Collision[0]->m_SphereData, &m_Collision[1]->m_SphereData);
}

//球と球の衝突判定
bool CHit::SphereAndSphre(SphereData* data1, SphereData* data2)
{
	//2点間の距離
	float PosLength;	
	PosLength = D3DXVec3Length(&(data1->m_vPos - data2->m_vPos));
	
	//２つの半径の和
	float RadiusLength;
	RadiusLength = data1->m_fRadius + data2->m_fRadius;

	//2点間の距離が半径の和より小さければ当たっている
	if (PosLength <= RadiusLength)
	{
		return true;
	}

	return false;
}
