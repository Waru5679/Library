#include "Aiming.h"
#include "GameHeader.h"

//�R���X�g���N�^
CAiming::CAiming(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale)
{
	m_vPos = vPos;
	m_vAngle = vAngle;
	m_vScale = vScale;
}

//������
void CAiming::Init()
{
	//�s�񏉊���
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matTrans);

	//���f��
	m_pMesh = LOADER->GetMesh(ModelAiming);

	//���[���h�s��
	m_matWorld=MakeMatWorld(m_vPos, m_vAngle, m_vScale);

	//�v���C���[�̃|�C���^���擾
	m_pPlayer = dynamic_cast<CPlayer*>(g_Task.GetObj(ObjName::ObjPlayer));
}

//�X�V
void CAiming::Update()
{
	//�v���C���[�̃|�C���^���擾
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = dynamic_cast<CPlayer*>(g_Task.GetObj(ObjName::ObjPlayer));
	}

	//�v���C���[�̌���	
	m_vAngle = m_pPlayer->GetDir();

	//���[���h�s��
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);

}

//�`��
void CAiming::Draw()
{
	LOADER->Draw(m_matWorld, m_pMesh, nullptr);
}

//�J��
void CAiming::Release()
{
	PointerRelease(m_pPlayer);
}