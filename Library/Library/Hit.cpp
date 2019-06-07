#include "Hit.h"
#include "Math.h"

 CHit g_Hit;
 
//当たり判定データ登録
CollisionData CHit::CollisionCreate(CObj3DBase* pCobj)
{
	//HitBox描画用オブジェクト
	g_Loader.LoadObj("Model/HitBox/HitBox.obj", &m_DrawObb);

	//当たり判定情報
	CollisionData coll_data;
	coll_data.m_pObj = pCobj;
	coll_data.m_pMesh = pCobj->GetMesh();
	coll_data.m_Id = pCobj->GetId();
	
	//球情報
	coll_data.m_SphereData = SetSphereData(pCobj, coll_data.m_pMesh);
	
	//Obbデータ
	coll_data.m_ObbData = SetObbData(pCobj,coll_data.m_pMesh);
	
	return coll_data;
}

//球情報セット
SphereData CHit::SetSphereData(CObj3DBase* pCobj, MY_MESH* pMesh)
{
	SphereData sphere_data;

	//半径求める
	D3DXVECTOR3 vMax, vMin;
	vMax = pMesh->vMax;
	vMin = pMesh->vMin;

	//スケール
	D3DXVECTOR3 vScale = pCobj->GetScale();
	
	//一番大きい倍率をとる
	float scale=MostLongComponent(vScale);
	
	//debug用球のサイズ変更
	pCobj->SetScale(scale);

	//一番長い距離から半径を設定
	float diameter = MostLongComponent(vMax - vMin)* scale;	//直径
	float radius = fabsf(diameter) / 2.0f;					//半径

	//球データを格納
	sphere_data.m_fRadius = radius;
	sphere_data.m_vPos = pCobj->GetPos();

	return sphere_data;
}

//OBBに数値セット
ObbData CHit::SetObbData(CObj3DBase* pCobj,MY_MESH* pMesh)
{
	ObbData obb_data;
	
	//OBBデータ
	obb_data.m_vPos = pCobj->GetPos();
	obb_data.m_vAngle = pCobj->GetAngle();
	obb_data.m_vScale = pCobj->GetScale();
	
	//行列
	D3DXMATRIX matRot = MakeMatRot(obb_data.m_vAngle);
	D3DXMATRIX matScale=MakeMatScale(obb_data.m_vScale);
	
	//スケールから最大・最小を求める
	D3DXVECTOR3 vMin, vMax;
	D3DXVec3TransformCoord(&vMax, &pMesh->vMax, &matScale);
	D3DXVec3TransformCoord(&vMin, &pMesh->vMin, &matScale);

	//中心点
	D3DXVECTOR3 vCenterPos = (vMin + vMax) / 2.0f + obb_data.m_vPos;
	obb_data.m_vCenterPos = vCenterPos;

	//中心点から各軸への長さ 
	obb_data.m_vLength = MakeAbsVector3(vMax - vMin) / 2.0f;

	//中心点から各軸への長さ
	obb_data.m_vDirect[0] = D3DXVECTOR3(matRot._11, matRot._12, matRot._13);
	obb_data.m_vDirect[1] = D3DXVECTOR3(matRot._21, matRot._22, matRot._23);
	obb_data.m_vDirect[2] = D3DXVECTOR3(matRot._31, matRot._32, matRot._33);

	//頂点の位置
	obb_data.m_vVertexPos[0] = D3DXVECTOR3(vCenterPos.x + vMin.x, vCenterPos.y + vMax.y, vCenterPos.z + vMin.z);
	obb_data.m_vVertexPos[1] = D3DXVECTOR3(vCenterPos.x + vMax.x, vCenterPos.y + vMax.y, vCenterPos.z + vMin.z);
	obb_data.m_vVertexPos[2] = D3DXVECTOR3(vCenterPos.x + vMax.x, vCenterPos.y + vMin.y, vCenterPos.z + vMin.z);
	obb_data.m_vVertexPos[3] = D3DXVECTOR3(vCenterPos.x + vMin.x, vCenterPos.y + vMin.y, vCenterPos.z + vMin.z);
	obb_data.m_vVertexPos[4] = D3DXVECTOR3(vCenterPos.x + vMin.x, vCenterPos.y + vMax.y, vCenterPos.z + vMax.z);
	obb_data.m_vVertexPos[5] = D3DXVECTOR3(vCenterPos.x + vMax.x, vCenterPos.y + vMax.y, vCenterPos.z + vMax.z);
	obb_data.m_vVertexPos[6] = D3DXVECTOR3(vCenterPos.x + vMax.x, vCenterPos.y + vMin.y, vCenterPos.z + vMax.z);
	obb_data.m_vVertexPos[7] = D3DXVECTOR3(vCenterPos.x + vMin.x, vCenterPos.y + vMin.y, vCenterPos.z + vMax.z);

	return obb_data;
}

//登録
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
