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

	//���f�����ǂݍ���
	ModelInfoLoad(fp, pPmxData);

	//���_��
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_VerNum = StrToInt(Data, sizeof(Data));

	//���_�Ȃ�
	if (pPmxData->m_VerNum <= 0)
	{
		return false;
	}

	//���_�������m��
	pPmxData->m_pVertex = new PMX_VERTEX[pPmxData->m_VerNum];
		
	//���_�ǂݍ���
	VertexLoad(fp, pPmxData);
	
	//�ʂ̃T�C�Y
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_FaceNum = StrToInt(Data, sizeof(Data));

	//3���_��1��
	pPmxData->m_FaceNum /= 3;

	//�ʂȂ�
	if (pPmxData->m_FaceNum <= 0)
	{
		return false;
	}

	//�ʃf�[�^�������m��
	pPmxData->m_pFace = new PMX_FACE[pPmxData->m_FaceNum];

	//�ʃf�[�^�ǂݍ���
	FaceLoad(fp, pPmxData);
		
	//�e�N�X�`�����ǂݍ���
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_TexNum = StrToInt(Data, sizeof(Data));

	//�e�N�X�`���Ȃ�
	if (pPmxData->m_TexNum <= 0)
	{
		return false;
	}

	//�e�N�X�`���f�[�^�������m��
	pPmxData->m_pTex = new PMX_TEXTURE[pPmxData->m_TexNum];

	//�e�N�X�`���ǂݍ���
	TexLoad(fp, pPmxData);

	return true;
}

//���f�����ǂݍ���
void CPmxLoader::ModelInfoLoad(FILE* fp, PMX_DATA* pPmxData)
{
	unsigned char Data[4];

	//���f����(��)�T�C�Y
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_ModelInfo.m_NameJpnSize = StrToInt(Data,sizeof(Data));

	//�������m��
	pPmxData->m_ModelInfo.m_pNameJap = new unsigned char[pPmxData->m_ModelInfo.m_NameJpnSize];

	//���f����(��)�ǂݍ���
	fread_s(pPmxData->m_ModelInfo.m_pNameJap, pPmxData->m_ModelInfo.m_NameJpnSize, pPmxData->m_ModelInfo.m_NameJpnSize, 1, fp);
	
	//�����񂩂�\0������
	ErasCharFromString(pPmxData->m_ModelInfo.m_pNameJap, pPmxData->m_ModelInfo.m_NameJpnSize, '\0');

	//���f����(�p)�T�C�Y
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_ModelInfo.m_NameEngSize = StrToInt(Data, sizeof(Data));

	//�������m��
	pPmxData->m_ModelInfo.m_pNameEng = new unsigned char[pPmxData->m_ModelInfo.m_NameEngSize];

	//���f����(�p)�ǂݍ���
	fread_s(pPmxData->m_ModelInfo.m_pNameEng, pPmxData->m_ModelInfo.m_NameEngSize, pPmxData->m_ModelInfo.m_NameEngSize, 1, fp);

	//�����񂩂�\0������
	ErasCharFromString(pPmxData->m_ModelInfo.m_pNameEng, pPmxData->m_ModelInfo.m_NameEngSize, '\0');

	//�R�����g(��)�T�C�Y
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_ModelInfo.m_CommentJpnSize = StrToInt(Data,sizeof(Data));

	//�������m��
	pPmxData->m_ModelInfo.m_pCommentJap = new unsigned char[pPmxData->m_ModelInfo.m_CommentJpnSize];

	//�R�����g(��)�ǂݍ���
	fread_s(pPmxData->m_ModelInfo.m_pCommentJap, pPmxData->m_ModelInfo.m_CommentJpnSize, pPmxData->m_ModelInfo.m_CommentJpnSize, 1, fp);

	//�����񂩂�\0������
	ErasCharFromString(pPmxData->m_ModelInfo.m_pCommentJap, pPmxData->m_ModelInfo.m_CommentJpnSize, '\0');

	//�R�����g(�p)�T�C�Y
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_ModelInfo.m_CommentEngSize = StrToInt(Data, sizeof(Data));

	//�������m��
	pPmxData->m_ModelInfo.m_pCommentEng = new unsigned char[pPmxData->m_ModelInfo.m_CommentEngSize];

	//�R�����g(�p)�ǂݍ���
	fread_s(pPmxData->m_ModelInfo.m_pCommentEng, pPmxData->m_ModelInfo.m_CommentEngSize, pPmxData->m_ModelInfo.m_CommentEngSize, 1, fp);

	//�����񂩂�\0������
	ErasCharFromString(pPmxData->m_ModelInfo.m_pCommentEng, pPmxData->m_ModelInfo.m_CommentEngSize, '\0');
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
	for (int i = 0; i < pPmxData->m_VerNum; i++)
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

		//�ό`�����ɉ����ēǂݍ���
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
					//�E�F�C�g
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
	for (int i = 0; i < pPmxData->m_FaceNum; i++)
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


//�e�N�X�`���ǂݍ���
void CPmxLoader::TexLoad(FILE* fp, PMX_DATA* pPmxData)
{
	unsigned char Data[4];
	int AfterByte;

	for (int i = 0; i < pPmxData->m_TexNum; i++)
	{
		//�㑱�o�C�g��
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		AfterByte = StrToInt(Data, sizeof(Data));

		//�e�N�X�`���p�X�������m��
		pPmxData->m_pTex[i].m_pPass = new unsigned char[AfterByte];

		//�e�N�X�`���p�X�ǂݍ���
		fread_s(pPmxData->m_pTex[i].m_pPass, AfterByte, AfterByte, 1, fp);

		//�����񂩂�\0������
		ErasCharFromString(pPmxData->m_pTex[i].m_pPass, AfterByte, '\0');
	}
}


//�����o��
bool CPmxLoader::Write(const char* FileName, PMX_DATA* pPmxData)
{
	FILE* fp = nullptr;
	fopen_s(&fp, FileName, "w");

	//�I�[�v�����s
	if (fp == nullptr)
		return false;

	fprintf_s(fp, "�t�@�C���^�C�v:%s\n", pPmxData->m_Head.m_FileType);
	fprintf_s(fp, "�o�[�W����:%f\n", pPmxData->m_Head.m_Ver);

	fprintf_s(fp, "�o�C�g��T�C�Y:%d\n", pPmxData->m_Head.m_Size);
	for (int i = 0; i < pPmxData->m_Head.m_Size; i++)
	{
		fprintf_s(fp, "%d,", pPmxData->m_Head.m_pData[i]);
	}
	fprintf_s(fp, "\n");

	fprintf_s(fp,"���f����(��):%s\n", pPmxData->m_ModelInfo.m_pNameJap);
	fprintf_s(fp,"���f����(�p):%s\n", pPmxData->m_ModelInfo.m_pNameEng);
	fprintf_s(fp,"�R�����g(��):%s\n", pPmxData->m_ModelInfo.m_pCommentJap);
	fprintf_s(fp,"�R�����g(�p):%s\n", pPmxData->m_ModelInfo.m_pCommentEng);
	fprintf_s(fp, "\n");

	fprintf_s(fp, "���_���F%d\n", pPmxData->m_VerNum);

	for (int i = 0; i < pPmxData->m_VerNum; i++)
	{
		fprintf_s(fp, "���_%8d\n", i);
	
		fprintf_s(fp, "Pos:");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pVertex[i].m_fPos[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "Norm:");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pVertex[i].m_fNorm[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "Uv:");
		for (int j = 0; j < 2; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pVertex[i].m_fUv[j]);
		}
		fprintf_s(fp, "\n");

		//�ǉ����_�������
		if (pPmxData->m_pVertex->m_pfAddUv != nullptr)
		{
			fprintf_s(fp, "AddUv:");
			for (int j = 0; j < 4; j++)
			{
				fprintf_s(fp, "%f,", pPmxData->m_pVertex[i].m_pfAddUv[i]);
			}
			fprintf_s(fp, "\n");
		}

		fprintf_s(fp, "�E�F�C�g�ό`����:%d\n",pPmxData->m_pVertex[i].m_WeightData.m_WeightType);

		//�ό`�����ɉ����ēf���o��
		switch (pPmxData->m_pVertex[i].m_WeightData.m_WeightType)
		{
			//BDEF1
			case 0:
			{
				fprintf_s(fp, "�{�[��ID:");
				fprintf_s(fp, "%d", pPmxData->m_pVertex[i].m_WeightData.m_Bdef1.m_BoneID);
				break;
			}
			//BDEF2
			case 1:
			{

				fprintf_s(fp, "�{�[��ID:");
				for (int j = 0; j < 2; j++)
				{
					fprintf_s(fp, "%d,", pPmxData->m_pVertex[i].m_WeightData.m_Bdef2.m_BoneID[j]);
				}
				fprintf_s(fp,"\n");
				
				fprintf_s(fp, "�E�F�C�g�l:");
				fprintf_s(fp, "%f\n", pPmxData->m_pVertex[i].m_WeightData.m_Bdef2.m_Weight);
				break;
			}
			//BDEF4
			case 2:
			{
				fprintf_s(fp, "�{�[��ID:");
				for (int j = 0; j < 4; j++)
				{
					fprintf_s(fp, "%d,", pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_BoneID[j]);
				}
				fprintf_s(fp, "\n");

				fprintf_s(fp, "�E�F�C�g�l:");
				for (int j = 0; j < 4; j++)
				{
					fprintf_s(fp, "%f,", pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_Weight[j]);
				}
				fprintf_s(fp, "\n");
				break;
			}
			//BDEF2
			case 3:
			{
				fprintf_s(fp, "�{�[��ID:");
				for (int j = 0; j < 2; j++)
				{
					fprintf_s(fp, "%d,", pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_BoneID[j]);
				}
				fprintf_s(fp, "\n");

				fprintf_s(fp, "�E�F�C�g�l:");
				fprintf_s(fp, "%f\n", pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_Weight);

				fprintf_s(fp, "�ό`�p�s��3x3:");
				for (int j = 0; j < 3; j++)
				{
					for (int k = 0; k < 3; k++)
					{
						fprintf_s(fp, "%f", pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_Matrix[j][k]);
					}
					fprintf_s(fp, "\n");
				}
				fprintf_s(fp, "\n");
				break;
			}
			default:
				break;
		}
		
		fprintf_s(fp, "�G�b�W�{���F");
		fprintf_s(fp, "%f\n", pPmxData->m_pVertex->m_EdgeMagn);
		fprintf_s(fp, "\n");
	}

	fprintf_s(fp, "�ʂ̐��F%d\n", pPmxData->m_FaceNum);

	for (int i = 0; i < pPmxData->m_FaceNum; i++)
	{
		fprintf_s(fp, "%5d:", i);
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%8d", pPmxData->m_pFace[i].m_VerIndex[j]);
		}
		fprintf_s(fp, "\n");
	}

	fprintf_s(fp, "�e�N�X�`�����F%d\n", pPmxData->m_TexNum);

	for (int i = 0; i < pPmxData->m_TexNum; i++)
	{
		fprintf_s(fp, "%s\n", pPmxData->m_pTex[i].m_pPass);
	}
	
	return true;
}