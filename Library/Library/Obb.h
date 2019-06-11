#ifndef	_NewOBB
#define	_NewOBB

#include "Main.h"
#include "CObj.h"
#include <D3D10.h>
#include <vector>
using namespace std;

#include "struct.h"
#include "Hit.h"
//
////OBB構造体
//struct OBB_Data
//{
//public:
//	//D3DXVECTOR3 Norm[6];	//面の法線
//	D3DXVECTOR3 VerPos[8];	//頂点の位置
//	D3DXVECTOR3 m_Pos;      //位置
//	D3DXVECTOR3 m_Direct[3];//各軸の回転
//	FLOAT		m_Length[3];//中心点から面への長さ(各軸方向の長さ)
//	D3DXVECTOR3 vMin;		//Mesh最小値
//	D3DXVECTOR3 vMax;		//Mesh最大値
//	D3DXMATRIX matWorld;
//	int Name;
//	CObjBase* p;
//
//	MY_MESH* pMesh;	//メッシュポインタ
//		
//	D3DXVECTOR3 m_Angle;
//	D3DXVECTOR3 m_Scale;
//};

//OBBクラス
class COBB
{
public:
	//void Init();	//初期化
	//void Draw();	//OBBの描画
	//void Release();	//解放

	////OBBセット
	//OBB_Data SetOBB(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale, D3DXVECTOR3 Min, D3DXVECTOR3 Max, int ObjName, CObjBase* p);

	////OBB登録
	//void Insert(OBB_Data* obb);

	////OBB更新
	//void Update(OBB_Data* obb, D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale, D3DXVECTOR3 Min, D3DXVECTOR3 Max);

	//ヒットチェック
	//bool ObjNameHit(OBB_Data* Obb,int Name);

	//めり込み修正
	D3DXVECTOR3 NestingCorrection(ObbData* StopObb,ObbData* MoveObb);
private:
	////衝突判定
	//bool Collision(OBB_Data* obb1, OBB_Data* obb2);

	//// 分離軸に投影された軸成分から投影線分長を算出
	//float LenSegOnSeparateAxis(D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3 );

	////OBBデータ
	//vector<OBB_Data*> m_Data;

	////OBB表示用メッシュ
	//MY_MESH m_Mesh;

	////描画色
	//ColorData m_Color;
};

//extern COBB g_Obb;

#endif

