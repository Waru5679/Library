#include <math.h>

//���C�u�����t�@�C��
#include "../Library/Math.h"
#include "../Library/Task.h"
#include "../Library/ObjLoader.h"
#include "../Library/Input.h"
#include "../Library/Camera.h"
#include "../Library/Font.h"
#include "../Library/Ray.h"

//�Q�[���t�@�C��
#include "Player.h"
#include "MainCamera.h"

//�R���X�g���N�^
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
	m_pMesh = g_Loader.GetMesh(ModelName::ModelSphere);
	
	//�ړ��x�N�g��
	m_vMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�ړ��X�s�[�h
	m_fSpeed=0.2f;

	//�����蔻��Z�b�g
	m_Obb = g_Obb.SetOBB(m_vPos, m_vAngle, m_vScale, m_pMesh->vMin, m_pMesh->vMax, m_id, this);
	g_Obb.Insert(&m_Obb);

	//�q�b�g�e�X�g
	m_bHit=false;
}

//�X�V
void CPlayer::Update()
{
	//����
	Input();

	//���K��
	D3DXVec3Normalize(&m_vMove, &m_vMove);

	//�J�����擾
	CMainCamera* m_pCamera=dynamic_cast<CMainCamera*> (g_Task.GetObj(ObjName::ObjMainCamera));

	//�J�����̌�������ړ��x�N�g����ϊ�
	m_matRot = MakeMatRot(m_pCamera->GetAngle());
	D3DXVec3TransformCoord(&m_vMove, &m_vMove, &m_matRot);

	//�ړ�
	m_vPos += m_vMove*m_fSpeed;

	//�Ō�̈ړ���ۑ�
	if(m_vMove!=D3DXVECTOR3(0.0f,0.0f,0.0f))
		m_vLastMove = m_vMove;
	
	//�����蔻��X�V
	g_Obb.Update(&m_Obb, m_vPos, m_vAngle, m_vScale, m_pMesh->vMin, m_pMesh->vMax);

	D3DXVECTOR3 vShear;
	
	if (g_Ray.RayHit(&vShear,this,m_vLastMove*m_fSpeed,ObjRayTest) == true)
	{
		m_bHit = true;

		//�ړ���߂�
		m_vPos -= m_vMove * m_fSpeed;

		//�ǎC��
		//m_vPos += vShear * m_fSpeed;
	}
	else
	{
		m_bHit = false;
	}

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
	if (g_input.GetKeyPush('Y') == true)
	{
		m_vMove += D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
	if (g_input.GetKeyPush('H') == true)
	{
		m_vMove += D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	}


	//��]
	//�ړ�
	if (g_input.GetKeyPush('J') == true)
	{
		m_vAngle += D3DXVECTOR3(0.0f, 0.2f, 0.0f);
	}
	if (g_input.GetKeyPush('L') == true)
	{
		m_vAngle += D3DXVECTOR3(0.0f, -0.2f, 0.0f);
	}
	if (g_input.GetKeyPush('I') == true)
	{
		m_vAngle += D3DXVECTOR3(0.2f, 0.0f, 0.0f);
	}
	if (g_input.GetKeyPush('K') == true)
	{
		m_vAngle += D3DXVECTOR3(-0.2f, 0.0f, 0.0f);
	}
	if (g_input.GetKeyPush('U') == true)
	{
		m_vAngle += D3DXVECTOR3(0.0f, 0.0f, 0.2f);
	}
	if (g_input.GetKeyPush('O') == true)
	{
		m_vAngle += D3DXVECTOR3(0.0f, 0.0f, -0.2f);
	}
}

//�`��
void CPlayer::Draw()
{
	//if(m_bHit==true)
	g_Font.DrawStr(L"Hit", 200.0f, 20.0f, 32.0f, 0.0f);

	g_Loader.Draw(m_matWorld, m_pMesh,NULL);
}