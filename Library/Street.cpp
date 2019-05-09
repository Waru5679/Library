#include "Street.h"
#include "Task.h"


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
	m_Mesh = g_Task.GetMesh(ModelName::ModelStreet);

	//最小値・最大値
	m_vMin = m_Mesh.vMin;
	m_vMax = m_Mesh.vMax;

}

//更新
void CStreet::Update()
{
	
}

//描画
void CStreet::Draw()
{
	g_Loader.Draw(m_matWorld, &m_Mesh);
}
