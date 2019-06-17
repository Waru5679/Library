#pragma once

//ライブラリファイル
#include "Main.h"
#include "CObj.h"
#include "Hit.h"
#include <vector>
using namespace std;

//出力データ
struct OutData
{
	D3DXVECTOR3 m_vRef;		//反射
	D3DXVECTOR3 m_vRub;		//壁すり
	D3DXVECTOR3 m_vInter;	//交点
};

//レイの判定用クラス
class CRay
{
public:
	void Release();	//解放

	//登録
	void Insert(CObj3DBase* pObj);

	//Ray判定を呼び出す
	bool RayHit(OutData* pOut, CObj3DBase* pOrigin, D3DXVECTOR3 vDir, int Id);

private:
	//Rayの判定
	bool TriangleRay(D3DXVECTOR3* OutPoint, D3DXVECTOR3* OutNorm,D3DXVECTOR3 vRayOrigin, D3DXVECTOR3 vRayDir, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC);

	//壁擦り
	D3DXVECTOR3 WallShear(D3DXVECTOR3* Front, D3DXVECTOR3* Norm);

	//反射ベクトル
	D3DXVECTOR3 Reflection(D3DXVECTOR3* Front, D3DXVECTOR3* Norm);

	vector<CObj3DBase*> m_Data;//Ray判定を行うリスト
};

extern CRay g_Ray;