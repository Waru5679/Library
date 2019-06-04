#include <math.h>

//���C�u�����t�@�C��
#include "../Library/Math.h"
#include "../Library/Task.h"
#include "../Library/ObjLoader.h"
#include "../Library/Camera.h"
#include "../Library/Font.h"
#include "../Library/Ray.h"

//�Q�[���t�@�C��
#include "RayTest.h"

//�R���X�g���N�^
CRayTest::CRayTest(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale)
{
	m_vPos = Pos;
	m_vAngle = Angle;
	m_vScale = Scale;
}

//������
void CRayTest::Init()
{
	//�s�񏉊���
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matTrans);
	D3DXMatrixIdentity(&m_matWorld);

	//���f��
	m_pMesh = g_Loader.GetMesh(ModelName::ModelRayTest);

	g_Ray.Insert(this);
}

//�X�V
void CRayTest::Update()
{
	//���[���h�s��쐬
	m_matWorld = MakeMatWorld(m_vPos, m_vAngle, m_vScale);
}

//�`��
void CRayTest::Draw()
{
	g_Loader.Draw(m_matWorld, m_pMesh,NULL);
}
