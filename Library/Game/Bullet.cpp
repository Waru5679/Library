#include "../Library/LibraryHeader.h"
#include "GameHeader.h"

#include "Bullet.h"

//�R���X�g���N�^
CBullet::CBullet(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale,D3DXVECTOR3 Dir)
{
	m_vPos = Pos;
	m_vAngle = Angle;
	m_vScale = Scale;
	m_vMove = Dir;
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
	m_pMesh = MODEL->GetModelData(ModelSphere);
	
	//�ړ��X�s�[�h
	m_fSpeed = 0.5f;
}

//�X�V
void CBullet::Update()
{
	//�ړ�
	m_vPos += m_vMove * m_fSpeed;

	//���[���h�s��쐬
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);
}

//�`��
void CBullet::Draw()
{
	LOADER->Draw(m_matWorld, m_pMesh, NULL);
}
