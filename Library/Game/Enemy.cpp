//ライブラリファイル
#include "../Library/LibraryHeader.h"

//ゲームファイル
#include "GameHeader.h"

//コンストラクタ
CEnemy::CEnemy(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale)
{
	m_vPos = Pos;
	m_vAngle = Angle;
	m_vScale = Scale;
}

//初期化
void CEnemy::Init()
{
	//行列初期化
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matTrans);

	//モデル
	m_pMesh= MODEL->GetModelData(ModelTarget);

	//攻撃用カウント
	m_AttackCount = 0;

	//攻撃の範囲
	m_AttackLength_x = 10.0f;
	m_AttackLength_y = 10.0f;
}

//更新
void CEnemy::Update()
{
	m_AttackCount++;

	//攻撃
	if (m_AttackCount > ATTACK_TIME)
	{
		float y = 0.0f;
		float x = 0.0f;

		for (int i = 0; i <= ATTACK_NUM_Y; i++)
		{
			y = ATTACK_RAD_Y * i;
			m_AttackLength_x = cosf(y) * 10.0f;
			for (int j = 0; j <= ATTACK_NUM_X; j++)
			{
				x = ATTACK_RAD_X * j;

				//生成位置
				D3DXVECTOR3 CreatePos(cosf(x) * m_AttackLength_x, sinf(y) * m_AttackLength_y, sinf(x) * m_AttackLength_x);

				//生成
				CEnemyBullet* pBullet = new CEnemyBullet(m_vPos + CreatePos, m_vAngle, D3DXVECTOR3(0.1f, 0.1f, 0.1f));
				g_Task.InsertObj(pBullet, ObjEnemyBullet);
			}
		}

		//攻撃のカウンタ初期化
		m_AttackCount = 0;
	}

	//ワールド行列作成
	m_matWorld=MakeMatWorld(m_vPos, m_vAngle, m_vScale);
}

//描画
void CEnemy::Draw()
{
	MODEL->Draw(m_matWorld, m_pMesh, NULL);
}
