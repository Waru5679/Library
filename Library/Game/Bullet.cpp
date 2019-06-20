#include "../Library/Math.h"
#include "Bullet.h"
#include "../Library/Task.h"

//�R���X�g���N�^
CBullet::CBullet(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale)
{
	m_vPos = Pos;
	m_vAngle = Angle;
	m_vScale = Scale;
}

//������
void CBullet::Init()
{
	//�s�񏉊���
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matTrans);

	//���f��
	m_pMesh = g_Loader.GetMesh(ModelSphere);

	//�v���C���[�̃|�C���^
	m_pPlayer = dynamic_cast<CPlayer*> (g_Task.GetObj(ObjPlayer));

	//�v���C���[�̈ʒu
	if (m_pPlayer != nullptr)
	{
		m_vPlayerPos = m_pPlayer->GetPos();
	}

	//�J�E���^
	m_Count = 0;

	//�ړ��X�s�[�h
	m_fSpeed = 0.5f;

	D3DXVec3Normalize( &m_vMove,&(m_vPlayerPos - m_vPos));
}

//�X�V
void CBullet::Update()
{
	//�J�E���^�X�V
	m_Count++;

	//�폜
	if (m_Count > DELETE_TIME)
	{
		this->SetDelete(true);
		return;
	}

	//�ړ�
	m_vPos += m_vMove * m_fSpeed;

	//���[���h�s��쐬
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);
}

//�`��
void CBullet::Draw()
{
	g_Loader.Draw(m_matWorld, m_pMesh, NULL);
}
