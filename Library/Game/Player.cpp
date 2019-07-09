#pragma once
//���C�u�����t�@�C��
#include "../Library/LibraryHeader.h"

//�Q�[���t�@�C��
#include  "../Library/GameHeader.h"

#include <math.h>

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
	m_pMesh = LOADER->GetMesh(ModelName::ModelPlayer);
	
	//�ړ��x�N�g��
	m_vMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�ړ��X�s�[�h
	m_fSpeed=1.0f;
		
	//�q�b�g�e�X�g
	m_bHit=false;

	//�����蔻��
	m_Collision = HIT->CollisionCreate(this);
	HIT->Insert(&m_Collision);
	
	//Ray�o�^
	RAY->Insert(this);
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

	//�����蔻��f�[�^�X�V
	HIT->UpData(&m_Collision);

	//�q�b�g�m�F
	if(HIT->Hit()==true)
	{
		m_bHit = true;
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
	if (INPUT->GetKeyPush('W') == true)
	{
		m_vMove += D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}
	if (INPUT->GetKeyPush('S') == true)
	{
		m_vMove += D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	}
	if (INPUT->GetKeyPush('D') == true)
	{
		m_vMove += D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	}
	if (INPUT->GetKeyPush('A') == true)
	{
		m_vMove += D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	}
	if (INPUT->GetKeyPush('Y') == true)
	{
		m_vMove += D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
	if (INPUT->GetKeyPush('H') == true)
	{
		m_vMove += D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	}


	//��]
	//�ړ�
	if (INPUT->GetKeyPush('J') == true)
	{
		m_vAngle += D3DXVECTOR3(0.0f, 0.2f, 0.0f);
	}
	if (INPUT->GetKeyPush('L') == true)
	{
		m_vAngle += D3DXVECTOR3(0.0f, -0.2f, 0.0f);
	}
	if (INPUT->GetKeyPush('I') == true)
	{
		m_vAngle += D3DXVECTOR3(0.2f, 0.0f, 0.0f);
	}
	if (INPUT->GetKeyPush('K') == true)
	{
		m_vAngle += D3DXVECTOR3(-0.2f, 0.0f, 0.0f);
	}
	if (INPUT->GetKeyPush('U') == true)
	{
		m_vAngle += D3DXVECTOR3(0.0f, 0.0f, 0.2f);
	}
	if (INPUT->GetKeyPush('O') == true)
	{
		m_vAngle += D3DXVECTOR3(0.0f, 0.0f, -0.2f);
	}
}

//�`��
void CPlayer::Draw()
{
	if(m_bHit==true)
		FONT->DrawStr(L"Hit", 200.0f, 20.0f, 32.0f, 0.0f);

	LOADER->Draw(m_matWorld, m_pMesh,NULL);
}
