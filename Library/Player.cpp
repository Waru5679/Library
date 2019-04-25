#include "Player.h"
#include "Math.h"
#include "Task.h"
#include "ObjLoader.h"
#include "Input.h"
#include "Camera.h"

CPlayer::CPlayer(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale)
{
	m_vPos = Pos;
	m_vAngle = Angle;
	m_vScale = Scale;
}

//������
void CPlayer::Init()
{
	//�s�񏉊���
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matTrans);
	D3DXMatrixIdentity(&m_matWorld);
	
	//���f��
	m_Mesh = g_Task.GetMesh(0);

	//�ŏ��l�E�ő�l
	m_vMin = m_Mesh.vMin;
	m_vMax = m_Mesh.vMax;

	//�ړ��x�N�g��
	m_vMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�ړ��X�s�[�h
	m_fSpeed=0.8f;

	//�����蔻��Z�b�g
	m_Obb = g_Obb.SetOBB(m_vPos, m_vAngle, m_vScale, m_vMin, m_vMax, m_id, this);
	g_Obb.Insert(&m_Obb);
}

//�X�V
void CPlayer::Update()
{
	//����
	Input();

	//���K��
	D3DXVec3Normalize(&m_vMove, &m_vMove);

	//�J�����擾
	CMainCamera* m_pCamera=dynamic_cast<CMainCamera*> (g_Task.GetObj(Obj_MainCamera));

	//�J�����̌�������ړ��x�N�g����ϊ�
	m_matRot = MakeMatRot(m_pCamera->GetAngle());
	D3DXVec3TransformCoord(&m_vMove, &m_vMove, &m_matRot);

	//�ړ�
	m_vPos += m_vMove * m_fSpeed;
	
	//�����蔻��X�V
	g_Obb.Update(&m_Obb, m_vPos, m_vAngle, m_vScale, m_vMin, m_vMax);
		
	//���[���h�s��쐬
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);

	//�ړ�������
	m_vMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//����
void CPlayer::Input()
{
	//�ړ�
	if (g_input.GetKeyPush('W') == true)
	{
		m_vMove += D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}
	if (g_input.GetKeyPush('S') == true)
	{
		m_vMove += D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	}
	if (g_input.GetKeyPush('D') == true)
	{
		m_vMove += D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	}
	if (g_input.GetKeyPush('A') == true)
	{
		m_vMove += D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	}
}

#include "Font.h"
//�`��
void CPlayer::Draw()
{
	CFont::DrawStr(L"��",0.0f, 0.0f, 32.0f,m_matWorld);
	//g_Loader.Draw(m_matWorld, &m_Mesh);

	
}
