#pragma once

#include "CObj.h"
#include "Singleton.h"
#include <vector>
using namespace std;

//球データ
struct SphereData
{
	D3DXVECTOR3 m_vPos;		//位置
	float m_fRadius;		//半径

	D3DXMATRIX m_matWorld;	//描画用
};

//Obbデータ
struct ObbData
{
	D3DXVECTOR3 m_vDirect[3];	//各軸の面の方向
	D3DXVECTOR3 m_vLength;		//中心点から各軸への長さ
	D3DXVECTOR3 m_vCenterPos;	//中心点の位置

	D3DXMATRIX	m_matWorld;		//描画用
};

//当たり判定
struct CollisionData
{
	CObj3DBase* m_pObj;	//オブジェクトのポインタ
	MY_MESH* m_pMesh;	//メッシュポインタ
	int m_Id;			//オブジェクトID

	SphereData	m_SphereData;	//球データ
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
	void UpData(CollisionData* pData);	//更新
	void Draw();						//描画
	void Release();						//解放

	//当たり判定
	bool Hit();

	//当たり判定データ登録
	CollisionData CollisionCreate(CObj3DBase* pCobj);
	void Insert(CollisionData* pData);

	//めり込み修正
	D3DXVECTOR3 Fixation(D3DXVECTOR3 Pos, D3DXVECTOR3 Norm, ObbData* pObb);
private:
	//球と球のヒット判定
	bool SphereAndSphre(SphereData* data1, SphereData* data2);

	//OBBにセット
	void SetObbData(CObj3DBase* pCobj, MY_MESH* pMesh, ObbData* pObb);

	//球情報セット
	void SetSphereData(CObj3DBase* pCobj,ObbData* pObb, MY_MESH* pMesh,SphereData* pSphere);
	
	//衝突判定
	bool ObbAndObb(ObbData* obb1, ObbData* obb2);

	// 分離軸に投影された軸成分から投影線分長を算出
	float LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3);

	vector<CollisionData* > m_Collision;	//当たり判定データ

	MY_MESH m_DrawObb;		//描画用Box
	MY_MESH m_DrawSphere;	//描画用Sphere
	ColorData m_Color;		//描画色
};