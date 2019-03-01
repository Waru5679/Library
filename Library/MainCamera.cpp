#include "MainCamera.h"
#include "Input.h"
#include "DirectX.h"
#include "Task.h"
#include "Player.h"
#include "Math.h"

//������
void CMainCamera::Init()
{
	//�����J�����̈ʒu
	m_vEye = D3DXVECTOR3(0.0f, 5.0f, -2.0f);
	m_vLook = D3DXVECTOR3(0.0f, 2.0f, 1.0f);
	m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	//�J��������
	m_fCameraDis = 3.0f;
	
	//�����_����
	m_fLookDis = 3.0f;

	//�p�x
	m_vAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�����ɂ��邽�߂̉�]��
	m_vVertical = D3DXVECTOR3(0.79,0.0f,0.0f);

}

//�X�V�֐�
void CMainCamera::Update()
{
	//��]
	if (g_input.GetKeyPush(VK_LEFT) == true)
	{
		m_vAngle.y -= 0.2f;
	}

	if (g_input.GetKeyPush(VK_RIGHT) == true)
	{
		m_vAngle.y += 0.2f;
	}

	//�v���C���[�̈ʒu
	CPlayer* m_pPlayer=dynamic_cast<CPlayer*>(g_Task.GetObj(Obj_Player));
	m_vPlayerPos = m_pPlayer->GetPos();

	//�v���C���[�̕���
	m_matRot = MakeMatRot(m_vAngle);
	D3DXVec3TransformCoord(&m_vPlayerFront, & D3DXVECTOR3(0.0f, 0.0f, 1.0f), &m_matRot);

	//�v���C���[�̈ʒu�����Ƃɒ����_�ƃJ�����ʒu�����߂�
	m_vLook = m_vPlayerPos + m_vPlayerFront * m_fLookDis;
	m_vEye = m_vPlayerPos - m_vPlayerFront * m_fCameraDis;

	m_vEye.y += 2.0f;


	//���_�V�F�[�_�p�̃}�g���b�N�X�쐬
	CreateVSMatrix(&m_vEye, &m_vLook, &m_vUp, &m_matView, &m_matProj, WINDOW_WIDTH, WINDOW_HEIGHT);
}
