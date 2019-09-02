#include "Test.h"
#include "..//Library/XLoader.h"

CTest::CTest(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale)
{
	m_vPos = vPos;
	m_vAngle = vAngle;
	m_vScale = vScale;
}

void CTest::Init()
{

	CXLoader::LoadMeshFromX("Model/hand_tex_static.x", &m_Mesh);
	m_matWorld=MakeMatWorld(m_vPos, m_vAngle, m_vScale);
}

void CTest::Update()
{

}

void CTest::Draw()
{
	MODEL->Draw(m_matWorld, &m_Mesh, nullptr);
}

void CTest::Release()
{

}