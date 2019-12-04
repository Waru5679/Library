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
	unsigned char Data[4];

	//�t�@�C���^�C�v
	fread_s(pPmxData->m_Head.m_FileType, sizeof(pPmxData->m_Head.m_FileType), sizeof(pPmxData->m_Head.m_FileType), 1, fp);

	//�o�[�W����
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_Head.m_Ver = StrToFloat(Data, sizeof(Data));

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
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->VerNum = StrToInt(Data, sizeof(Data));

	//���_�Ȃ�
	if (pPmxData->VerNum <= 0)
	{
		return false;
	}

	//���_�������m��
	pPmxData->m_pVertex = new PMX_VERTEX[pPmxData->VerNum];
		
	//���_�ǂݍ���
	VertexLoad(fp, pPmxData);
	
	//�ʂ̐�
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->FaceNum = StrToInt(Data, sizeof(Data));

	//�ʂȂ�
	if (pPmxData->FaceNum <= 0)
	{
		return false;
	}

	//�ʃf�[�^�������m��
	pPmxData->m_pFace = new PMX_FACE[pPmxData->FaceNum];

	//�ʃf�[�^�ǂݍ���
	FaceLoad(fp, pPmxData);

	return true;
}

//���_�ǂݍ���
void CPmxLoader::VertexLoad(FILE* fp, PMX_DATA* pPmxData)
{
	//�ǂݍ��ݗp
	unsigned char Data[4];

	//�{�[���C���f�b�N�X�T�C�Y
	int BoneIndexSize = pPmxData->m_Head.m_pData[5];

	//�{�[���C���f�b�N�X�f�[�^�̓ǂݍ��ݗp
	unsigned char* pBoneIndex;
	pBoneIndex = new unsigned char[BoneIndexSize];

	//���_�̐������ǂݍ���
	for (int i = 0; i < pPmxData->VerNum; i++)
	{
		//Pos
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pVertex[i].m_fPos[j] = StrToFloat(Data, sizeof(Data));
		}
		
		//Norm
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pVertex[i].m_fNorm[j] = StrToFloat(Data, sizeof(Data));
		}

		//UV
		for (int j = 0; j < 2; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pVertex[i].m_fUv[j] = StrToFloat(Data, sizeof(Data));
		}
		
		//�ǉ�UV������ꍇ
		if (pPmxData->m_Head.m_pData[1] != 0)
		{
			//�ǉ����̃������m��
			pPmxData->m_pVertex[i].m_pfAddUv = new float[pPmxData->m_Head.m_pData[1]];

			//�ǉ���
			for (int j = 0; j < pPmxData->m_Head.m_pData[1]; j++)
			{
				fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
				pPmxData->m_pVertex[i].m_pfAddUv[j] = StrToFloat(Data, sizeof(Data));
			}
		}

		//�E�F�C�g�ό`����
		fread_s(&pPmxData->m_pVertex[i].m_WeightData.m_WeightType, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

		//�ό`�����ɉ����ă������m��
		switch (pPmxData->m_pVertex[i].m_WeightData.m_WeightType)
		{
			//BDFE1����
			case 0:
			{
				//�{�[��ID
				fread_s(pBoneIndex, BoneIndexSize, BoneIndexSize, 1, fp);
				pPmxData->m_pVertex[i].m_WeightData.m_Bdef1.m_BoneID = StrToInt(pBoneIndex, BoneIndexSize);

				break;
			}
			//BDFE2����
			case 1:
			{
				//�{�[��ID*2
				for (int j = 0; j < 2; j++)
				{
					fread_s(pBoneIndex, BoneIndexSize, BoneIndexSize, 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_Bdef2.m_BoneID[j] = StrToInt(pBoneIndex, BoneIndexSize);
				}
			
				//�E�F�C�g
				fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
				pPmxData->m_pVertex[i].m_WeightData.m_Bdef2.m_Weight = StrToFloat(Data, sizeof(Data));

				break;
			}
			//BDFE4����
			case 2:
			{
				//�{�[��ID*4
				for (int j = 0; j < 4; j++)
				{		
					fread_s(pBoneIndex, BoneIndexSize, BoneIndexSize, 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_BoneID[j] = StrToInt(pBoneIndex, BoneIndexSize);
				}
	
				//�E�F�C�g*4
				for (int j = 0; j < 4; j++)
				{
					//�E�F�C�g_1
					fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_Weight[j] = StrToFloat(Data, sizeof(Data));
				}
				break;
			}
			//SDEF����
			case 3:
			{
				//�{�[��ID*2
				for (int j = 0; j < 2; j++)
				{
					fread_s(pBoneIndex, BoneIndexSize, BoneIndexSize, 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_BoneID[j] = StrToInt(pBoneIndex, BoneIndexSize);
				}
			
				//�E�F�C�g
				fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
				pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_Weight = StrToFloat(Data, sizeof(Data));

				//3x3�}�g���b�N�X
				for (int j = 0; j < 3; j++)
				{
					for (int k = 0; k < 3; k++)
					{
						fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
						pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_Matrix[j][k] = StrToFloat(Data, sizeof(Data));
					}
				}
				break;
			}
			default:
				break;
		}

		//�G�b�W�{��
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pVertex[i].m_EdgeMagn = StrToFloat(Data, sizeof(Data));
	}
	delete[] pBoneIndex;
	pBoneIndex = nullptr;
}

//�ʓǂݍ���
void CPmxLoader::FaceLoad(FILE* fp, PMX_DATA* pPmxData)
{
	//���_�C���f�b�N�X�T�C�Y
	int IndexSize = pPmxData->m_Head.m_pData[2];

	unsigned char* pData;
	pData = new unsigned char[IndexSize];
	
	//�ʂ̐���
	for (int i = 0; i < pPmxData->FaceNum; i++)
	{
		//���_�C���f�b�N�X�ǂݍ���
		for (int j = 0; j < 3; j++)
		{
			fread_s(pData,IndexSize, IndexSize, 1, fp);
			
			pPmxData->m_pFace[i].m_VerIndex[j] = StrToInt(pData, IndexSize);
		}
	}

	delete[] pData;
	pData = nullptr;
}