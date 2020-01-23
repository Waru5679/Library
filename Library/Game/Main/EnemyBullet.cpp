#include "../../Library/LibraryHeader.h"
#include "../GameHeader.h"
#include "EnemyBullet.h"

//�R���X�g���N�^
CEnemyBullet::CEnemyBullet(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale)
{
	m_vPos = Pos;
	m_vAngle = Angle;
	m_vScale = Scale;
}

//������
void CEnemyBullet::Init()
{
	//�s�񏉊���
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matTrans);

	//���f��
	m_pMesh = MODEL->GetModelData(ModelSphere);

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
void CEnemyBullet::Update()
{
	//�J�E���^�X�V
	m_Count++;

	//�폜
	if (m_Count > BULLET_DELETE_TIME)
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
void CEnemyBullet::Draw()
{
	MODEL->Draw(m_matWorld, m_pMesh, NULL);
}

//���
void CEnemyBullet::Release()
{
	m_pPlayer = nullptr;
}