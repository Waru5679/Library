#include "Test.h"
#include "../../Library/XLoader_Skin.h"

#define SKIN CX_Skin::GetInstance()

CTest::CTest(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale)
{
	m_vPos = vPos;
	m_vAngle = vAngle;
	m_vScale = vScale;
}

void CTest::Init()
{

	//X“Ç‚Ýž‚Ý—p
	SKIN->LoadSkinMesh("Model/Hand/hand_tex.x",&m_SkinMesh);
		
	m_matWorld=MakeMatWorld(m_vPos, m_vAngle, m_vScale);
	
	m_Frame=0;

}

void CTest::Update()
{
	m_Frame+=15;

	if (m_Frame >= 9000)
		m_Frame = 0;

	for (int i = 0; i < m_SkinMesh.m_AnimeNum; i++)
	{
		SKIN->Animation(i, m_Frame, &m_SkinMesh);
	}
}

void CTest::Draw()
{
	SKIN->DrawMesh(m_matWorld, &m_SkinMesh, nullptr);	
}

void CTest::Release()
{
}