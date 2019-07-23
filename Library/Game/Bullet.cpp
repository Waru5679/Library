#include "../Library/LibraryHeader.h"
#include "GameHeader.h"

#include "Bullet.h"

//コンストラクタ
CBullet::CBullet(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale,D3DXVECTOR3 Dir)
{
	m_vPos = Pos;
	m_vAngle = Angle;
	m_vScale = Scale;
	m_vMove = Dir;
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
	m_pMesh = MODEL->GetModelData(ModelSphere);
	
	//移動スピード
	m_fSpeed = 0.5f;

	//カウンタ初期化
	m_Count = 0;
}

//更新
void CBullet::Update()
{
	//カウンタ更新
	m_Count++;

	//移動
	m_vPos += m_vMove * m_fSpeed;

	//削除
	if (m_Count > BULLET_DELETE_TIME)
	{
		this->SetDelete(true);
		return;
	}

	//ワールド行列作成
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);
}

//描画
void CBullet::Draw()
{
	MODEL->Draw(m_matWorld, m_pMesh, NULL);
}
