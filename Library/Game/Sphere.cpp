#include "../Library/Math.h"
#include "Sphere.h"
#include <math.h>
#include "../Library/Hit.h"

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
	m_pMesh = g_Loader.GetMesh(ModelName::ModelSphere);
	   	
	//�o�^
	m_Collision =g_Hit.CollisionCreate(this);
	g_Hit.Insert(&m_Collision);

	m_Obb = g_Obb.SetOBB(m_vPos, m_vAngle, m_vScale, m_pMesh->vMin, m_pMesh->vMax, m_Id, this);
	g_Obb.Insert(&m_Obb);

	//���[���h�s��쐬
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);
}

//�X�V
void CSphere::Update()
{
	//���[���h�s��쐬
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);

	g_Hit.UpData(&m_Collision, m_vPos);
	g_Obb.Update(&m_Obb, m_vPos, m_vAngle, m_vScale, m_pMesh->vMin, m_pMesh->vMax);
}

//�`��
void CSphere::Draw()
{
	g_Loader.Draw(m_matWorld, m_pMesh,NULL);
}