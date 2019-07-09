#include "LibraryHeader.h"

//インスタンス
CHit* CHit::m_pInstance = nullptr;
 
 //登録
 void CHit::Insert(CollisionData* pData)
 {
	 // 登録
	 m_Collision.push_back(pData);
 }

 //初期化
 void CHit::Init()
 {
	 //HitBox
	 LOADER->LoadObj("Model/Hit/HitBox.obj", &m_DrawObb);

	 //球
	 LOADER->LoadObj("Model/Hit/Sphere.obj", &m_DrawSphere);

	 //色
	 m_Color = ColorData(1.0f, 1.0f, 1.0f, 0.5f);

 }

//当たり判定データ登録
CollisionData CHit::CollisionCreate(CObj3DBase* pCobj)
{
	//当たり判定情報
	CollisionData coll_data;
	coll_data.m_pObj = pCobj;
	coll_data.m_pMesh = pCobj->GetMesh();
	coll_data.m_Id = pCobj->GetId();
	
	//Obbデータ
	ObbData obb_data;
	SetObbData(pCobj, coll_data.m_pMesh, &obb_data);
	coll_data.m_ObbData = obb_data;

	//球情報
	SphereData sphere_data;
	SetSphereData(pCobj,&coll_data.m_ObbData, coll_data.m_pMesh, &sphere_data);
	coll_data.m_SphereData = sphere_data;
		
	return coll_data;
}

//衝突判定
bool CHit::Hit()
{
	for (unsigned int i = 0; i < m_Collision.size() - 1; i++)
	{
		for (unsigned int j = i + 1; j < m_Collision.size(); j++)
		{
			//球の衝突判定
			if (SphereAndSphre(&m_Collision[i]->m_SphereData, &m_Collision[j]->m_SphereData) == true)
			{
				//OBBの衝突判定
				if (ObbAndObb(&m_Collision[i]->m_ObbData, &m_Collision[j]->m_ObbData) == true)
				{
					return true;
				}
			}
		}
	}

	return false;
}

//更新
void CHit::UpData(CollisionData* pData)
{
	//球データ
	SetSphereData(pData->m_pObj,&pData->m_ObbData,pData->m_pMesh, &pData->m_SphereData);

	//OBBデータ
	SetObbData(pData->m_pObj, pData->m_pMesh, &pData->m_ObbData);
}

//描画
void CHit::Draw()
{
	D3DXMATRIX matWorld;
 
	for (unsigned int i = 0; i < m_Collision.size(); i++)
	{	
		//Obb描画
		LOADER->Draw(m_Collision[i]->m_ObbData.m_matWorld, &m_DrawObb,&m_Color);

		//球描画
		LOADER->Draw(m_Collision[i]->m_SphereData.m_matWorld,&m_DrawSphere, &m_Color);
	}
}

//解放
void CHit::Release()
{
	VectorRelease(m_Collision);

	//インスタンス破棄
	PointerRelease(m_pInstance);
}

// 分離軸に投影された軸成分から投影線分長を算出
float CHit::LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3 = 0)
{
	// 3つの内積の絶対値の和で投影線分長を計算
	float r1 = (float)fabs(D3DXVec3Dot(Sep, e1));
	float r2 = (float)fabs(D3DXVec3Dot(Sep, e2));
	float r3 = e3 ? ((float)fabs(D3DXVec3Dot(Sep, e3))) : 0;
	return r1 + r2 + r3;
}

//OBBとOBBの衝突判定
bool CHit::ObbAndObb(ObbData* obb1, ObbData* obb2)
{
	// 各方向ベクトルの確保
	D3DXVECTOR3 NAe1 = obb1->m_vDirect[0], Ae1 = NAe1 * obb1->m_vLength.x;
	D3DXVECTOR3 NAe2 = obb1->m_vDirect[1], Ae2 = NAe2 * obb1->m_vLength.y;
	D3DXVECTOR3 NAe3 = obb1->m_vDirect[2], Ae3 = NAe3 * obb1->m_vLength.z;

	D3DXVECTOR3 NBe1 = obb2->m_vDirect[0], Be1 = NBe1 * obb2->m_vLength.x;
	D3DXVECTOR3 NBe2 = obb2->m_vDirect[1], Be2 = NBe2 * obb2->m_vLength.y;
	D3DXVECTOR3 NBe3 = obb2->m_vDirect[2], Be3 = NBe3 * obb2->m_vLength.z;
	D3DXVECTOR3 Interval = obb1->m_vCenterPos - obb2->m_vCenterPos;

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
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C12
	D3DXVec3Cross(&Cross, &NAe1, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C13
	D3DXVec3Cross(&Cross, &NAe1, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C21
	D3DXVec3Cross(&Cross, &NAe2, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C22
	D3DXVec3Cross(&Cross, &NAe2, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C23
	D3DXVec3Cross(&Cross, &NAe2, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C31
	D3DXVec3Cross(&Cross, &NAe3, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C32
	D3DXVec3Cross(&Cross, &NAe3, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C33
	D3DXVec3Cross(&Cross, &NAe3, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
		return false;

	// 分離平面が存在しないので「衝突している」
	return true;
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

//球情報セット
void CHit::SetSphereData(CObj3DBase* pCobj,ObbData* pObb, MY_MESH* pMesh,SphereData* pSphere)
{
	//中心点から面の長さ
	float x, y, z;
	x = pObb->m_vLength.x;
	y = pObb->m_vLength.y;
	z = pObb->m_vLength.z;

	//円の半径
	float xy, xz, yz;
	xy = sqrtf(x * x + y * y);
	xz = sqrtf(x * x + z * z);
	yz = sqrtf(y * y + z * z);

	//球の半径
	float xy_z, xz_y, yz_x;
	xy_z = sqrtf(xy * xy + z * z);
	xz_y = sqrtf(xz * xz + y * y);
	yz_x = sqrtf(yz * yz + x * x);

	//半径
	float radius = MostLongComponent(xy_z, xz_y, yz_x);

	//描画用にワールドマトリックスを作る
	D3DXVECTOR3 vPos = pCobj->GetPos();
	D3DXVECTOR3 vAngle = pCobj->GetAngle();
	D3DXVECTOR3 vScale = D3DXVECTOR3(radius,radius,radius);
	D3DXMATRIX	matWorld = MakeMatWorld(vPos, vAngle, vScale);

	//球データを格納
	pSphere->m_fRadius = radius;
	pSphere->m_vPos = vPos;
	pSphere->m_matWorld = matWorld;
}

//OBBに数値セット
void CHit::SetObbData(CObj3DBase* pCobj, MY_MESH* pMesh, ObbData* pObb)
{
	D3DXVECTOR3 vPos, vAngle, vScale;
	vPos	= pCobj->GetPos();
	vAngle	= pCobj->GetAngle();
	vScale	= pCobj->GetScale();
	
	//行列
	D3DXMATRIX matRot = MakeMatRot(vAngle);
	D3DXMATRIX matScale = MakeMatScale(vScale);

	//スケールから最大・最小を求める
	D3DXVECTOR3 vMin, vMax;
	D3DXVec3TransformCoord(&vMax, &pMesh->vMax, &matScale);
	D3DXVec3TransformCoord(&vMin, &pMesh->vMin, &matScale);

	//中心点
	D3DXVECTOR3 vCenterPos = (vMin + vMax) / 2.0f + vPos;
	pObb->m_vCenterPos = vCenterPos;

	//中心点から各軸への長さ 
	pObb->m_vLength = MakeAbsVector3(vMax - vMin) / 2.0f;

	//中心点から各軸への長さ
	pObb->m_vDirect[0] = D3DXVECTOR3(matRot._11, matRot._12, matRot._13);
	pObb->m_vDirect[1] = D3DXVECTOR3(matRot._21, matRot._22, matRot._23);
	pObb->m_vDirect[2] = D3DXVECTOR3(matRot._31, matRot._32, matRot._33);

	//描画用にワールドマトリックス作る
	pObb->m_matWorld = MakeMatWorld(vCenterPos, vAngle, vScale);
}

//めり込み修正
D3DXVECTOR3 CHit::Fixation(D3DXVECTOR3 Pos, D3DXVECTOR3 Norm, ObbData* pObb)
{
	//法線正規化
	D3DXVECTOR3 Norm_n;
	D3DXVec3Normalize(&Norm_n, &Norm);

	//Obbの中心点
	D3DXVECTOR3 Center = pObb->m_vCenterPos;

	//面から中心点へのベクトル
	D3DXVECTOR3 FaceFromCenter = Center - Pos;

	//各軸の方向ベクトル
	D3DXVECTOR3 Dir_x = pObb->m_vDirect[0];
	D3DXVECTOR3 Dir_y = pObb->m_vDirect[1];
	D3DXVECTOR3 Dir_z = pObb->m_vDirect[2];

	//各軸のベクトル長さ
	float Length_x = pObb->m_vLength.x;
	float Length_y = pObb->m_vLength.y;
	float Length_z = pObb->m_vLength.z;

	//各軸のベクトルと面の法線の内積
	float dot_x = D3DXVec3Dot(&(Dir_x * Length_x), &Norm_n);
	float dot_y = D3DXVec3Dot(&(Dir_y * Length_y), &Norm_n);
	float dot_z = D3DXVec3Dot(&(Dir_z * Length_z), &Norm_n);
	
	//中心と頂点の長さ
	float r = fabsf(dot_x) + fabsf(dot_y) + fabsf(dot_z);

	//面と中心の長さ
	float s = fabsf(D3DXVec3Dot(&FaceFromCenter, &Norm_n));

	//面から中心点へのベクトルと法線の内積
	float dot = D3DXVec3Dot(&FaceFromCenter, &Norm_n);

	//戻す長さ
	float fix_length;

	//中心が面の法線と同じ方向にある
	if (dot > 0.0f)
	{
		fix_length = r - s;
	}
	//中心が面の法線と反対の方向にある
	else
	{
		fix_length = r + s;
	}

	//面の法線の方向へ戻す
	D3DXVECTOR3 vOut = Norm * fix_length;

	return vOut;
}