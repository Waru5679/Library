#include "../Library/Math.h"
#include "Bullet.h"
#include "../Library/Task.h"

//コンストラクタ
CBullet::CBullet(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale)
{
	m_vPos = Pos;
	m_vAngle = Angle;
	m_vScale = Scale;
}

//初期化
void CBullet::Init()
{
	//行列初期化
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matTrans);

	//モデル
	m_pMesh = g_Loader.GetMesh(ModelSphere);

	//プレイヤーのポインタ
	m_pPlayer = dynamic_cast<CPlayer*> (g_Task.GetObj(ObjPlayer));

	//プレイヤーの位置
	if (m_pPlayer != nullptr)
	{
		m_vPlayerPos = m_pPlayer->GetPos();
	}

	//カウンタ
	m_Count = 0;

	//移動スピード
	m_fSpeed = 0.5f;

	D3DXVec3Normalize( &m_vMove,&(m_vPlayerPos - m_vPos));
}

//更新
void CBullet::Update()
{
	//カウンタ更新
	m_Count++;

	//削除
	if (m_Count > DELETE_TIME)
	{
		this->SetDelete(true);
		return;
	}

	//移動
	m_vPos += m_vMove * m_fSpeed;

	//ワールド行列作成
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);
}

//描画
void CBullet::Draw()
{
	g_Loader.Draw(m_matWorld, m_pMesh, NULL);
}
