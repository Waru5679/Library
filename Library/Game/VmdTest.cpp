#include "VmdTest.h"
#include "..//Library/VmdLoader.h"
#include "..//Library/PmxLoader.h"
#include "..//Library/ModelLoader.h"

#define VMD CVmdLoader::GetInstance()
#define PMX CPmxLoader::GetInstance()
#define MODEL_LOAD CModelLoader::GetInstance()

CVmdTest::CVmdTest(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale)
{
	m_vPos	 = vPos;
	m_vAngle = vAngle;
	m_vScale = vScale;
}

void CVmdTest::Init()
{
	VMD->Load("Model/Test.vmd",&m_data);
	VMD->Write("VmdOutTest.txt",&m_data);

	//PMX->Load("Model/Alicia_solid/Alicia_solid.pmx", &m_PmxData);
	//PMX->Load("Model/01_Normal_通常/プロ生ちゃん.pmx", &m_PmxData);
	//PMX->Write("PmxOutTest.txt", &m_PmxData);
	MODEL_LOAD->Load("Model/Alicia/アリシア.txt", &m_Model);
}

void CVmdTest::Update()
{

}

void CVmdTest::Draw()
{

}

void CVmdTest::Release()
{

}