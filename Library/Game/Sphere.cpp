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

	//���a���߂�
	D3DXVECTOR3 vMax, vMin;
	vMax = m_pMesh->vMax;
	vMin = m_pMesh->vMin;

	//��Ԓ����������甼�a��ݒ�
	float Dia = MostLongComponent(vMax - vMin);
	Dia = fabsf(Dia);
	m_fRadius = Dia / 2.0f;


	
	//�o�^
	m_SphereData = g_Hit.Create(m_vPos, m_fRadius,m_vScale);
	g_Hit.Insert(&m_SphereData);

	//���[���h�s��쐬
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);
}

//�X�V
void CSphere::Update()
{
	g_Hit.UpData(&m_SphereData, m_vPos);
}

//�`��
void CSphere::Draw()
{
	g_Loader.Draw(m_matWorld, m_pMesh,NULL);
}