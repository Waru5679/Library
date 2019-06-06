#include "Hit.h"
#include "Math.h"

 CHit g_Hit;

//作成
SphereData CHit::Create(D3DXVECTOR3 vPos, float fRadius,D3DXVECTOR3 vScale)
{
	SphereData data;
	data.m_vPos = vPos;
	data.m_fRadius=fRadius;
	
	return data;
}

//登録
void CHit::Insert(SphereData* pData)
{
	m_data.push_back(pData);
}

//更新
void CHit::UpData(SphereData* pData,D3DXVECTOR3 vPos)
{
	pData->m_vPos = vPos;
}

//球衝突判定
bool CHit::SphereHit()
{
	return SphereAndSphre(m_data[0], m_data[1]);
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
