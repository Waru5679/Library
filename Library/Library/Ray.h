#pragma once
#include "Main.h"
#include "Obb.h"

//レイの判定用クラス
class CRay
{
public:
	//Ray判定を呼び出す
	bool RayHit(D3DXVECTOR3* OutPoint,D3DXVECTOR3 Pos, D3DXVECTOR3 RayVec, OBB_Data Obb);

	//交点を取得する
	D3DXVECTOR3 GetShear(){ return m_vShear; }
private:
	//Rayの判定
	bool Ray(D3DXVECTOR3* OutPoint, D3DXVECTOR3 RayPos, D3DXVECTOR3 RayVec, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC, D3DXVECTOR3 vD);

	//壁ずり
	D3DXVECTOR3* WallShear(D3DXVECTOR3* Out, D3DXVECTOR3 Front, D3DXVECTOR3 Normal);

	D3DXVECTOR3 m_vNorm;	//法線
	D3DXVECTOR3 m_vShear;	//衝突時の交点
};

extern CRay g_Ray;