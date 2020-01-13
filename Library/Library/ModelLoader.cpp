#include "ModelLoader.h"
#include <stdio.h>

//�C���X�^���X
CModelLoader* CModelLoader::m_pInstance = nullptr;

//�ǂݍ���
bool CModelLoader::Load(const char* FileName, MODEL_DATA* pModel)
{
	//�t�@�C���I�[�v��
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	//�t�@�C���ǂݍ��ݎ��s
	if (fp == NULL)
	{
		return false;
	}

	return true;
}

//���
void CModelLoader::Release()
{
	//�C���X�^���X�j��
	delete m_pInstance;
	m_pInstance = nullptr;
}
