#include "../Library/Math.h"
#include "Sphere.h"
#include <math.h>
#include "../Library/Hit.h"

//コンストラクタ
CSphere::CSphere(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale)
{
	m_vPos	 = vPos;
	m_vAngle = vAngle;
	m_vScale = vScale;
}

//初期化
void CSphere::Init()
{
	//行列初期化
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matTrans);
	D3DXMatrixIdentity(&m_matWorld);

	//モデル
	m_pMesh = g_Loader.GetMesh(ModelName::ModelSphere);
	   	
	//登録
	m_Collision =g_Hit.CollisionCreate(this);
	g_Hit.Insert(&m_Collision);

	//ワールド行列作成
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);
}

//更新
void CSphere::Update()
{
	g_Hit.UpData(&m_Collision, m_vPos);
}

//描画
void CSphere::Draw()
{
	g_Loader.Draw(m_matWorld, m_pMesh,NULL);
}