#include "PmxLoader.h"
#include <stdio.h>
#include "Binary.h"

//�C���X�^���X
CPmxLoader* CPmxLoader::m_pInstance = nullptr;

//�ǂݍ���
bool CPmxLoader::Load(const char* FileName, PMX_DATA* pPmxData)
{
	//�t�@�C���I�[�v��
	FILE* fp = nullptr;
	fopen_s(&fp, FileName, "rb");

	//�t�@�C���I�[�v�����s
	if (fp == nullptr)
		return false;

	//�ǂݍ��ݗp
	unsigned char pData[4];

	//�t�@�C���^�C�v
	fread_s(pPmxData->m_Head.m_FileType, sizeof(pPmxData->m_Head.m_FileType), sizeof(pPmxData->m_Head.m_FileType), 1, fp);

	//�o�[�W����
	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	pPmxData->m_Head.m_Ver = CtoL(pData);

	//�t�@�C���^�C�v
	fread_s(&pPmxData->m_Head.m_Size, sizeof(pPmxData->m_Head.m_Size), sizeof(pPmxData->m_Head.m_Size), 1, fp);

	//�o�C�g��T�C�Y
	int Size=pPmxData->m_Head.m_Size;
	pPmxData->m_Head.m_pData = new unsigned char[Size];
	
	//�o�C�g��ǂݍ���
	for (int i = 0; i < Size; i++)
	{
		//�t�@�C���^�C�v
		fread_s(&pPmxData->m_Head.m_pData[i], sizeof(unsigned char), sizeof(unsigned char), 1, fp);		
	}

	//���f����&�R�����g�X�L�b�v
	char test_1[16];
	fread_s(test_1, sizeof(test_1), sizeof(test_1), 1, fp);
	
	//���_��
	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	pPmxData->VerNum = CtoL(pData);

	//���_�������m��
	pPmxData->m_pVertex = new PMX_VERTEX[pPmxData->VerNum];

	//�ǂ��������
	////���_�ǂݍ���
	//for (int i = 0; i < pPmxData->VerNum; i++)
	//{
	//	//Pos_X
	//	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	//	pPmxData->m_pVertex[i].m_fPos[0] = CtoL(pData);

	//	//Pos_Y
	//	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	//	pPmxData->m_pVertex[i].m_fPos[1] = CtoL(pData);

	//	//Pos_Z
	//	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	//	pPmxData->m_pVertex[i].m_fPos[2] = CtoL(pData);

	//	//Norm_X
	//	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	//	pPmxData->m_pVertex[i].m_fNorm[0] = CtoL(pData);

	//	//Norm_Y
	//	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	//	pPmxData->m_pVertex[i].m_fNorm[1] = CtoL(pData);

	//	//Norm_Z
	//	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	//	pPmxData->m_pVertex[i].m_fNorm[2] = CtoL(pData);
	//	
	//	//UV_X
	//	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	//	pPmxData->m_pVertex[i].m_fUv[0] = CtoL(pData);

	//	//UV_Y
	//	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	//	pPmxData->m_pVertex[i].m_fUv[1] = CtoL(pData);

	//	//�ǉ�UV������ꍇ
	//	if (pPmxData->m_Head.m_pData[1] != 0)
	//	{
	//		//�ǉ����̃������m��
	//		pPmxData->m_pVertex[i].m_pfAddUv = new float[pPmxData->m_Head.m_pData[1]];

	//		//�ǉ���
	//		for (int j = 0; j < pPmxData->m_Head.m_pData[1]; j++)
	//		{
	//			fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	//			pPmxData->m_pVertex[i].m_pfAddUv[j] = CtoL(pData);				
	//		}
	//	}
	//	
	//	//�E�F�C�g�ό`����
	//	fread_s(&pPmxData->m_pVertex[i].WeightType, sizeof(unsigned char), sizeof(unsigned char), 1, fp);
	//	int a = 0;
	//}

	


	return true;
}