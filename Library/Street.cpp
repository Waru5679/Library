#include "Street.h"
#include "ObjLoader.h"



CStreet::CStreet(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale)
{
	m_vPos = Pos;
	m_vAngle = Angle;
	m_vScale = Scale;
}

//������
void CStreet::Init()
{
	//�s�񏉊���
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matTrans);
	D3DXMatrixIdentity(&m_matWorld);
	
	//���f��
	m_pMesh = g_Loader.GetMesh(ModelName::ModelStreet);
	
	//Obb�Z�b�g
	m_Obb = g_Obb.SetOBB(m_vPos, m_vAngle, m_vScale, m_pMesh->vMin, m_pMesh->vMax, ObjStreet, this);
	g_Obb.Insert(&m_Obb);
}

//�X�V
void CStreet::Update()
{
	//�����蔻��X�V
	g_Obb.Update(&m_Obb, m_vPos, m_vAngle, m_vScale, m_pMesh->vMin, m_pMesh->vMax);

}

//�`��
void CStreet::Draw()
{
	g_Loader.Draw(m_matWorld, m_pMesh);
}
