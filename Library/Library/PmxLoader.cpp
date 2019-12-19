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

	//�w�b�_���ǂݍ���
	HeadLoad(fp, pPmxData);	

	//���f�����ǂݍ���
	ModelInfoLoad(fp, pPmxData);

	//���_�ǂݍ���
	if (VertexLoad(fp, pPmxData) == false)
	{
		return false;
	}
	
	//�ʃf�[�^�ǂݍ���
	if (FaceLoad(fp, pPmxData) == false)
	{
		return false;
	}
		
	//�e�N�X�`���ǂݍ���
	if (TextureLoad(fp, pPmxData) == false)
	{
		return false;
	}

	//�}�e���A���ǂݍ���
	if (MaterialLoad(fp, pPmxData) == false)
	{
		return false;
	}

	//�{�[���ǂݍ���
	if (BoneLoad(fp, pPmxData) == false)
	{
		return false;
	}
	
	//���[�t�ǂݍ���
	if (MorphLoad(fp, pPmxData) == false)
	{
		return false;
	}

	return true;
}


//�w�b�_���ǂ݂���
void CPmxLoader::HeadLoad(FILE* fp, PMX_DATA* pPmxData)
{
	//�ǂݍ��ݗp
	unsigned char Data[4];

	//�t�@�C���^�C�v
	fread_s(pPmxData->m_Head.m_FileType, sizeof(pPmxData->m_Head.m_FileType), sizeof(pPmxData->m_Head.m_FileType), 1, fp);

	//�o�[�W����
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_Head.m_Ver = StrToFloat(Data);

	//�t�@�C���^�C�v
	fread_s(&pPmxData->m_Head.m_Size, sizeof(pPmxData->m_Head.m_Size), sizeof(pPmxData->m_Head.m_Size), 1, fp);

	//�o�C�g��T�C�Y
	int Size = pPmxData->m_Head.m_Size;
	pPmxData->m_Head.m_pData = new unsigned char[Size];

	//�o�C�g��ǂݍ���
	for (int i = 0; i < Size; i++)
	{
		//�t�@�C���^�C�v
		fread_s(&pPmxData->m_Head.m_pData[i], sizeof(unsigned char), sizeof(unsigned char), 1, fp);
	}
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
bool CPmxLoader::VertexLoad(FILE* fp, PMX_DATA* pPmxData)
{
	unsigned char Data[4];
	
	//���_��
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_VerNum = StrToInt(Data, sizeof(Data));

	//���_�Ȃ�
	if (pPmxData->m_VerNum <= 0)
	{
		return false;
	}

	//�������m��
	pPmxData->m_pVertex = new PMX_VERTEX[pPmxData->m_VerNum];
	
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
			pPmxData->m_pVertex[i].m_fPos[j] = StrToFloat(Data);
		}
		
		//Norm
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pVertex[i].m_fNorm[j] = StrToFloat(Data);
		}

		//UV
		for (int j = 0; j < 2; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pVertex[i].m_fUv[j] = StrToFloat(Data);
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
				pPmxData->m_pVertex[i].m_pfAddUv[j] = StrToFloat(Data);
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
				//�������m��
				pPmxData->m_pVertex[i].m_WeightData.m_pBdef1 = new BDEF1;

				//�{�[��ID
				fread_s(pBoneIndex, BoneIndexSize, BoneIndexSize, 1, fp);
				pPmxData->m_pVertex[i].m_WeightData.m_pBdef1->m_BoneID = StrToInt(pBoneIndex, BoneIndexSize);

				break;
			}
			//BDFE2����
			case 1:
			{
				//�������m��
				pPmxData->m_pVertex[i].m_WeightData.m_pBdef2 = new BDEF2;

				//�{�[��ID*2
				for (int j = 0; j < 2; j++)
				{
					fread_s(pBoneIndex, BoneIndexSize, BoneIndexSize, 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_pBdef2->m_BoneID[j] = StrToInt(pBoneIndex, BoneIndexSize);
				}
			
				//�E�F�C�g
				fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
				pPmxData->m_pVertex[i].m_WeightData.m_pBdef2->m_Weight = StrToFloat(Data);

				break;
			}
			//BDFE4����
			case 2:
			{
				//�������m��
				pPmxData->m_pVertex[i].m_WeightData.m_pBdef4 = new BDEF4;

				//�{�[��ID*4
				for (int j = 0; j < 4; j++)
				{		
					fread_s(pBoneIndex, BoneIndexSize, BoneIndexSize, 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_pBdef4->m_BoneID[j] = StrToInt(pBoneIndex, BoneIndexSize);
				}
	
				//�E�F�C�g*4
				for (int j = 0; j < 4; j++)
				{
					//�E�F�C�g
					fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_pBdef4->m_Weight[j] = StrToFloat(Data);
				}
				break;
			}
			//SDEF����
			case 3:
			{
				//�������m��
				pPmxData->m_pVertex[i].m_WeightData.m_pSdef = new SDEF;

				//�{�[��ID*2
				for (int j = 0; j < 2; j++)
				{
					fread_s(pBoneIndex, BoneIndexSize, BoneIndexSize, 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_pSdef->m_BoneID[j] = StrToInt(pBoneIndex, BoneIndexSize);
				}
			
				//�E�F�C�g
				fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
				pPmxData->m_pVertex[i].m_WeightData.m_pSdef->m_Weight = StrToFloat(Data);

				//3x3�}�g���b�N�X
				for (int j = 0; j < 3; j++)
				{
					for (int k = 0; k < 3; k++)
					{
						fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
						pPmxData->m_pVertex[i].m_WeightData.m_pSdef->m_Matrix[j][k] = StrToFloat(Data);
					}
				}
				break;
			}
			default:
				break;
		}

		//�G�b�W�{��
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pVertex[i].m_EdgeMagn = StrToFloat(Data);
	}

	//�ǂݍ��ݗp�j��
	delete[] pBoneIndex;
	pBoneIndex = nullptr;

	return true;
}

//�ʓǂݍ���
bool CPmxLoader::FaceLoad(FILE* fp, PMX_DATA* pPmxData)
{
	unsigned char Data[4];

	//�ʂ̐�
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
	
	//���_�C���f�b�N�X�T�C�Y
	int IndexSize = pPmxData->m_Head.m_pData[2];

	//���_�C���f�b�N�X�ǂݍ��ݗp
	unsigned char* pFaceData;
	pFaceData = new unsigned char[IndexSize];
	
	//�ʂ̐���
	for (int i = 0; i < pPmxData->m_FaceNum; i++)
	{
		//���_�C���f�b�N�X�ǂݍ���
		for (int j = 0; j < 3; j++)
		{
			fread_s(pFaceData,IndexSize, IndexSize, 1, fp);
			pPmxData->m_pFace[i].m_VerIndex[j] = StrToInt(pFaceData, IndexSize);
		}
	}

	//�ǂݍ��ݗp�j��
	delete[] pFaceData;
	pFaceData = nullptr;

	return true;
}

//�e�N�X�`���ǂݍ���
bool CPmxLoader::TextureLoad(FILE* fp, PMX_DATA* pPmxData)
{
	unsigned char Data[4];
	
	//�e�N�X�`�����ǂݍ���
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_TexNum = StrToInt(Data, sizeof(Data));

	//�e�N�X�`���Ȃ�
	if (pPmxData->m_TexNum <= 0)
	{
		return false;
	}

	//�������m��
	pPmxData->m_pTex = new PMX_TEXTURE[pPmxData->m_TexNum];

	//�㑱�o�C�g��
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

	return true;
}


//�}�e���A���ǂݍ���
bool CPmxLoader::MaterialLoad(FILE* fp, PMX_DATA* pPmxData)
{
	unsigned char Data[4];

	//�}�e���A����
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_MaterialNum = StrToInt(Data, sizeof(Data));

	//�}�e���A���Ȃ�
	if (pPmxData->m_MaterialNum <= 0)
	{
		return false;
	}

	//�������m��
	pPmxData->m_pMaterial = new PMX_MATERIAL[pPmxData->m_MaterialNum];


	//�ގ����p�T�C�Y
	int JapSize;
	int EngSize;

	//�����T�C�Y
	int MemoSize;

	//�e�N�X�`���C���f�b�N�X�T�C�Y
	int TexIndexSize = pPmxData->m_Head.m_pData[3];
	
	//�e�N�X�`���f�[�^�ǂݍ��ݗp
	unsigned char* pTexData = nullptr;
	pTexData = new unsigned char[TexIndexSize];
		
	//�ǂݍ���
	for (int i = 0; i < pPmxData->m_MaterialNum; i++)
	{
		//�ގ���(���j�T�C�Y
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		JapSize = StrToInt(Data, sizeof(Data));

		//�������m��
		pPmxData->m_pMaterial[i].m_pNameJap = new unsigned char[JapSize];

		//�ގ���(���j�ǂݍ���
		fread_s(pPmxData->m_pMaterial[i].m_pNameJap, JapSize, JapSize, 1, fp);

		//�����񂩂�\0������
		ErasCharFromString(pPmxData->m_pMaterial[i].m_pNameJap, JapSize, '\0');

		//�ގ���(�p�j�T�C�Y
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		EngSize = StrToInt(Data, sizeof(Data));

		//�������m��
		pPmxData->m_pMaterial[i].m_pNameEng = new unsigned char[EngSize];

		//�ގ���(�p�j�ǂݍ���
		fread_s(pPmxData->m_pMaterial[i].m_pNameEng, EngSize, EngSize, 1, fp);

		//�����񂩂�\0������
		ErasCharFromString(pPmxData->m_pMaterial[i].m_pNameEng, EngSize, '\0');

		//�f�B�t���[�Y
		for (int j = 0; j < 4; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pMaterial[i].m_Diffuse[j] = StrToFloat(Data);
		}

		//�X�y�L�����[
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pMaterial[i].m_Specular[j] = StrToFloat(Data);
		}

		//�X�y�L�����p���[
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pMaterial[i].m_SpePower = StrToFloat(Data);

		//�A���r�G���g
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pMaterial[i].m_Ambient[j] = StrToFloat(Data);
		}

		//�`��t���O
		fread_s(&pPmxData->m_pMaterial[i].m_BitFlag, sizeof(pPmxData->m_pMaterial[i].m_BitFlag), sizeof(pPmxData->m_pMaterial[i].m_BitFlag), 1, fp);

		//�G�b�W�F
		for (int j = 0; j < 4; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pMaterial[i].m_Edge[j] = StrToFloat(Data);
		}

		//�G�b�W�T�C�Y
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pMaterial[i].m_EdgeSize = StrToFloat(Data);

		//�ʏ�e�N�X�`��
		fread_s(pTexData, TexIndexSize, TexIndexSize, 1, fp);
		pPmxData->m_pMaterial[i].m_NormTex = StrToInt(pTexData, TexIndexSize);
		
		//�X�t�B�A�e�N�X�`��
		fread_s(pTexData, TexIndexSize, TexIndexSize, 1, fp);
		pPmxData->m_pMaterial[i].m_SphereTex = StrToInt(pTexData, TexIndexSize);

		//�X�t�B�A���[�h
		fread_s(&pPmxData->m_pMaterial[i].m_SphereMode, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

		//���LToon�t���O
		fread_s(&pPmxData->m_pMaterial[i].m_ToonFlag, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

		switch (pPmxData->m_pMaterial[i].m_ToonFlag)
		{
			//Toon�e�N�X�`��
			case 0:
			{
				fread_s(pTexData, TexIndexSize, TexIndexSize, 1, fp);
				pPmxData->m_pMaterial[i].m_ToonTex = StrToInt(pTexData, TexIndexSize);
				break;
			}
			//���LToon�e�N�X�`��
			case 1:
			{
				unsigned char read;
				fread_s(&read, sizeof(unsigned char), sizeof(unsigned char), 1, fp);
				pPmxData->m_pMaterial[i].m_ToonTex = StrToInt(&read, sizeof(read));
				break;
			}
		}

		//�����T�C�Y
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		MemoSize = StrToInt(Data, sizeof(Data));

		//�������m��
		pPmxData->m_pMaterial[i].m_pMemo = new unsigned char[MemoSize];

		//�����ǂݍ���
		fread_s(pPmxData->m_pMaterial[i].m_pMemo, MemoSize, MemoSize, 1, fp);

		//�����񂩂�\0������
		ErasCharFromString(pPmxData->m_pMaterial[i].m_pMemo, MemoSize, '\0');

		//�g�p���钸�_��
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pMaterial[i].m_UseVerNum = StrToInt(Data, sizeof(Data));
	}

	//�ǂݍ��ݗp�j��
	delete[] pTexData;
	pTexData = nullptr;

	return true;
}

//�{�[���ǂݍ���
bool CPmxLoader::BoneLoad(FILE* fp, PMX_DATA* pPmxData)
{
	unsigned char Data[4];

	//�{�[����
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_BoneNum = StrToInt(Data, sizeof(Data));

	//�{�[���Ȃ�
	if (pPmxData->m_BoneNum <= 0)
	{
		return false;
	}

	//�������m��
	pPmxData->m_pBone = new PMX_BONE[pPmxData->m_BoneNum];

	//�{�[�����p�T�C�Y
	int JapSize;
	int EngSize;

	//�{�[���C���f�b�N�X�T�C�Y
	int BoneIndexSize = pPmxData->m_Head.m_pData[5];

	//�{�[���f�[�^�ǂݍ��ݗp
	unsigned char* pBoneData = nullptr;
	pBoneData = new unsigned char[BoneIndexSize];
	
	//bit�t���O�ϊ��p
	int Flag;

	//�ǂݍ���
	for (int i = 0; i < pPmxData->m_BoneNum; i++)
	{
		//�{�[����(��)�T�C�Y
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		JapSize = StrToInt(Data, sizeof(Data));

		//�������m��
		pPmxData->m_pBone[i].m_pNameJap = new unsigned char[JapSize];

		//�{�[����(��)�ǂݍ���
		fread_s(pPmxData->m_pBone[i].m_pNameJap, JapSize, JapSize, 1, fp);

		//�����񂩂�\0������
		ErasCharFromString(pPmxData->m_pBone[i].m_pNameJap, JapSize, '\0');

		//�{�[����(�p)�T�C�Y
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		EngSize = StrToInt(Data, sizeof(Data));

		//�������m��
		pPmxData->m_pBone[i].m_pNameEng = new unsigned char[EngSize];

		//�{�[����(�p)�ǂݍ���
		fread_s(pPmxData->m_pBone[i].m_pNameEng, EngSize, EngSize, 1, fp);

		//�����񂩂�\0������
		ErasCharFromString(pPmxData->m_pBone[i].m_pNameEng, EngSize, '\0');

		//�ʒu
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pBone[i].m_fPos[j] = StrToFloat(Data);
		}

		//�e�{�[��ID
		fread_s(pBoneData, BoneIndexSize, BoneIndexSize, 1, fp);
		pPmxData->m_pBone[i].m_ParentId = StrToInt(pBoneData, BoneIndexSize);
		
		//�ό`�K�w
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pBone[i].m_Hierarchy = StrToInt(Data,sizeof(Data));

		//Bit�t���O
		for (int j = 0; j < 2; j++)
		{
			fread_s(&pPmxData->m_pBone[i].m_BitFlag[j], sizeof(unsigned char), sizeof(unsigned char), 1, fp);
		}

		//�r�b�g�t���O��int�ɂ���
		Flag=StrToInt(pPmxData->m_pBone[i].m_BitFlag, sizeof(pPmxData->m_pBone[i].m_BitFlag));
		
		//�ڑ���0
		if ((Flag & 0x0001) == 0)
		{
			//�I�t�Z�b�g
			for (int j = 0; j < 3; j++)
			{
				fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
				pPmxData->m_pBone[i].m_fOffset[j] = StrToFloat(Data);
			}
		}
		//�ڑ���1
		else
		{
			//�ڑ���{�[��ID
			fread_s(pBoneData, BoneIndexSize, BoneIndexSize, 1, fp);
			pPmxData->m_pBone[i].m_ConnectId = StrToInt(pBoneData, BoneIndexSize);
		}

		//��]�܂��͈ړ��̕t�^������Ƃ�
		if ((Flag & 0x0100) != 0 || (Flag & 0x0200) != 0)
		{
			//�t�^�e�{�[��ID
			fread_s(pBoneData, BoneIndexSize, BoneIndexSize, 1, fp);
			pPmxData->m_pBone[i].m_GrantId = StrToInt(pBoneData, BoneIndexSize);
						
			//�t�^��
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pBone[i].m_fGrantRate = StrToFloat(Data);
		}
	
		//���Œ�
		if ((Flag & 0x0400) != 0)
		{
			for (int j = 0; j < 3; j++)
			{
				fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
				pPmxData->m_pBone[i].m_fFixedAxis[j] = StrToFloat(Data);
			}
		}

		//���[�J����
		if ((Flag & 0x0800) != 0)
		{
			for (int j = 0; j < 3; j++)
			{
				fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
				pPmxData->m_pBone[i].m_fAxisX[j] = StrToFloat(Data);
			}
			for (int j = 0; j < 3; j++)
			{
				fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
				pPmxData->m_pBone[i].m_fAxisZ[j] = StrToFloat(Data);
			}
		}

		//�O���e�ό`
		if ((Flag & 0x2000) != 0)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pBone[i].m_Key = StrToInt(Data,sizeof(Data));
		}

		//IK
		if ((Flag & 0x0020) != 0)
		{
			//�^�[�Q�b�g�{�[��ID
			fread_s(pBoneData, BoneIndexSize, BoneIndexSize, 1, fp);
			pPmxData->m_pBone[i].m_Ik.m_TargetId = StrToInt(pBoneData, BoneIndexSize);

			//���[�v��
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pBone[i].m_Ik.m_RoopTime = StrToInt(Data, sizeof(Data));

			//��]�p
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pBone[i].m_Ik.m_fRad = StrToFloat(Data);

			//IK�����N��
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pBone[i].m_Ik.m_LinkNum = StrToInt(Data, sizeof(Data));

			//�������m��
			pPmxData->m_pBone[i].m_Ik.m_pLink = new PMX_IK_LINK[pPmxData->m_pBone[i].m_Ik.m_LinkNum];

			//IK�����N�ǂݍ���
			for (int j = 0; j < pPmxData->m_pBone[i].m_Ik.m_LinkNum; j++)
			{
				//�����N�{�[��ID
				fread_s(pBoneData, BoneIndexSize, BoneIndexSize, 1, fp);
				pPmxData->m_pBone[i].m_Ik.m_pLink[j].m_LinkBoneId = StrToInt(pBoneData, BoneIndexSize);
				
				//�p�x����
				fread_s(&pPmxData->m_pBone[i].m_Ik.m_pLink[j].m_RadRest, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

				//�p�x��������̏ꍇ
				if (pPmxData->m_pBone[i].m_Ik.m_pLink[j].m_RadRest == 1)
				{
					//����
					for (int k = 0; k < 3; k++)
					{
						fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
						pPmxData->m_pBone[i].m_Ik.m_pLink[j].m_fLowerRad[k] = StrToFloat(Data);
					}
					//���
					for (int k = 0; k < 3; k++)
					{
						fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
						pPmxData->m_pBone[i].m_Ik.m_pLink[j].m_fUpperRad[k] = StrToFloat(Data);
					}
				}
			}
		}
	}
	
	//�ǂݍ��ݗp�j��
	delete[] pBoneData;
	pBoneData = nullptr;

	return true;
}

//���[�t�ǂݍ���
bool CPmxLoader::MorphLoad(FILE* fp, PMX_DATA* pPmxData)
{
	unsigned char Data[4];

	//���[�t��
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_MorphNum = StrToInt(Data, sizeof(Data));

	//���[�t
	if (pPmxData->m_MorphNum <= 0)
	{
		return false;
	}

	//�������m��
	pPmxData->m_pMorph = new PMX_MORPH[pPmxData->m_MorphNum];

	//���[�t���T�C�Y
	int JapSize;
	int EngSize;

	////�ǂݍ���
	//for (int i = 0; i < pPmxData->m_MorphNum; i++)
	//{
	//	long ReadStartPos = ftell(fp);

	//	//���[�t��(��)�T�C�Y
	//	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	//	JapSize = StrToInt(Data, sizeof(Data));

	//	//�������m��
	//	pPmxData->m_pMorph[i].m_pNameJap = new unsigned char[JapSize];

	//	//���[�t��(��)�ǂݍ���
	//	fread_s(pPmxData->m_pMorph[i].m_pNameJap, JapSize, JapSize, 1, fp);

	//	//�����񂩂�\0������
	//	ErasCharFromString(pPmxData->m_pMorph[i].m_pNameJap, JapSize, '\0');

	//	//���[�t��(�p)�T�C�Y
	//	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	//	EngSize = StrToInt(Data, sizeof(Data));

	//	//�������m��
	//	pPmxData->m_pMorph[i].m_pNameEng = new unsigned char[EngSize];

	//	//�{�[����(�p)�ǂݍ���
	//	fread_s(pPmxData->m_pMorph[i].m_pNameEng, EngSize, EngSize, 1, fp);

	//	//�����񂩂�\0������
	//	ErasCharFromString(pPmxData->m_pMorph[i].m_pNameEng, EngSize, '\0');

	//	//PMD�J�e�S��
	//	fread_s(&pPmxData->m_pMorph[i].m_PmdType, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

	//	//���[�t�^�C�v
	//	fread_s(&pPmxData->m_pMorph[i].m_MorphType, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

	//	//�f�[�^��
	//	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	//	pPmxData->m_pMorph[i].m_DataNum = StrToInt(Data, sizeof(Data));

	//	switch (pPmxData->m_pMorph[i].m_MorphType)
	//	{
	//		//�O���[�v
	//		case 0:
	//		{
	//			//�������m��
	//			pPmxData->m_pMorph[i].m_pGroupMorph = new PMX_GROUP_MORPH[pPmxData->m_pMorph[i].m_DataNum];

	//			//���[�t�C���f�b�N�X�T�C�Y
	//			int MorphIndexSize= pPmxData->m_Head.m_pData[6];

	//			//���[�t�C���f�b�N�X�ǂݍ��ݗp
	//			unsigned char* pMorph = nullptr;
	//			pMorph = new unsigned char[MorphIndexSize];

	//			//�ǂݍ���
	//			for (int j = 0; j < pPmxData->m_pMorph[i].m_DataNum; j++)
	//			{
	//				//���[�t�C���f�b�N�X
	//				fread_s(pMorph, MorphIndexSize, MorphIndexSize, 1, fp);
	//				pPmxData->m_pMorph[i].m_pGroupMorph[j].m_MorphId = StrToInt(pMorph, MorphIndexSize);

	//				//���[�t��
	//				fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	//				pPmxData->m_pMorph[i].m_pGroupMorph[j].m_fRate = StrToFloat(Data);
	//			}

	//			//�ǂݍ��ݗp�j��
	//			delete[] pMorph;
	//			pMorph = nullptr;

	//			break;
	//		}
	//		//���_
	//		case 1:
	//		{
	//			//�������m��
	//			pPmxData->m_pMorph[i].m_pVerMorph = new PMX_VER_MORPH[pPmxData->m_pMorph[i].m_DataNum];

	//			//���_�C���f�b�N�X�T�C�Y
	//			int VerIndexSize = pPmxData->m_Head.m_pData[2];

	//			//���_�C���f�b�N�X�ǂݍ��ݗp
	//			unsigned char* pVer = nullptr;
	//			pVer = new unsigned char[VerIndexSize];

	//			//�ǂݍ���
	//			for (int j = 0; j < pPmxData->m_pMorph[i].m_DataNum; j++)
	//			{
	//				//���_ID
	//				fread_s(pVer, VerIndexSize, VerIndexSize, 1, fp);
	//				pPmxData->m_pMorph[i].m_pVerMorph[j].m_VerId=StrToInt(pVer,VerIndexSize);

	//				//�I�t�Z�b�g�l
	//				for (int k = 0; k < 3; k++)
	//				{
	//					fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	//					pPmxData->m_pMorph[i].m_pVerMorph[j].m_fOffset[k] = StrToFloat(Data);
	//				}
	//			}

	//			//�ǂݍ��ݗp�j��
	//			delete[] pVer;
	//			pVer = nullptr;

	//			break;
	//		}
	//		//�{�[��
	//		case 2:
	//		{
	//			//�������m��
	//			pPmxData->m_pMorph[i].m_pBoneMorph = new PMX_BONE_MORPH[pPmxData->m_pMorph[i].m_DataNum];
	//			
	//			//�{�[���C���f�b�N�X�T�C�Y
	//			int BoneIndexSize = pPmxData->m_Head.m_pData[5];

	//			//���_�C���f�b�N�X�ǂݍ��ݗp
	//			unsigned char* pBone = nullptr;
	//			pBone = new unsigned char[BoneIndexSize];

	//			//�ǂݍ���
	//			for (int j = 0; j < pPmxData->m_pMorph[i].m_DataNum; j++)
	//			{
	//				//�{�[��ID
	//				fread_s(pBone, BoneIndexSize, BoneIndexSize, 1, fp);
	//				pPmxData->m_pMorph[i].m_pBoneMorph[j].m_BoneId = StrToInt(pBone, BoneIndexSize);

	//				//�ړ���
	//				for (int k = 0; k < 3; k++)
	//				{
	//					fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	//					pPmxData->m_pMorph[i].m_pBoneMorph[j].m_fMove[k] = StrToFloat(Data);
	//				}
	//				//��]��
	//				for (int k = 0; k < 4; k++)
	//				{
	//					fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	//					pPmxData->m_pMorph[i].m_pBoneMorph[j].m_fRot[k] = StrToFloat(Data);
	//				}
	//			}

	//			//�ǂݍ��ݗp�j��
	//			delete[] pBone;
	//			pBone = nullptr;

	//			break;
	//		}
	//		//�ގ�
	//		case 8:
	//		{
	//			//�������m��
	//			pPmxData->m_pMorph[i].m_pMateMorph = new PMX_MATE_MORPH[pPmxData->m_pMorph[i].m_DataNum];

	//			//�ގ��T�C�Y
	//			int MateIndexSize = pPmxData->m_Head.m_pData[4];

	//			//�ގ��C���f�b�N�X�ǂݍ��ݗp
	//			unsigned char* pMate = nullptr;
	//			pMate = new unsigned char[MateIndexSize];

	//			//�ǂݍ���
	//			for (int j = 0; j < pPmxData->m_pMorph[i].m_DataNum; j++)
	//			{

	//				//��������I�I�I�I
	//			}

	//			//�ǂݍ��ݗp�j��
	//			delete[] pMate;
	//			pMate = nullptr;

	//			break;
	//		}
	//		//UVor�ǉ�UV
	//		default:
	//		{
	//			//�������m��
	//			pPmxData->m_pMorph[i].m_pUvMorph= new PMX_UV_MORPH[pPmxData->m_pMorph[i].m_DataNum];
	//			
	//			//���_�C���f�b�N�X�T�C�Y
	//			int VerIndexSize = pPmxData->m_Head.m_pData[2];

	//			//���_�C���f�b�N�X�ǂݍ��ݗp
	//			unsigned char* pVer = nullptr;
	//			pVer = new unsigned char[VerIndexSize];

	//			//�ǂݍ���
	//			for (int j = 0; j < pPmxData->m_pMorph[i].m_DataNum; j++)
	//			{
	//				//���_ID
	//				fread_s(pVer, VerIndexSize, VerIndexSize, 1, fp);
	//				pPmxData->m_pMorph[i].m_pUvMorph[j].m_VerId = StrToInt(pVer, VerIndexSize);

	//				//�I�t�Z�b�g�l
	//				for (int k = 0; k < 4; k++)
	//				{
	//					fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	//					pPmxData->m_pMorph[i].m_pUvMorph[j].m_fOffset[k] = StrToFloat(Data);
	//				}

	//			}

	//			//�ǂݍ��ݗp�j��
	//			delete[] pVer;
	//			pVer = nullptr;

	//			break;
	//		}
	//	}
	//	
	//}

	return true;
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
				fprintf_s(fp, "%d\n", pPmxData->m_pVertex[i].m_WeightData.m_pBdef1->m_BoneID);
				break;
			}
			//BDEF2
			case 1:
			{

				fprintf_s(fp, "�{�[��ID:");
				for (int j = 0; j < 2; j++)
				{
					fprintf_s(fp, "%d,", pPmxData->m_pVertex[i].m_WeightData.m_pBdef2->m_BoneID[j]);
				}
				fprintf_s(fp,"\n");
				
				fprintf_s(fp, "�E�F�C�g�l:");
				fprintf_s(fp, "%f\n", pPmxData->m_pVertex[i].m_WeightData.m_pBdef2->m_Weight);
				break;
			}
			//BDEF4
			case 2:
			{
				fprintf_s(fp, "�{�[��ID:");
				for (int j = 0; j < 4; j++)
				{
					fprintf_s(fp, "%d,", pPmxData->m_pVertex[i].m_WeightData.m_pBdef4->m_BoneID[j]);
				}
				fprintf_s(fp, "\n");

				fprintf_s(fp, "�E�F�C�g�l:");
				for (int j = 0; j < 4; j++)
				{
					fprintf_s(fp, "%f,", pPmxData->m_pVertex[i].m_WeightData.m_pBdef4->m_Weight[j]);
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
					fprintf_s(fp, "%d,", pPmxData->m_pVertex[i].m_WeightData.m_pSdef->m_BoneID[j]);
				}
				fprintf_s(fp, "\n");

				fprintf_s(fp, "�E�F�C�g�l:");
				fprintf_s(fp, "%f\n", pPmxData->m_pVertex[i].m_WeightData.m_pSdef->m_Weight);

				fprintf_s(fp, "�ό`�p�s��3x3:");
				for (int j = 0; j < 3; j++)
				{
					for (int k = 0; k < 3; k++)
					{
						fprintf_s(fp, "%f", pPmxData->m_pVertex[i].m_WeightData.m_pSdef->m_Matrix[j][k]);
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
	
	fprintf_s(fp, "�}�e���A�����F%d\n", pPmxData->m_MaterialNum);

	for (int i = 0; i < pPmxData->m_MaterialNum; i++)
	{
		fprintf_s(fp, "�}�e���A����(��):%s\n", pPmxData->m_pMaterial[i].m_pNameJap);
		fprintf_s(fp, "�}�e���A����(�p):%s\n", pPmxData->m_pMaterial[i].m_pNameEng);
		
		fprintf_s(fp, "Diffuse:");
		for (int j = 0; j < 4; j++)
		{
			fprintf_s(fp, "%f,",pPmxData->m_pMaterial[i].m_Diffuse[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "Specular:");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pMaterial[i].m_Specular[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "SpecularPower:%f\n", pPmxData->m_pMaterial[i].m_SpePower);

		fprintf_s(fp, "Ambient:");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pMaterial[i].m_Ambient[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "�`��t���O:%d\n", pPmxData->m_pMaterial[i].m_BitFlag);

		fprintf_s(fp, "Edge:");
		for (int j = 0; j < 4; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pMaterial[i].m_Edge[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "EdgeSize:%f\n", pPmxData->m_pMaterial[i].m_EdgeSize);

		fprintf_s(fp, "Texture:%d\n", pPmxData->m_pMaterial[i].m_NormTex);
		fprintf_s(fp, "SphereTexture:%d\n", pPmxData->m_pMaterial[i].m_SphereTex);
		fprintf_s(fp, "SphereMode:%d\n", pPmxData->m_pMaterial[i].m_SphereMode);
		fprintf_s(fp, "ToonTexture:%d\n", pPmxData->m_pMaterial[i].m_ToonTex);
		fprintf_s(fp, "ToonFlag:%d\n", pPmxData->m_pMaterial[i].m_ToonFlag);

		fprintf_s(fp, "����:%s\n", pPmxData->m_pMaterial[i].m_pMemo);
		fprintf_s(fp, "�g�p���钸�_��:%d\n", pPmxData->m_pMaterial[i].m_UseVerNum);

		fprintf_s(fp, "\n");
	}

	fprintf_s(fp, "�{�[�����F%d\n", pPmxData->m_BoneNum);

	for (int i = 0; i < pPmxData->m_BoneNum; i++)
	{
		fprintf_s(fp, "�{�[����(��):%s\n", pPmxData->m_pBone[i].m_pNameJap);
		fprintf_s(fp, "�{�[����(�p):%s\n", pPmxData->m_pBone[i].m_pNameEng);

		fprintf_s(fp, "Pos:");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pBone[i].m_fPos[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "�e�{�[��ID:%d\n", pPmxData->m_pBone[i].m_ParentId);
		fprintf_s(fp, "�ό`�K�w:%d\n", pPmxData->m_pBone[i].m_Hierarchy);
		fprintf_s(fp, "BitFlag:");
		for (int j = 0; j < 2; j++)
		{
			fprintf_s(fp, "%d,",pPmxData->m_pBone[i].m_BitFlag[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "Offset:");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pBone[i].m_fOffset[j]);
		}
		fprintf_s(fp, "\n");
		
		fprintf_s(fp, "�ڑ���{�[��:%d\n",pPmxData->m_pBone[i].m_ConnectId);

		fprintf_s(fp, "�t�^�e�{�[��:%d\n", pPmxData->m_pBone[i].m_GrantId);
		fprintf_s(fp, "�t�^��:%f\n", pPmxData->m_pBone[i].m_fGrantRate);

		fprintf_s(fp, "���Œ�:");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pBone[i].m_fFixedAxis[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "���[�J����X:");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pBone[i].m_fAxisX[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "���[�J����Z:");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pBone[i].m_fAxisZ[j]);
		}
		fprintf_s(fp, "\n");
		
		fprintf_s(fp, "Key:%d\n",pPmxData->m_pBone[i].m_Key);
			
		fprintf_s(fp, "�^�[�Q�b�g�{�[��:%d\n", pPmxData->m_pBone[i].m_Ik.m_TargetId);
		fprintf_s(fp, "���[�v��:%d\n", pPmxData->m_pBone[i].m_Ik.m_RoopTime);
		fprintf_s(fp, "��]�p:%f\n", pPmxData->m_pBone[i].m_Ik.m_fRad);

		fprintf_s(fp, "�����N��:%d\n", pPmxData->m_pBone[i].m_Ik.m_LinkNum);
		
		for (int j = 0; j < pPmxData->m_pBone[i].m_Ik.m_LinkNum; j++)
		{
			fprintf_s(fp, "�����N�{�[��:%d\n", pPmxData->m_pBone[i].m_Ik.m_pLink[j].m_LinkBoneId);
			fprintf_s(fp, "�p�x����:%d\n", pPmxData->m_pBone[i].m_Ik.m_pLink[j].m_RadRest);

			fprintf_s(fp, "�����p:");
			for (int k = 0; k < 3; k++)
			{
				fprintf_s(fp, "%f,", pPmxData->m_pBone[i].m_Ik.m_pLink[j].m_fLowerRad[k]);
			}
			fprintf_s(fp, "\n");

			fprintf_s(fp, "����p:");
			for (int k = 0; k < 3; k++)
			{
				fprintf_s(fp, "%f,", pPmxData->m_pBone[i].m_Ik.m_pLink[j].m_fUpperRad[k]);
			}
			fprintf_s(fp, "\n");
		}
		fprintf_s(fp, "\n");
	}

	/*fprintf_s(fp, "���[�t��:%d\n", pPmxData->m_MorphNum);

	for (int i = 0; i < pPmxData->m_MorphNum; i++)
	{
		fprintf_s(fp, "���[�t��(��):%s\n", pPmxData->m_pMorph[i].m_pNameJap);
		fprintf_s(fp, "���[�t��(�p):%s\n", pPmxData->m_pMorph[i].m_pNameEng);

		fprintf_s(fp, "PMD�J�e�S��:%d\n", pPmxData->m_pMorph[i].m_PmdType);
		fprintf_s(fp, "���[�t�^�C�v:%d\n", pPmxData->m_pMorph[i].m_MorphType);


		fprintf_s(fp, "\n");
	}*/
	return true;
}