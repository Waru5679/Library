
#include "Obb.h"
#include "Math.h"
#include "CObj.h"

COBB g_Obb;

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

//初期化
void COBB::Init()
{
	//HitBox描画用オブジェクト
	g_Loader.LoadObj("HitBox/HitBox.obj", &m_Mesh);

	//描画色
	SetColor(1.0f, 1.0f, 1.0f, 0.5f, m_fColor);
}

//OBB登録
void COBB::Insert(OBB_Data* obb)
{
	m_Data.push_back(obb);
}

//OBBのセット
OBB_Data COBB::SetOBB(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale, D3DXVECTOR3 Min, D3DXVECTOR3 Max, int ObjName, CObj* p)
{
	OBB_Data obb;

	D3DXMATRIX matScale = MakeMatScale(Scale);
	D3DXMATRIX matRot = MakeMatRot(Angle);
	D3DXMATRIX matTrans = MakeMatTrans(Pos);
	obb.matWorld = matScale * matRot*matTrans;

	//スケールから最大・最小を求める
	D3DXVec3TransformCoord(&obb.vMax, &Max, &matScale);
	D3DXVec3TransformCoord(&obb.vMin, &Min, &matScale);

	//中心点
	D3DXVECTOR3 vTempPos = (obb.vMin + obb.vMax) * 0.5f + Pos;
	obb.m_Pos = vTempPos;

	//回転
	obb.m_Angle = Angle;

	//スケール
	obb.m_Scale = Scale;

	//長さ取得 
	obb.m_Length[0] = fabsf(obb.vMax.x - obb.vMin.x) /** 0.5f*/;
	obb.m_Length[1] = fabsf(obb.vMax.y - obb.vMin.y) /** 0.5f*/;
	obb.m_Length[2] = fabsf(obb.vMax.z - obb.vMin.z) /** 0.5f*/;

	//中心からの長さ
	obb.m_Direct[0] = D3DXVECTOR3(matRot._11, matRot._12, matRot._13);
	obb.m_Direct[1] = D3DXVECTOR3(matRot._21, matRot._22, matRot._23);
	obb.m_Direct[2] = D3DXVECTOR3(matRot._31, matRot._32, matRot._33);

	obb.Name=ObjName;
	obb.p=p;

	//頂点
	obb.VerPos[0] = D3DXVECTOR3(vTempPos.x + obb.vMin.x, vTempPos.y + obb.vMax.y, vTempPos.z + obb.vMin.z);
	obb.VerPos[1] = D3DXVECTOR3(vTempPos.x + obb.vMax.x, vTempPos.y + obb.vMax.y, vTempPos.z + obb.vMin.z);
	obb.VerPos[2] = D3DXVECTOR3(vTempPos.x + obb.vMax.x, vTempPos.y + obb.vMin.y, vTempPos.z + obb.vMin.z);
	obb.VerPos[3] = D3DXVECTOR3(vTempPos.x + obb.vMin.x, vTempPos.y + obb.vMin.y, vTempPos.z + obb.vMin.z);

	obb.VerPos[4] = D3DXVECTOR3(vTempPos.x + obb.vMin.x, vTempPos.y + obb.vMax.y, vTempPos.z + obb.vMax.z);
	obb.VerPos[5] = D3DXVECTOR3(vTempPos.x + obb.vMax.x, vTempPos.y + obb.vMax.y, vTempPos.z + obb.vMax.z);
	obb.VerPos[6] = D3DXVECTOR3(vTempPos.x + obb.vMax.x, vTempPos.y + obb.vMin.y, vTempPos.z + obb.vMax.z);
	obb.VerPos[7] = D3DXVECTOR3(vTempPos.x + obb.vMin.x, vTempPos.y + obb.vMin.y, vTempPos.z + obb.vMax.z);

	//法線
	for (int i = 0; i < 6; i++)
	{
		D3DXVec3TransformCoord(&obb.Norm[i], &g_Norm[i], &matRot);
	}

	//メッシュ情報
	obb.pMesh = dynamic_cast<CObj3D*>(p)->GetMesh();

	return obb;

}

//当たり判定(オブジェクト名指定)
vector<HIT_DATA> COBB::ObjNameHit(OBB_Data* Obb,int Name)
{
	vector<HIT_DATA> vecData;
	HIT_DATA data;
	bool bHit=false;
	for (unsigned int i = 0; i<m_Data.size(); i++)
	{
		if (m_Data[i] != Obb)
		{
			if (m_Data[i]->Name == Name)
			{
				bHit = Collision(Obb, m_Data[i]);
				if (bHit == true)
				{
					//詳細情報を入れる

					//方向
					D3DXVECTOR3 vDir = Obb->m_Pos - m_Data[i]->m_Pos;
					data.vDir = vDir;
					
					//距離
					data.fDis = D3DXVec3Length(&vDir);
				
					//方向の正規化
					D3DXVec3Normalize(&vDir, &vDir);
					data.vnDir = vDir;	

					//オブジェクトのサイズ
					data.vMin = m_Data[i]->vMin;
					data.vMax = m_Data[i]->vMax;
					
					//オブジェクトのポインタ
					data.p=m_Data[i]->p;

					//OBBのポインタ
					data.obb = m_Data[i];

					vecData.push_back(data);
				}
			}
		}
	}
	return vecData;
}


//更新
void COBB::Update(OBB_Data* obb,D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale, D3DXVECTOR3 Min, D3DXVECTOR3 Max)
{
	D3DXMATRIX matScale = MakeMatScale(Scale);
	D3DXMATRIX matRot = MakeMatRot(Angle);
	D3DXMATRIX matTrans = MakeMatTrans(Pos);
	obb->matWorld = matScale * matRot*matTrans;

	//最小点、最大点
	D3DXVec3TransformCoord(&obb->vMax, &Max, &matScale);
	D3DXVec3TransformCoord(&obb->vMin, &Min, &matScale);

	//中心点
	D3DXVECTOR3 vTempPos = (Min + Max) * 0.5f + Pos;
	obb->m_Pos = vTempPos;

	//回転
	obb->m_Angle = Angle;

	//スケール
	obb->m_Scale = Scale;

	//長さ取得
	obb->m_Length[0] = fabsf(obb->vMax.x - obb->vMin.x)/**0.5f*/;
	obb->m_Length[1] =fabsf(obb->vMax.y - obb->vMin.y)/**0.5f*/;
	obb->m_Length[2] =fabsf(obb->vMax.z - obb->vMin.z)/**0.5f*/;

	//中心からの長さ
	obb->m_Direct[0] = D3DXVECTOR3(matRot._11, matRot._12, matRot._13);
	obb->m_Direct[1] = D3DXVECTOR3(matRot._21, matRot._22, matRot._23);
	obb->m_Direct[2] = D3DXVECTOR3(matRot._31, matRot._32, matRot._33);
	
	//頂点
	obb->VerPos[0] = D3DXVECTOR3(vTempPos.x + obb->vMin.x, vTempPos.y + obb->vMax.y, vTempPos.z + obb->vMin.z);
	obb->VerPos[1] = D3DXVECTOR3(vTempPos.x + obb->vMax.x, vTempPos.y + obb->vMax.y, vTempPos.z + obb->vMin.z);
	obb->VerPos[2] = D3DXVECTOR3(vTempPos.x + obb->vMax.x, vTempPos.y + obb->vMin.y, vTempPos.z + obb->vMin.z);
	obb->VerPos[3] = D3DXVECTOR3(vTempPos.x + obb->vMin.x, vTempPos.y + obb->vMin.y, vTempPos.z + obb->vMin.z);

	obb->VerPos[4] = D3DXVECTOR3(vTempPos.x + obb->vMin.x, vTempPos.y + obb->vMax.y, vTempPos.z + obb->vMax.z);
	obb->VerPos[5] = D3DXVECTOR3(vTempPos.x + obb->vMax.x, vTempPos.y + obb->vMax.y, vTempPos.z + obb->vMax.z);
	obb->VerPos[6] = D3DXVECTOR3(vTempPos.x + obb->vMax.x, vTempPos.y + obb->vMin.y, vTempPos.z + obb->vMax.z);
	obb->VerPos[7] = D3DXVECTOR3(vTempPos.x + obb->vMin.x, vTempPos.y + obb->vMin.y, vTempPos.z + obb->vMax.z);

	//法線
	for (int i = 0; i < 6; i++)
	{
		D3DXVec3TransformCoord(&obb->Norm[i], &g_Norm[i], &matRot);
	}

}

//OBBの描画
void COBB::Draw()
{
	for (unsigned int i = 0; i < m_Data.size(); i++)
	{
		g_Loader.Draw(m_Data[i]->matWorld, &m_Mesh, m_fColor);
	}
}

//めり込み修正
D3DXVECTOR3 COBB::NestingCorrection(OBB_Data* StopObb, OBB_Data* MoveObb)
{
	//OBBの頂点の要素数
	int VerNum[6] = { 0,1,5,4,0,3 };

	D3DXVECTOR3 Point[6];	//平面上の点
	D3DXVECTOR3 Norm[6];	//法線

	for (int i = 0; i < 6; i++)
	{
		Norm[i] = StopObb->Norm[i];
		Point[i] = StopObb->VerPos[ VerNum[i] ];
	}

	//スケール
	D3DXVECTOR3 Scale = MoveObb->m_Scale;

	//中心点から面へのの方向ベクトル
	D3DXVECTOR3 Dir_x = MoveObb->m_Direct[0];
	D3DXVECTOR3 Dir_y = MoveObb->m_Direct[1];
	D3DXVECTOR3 Dir_z = MoveObb->m_Direct[2];

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
		r =  ux+uy+uz;
		
		D3DXVECTOR3 PC = MoveObb->m_Pos - Point[i];
		b = fabsf(D3DXVec3Dot(&PC, &Norm[i]));

		if (r < b)
		{
			//当たってない
			a = -1;
		}
		else
		{
			float e = D3DXVec3Dot(&PC, &Norm[i]);
			if ( e<= 0)
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

// 分離軸に投影された軸成分から投影線分長を算出
float COBB::LenSegOnSeparateAxis(D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3 =0)
{
	// 3つの内積の絶対値の和で投影線分長を計算
	// 分離軸Sepは標準化されていること
	float r1 = (float)fabs(D3DXVec3Dot(Sep, e1));
	float r2 = (float)fabs(D3DXVec3Dot(Sep, e2));
	float r3 = e3 ? ((float)fabs(D3DXVec3Dot(Sep, e3))) : 0;
	return r1 + r2 + r3;
}

//衝突判定
bool COBB::Collision(OBB_Data* obb1, OBB_Data* obb2)
{
	// 各方向ベクトルの確保
	// （N***:標準化方向ベクトル）
	D3DXVECTOR3 NAe1 = obb1->m_Direct[0], Ae1 = NAe1 * obb1->m_Length[0];
	D3DXVECTOR3 NAe2 = obb1->m_Direct[1], Ae2 = NAe2 * obb1->m_Length[1];
	D3DXVECTOR3 NAe3 = obb1->m_Direct[2], Ae3 = NAe3 * obb1->m_Length[2];

	D3DXVECTOR3 NBe1 = obb2->m_Direct[0], Be1 = NBe1 * obb2->m_Length[0];
	D3DXVECTOR3 NBe2 = obb2->m_Direct[1], Be2 = NBe2 * obb2->m_Length[1];
	D3DXVECTOR3 NBe3 = obb2->m_Direct[2], Be3 = NBe3 * obb2->m_Length[2];
	D3DXVECTOR3 Interval = obb1->m_Pos - obb2->m_Pos;
	
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
	L =(float)fabs(D3DXVec3Dot(&Interval, &NBe1));
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
	L =(float) fabs(D3DXVec3Dot(&Interval, &Cross));
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



