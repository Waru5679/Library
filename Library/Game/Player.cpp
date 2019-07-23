#pragma once
//���C�u�����t�@�C��
#include "../Library/LibraryHeader.h"

//�Q�[���t�@�C��
#include "GameHeader.h"
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
	m_pMesh = MODEL->GetModelData(ModelName::ModelPlayer);
	
	//�ړ��x�N�g��
	m_vMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//����
	m_vDir = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	//�ړ��X�s�[�h
	m_fSpeed=0.2f;
		
	//�q�b�g�e�X�g
	m_bHit=false;

	//�����蔻��
	m_Collision = HIT->CollisionCreate(this);
	HIT->Insert(&m_Collision);
	
	//Ray�o�^
	RAY->Insert(this);

	//�Ə��̋���
	m_fAimDis = 10.0f;

	//�Ə�����
	CAiming* pAiming = new CAiming(m_vPos + m_vDir * m_fAimDis, m_vAngle, D3DXVECTOR3(3.0f, 3.0f, 3.0f));
	g_Task.InsertObj(pAiming, ObjName::ObjAiming);
	pAiming = nullptr;
}

//�X�V
void CPlayer::Update()
{
	//����
	Input();

	//�ړ��̐��K��
	D3DXVec3Normalize(&m_vMove, &m_vMove);

	//�J�����擾
	CMainCamera*pCamera = dynamic_cast<CMainCamera*> (g_Task.GetObj(ObjName::ObjMainCamera));

	//�J�����̌�������ړ��x�N�g����ϊ�
	m_matRot = MakeMatRot(pCamera->GetAngle());
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

	//�J�����X�V
	pCamera->Update();

	//�J�����|�C���^���
	pCamera = nullptr;
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
	if (INPUT->GetKeyPush('E') == true)
	{
		m_vMove += D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
	if (INPUT->GetKeyPush('Q') == true)
	{
		m_vMove += D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	}

	//��]
	if (INPUT->GetKeyPush(VK_LEFT) == true)
	{
		m_vAngle.y -= 0.05f;
	}

	if (INPUT->GetKeyPush(VK_RIGHT) == true)
	{
		m_vAngle.y += 0.05f;
	}

	//�U��
	if (INPUT->GetTriggerKeyPush(VK_SPACE))
	{
		//�G�C���|�C���^
		CAiming* pAiming = dynamic_cast<CAiming*>(g_Task.GetObj(ObjName::ObjAiming));
		
		//�e�̈ړ�����
		D3DXVECTOR3 vDir;
		if (pAiming != nullptr)
		{
			vDir = pAiming->GetPos() - m_vPos;
			D3DXVec3Normalize(&vDir, &vDir);
		}

		//����
		CBullet* pBullet = new CBullet(m_vPos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), vDir);
		g_Task.InsertObj(pBullet, ObjName::ObjBullet);

		//�|�C���^���
		pAiming = nullptr;
		pBullet = nullptr;
	}
}

//�`��
void CPlayer::Draw()
{
	MODEL->Draw(m_matWorld, m_pMesh,NULL);
}

//�J��
void CPlayer::Release()
{
	//���b�V���f�[�^���
	m_pMesh->Release();
	m_pMesh = nullptr;

	//�����蔻����
	m_Collision.Release();
}
