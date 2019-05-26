//ライブラリファイル
#include "../Library/ObjLoader.h"
#include "../Library/Ray.h"

//ゲームファイル
#include "Street.h"


//コンストラクタ
CStreet::CStreet(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale)
{
	m_vPos = Pos;
	m_vAngle = Angle;
	m_vScale = Scale;
}

//初期化
void CStreet::Init()
{
	//行列初期化
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matTrans);
	D3DXMatrixIdentity(&m_matWorld);
	
	//モデル
	m_pMesh = g_Loader.GetMesh(ModelName::ModelStreet);
	
	//Obbセット
	m_Obb = g_Obb.SetOBB(m_vPos, m_vAngle, m_vScale, m_pMesh->vMin, m_pMesh->vMax, ObjStreet, this);
	g_Obb.Insert(&m_Obb);

	//Rayセット
	g_Ray.Insert(this);
	
}

//更新
void CStreet::Update()
{
	//当たり判定更新
	g_Obb.Update(&m_Obb, m_vPos, m_vAngle, m_vScale, m_pMesh->vMin, m_pMesh->vMax);

}

//描画
void CStreet::Draw()
{
	g_Loader.Draw(m_matWorld, m_pMesh);
}
