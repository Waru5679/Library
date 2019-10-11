#include "Test.h"
#include "../Library/XLoader.h"
#include "../Library/XLoader_Skin.h"

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
}

void CTest::Update()
{

}

void CTest::Draw()
{
	SKIN->DrawMesh(m_matWorld, &m_SkinMesh, nullptr);
	
}

void CTest::Release()
{
}