#include "PmxLoader.h"
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

	//���_�ǂݍ���
	if (pPmxData->m_pVertex <= 0)
	{
		//���_�Ȃ��H
		return false;
	}
	
	//���_�ǂݍ���
	VertexLoad(fp, pPmxData);
	
	//�ʂ̐�
	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	pPmxData->FaceNum = CtoL(pData);

	//�ʃf�[�^�������m��
	pPmxData->m_pFace = new PMX_FACE[pPmxData->FaceNum];

	return true;
}

//���_�ǂݍ���
void CPmxLoader::VertexLoad(FILE* fp, PMX_DATA* pPmxData)
{
	//�ǂݍ��ݗp
	unsigned char pData[4];

	//���_�̐������ǂݍ���
	for (int i = 0; i < pPmxData->VerNum; i++)
	{
		//Pos_X
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pPmxData->m_pVertex[i].m_fPos[0] = CtoL(pData);

		//Pos_Y
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pPmxData->m_pVertex[i].m_fPos[1] = CtoL(pData);

		//Pos_Z
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pPmxData->m_pVertex[i].m_fPos[2] = CtoL(pData);

		//Norm_X
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pPmxData->m_pVertex[i].m_fNorm[0] = CtoL(pData);

		//Norm_Y
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pPmxData->m_pVertex[i].m_fNorm[1] = CtoL(pData);

		//Norm_Z
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pPmxData->m_pVertex[i].m_fNorm[2] = CtoL(pData);

		//UV_X
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pPmxData->m_pVertex[i].m_fUv[0] = CtoL(pData);

		//UV_Y
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pPmxData->m_pVertex[i].m_fUv[1] = CtoL(pData);

		//�ǉ�UV������ꍇ
		if (pPmxData->m_Head.m_pData[1] != 0)
		{
			//�ǉ����̃������m��
			pPmxData->m_pVertex[i].m_pfAddUv = new float[pPmxData->m_Head.m_pData[1]];

			//�ǉ���
			for (int j = 0; j < pPmxData->m_Head.m_pData[1]; j++)
			{
				fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
				pPmxData->m_pVertex[i].m_pfAddUv[j] = CtoL(pData);
			}
		}

		//�E�F�C�g�ό`����
		fread_s(&pPmxData->m_pVertex[i].m_WeightData.m_WeightType, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

		//�{�[���C���f�b�N�X�T�C�Y
		int BoneIndexSize = pPmxData->m_Head.m_pData[5];

		//�ό`�����ɉ����ă������m��
		switch (pPmxData->m_pVertex[i].m_WeightData.m_WeightType)
		{
			//BDFE1����
		case 0:
		{
			//�{�[��ID
			fread_s(pData, sizeof(pData), sizeof(char) * BoneIndexSize, 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef1.m_BoneID = CtoL(pData);

			break;
		}
		//BDFE2����
		case 1:
		{
			//�{�[��ID_1
			fread_s(pData, sizeof(pData), sizeof(char) * BoneIndexSize, 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef2.m_BoneID[0] = CtoL(pData);

			//�{�[��ID_2
			fread_s(pData, sizeof(pData), sizeof(char) * BoneIndexSize, 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef2.m_BoneID[1] = CtoL(pData);

			//�E�F�C�g
			fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef2.m_Weight = CtoL(pData);

			break;

		}
		//BDFE4����
		case 2:
		{
			//�{�[��ID_1
			fread_s(pData, sizeof(pData), sizeof(char) * BoneIndexSize, 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_BoneID[0] = CtoL(pData);

			//�{�[��ID_2
			fread_s(pData, sizeof(pData), sizeof(char) * BoneIndexSize, 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_BoneID[1] = CtoL(pData);

			//�{�[��ID_3
			fread_s(pData, sizeof(pData), sizeof(char) * BoneIndexSize, 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_BoneID[2] = CtoL(pData);

			//�{�[��ID_4
			fread_s(pData, sizeof(pData), sizeof(char) * BoneIndexSize, 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_BoneID[3] = CtoL(pData);

			//�E�F�C�g_1
			fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_Weight[0] = CtoL(pData);

			//�E�F�C�g_2
			fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_Weight[1] = CtoL(pData);

			//�E�F�C�g_3
			fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_Weight[2] = CtoL(pData);

			//�E�F�C�g_4
			fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_Weight[3] = CtoL(pData);

			break;
		}
		//SDEF����
		case 3:
		{

			//�{�[��ID_1
			fread_s(pData, sizeof(pData), sizeof(char) * BoneIndexSize, 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_BoneID[0] = CtoL(pData);

			//�{�[��ID_2
			fread_s(pData, sizeof(pData), sizeof(char) * BoneIndexSize, 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_BoneID[1] = CtoL(pData);

			//�E�F�C�g
			fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_Weight = CtoL(pData);

			//3x3�}�g���b�N�X
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_Matrix[j][k] = CtoL(pData);
				}
			}

			break;
		}
		default:
			break;
		}

		//�G�b�W�{��
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pPmxData->m_pVertex[i].m_EdgeMagn = CtoL(pData);
	}
}