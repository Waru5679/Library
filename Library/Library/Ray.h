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
	bool RayHit(D3DXVECTOR3*OutPoint, CObj3D* pOrigin, D3DXVECTOR3 vDir, int Id);

private:
	//Rayの判定(四角)
	bool SquareRay(D3DXVECTOR3* OutPoint, D3DXVECTOR3 RayPos, D3DXVECTOR3 RayVec, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC, D3DXVECTOR3 vD, D3DXVECTOR3 vNorm);

	//Rayの判定(三角)
	bool TriangleRay(D3DXVECTOR3* OutPoint,D3DXVECTOR3 vRayOrigin, D3DXVECTOR3 vRayDir, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC);

	//壁擦り
	D3DXVECTOR3 WallShear(D3DXVECTOR3 Front, D3DXVECTOR3 Normal);


	vector<CObj3D*> m_Data;//Ray判定を行うリスト
};

extern CRay g_Ray;