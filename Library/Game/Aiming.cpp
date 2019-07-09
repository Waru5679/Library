#include "Aiming.h"
#include "GameHeader.h"

//コンストラクタ
CAiming::CAiming(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale)
{
	m_vPos = vPos;
	m_vAngle = vAngle;
	m_vScale = vScale;
}

//初期化
void CAiming::Init()
{
	//行列初期化
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matTrans);

	//モデル
	m_pMesh = LOADER->GetMesh(ModelAiming);

	//ワールド行列
	m_matWorld=MakeMatWorld(m_vPos, m_vAngle, m_vScale);

	//プレイヤーのポインタを取得
	m_pPlayer = dynamic_cast<CPlayer*>(g_Task.GetObj(ObjName::ObjPlayer));
}

//更新
void CAiming::Update()
{
	//プレイヤーのポインタを取得
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = dynamic_cast<CPlayer*>(g_Task.GetObj(ObjName::ObjPlayer));
	}

	//プレイヤーの向き	
	m_vAngle = m_pPlayer->GetDir();

	//ワールド行列
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);

}

//描画
void CAiming::Draw()
{
	LOADER->Draw(m_matWorld, m_pMesh, nullptr);
}

//開放
void CAiming::Release()
{
	PointerRelease(m_pPlayer);
}