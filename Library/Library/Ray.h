#pragma once

//ライブラリファイル
#include "Main.h"
#include "CObj.h"
#include <vector>
using namespace std;

//レイの判定用クラス
class CRay
{
public:
	void Release();	//解放

	//登録
	void Insert(CObj3DBase* pObj);

	//Ray判定を呼び出す
	bool RayHit(D3DXVECTOR3*OutPoint, CObj3DBase* pOrigin,D3DXVECTOR3 vDir, int Id);

private:
	//Rayの判定
	bool TriangleRay(D3DXVECTOR3* OutPoint,D3DXVECTOR3 vRayOrigin, D3DXVECTOR3 vRayDir, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC);

	////壁擦り
	//D3DXVECTOR3 WallShear(D3DXVECTOR3 Front, D3DXVECTOR3 Normal);

	vector<CObj3DBase*> m_Data;//Ray判定を行うリスト
};

extern CRay g_Ray;