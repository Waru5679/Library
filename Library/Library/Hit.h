#pragma once

#include "CObj.h"
#include "Main.h"
#include "ObjLoader.h"
#include <vector>
using namespace std;

//球データ
struct SphereData
{
	D3DXVECTOR3 m_vPos;	//位置
	float m_fRadius;	//半径
};

//Obbデータ
struct ObbData
{
	D3DXVECTOR3 m_vPos;		//位置
	D3DXVECTOR3 m_vAngle;	//回転
	D3DXVECTOR3 m_vScale;	//スケール

	D3DXVECTOR3 m_vDirect[3];	//各軸の面の方向
	D3DXVECTOR3 m_vLength;		//中心点から各軸への長さ
	D3DXVECTOR3 m_vCenterPos;	//中心点の位置

	D3DXVECTOR3 m_vVertexPos[8];//頂点の位置

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
class CHit
{
public:
	bool Hit(D3DXVECTOR3* vOut);
	void UpData(CollisionData* pData, D3DXVECTOR3 vPos);

	//当たり判定データ登録
	CollisionData CollisionCreate(CObj3DBase* pCobj);
	void Insert(CollisionData* pData);
private:
	//球と球のヒット判定
	bool SphereAndSphre(SphereData* pData1, SphereData* pData2);

	//OBBにセット
	ObbData SetObbData(CObj3DBase* pCobj, MY_MESH* pMesh);
	
	//OBBとOBBの判定
	bool ObbAndObb(ObbData* pData1, ObbData* pData2);

	//球情報セット
	SphereData SetSphereData(CObj3DBase* pCobj, MY_MESH* pMesh);

	// 分離軸に投影された軸成分から投影線分長を算出
	float LenSegOnSeparateAxis(D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3 );

	//OBBのめり込み修正
	D3DXVECTOR3 NestingCorrection(ObbData* StopObb,ObbData* MoveObb);

	vector<CollisionData* > m_Collision;	//当たり判定データ

	MY_MESH m_DrawObb;		//描画用Box
	MY_MESH m_DrawSphere;	//描画用Sphere

};

extern CHit g_Hit;