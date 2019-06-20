#include "..//Library/Math.h"
#include "../Library/Task.h"
#include "Enemy.h"
#include "Bullet.h"

//�R���X�g���N�^
CEnemy::CEnemy(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale)
{
	m_vPos = Pos;
	m_vAngle = Angle;
	m_vScale = Scale;
}

//������
void CEnemy::Init()
{
	//�s�񏉊���
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matTrans);

	//���f��
	m_pMesh=g_Loader.GetMesh(ModelSphere);

	//�U���p�J�E���g
	m_AttackCount = 0;

	//�U���͈̔�
	m_AttackLength_x = 10.0f;
	m_AttackLength_y = 10.0f;
}

//�X�V
void CEnemy::Update()
{
	m_AttackCount++;

	//�U��
	if (m_AttackCount > ATTACK_TIME)
	{
		float y = 0.0f;
		float x = 0.0f;

		for (int i = 0; i <= ATTACK_NUM_Y; i++)
		{
			y = ATTACK_RAD_Y * i;
			m_AttackLength_x = cos(y) * 10.0f;
			for (int j = 0; j <= ATTACK_NUM_X; j++)
			{
				x = ATTACK_RAD_X * j;

				D3DXVECTOR3 CreatePos(cosf(x) * m_AttackLength_x, sinf(y) * m_AttackLength_y, sinf(x) * m_AttackLength_x);

				CBullet* pBullet = new CBullet(m_vPos + CreatePos, m_vAngle, D3DXVECTOR3(0.1f, 0.1f, 0.1f));
				g_Task.InsertObj(pBullet, ObjBullet);
			}
		}

		//�U���̃J�E���^������
		m_AttackCount = 0;
	}

	//���[���h�s��쐬
	m_matWorld=MakeMatWorld(m_vPos, m_vAngle, m_vScale);
}

//�`��
void CEnemy::Draw()
{
	g_Loader.Draw(m_matWorld, m_pMesh, NULL);
}
