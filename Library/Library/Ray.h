#pragma once

//ライブラリファイル
#include "Main.h"
#include "Obb.h"
#include <vector>
using namespace std;

//レイの判定用クラス
class CRay
{
public:
	//登録
	void Insert(CObj3D* pObj);

	//Ray判定を呼び出す
	bool RayHit(D3DXVECTOR3* OutPoint,D3DXVECTOR3 Pos, D3DXVECTOR3 RayVec);

	//交点を取得する
	D3DXVECTOR3 GetShear(){ return m_vShear; }
private:
	//Rayの判定(四角)
	bool SquareRay(D3DXVECTOR3* OutPoint, D3DXVECTOR3 RayPos, D3DXVECTOR3 RayVec, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC, D3DXVECTOR3 vD);

	//Rayの判定(三角)
	bool TriangleRay(D3DXVECTOR3* OutPoint, D3DXVECTOR3 RayPos, D3DXVECTOR3 RayVec, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC);

	//壁ずり
	D3DXVECTOR3* WallShear(D3DXVECTOR3* Out, D3DXVECTOR3 Front, D3DXVECTOR3 Normal);

	D3DXVECTOR3 m_vNorm;	//法線
	D3DXVECTOR3 m_vShear;	//衝突時の交点

	vector<CObj3D*> m_Data;//Ray判定を行うリスト
};

extern CRay g_Ray;