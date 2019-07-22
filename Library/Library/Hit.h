#pragma once

#include "CObj.h"
#include "ModelManager.h"
#include "Singleton.h"
#include <vector>
using namespace std;

//球データ
class CSphereData
{
public:
	//開放
	void Release() {};

	D3DXVECTOR3 m_vPos;		//位置
	float m_fRadius;		//半径

	D3DXMATRIX m_matWorld;	//描画用
};

//Obbデータ
class ObbData
{
public:
	//開放
	void Release() {};

	D3DXVECTOR3 m_vDirect[3];	//各軸の面の方向
	D3DXVECTOR3 m_vLength;		//中心点から各軸への長さ
	D3DXVECTOR3 m_vCenterPos;	//中心点の位置

	D3DXMATRIX	m_matWorld;		//描画用
};

//当たり判定
class CCollisionData
{
public:
	//開放
	void Release()
	{
		m_SphereData.Release();
		m_ObbData.Release();
		m_pObj=nullptr;
		m_pMesh=nullptr;
	}

	CObj3DBase* m_pObj;	//オブジェクトのポインタ
	CModelData* m_pMesh;//メッシュポインタ
	int m_Id;			//オブジェクトID

	CSphereData	m_SphereData;	//球データ
	ObbData		m_ObbData;		//OBBデータ
};

//当たり判定クラス
class CHit:public CSingleton<CHit>
{
private:
	//シングルトン
	friend CSingleton<CHit>;
	CHit() {}	//コンストラクタ
	~CHit() {}	//デストラクタ
public:
	void Init();						//初期化
	void UpData(CCollisionData* pData);	//更新
	void Draw();						//描画
	void Release();						//解放

	//当たり判定
	bool Hit();

	//当たり判定データ登録
	CCollisionData CollisionCreate(CObj3DBase* pCobj);
	void Insert(CCollisionData* pData);

	//めり込み修正
	D3DXVECTOR3 Fixation(D3DXVECTOR3 Pos, D3DXVECTOR3 Norm, ObbData* pObb);
private:
	//球と球のヒット判定
	bool SphereAndSphre(CSphereData* data1, CSphereData* data2);

	//OBBにセット
	void SetObbData(CObj3DBase* pCobj, CModelData* pMesh, ObbData* pObb);

	//球情報セット
	void SetSphereData(CObj3DBase* pCobj,ObbData* pObb, CModelData* pMesh,CSphereData* pSphere);
	
	//衝突判定
	bool ObbAndObb(ObbData* obb1, ObbData* obb2);

	// 分離軸に投影された軸成分から投影線分長を算出
	float LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3);

	vector<CCollisionData*> m_Collision;	//当たり判定データ

	CModelData m_DrawObb;		//描画用Box
	CModelData m_DrawSphere;	//描画用Sphere
	CColorData m_Color;		//描画色
};