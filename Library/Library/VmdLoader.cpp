#include "VmdLoader.h"
#include <stdio.h>
#include "Binary.h"

//�C���X�^���X
CVmdLoader* CVmdLoader::m_pInstance = nullptr;

//�ǂݍ���
bool CVmdLoader::Load(const char* FileName, VMD_DATA* pVmdData)
{
	//�t�@�C���I�[�v��
	FILE* fp = nullptr;
	fopen_s(&fp, FileName, "rb");

	//�t�@�C���I�[�v�����s
	if (fp == nullptr)
		return false;
	
	//�t�@�C���V�O�l�`��
	fread_s(pVmdData->m_Head.m_Signature, sizeof(pVmdData->m_Head.m_Signature), sizeof(pVmdData->m_Head.m_Signature), 1, fp);

	//���f����
	fread_s(pVmdData->m_Head.m_ModelName, sizeof(pVmdData->m_Head.m_ModelName), sizeof(pVmdData->m_Head.m_ModelName), 1, fp);

	//���[�V�������R�[�h��
	unsigned char pData[4];
	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	pVmdData->m_MotionRecordNum = CtoL(pData);
	
	//���[�V�������̃������m��
	pVmdData->m_pMotion = new VMD_MOTION[pVmdData->m_MotionRecordNum];

	//���[�V�����f�[�^�ǂݍ���
	for (unsigned int i = 0; i < pVmdData->m_MotionRecordNum; i++)
	{
		//�{�[����
		fread_s(pVmdData->m_pMotion[i].BoneName, sizeof(pVmdData->m_pMotion[i].BoneName), sizeof(pVmdData->m_pMotion[i].BoneName), 1, fp);
		
		//�t���[���i���o�[
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pVmdData->m_pMotion[i].FrameNum = CtoL(pData);

		//Pos_X
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pVmdData->m_pMotion[i].Pos[0]= CtoL(pData);

		//Pos_Y
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pVmdData->m_pMotion[i].Pos[1] = CtoL(pData);
		
		//Pos_Z
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pVmdData->m_pMotion[i].Pos[2] = CtoL(pData);

		//Rot_X
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pVmdData->m_pMotion[i].Rot[0] = CtoL(pData);

		//Rot_Y
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pVmdData->m_pMotion[i].Rot[1] = CtoL(pData);

		//Rot_Z
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pVmdData->m_pMotion[i].Rot[2] = CtoL(pData);

		//Rot_W
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pVmdData->m_pMotion[i].Rot[3] = CtoL(pData);

		//�⊮�f�[�^
		fread_s(pVmdData->m_pMotion[i].Data, sizeof(pVmdData->m_pMotion[i].Data), sizeof(pVmdData->m_pMotion[i].Data), 1, fp);
	}

	//�X�L�����R�[�h��
	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	pVmdData->m_SkinRecordNum = CtoL(pData);

	//�X�L�����̃������m��
	pVmdData->m_pSkin = new VMD_SKIN[pVmdData->m_SkinRecordNum];
		
	//�X�L�����ǂݍ���
	for (unsigned int i = 0; i < pVmdData->m_SkinRecordNum; i++)
	{
		//�X�L����
		fread_s(pVmdData->m_pSkin[i].SkinName, sizeof(pVmdData->m_pSkin[i].SkinName), sizeof(pVmdData->m_pSkin[i].SkinName), 1, fp);

		//�t���[���i���o�[
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pVmdData->m_pSkin[i].FlameNum = CtoL(pData);

		//�E�F�C�g
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pVmdData->m_pSkin[i].Weight = CtoL(pData);
	}

	//�t�@�C���N���[�Y
	fclose(fp);
	
	return true;
}

//�����o��
bool CVmdLoader::Write(const char* FileName, VMD_DATA* pVmdData)
{
	FILE* fp = nullptr;
	fopen_s(&fp, FileName, "w");

	//�I�[�v�����s
	if (fp == nullptr)
		return false;

	fprintf_s(fp, "�t�@�C���V�O�l�`��:%s\n", pVmdData->m_Head.m_Signature);
	fprintf_s(fp, "���f����:%s\n", pVmdData->m_Head.m_ModelName);

	for (unsigned int i = 0; i < pVmdData->m_MotionRecordNum; i++)
	{
		fprintf_s(fp, "---���[�V����%d(�n)---\n",i);

		fprintf_s(fp, "�{�[����:%s\n", pVmdData->m_pMotion[i].BoneName);
		
		fprintf_s(fp, "�t���[��No:%d\n", pVmdData->m_pMotion[i].FrameNum);

		fprintf_s(fp, "�ʒu\n");
		fprintf_s(fp, "X:%f,Y:%f,Z:%f\n", pVmdData->m_pMotion[i].Pos[0], pVmdData->m_pMotion[i].Pos[1], pVmdData->m_pMotion[i].Pos[2]);

		fprintf_s(fp, "��]\n");
		fprintf_s(fp, "X:%f,Y:%f,Z:%f,W:%f\n",
			pVmdData->m_pMotion[i].Rot[0], pVmdData->m_pMotion[i].Rot[1], pVmdData->m_pMotion[i].Rot[2], pVmdData->m_pMotion[i].Rot[3]);

		fprintf_s(fp, "�⊮�p�����^\n");
		fprintf_s(fp, "%s\n", pVmdData->m_pMotion[i].Data);
		
		fprintf_s(fp, "---���[�V����%d(�I)---\n",i);
		fprintf_s(fp, "\n");
	}

	for (unsigned int i = 0; i < pVmdData->m_SkinRecordNum; i++)
	{
		fprintf_s(fp, "---�X�L��%d(�n)---\n", i);

		fprintf_s(fp, "�\�:%s\n", pVmdData->m_pSkin[i].SkinName);

		fprintf_s(fp, "�t���[��No:%d\n", pVmdData->m_pSkin[i].FlameNum);

		fprintf_s(fp, "�E�F�C�g:%f\n", pVmdData->m_pSkin[i].Weight);

		fprintf_s(fp, "---�X�L��%d(�I)---\n", i);
		fprintf_s(fp, "\n");
	}

	//�t�@�C���N���[�Y
	fclose(fp);

	return true;
}