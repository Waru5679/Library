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
	
	//一番長い距離から半径を設定
	float diameter = MostLongComponent(vMax - vMin)* scale;	//直径
	float radius = fabsf(diameter) / 2.0f;					//半径

	//debug用球のサイズ変更
	pCobj->SetScale(scale);

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
	pData->m_ObbData = SetObbData(pData->m_pObj, pData->m_pMesh);
}

//球衝突判定
bool CHit::Hit(D3DXVECTOR3* vShear)
{
	if (ObbAndObb(&m_Collision[0]->m_ObbData, &m_Collision[1]->m_ObbData) == true)
	{
		D3DXVECTOR3 vOut;
		vOut=NestingCorrection(&m_Collision[1]->m_ObbData, &m_Collision[0]->m_ObbData);
		vShear->x = vOut.x;
		vShear->y = vOut.y;
		vShear->z = vOut.z;

		return true;
	}
	return false;
	//return SphereAndSphre( &m_Collision[0]->m_SphereData, &m_Collision[1]->m_SphereData);
}

//球と球の衝突判定
bool CHit::SphereAndSphre(SphereData* pData1, SphereData* pData2)
{
	//2点間の距離
	float PosLength;	
	PosLength = D3DXVec3Length(&(pData1->m_vPos - pData2->m_vPos));
	
	//２つの半径の和
	float RadiusLength;
	RadiusLength = pData1->m_fRadius + pData2->m_fRadius;

	//2点間の距離が半径の和より小さければ当たっている
	if (PosLength <= RadiusLength)
	{
		return true;
	}

	return false;
}

//OBBとOBBの判定
bool CHit::ObbAndObb(ObbData* pData1, ObbData* pData2)
{
	// 各方向ベクトルの確保
	// （N***:標準化方向ベクトル）
	D3DXVECTOR3 NAe1 = pData1->m_vDirect[0], Ae1 = NAe1 * pData1->m_vLength.x;
	D3DXVECTOR3 NAe2 = pData1->m_vDirect[1], Ae2 = NAe2 * pData1->m_vLength.y;
	D3DXVECTOR3 NAe3 = pData1->m_vDirect[2], Ae3 = NAe3 * pData1->m_vLength.z;

	D3DXVECTOR3 NBe1 = pData2->m_vDirect[0], Be1 = NBe1 * pData2->m_vLength.x;
	D3DXVECTOR3 NBe2 = pData2->m_vDirect[1], Be2 = NBe2 * pData2->m_vLength.y;
	D3DXVECTOR3 NBe3 = pData2->m_vDirect[2], Be3 = NBe3 * pData2->m_vLength.z;
	D3DXVECTOR3 Interval = pData1->m_vCenterPos - pData2->m_vCenterPos;

	// 分離軸 : Ae1
	FLOAT rA = D3DXVec3Length(&Ae1);
	FLOAT rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	FLOAT L = (float)fabs(D3DXVec3Dot(&Interval, &NAe1));
	if (L > rA + rB)
		return false; // 衝突していない

	// 分離軸 : Ae2
	rA = D3DXVec3Length(&Ae2);
	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &NAe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Ae3
	rA = D3DXVec3Length(&Ae3);
	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &NAe3));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be1
	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be1);
	L = (float)fabs(D3DXVec3Dot(&Interval, &NBe1));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be2
	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be2);
	L = (float)fabs(D3DXVec3Dot(&Interval, &NBe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be3
	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &NBe3));
	if (L > rA + rB)
		return false;

	// 分離軸 : C11
	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross, &NAe1, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3,NULL);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3,NULL);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C12
	D3DXVec3Cross(&Cross, &NAe1, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3,NULL);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3,NULL);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C13
	D3DXVec3Cross(&Cross, &NAe1, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3,NULL);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2,NULL);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C21
	D3DXVec3Cross(&Cross, &NAe2, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3,NULL);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3,NULL);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C22
	D3DXVec3Cross(&Cross, &NAe2, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3,NULL);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3,NULL);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C23
	D3DXVec3Cross(&Cross, &NAe2, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3,NULL);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2,NULL);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C31
	D3DXVec3Cross(&Cross, &NAe3, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2,NULL);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3,NULL);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C32
	D3DXVec3Cross(&Cross, &NAe3, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2,NULL);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3,NULL);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C33
	D3DXVec3Cross(&Cross, &NAe3, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2,NULL);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2,NULL);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離平面が存在しないので「衝突している」
	return true;
}

// 分離軸に投影された軸成分から投影線分長を算出
float CHit::LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3)
{
	// 3つの内積の絶対値の和で投影線分長を計算
	// 分離軸Sepは標準化されていること
	float r1 = (float)fabs(D3DXVec3Dot(Sep, e1));
	float r2 = (float)fabs(D3DXVec3Dot(Sep, e2));

	float r3;
	if (e3 == NULL)
		r3 = 0;
	else
		r3 = ((float)fabs(D3DXVec3Dot(Sep, e3)));

	return r1 + r2 + r3;
}

//法線
D3DXVECTOR3 g_Norm[] =
{
	D3DXVECTOR3(0,0,-1),
	D3DXVECTOR3(1,0,0),
	D3DXVECTOR3(0,0,1),
	D3DXVECTOR3(-1,0,0),
	D3DXVECTOR3(0,1,0),
	D3DXVECTOR3(0,-1,0)
};

//OBBのめり込み修正
D3DXVECTOR3 CHit::NestingCorrection(ObbData* StopObb, ObbData* MoveObb)
{
	//OBBの頂点の要素数
	int VerNum[6] = { 0,1,5,4,0,3 };

	D3DXVECTOR3 Point[6];	//平面上の点
	D3DXVECTOR3 Norm[6];	//法線

	//回転行列
	D3DXMATRIX matRot = MakeMatRot(StopObb->m_vAngle);

	for (int i = 0; i < 6; i++)
	{
		//法線
		D3DXVec3TransformCoord(&Norm[i], &g_Norm[i], &matRot);
		Point[i] = StopObb->m_vVertexPos[ VerNum[i] ];
	}

	//スケール
	D3DXVECTOR3 Scale = MoveObb->m_vScale;

	//中心点から面へのの方向ベクトル
	D3DXVECTOR3 Dir_x = MoveObb->m_vDirect[0];
	D3DXVECTOR3 Dir_y = MoveObb->m_vDirect[1];
	D3DXVECTOR3 Dir_z = MoveObb->m_vDirect[2];

	//中心から面へのベクトル
	D3DXVECTOR3 ex, ey, ez;
	ex = Dir_x * Scale.x;
	ey = Dir_y * Scale.y;
	ez = Dir_z * Scale.z;

	float r;	//中心点から衝突してる頂点への長さ
	float b;	//中心点から平面への長さ
	float a;	//めり込みの長さ
	for (int i = 0; i < 6; i++)
	{
		float ux, uy, uz;
		ux = fabsf(D3DXVec3Dot(&ex, &Norm[i]));
		uy = fabsf(D3DXVec3Dot(&ey, &Norm[i]));
		uz = fabsf(D3DXVec3Dot(&ez, &Norm[i]));
		r = ux + uy + uz;

		D3DXVECTOR3 PC = MoveObb->m_vCenterPos - Point[i];
		b = fabsf(D3DXVec3Dot(&PC, &Norm[i]));

		if (r < b)
		{
			//当たってない
			a = -1;
		}
		else
		{
			float e = D3DXVec3Dot(&PC, &Norm[i]);
			if (e <= 0)
			{
				//PCとnの内積がマイナス
				a = r + b;
			}
			else
			{
				//PCとnの内積がプラス
				a = r - b;
			}
		}

		//衝突してるとき
		if (a != -1)
		{
			//法線＊めり込みの長さ
			return Norm[i] * a;
		}
	}

	//衝突なし
	return D3DXVECTOR3(0, 0, 0);
}

