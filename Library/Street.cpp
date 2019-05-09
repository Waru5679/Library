#include "Street.h"
#include "ObjLoader.h"


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

}

//更新
void CStreet::Update()
{
	
}

//描画
void CStreet::Draw()
{
	g_Loader.Draw(m_matWorld, m_pMesh);
}
