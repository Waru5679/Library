#pragma once

//ライブラリファイル
#include "Main.h"
#include "CObj.h"
#include "Singleton.h"
#include <vector>
using namespace std;

//出力データ
struct OutData
{
	D3DXVECTOR3 m_vRef;		//反射
	D3DXVECTOR3 m_vRub;		//壁すり
	D3DXVECTOR3 m_vInter;	//交点
	
	D3DXVECTOR3 m_vNorm;	//衝突面の法線
	CFaceData m_HitFace;	//衝突面
};

//レイの判定用クラス
class CRay:public CSingleton<CRay>
{
private:
	friend CSingleton<CRay>;
	CRay() {}	//コンストラクタ
	~CRay() {}	//デストラクタ
public:
	void Release();	//解放

	//登録
	void Insert(CObj3DBase* pObj);

	//RayのHit判定
	bool RayHit(OutData* pOut, CObj3DBase* pOrigin, D3DXVECTOR3 vDir);
	
private:
	//Rayの判定
	bool TriangleRay(D3DXVECTOR3* OutPoint, D3DXVECTOR3* OutNorm,D3DXVECTOR3 vRayOrigin, D3DXVECTOR3 vRayDir, D3DXVECTOR3 vA, D3DXVECTOR3 vB, D3DXVECTOR3 vC);

	//3角ポリゴンの重心の位置を求める
	D3DXVECTOR3 CenterOfGravity(D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3);

	//ターゲットになるポリゴンを決める
	bool TargetSet(OutData* pOut,D3DXVECTOR3 vOrigin, D3DXVECTOR3 vDir);
	
	//Ray発射点を決める
	bool OriginSet(OutData* pOut, CObj3DBase* pOrigin, D3DXVECTOR3 vDir);

	//壁擦り
	D3DXVECTOR3 WallShear(D3DXVECTOR3* Front, D3DXVECTOR3* Norm);

	//反射ベクトル
	D3DXVECTOR3 Reflection(D3DXVECTOR3* Front, D3DXVECTOR3* Norm);

	vector<CObj3DBase*> m_Data;//Ray判定を行うリスト
};