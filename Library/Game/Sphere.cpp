#include "GameHeader.h"
#include <math.h>

//�R���X�g���N�^
CSphere::CSphere(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale)
{
	m_vPos	 = vPos;
	m_vAngle = vAngle;
	m_vScale = vScale;
}

//������
void CSphere::Init()
{
	//�s�񏉊���
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matTrans);
	D3DXMatrixIdentity(&m_matWorld);

	//���f��
	m_pMesh = LOADER->GetMesh(ModelName::ModelSphere);
	   	
	//�o�^
	m_Collision = HIT->CollisionCreate(this);
	HIT->Insert(&m_Collision);

	//Ray�o�^
	RAY->Insert(this);

	//���[���h�s��쐬
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);
}

//�X�V
void CSphere::Update()
{
	//���[���h�s��쐬
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);

	HIT->UpData(&m_Collision);
}

//�`��
void CSphere::Draw()
{
	LOADER->Draw(m_matWorld, m_pMesh,NULL);
}