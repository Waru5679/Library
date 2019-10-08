#include "Test.h"
#include "..//Library/XLoader.h"

CXLoader* g_pInstance = new CXLoader();

CTest::CTest(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale)
{
	m_vPos = vPos;
	m_vAngle = vAngle;
	m_vScale = vScale;
}

void CTest::Init()
{

	//X“Ç‚Ýž‚Ý—p
	g_pInstance->LoadMesh("Model/hand_tex_static.x",&m_Mesh);
	
	m_matWorld=MakeMatWorld(m_vPos, m_vAngle, m_vScale);
}

void CTest::Update()
{

}

void CTest::Draw()
{
	g_pInstance->Draw(m_matWorld, &m_Mesh, nullptr);
	
}

void CTest::Release()
{
	delete g_pInstance;
	g_pInstance = nullptr;
}