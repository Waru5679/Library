#include "Street.h"
#include "Task.h"


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
	m_Mesh = g_Task.GetMesh(ModelName::ModelStreet);

	//�ŏ��l�E�ő�l
	m_vMin = m_Mesh.vMin;
	m_vMax = m_Mesh.vMax;

}

//�X�V
void CStreet::Update()
{
	
}

//�`��
void CStreet::Draw()
{
	g_Loader.Draw(m_matWorld, &m_Mesh);
}
