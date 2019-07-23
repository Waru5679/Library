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
	m_pMesh = MODEL->GetModelData(ModelAiming);

	//���[���h�s��
	m_matWorld=MakeMatWorld(m_vPos, m_vAngle, m_vScale);

	//�v���C���[�̃|�C���^���擾
	m_pPlayer = dynamic_cast<CPlayer*>(g_Task.GetObj(ObjName::ObjPlayer));

	//�J�����̃|�C���^
	m_pCamera = dynamic_cast<CMainCamera*>(g_Task.GetObj(ObjName::ObjMainCamera));

	//�ړ��x�N�g��
	m_vMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//�X�V
void CAiming::Update()
{
	//�v���C���[�̃|�C���^���擾
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = dynamic_cast<CPlayer*>(g_Task.GetObj(ObjName::ObjPlayer));
	}

	//�J�����̃|�C���^�擾
	if (m_pCamera == nullptr)
	{
		m_pCamera = dynamic_cast<CMainCamera*>(g_Task.GetObj(ObjName::ObjMainCamera));
	}

	//�J�������v���C���[�̃x�N�g��
	D3DXVECTOR3 vDir;
	vDir = m_pPlayer->GetPos()- m_pCamera->GetEye();
	D3DXVec3Normalize(&vDir, &vDir);

	//�J�����̉�]
	m_vAngle.y = m_pCamera->GetAngle().y;

	//�J�����̌�������ʒu���X�V
	m_vMove = vDir * m_pPlayer->GetAimDis();
	m_vMove.y = 0.0f;

	m_vPos = m_pPlayer->GetPos() + m_vMove;

	//���[���h�s��
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);
}

//�`��
void CAiming::Draw()
{
	MODEL->Draw(m_matWorld, m_pMesh, nullptr);
}

//�J��
void CAiming::Release()
{
}