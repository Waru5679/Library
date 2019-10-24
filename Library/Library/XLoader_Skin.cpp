#include "XLoader_Skin.h"
#include "Release.h"
#include "LibraryHeader.h"

//�C���X�^���X
CX_Skin* CX_Skin::m_pInstance = nullptr;

//�X�L�����b�V���̓ǂݍ���
bool CX_Skin::LoadSkinMesh(const char* FileName, SKIN_MESH* pSkinMesh)
{
	//�t�@�C���ǂݍ���
	FILE* fp = nullptr;
	fopen_s(&fp, FileName, "rt");
	
	if (fp == nullptr)
	{
		//�t�@�C���I�[�v�����s
		return false;
	}

	//template���Ȃ����t�@�C���̓ǂݍ��݊J�n�ʒu��ۑ�
	long ReadStartPos = GetTemplateSkipStartPos(fp);

	//���b�V���̓ǂݍ���
	if (LoadMesh(fp, &pSkinMesh->m_Mesh,ReadStartPos) == false)
	{
		//���b�V���ǂݍ��ݎ��s
		return false;
	}

	//�{�[���ǂݍ���
	if (LoadBone(fp, pSkinMesh, ReadStartPos) == false)
	{
		//�{�[���ǂݍ��ݎ��s
		return false;
	}
	
	//�X�L�����̓ǂݍ���
	if (LoadSkin(fp, pSkinMesh, ReadStartPos) == false)
	{
		//�X�L�����ǂݍ��ݎ��s
		return false;
	}

	//�A�j���[�V�����ǂݍ���
	if (LoadAnimation(fp, pSkinMesh, ReadStartPos) == false)
	{
		//�A�j���[�V�����ǂݍ��ݎ��s
		return false;
	}

	//�{�[�����ƃX�L�������܂Ƃ߂�
	BoneWithSkin(pSkinMesh);

	return true;
}

//template���΂����ǂݍ��݊J�n�ʒu���擾����
long CX_Skin::GetTemplateSkipStartPos(FILE* fp)
{
	//�t�@�C���̐擪�ɃZ�b�g
	fseek(fp, 0, SEEK_SET);

	long pos;//�t�@�C���̌��݈ʒu
	
	//�L�[���[�h�ǂݍ���
	char str[READ_ARRAY_SIZE];

	while (!feof(fp))
	{
		//�t�@�C���̌��݈ʒu�ۑ�
		pos = ftell(fp);

		//�L�[���[�h �ǂݍ���
		fscanf_s(fp, "%s ", str, sizeof(str));

		//Frame���o��܂Ŕ�΂�
		if (strcmp(str, "Frame") == 0)
		{
			return pos;
		}
	}
}

//���b�V�����̓ǂݍ���
bool CX_Skin::LoadMesh(FILE* fp, MESH* pMesh,long lStartPos)
{
	//�ǂݍ��݂̊J�n�ʒu�ɃZ�b�g
	fseek(fp, lStartPos, SEEK_SET);

	int verNum	=0;//���_��
	int faceNum	=0;//�ʂ̐�
	int normNum	=0;//�@����
	int uvNum	=0;//UV��
	int mateNum	=0;//�}�e���A���̐�
		
	//�ǂݍ��ݗp�i�ǂݍ��݌�j���j
	int*		 pFaceOfMate= nullptr;//�ʂ̑Ή��}�e���A���̃C���f�b�N�X
	D3DXVECTOR3* pvPos		= nullptr;//���_���W
	D3DXVECTOR3* pvNormal	= nullptr;//�@��
	D3DXVECTOR2* pvTex		= nullptr;//�e�N�X�`�����W
	VERTEX*		 pVertex	= nullptr;//���_���X�g

	//�ǂݍ��݌セ�̂܂܎g������
	FACE*		pFace		= pMesh->m_pFace;		//�ʂ̃��X�g
	MATERIAL*	pMaterial	= pMesh->m_pMaterial;	//�}�e���A���̃��X�g

	//�ǂݍ��ݗp
	float x, y, z, w;
	int v1, v2, v3, v4;
	v1 = v2 = v3 = v4 = 0;

	//�L�[���[�h�ǂݍ���
	char str[READ_ARRAY_SIZE];

	while (!feof(fp))
	{
		//�L�[���[�h �ǂݍ���
		fscanf_s(fp, "%s ", str, sizeof(str));

		//���_
		if (strcmp(str, "Mesh") == 0)
		{
			//���b�V����
			fgets(str, sizeof(str), fp);

			//���_��
			fgets(str, sizeof(str), fp);
			verNum = atoi(str);
			
			//���_���W�������m��
			pvPos = new D3DXVECTOR3[verNum];

			//���_���W�ǂݍ���
			for (int i = 0; i < verNum; i++)
			{
				fscanf_s(fp, "%f;%f;%f;,", &x, &y, &z);
				pvPos[i].x = x;
				pvPos[i].y = y;
				pvPos[i].z = z;
			}

			//;����
			fgets(str, sizeof(str), fp);

			//�ʂ̐�
			fgets(str, sizeof(str), fp);
			faceNum = atoi(str);
						
			//�ʂ̃������m��
			pFace = new FACE[faceNum];
		
			int faceOfVer = 0;//�ʂ��\�����钸�_��

			//�ʓǂݍ���
			for (int i = 0; i < faceNum; i++)
			{
				//�ʂ��\�����钸�_�̐�
				fscanf_s(fp, "%d;", &faceOfVer);
				pFace[i].m_FaceOfVer = faceOfVer;

				//�ʍ\���̃C���f�b�N�X���X�g�̃������m��
				pFace[i].m_pIndex = new int[faceOfVer];

				//�O�p�|���S��
				if (faceOfVer == TRIANGLE_POLYGON)
				{
					fscanf_s(fp, "%d,%d,%d;,", &v1, &v2, &v3);
					pFace[i].m_pIndex[0] = v1;
					pFace[i].m_pIndex[1] = v2;
					pFace[i].m_pIndex[2] = v3;
				}
				//�l�p�|���S��
				if (faceOfVer == QUAD_POLYGON)
				{
					fscanf_s(fp, "%d,%d,%d,%d;,", &v1, &v2, &v3, &v4);
					pFace[i].m_pIndex[0] = v1;
					pFace[i].m_pIndex[1] = v2;
					pFace[i].m_pIndex[2] = v3;
					pFace[i].m_pIndex[3] = v4;
				}
			}
		}
		//�@��
		if (strcmp(str, "MeshNormals") == 0)
		{
			//{����
			fgets(str, sizeof(str), fp);

			//�@����
			fgets(str, sizeof(str), fp);
			normNum = atoi(str);

			//�@���������m��
			pvNormal = new D3DXVECTOR3[normNum];
			
			//�@���ǂݍ���
			for (int i = 0; i < normNum; i++)
			{
				fscanf_s(fp, "%f;%f;%f;,", &x, &y, &z);
				pvNormal[i].x = x;
				pvNormal[i].y = y;
				pvNormal[i].z = z;
			}
		}

		//�e�N�X�`���[���W �ǂݍ���
		if (strcmp(str, "MeshTextureCoords") == 0)
		{
			//{����
			fgets(str, sizeof(str), fp);

			//UV�̐�
			fgets(str, sizeof(str), fp);
			uvNum = atoi(str);

			//UV�������m��
			pvTex = new D3DXVECTOR2[uvNum];

			//UV�ǂݍ���
			for (int i = 0; i < uvNum; i++)
			{
				fscanf_s(fp, "%f;%f;,", &x, &y);
				pvTex[i].x = x;
				pvTex[i].y = y;
			}
		}

		//�}�e���A�����X�g�ǂݍ���
		if (strcmp(str, "MeshMaterialList") == 0)
		{
			//{����
			fgets(str, sizeof(str), fp);

			//�}�e���A���̐�
			fgets(str, sizeof(str), fp);
			mateNum = atoi(str);

			//�}�e���A���������m��
			pMaterial = new MATERIAL[mateNum];

			//�ʂ̐�
			fgets(str, sizeof(str), fp);
			faceNum = atoi(str);

			//�ʂ̑Ή��}�e���A���������m��
			pFaceOfMate = new int[faceNum];
			
			//�ʂɉ��Ԃ̃}�e���A�����g����
			for (int i = 0; i < faceNum; i++)
			{
				//�ǂݍ���
				fscanf_s(fp, "%d", &pFaceOfMate[i]);

				//,�܂���;�̏���
				fgets(str, sizeof(str), fp);
			}
		}
	}

	//�ǂݍ��݂̊J�n�ʒu�ɖ߂�
	fseek(fp, lStartPos, SEEK_SET);

	while (!feof(fp))
	{
		//�L�[���[�h �ǂݍ���
		fscanf_s(fp, "%s ", str, sizeof(str));

		//�}�e���A���ǂݍ���
		for (int i = 0; i < mateNum; i++)
		{
			if (strcmp(str, "Material") == 0)
			{
				//{����
				fgets(str, sizeof(str), fp);

				//�ʂ̐F
				fscanf_s(fp, "%f;%f;%f;%f;;", &x, &y, &z, &w);
				pMaterial[i].m_vFaceColor.x = x;
				pMaterial[i].m_vFaceColor.y = y;
				pMaterial[i].m_vFaceColor.z = z;
				pMaterial[i].m_vFaceColor.w = w;

				//�X�y�L�����[�̃p���[
				fscanf_s(fp, "%f;", &pMaterial[i].m_fPower);

				//�X�y�L�����[
				fscanf_s(fp, "%f;%f;%f;;", &x, &y, &z);
				pMaterial[i].m_vKs.x = x;
				pMaterial[i].m_vKs.y = y;
				pMaterial[i].m_vKs.z = z;

				//�G�~�b�V�u
				fscanf_s(fp, "%f;%f;%f;;", &x, &y, &z);
				pMaterial[i].m_vKe.x = x;
				pMaterial[i].m_vKe.y = y;
				pMaterial[i].m_vKe.z = z;

				//�e�N�X�`��
				fscanf_s(fp, "%s ", str, sizeof(str));
				if (strcmp(str, "TextureFilename") == 0)
				{
					fgets(str, sizeof(str), fp);//{����

					//�ǂݍ��ݍő�T�C�Y
					int size = sizeof(pMaterial[i].m_TexName);

					//�e�N�X�`����
					fscanf_s(fp, "%s", pMaterial[i].m_TexName, size);

					//"��;����������
					ErasCharFromString(pMaterial->m_TexName,size, '\"');
					ErasCharFromString(pMaterial->m_TexName,size, ';');

					//�e�N�X�`���[���쐬
					if (FAILED(D3DX10CreateShaderResourceViewFromFileA(DX->GetDevice(), pMaterial[i].m_TexName, NULL, NULL, &pMaterial[i].m_pTexture, NULL)))
					{
						//�e�N�X�`���쐬���s
						return false;
					}
				}
			}
		}
	}

	//���_�\���̃������m��
	pVertex = new VERTEX[verNum];

	//���_�\���̂ɂ܂Ƃ߂�
	for (int i = 0; i < verNum; i++)
	{
		pVertex[i].m_vPos = pvPos[i];
		pVertex[i].m_vNorm = pvNormal[i];
		pVertex[i].m_vTex = pvTex[i];
	}

	//�o�[�e�b�N�X�o�b�t�@�[���쐬
	pMesh->m_pVertexBuffer = DRAW->BufferCreate(pVertex, sizeof(VERTEX) * verNum, D3D10_BIND_VERTEX_BUFFER);

	//�}�e���A�����ɃC���f�b�N�X�o�b�t�@���쐬
	for (int i = 0; i < mateNum;i++)
	{
		//���̃}�e���A�����g�p����ʂ̐��𐔂���
		int UseFaceNum=0;
		for (int j = 0; j < faceNum; j++)
		{
			if (pFaceOfMate[j] == i)
			{
				UseFaceNum++;
			}
		}

		//�ʂ̐�
		pMaterial[i].m_FaceNum = UseFaceNum;

		//�ʂ̐������������m��
		pMaterial[i].m_ppIndexBuffer = new ID3D10Buffer * [UseFaceNum];
		pMaterial[i].m_pVerNum = new int[UseFaceNum];

		//�ʂ��ƂɃC���f�b�N�X�o�b�t�@���쐬
		for (int j = 0; j < faceNum; j++)
		{
			//���̃}�e���A�����g�p����ʂ̂�
			if (pFaceOfMate[j] == i)
			{
				//�C���f�b�N�X�o�b�t�@�쐬
				pMaterial[i].m_ppIndexBuffer[j] = DRAW->BufferCreate(pFace[j].m_pIndex, sizeof(int) * pFace[j].m_FaceOfVer, D3D10_BIND_INDEX_BUFFER);

				//���_����ۑ�
				pMaterial[i].m_pVerNum[j] = pFace[j].m_FaceOfVer;
			}
		}
	}
		
	//�ꎞ�ۑ����烁�b�V���|�C���^�փf�[�^���ڂ�
	pMesh->m_MaterialNum = mateNum;
	pMesh->m_pMaterial = pMaterial;
	pMesh->m_FaceNum = faceNum;
	pMesh->m_pFace = pFace;

	//�ꎞ�ۑ��͔j��
	PointerRelease(pvPos);
	PointerRelease(pvNormal);
	PointerRelease(pvTex);
	PointerRelease(pVertex);
	PointerRelease(pFaceOfMate);

	return true;
}

//�w�蕶���𕶎��񂩂����
void CX_Skin::ErasCharFromString(char* pSource,int Size, char Erace)
{
	int count = 0;//�����J�E���g

	for (int i = 0; i < Size; i++)
	{
		if (pSource[i] == Erace)
		{
			//���������J�E���g
			count++;
		}
		else
		{
			//�J�E���g�����l�߂ăR�s�[
			pSource[i - count] = pSource[i];
		}
	}
}

//�{�[���ǂݍ���
bool CX_Skin::LoadBone(FILE* fp, SKIN_MESH* pSkinMesh,long lStartPos)
{
	//�t�@�C���̐擪�ɃZ�b�g
	fseek(fp, lStartPos, SEEK_SET);

	int boneNum=0;	//�{�[���̐�
		
	//�L�[���[�h�ǂݍ��ݗp
	char str[READ_ARRAY_SIZE];

	//�{�[���̐��𐔂���
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));
	
		if (strcmp(str, "Frame") == 0)
		{
			boneNum++;
		}
	}
	//�{�[�����ۑ�
	pSkinMesh->m_BoneNum = boneNum;

	if (boneNum == 0)
	{
		//�{�[���Ȃ�
		return false;
	}
	
	//�t�@�C���̐擪�ɃZ�b�g
	fseek(fp, 0, SEEK_SET);

	int start_count = 0;//{�𐔂���
	int end_count	= 0;//}�𐔂���
		
	//�{�[�����X�g�������m��
	pSkinMesh->m_pBone = new BONE[boneNum];

	int boneIndex = 0;//�C���f�b�N�X�J�E���^�[

	//�{�[���ǂݍ���
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//�{�[��
		if (strcmp(str, "Frame") == 0)
		{
			//�{�[�������X�g�ɕۑ�
			BONE bone= LoadBoneInfo(fp, &boneIndex,pSkinMesh);
			pSkinMesh->m_pBone[bone.m_index]=bone; 
		}
	}

	return true;
}

//�{�[�����̓ǂݍ���
BONE CX_Skin::LoadBoneInfo(FILE* fp, int* pBoneIndex,SKIN_MESH* pSkinMesh)
{
	//�֐��Ăяo�����̃t�@�C���̈ʒu��ۑ�
	long ReadStartPos = ftell(fp);

	int start_count = 0;//{�J�E���g
	int end_count	= 0;//}�J�E���g
	int childNum	= 0;//�q�{�[���̐�

	//�ǂݍ��ݗp
	char str[READ_ARRAY_SIZE];

	//�{�[���ǂݍ��ݗp
	BONE bone;

	//���g�̃C���f�b�N�X
	bone.m_index = *(pBoneIndex);
	
	//��Ɏq�{�[���̐��𐔂���
	while (start_count != end_count || start_count == 0 || end_count == 0)
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//{�J�E���g
		if (strcmp(str, "{") == 0)
		{
			start_count++;
		}

		//}�J�E���g
		if (strcmp(str, "}") == 0)
		{
			end_count++;
		}
		//�q�{�[��
		if (strcmp(str, "Frame") == 0)
		{
			//���̃{�[���̒����̎q
			if (start_count - end_count == 1)
			{
				childNum++;
			}
			else
			{
				;//���{�[���Ȃǂ̓J�E���g���Ȃ�
			}
		}
	}

	//���̊֐����Ă΂ꂽ�Ǝ��̈ʒu�ɖ߂�
	fseek(fp, ReadStartPos, SEEK_SET);

	//�q�̐���ۑ�
	bone.m_ChildNum = childNum;

	//�q�{�[���̃C���f�b�N�X���X�g�������m��
	bone.m_pChildIndex = new int[childNum];

	//�J�E���^��������
	start_count = 0;
	end_count = 0;
	childNum = 0;

	//�{�[����
	fscanf_s(fp, "%s", bone.m_Name, sizeof(bone.m_Name));

	//�{�ǂݍ���
	while (start_count != end_count || start_count == 0 || end_count == 0)
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//{�J�E���g
		if (strcmp(str, "{") == 0)
		{
			start_count++;
		}

		//}�J�E���g
		if (strcmp(str, "}") == 0)
		{
			end_count++;
		}

		//�����|�[�Y
		if (strcmp(str, "FrameTransformMatrix") == 0)
		{
			//{����
			fgets(str, sizeof(str), fp);

			//�s��ǂݍ���
			D3DXMATRIX mat;
			fgets(str, sizeof(str), fp);
			sscanf_s(str, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f;;",
				&mat._11, &mat._12, &mat._13, &mat._14,
				&mat._21, &mat._22, &mat._23, &mat._24,
				&mat._31, &mat._32, &mat._33, &mat._34,
				&mat._41, &mat._42, &mat._43, &mat._44);

			//�ۑ�
			bone.m_matBindPose = mat;

			//}����
			fgets(str, sizeof(str), fp);
		}

		//�q�{�[��
		if (strcmp(str, "Frame") == 0)
		{
			//�{�[���̃C���f�b�N�X�X�V
			*pBoneIndex = *(pBoneIndex)+1;

			//�q�{�[���̃C���f�b�N�X��ۑ�
			bone.m_pChildIndex[childNum++] = *pBoneIndex;

			//�{�[�������X�g�ɕۑ�
			BONE read = LoadBoneInfo(fp, pBoneIndex, pSkinMesh);
			pSkinMesh->m_pBone[read.m_index] = read;
		}
	}

	return bone;
}

//�X�L�����̓ǂݍ���
bool CX_Skin::LoadSkin(FILE* fp, SKIN_MESH* pSkinMesh,long lStartPos)
{
	//�ǂݍ��݊J�n�ʒu�ɃZ�b�g����
	fseek(fp, lStartPos, SEEK_SET);

	//�X�L���E�F�C�g�̐�
	int SkinWeightNum = 0;

	//�L�[���[�h�ǂݍ��ݗp
	char str[READ_ARRAY_SIZE];

	//�X�L���E�F�C�g�̐����J�E���g����
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		if (strcmp(str, "SkinWeights") == 0)
		{
			SkinWeightNum++;
		}
	}

	if (SkinWeightNum == 0)
	{
		//�X�L���E�F�C�g�Ȃ�
		return false;
	}

	//�ǂݍ��݊J�n�ʒu�܂Ŗ߂�
	fseek(fp, lStartPos, SEEK_SET);

	//�E�F�C�g��
	pSkinMesh->m_WeightNum = SkinWeightNum;

	//�X�L���E�F�C�g�������m��
	pSkinMesh->m_pSkinWeight = new SKIN_WEIGHT[SkinWeightNum];
		
	//�ǂݍ��ݗp
	char boneName[NAME_ARRAY_SIZE];	//�{�[����
	int count		= 0;			//�J�E���^�[		
	int weightNum	= 0;			//�E�F�C�g��
	D3DXMATRIX mat;					//�s��

	//�ǂݍ���
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//�X�L���E�F�C�g
		if (strcmp(str, "SkinWeights") == 0)
		{
			fgets(str, sizeof(str), fp);//{����
			
			//�{�[����
			fscanf_s(fp, "%s", boneName,sizeof(boneName));

			//"��;����������
			ErasCharFromString(boneName, sizeof(boneName), '\"');
			ErasCharFromString(boneName, sizeof(boneName), ';');

			//�ۑ�
			strcpy_s(pSkinMesh->m_pSkinWeight[count].m_BoneName, boneName);
						
			//�E�F�C�g�̐�
			fscanf_s(fp, "%d;", &weightNum);		
			pSkinMesh->m_pSkinWeight[count].m_WeightNum = weightNum;

			//�C���f�b�N�X�ƃE�F�C�g�̃������m��
			pSkinMesh->m_pSkinWeight[count].m_pIndex = new int[weightNum];
			pSkinMesh->m_pSkinWeight[count].m_pWeight = new float[weightNum];

			//�C���f�b�N�X�ǂݍ���
			for (int i = 0; i < weightNum; i++)
			{
				fscanf_s(fp, "%d", &pSkinMesh->m_pSkinWeight[count].m_pIndex[i]);

				//,�܂���;�̏���
				fgets(str, sizeof(str), fp);
			}

			//�E�F�C�g�ǂݍ���
			for (int i = 0; i < weightNum; i++)
			{
				fscanf_s(fp, "%f" ,&pSkinMesh->m_pSkinWeight[count].m_pWeight[i]);

				//,�܂���;�̏���
				fgets(str, sizeof(str), fp);
			}
			//�I�t�Z�b�g�s��
			fscanf_s(fp,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f;;",
				&mat._11, &mat._12, &mat._13, &mat._14,
				&mat._21, &mat._22, &mat._23, &mat._24,
				&mat._31, &mat._32, &mat._33, &mat._34,
				&mat._41, &mat._42, &mat._43, &mat._44);
			pSkinMesh->m_pSkinWeight[count].m_matOffset = mat;

			//�J�E���^�[�X�V
			count++;
		}
	}
	return true;
}

//�A�j���[�V�����ǂݍ���
bool CX_Skin::LoadAnimation(FILE* fp, SKIN_MESH* pSkinMesh, long lStartPos)
{
	//�ǂݍ��݊J�n�ʒu�ɃZ�b�g����
	fseek(fp, lStartPos, SEEK_SET);

	//�ǂݍ��ݗp
	char str[READ_ARRAY_SIZE];

	//�A�j���[�V�����̐��𐔂���
	int animeNum = 0;
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		if (strcmp(str, "Animation") == 0)
		{
			animeNum++;
		}
	}

	//�A�j���[�V�����̐��ۑ�
	pSkinMesh->m_AnimeNum = animeNum;

	if (animeNum == 0)
	{
		//�A�j���[�V�����Ȃ�
		return false;
	}

	//�ǂݍ��݊J�n�ʒu�ɖ߂�
	fseek(fp, lStartPos, SEEK_SET);

	//�ǂݍ��ݗp
	char animeName[NAME_ARRAY_SIZE];	//�A�j���[�V������
	char boneName[NAME_ARRAY_SIZE];		//�Ή��{�[����
	int keyNum		= 0;				//�L�[�̐�
	int animeCount	= 0;				//�A�j���[�V�����J�E���^�[
	int keyCount	= 0;				//�L�[�J�E���^�[
	KEY key;							//�L�[
	
	//�A�j���[�V�����������m��
	pSkinMesh->m_pAnimation = new ANIMATION[animeNum];

	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//�A�j���[�V����
		if (strcmp(str, "Animation") == 0)
		{
			//�A�j���[�V������
			fscanf_s(fp, "%s ", animeName, sizeof(animeName));
			strcpy_s(pSkinMesh->m_pAnimation[animeCount].m_Name, animeName);

			//{����
			fgets(str, sizeof(str), fp);
			
			//�Ή��{�[����
			fgets(boneName, sizeof(boneName), fp);

			//{��}�A�󔒂�����
			ErasCharFromString(boneName, sizeof(boneName), '{');
			ErasCharFromString(boneName, sizeof(boneName), '}');
			ErasCharFromString(boneName, sizeof(boneName), ' ');
						
			//�ۑ�
			strcpy_s(pSkinMesh->m_pAnimation[animeCount].m_AffectBoneName,boneName);

			//1�s��΂�
			fgets(str, sizeof(str), fp);//AnimationKey{
			
			//�L�[�^�C�v
			fscanf_s(fp, "%d;", &pSkinMesh->m_pAnimation[animeCount].m_KeyType);

			//�L�[�̐�
			fscanf_s(fp, "%d;", &keyNum);
			pSkinMesh->m_pAnimation[animeCount].m_KeyNum = keyNum;

			//�L�[�̃������m��
			pSkinMesh->m_pAnimation[animeCount].m_pKey = new KEY[keyNum];

			//�L�[�̓ǂݍ���
			for (int i = 0; i < keyNum; i++)
			{
				//�R�}
				fscanf_s(fp, "%d;", &key.m_Time);
				
				//�f�[�^�̐�
				fscanf_s(fp, "%d;", &key.m_ValueNum);
				
				//�f�[�^�̃������m��
				key.m_pValue = new float[key.m_ValueNum];

				//�J�E���^�[������
				keyCount = 0;

				//�ŏ��̈��
				fscanf_s(fp, "%f", &key.m_pValue[keyCount++]);
				
				//2�ڈȍ~��,���܂߂ēǂ�
				while(keyCount<key.m_ValueNum)
				{
					fscanf_s(fp, ",%f", &key.m_pValue[keyCount++]);
				}

				//;;,������
				fgets(str, sizeof(str), fp);

				//�L�[�̕ۑ�
				pSkinMesh->m_pAnimation[animeCount].m_pKey[i] = key;
			}

			//�J�E���^�[�X�V
			animeCount++;
		}
	}
}

//�{�[���ƃX�L�������܂Ƃ߂�
void CX_Skin::BoneWithSkin(SKIN_MESH* pSkinMesh)
{
	//�E�F�C�g��
	int weightNum = pSkinMesh->m_WeightNum;	

	//�{�[����
	int boneNum = pSkinMesh->m_BoneNum;

	//�������m��
	pSkinMesh->m_pSkinBone = new SKIN_BONE[weightNum];

	//�X�L����񂩂�Ή��{�[����T��
	for (int i = 0; i < weightNum; i++)
	{
		//�X�L���������ׂ��{�[����
		char HaveBoneName[NAME_ARRAY_SIZE];
		strcpy_s(HaveBoneName, pSkinMesh->m_pSkinWeight[i].m_BoneName);

		bool bFind = false;
		for (int j = 0; j < boneNum && bFind == false; j++)
		{
			//�Ή��{�[���𔭌�
			if (strcmp(HaveBoneName, pSkinMesh->m_pBone[j].m_Name) == 0)
			{
				bFind=true;
				BONE bone = pSkinMesh->m_pBone[j];								
				SKIN_WEIGHT skin_weight = pSkinMesh->m_pSkinWeight[i];

				//�{�[�����ƃX�L�����̓������s��
				SKIN_BONE skin_bone;
				strcpy_s(skin_bone.m_Name, bone.m_Name);		//�{�[����
				skin_bone.m_index = bone.m_index;				//�{�[�����g�̃C���f�b�N�X
				skin_bone.m_ChildNum = bone.m_ChildNum;			//�q�̐�
				skin_bone.m_pChildIndex = bone.m_pChildIndex;	//���̃C���f�b�N�X���X�g
				skin_bone.m_matBindPose = bone.m_matBindPose;	//�����|�[�Y
				skin_bone.m_WeightNum = skin_weight.m_WeightNum;//�E�F�C�g��
				skin_bone.m_pIndex = skin_weight.m_pIndex;		//�E�F�C�g���X�g
				skin_bone.m_pWeight = skin_weight.m_pWeight;	//�e�����钸�_�̃C���f�b�N�X���X�g
				skin_bone.m_matOffset = skin_weight.m_matOffset;//�I�t�Z�b�g

				//�ۑ�����
				pSkinMesh->m_pSkinBone[i] = skin_bone;
				int a = 0;
			}
		}
	}	
}

//���b�V���`��
void CX_Skin::DrawMesh(D3DMATRIX matWorld, SKIN_MESH* pSkinMesh, CColorData* pColor)
{
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g�j
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	DX->GetDevice()->IASetVertexBuffers(0, 1, &pSkinMesh->m_Mesh.m_pVertexBuffer, &stride, &offset);

	//�}�e���A���̐������A���ꂼ��̃}�e���A���̃C���f�b�N�X�o�b�t�@�|��`��
	for (int i = 0; i < pSkinMesh->m_Mesh.m_MaterialNum; i++)
	{
		//�ʂ��Ƃɕ`��
		for (int j = 0; j < pSkinMesh->m_Mesh.m_pMaterial[i].m_FaceNum; j++)
		{
			//�C���f�b�N�X�o�b�t�@�[���Z�b�g
			stride = sizeof(int);
			offset = 0;
			DX->GetDevice()->IASetIndexBuffer(pSkinMesh->m_Mesh.m_pMaterial[i].m_ppIndexBuffer[j], DXGI_FORMAT_R32_UINT, 0);

			//�v���~�e�B�u�E�g�|���W�[���Z�b�g
			if (pSkinMesh->m_Mesh.m_pMaterial[i].m_pVerNum[j] == TRIANGLE_POLYGON)
			{
				//�O�p�|���S��
				DX->GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			}
			if (pSkinMesh->m_Mesh.m_pMaterial[i].m_pVerNum[j] == QUAD_POLYGON)
			{
				//�l�p�|���S��
				DX->GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			}

			//�V�F�[�_�Z�b�g
			SHADER->SetShader(pSkinMesh->m_Mesh.m_pMaterial[i].m_pTexture, NULL, pColor, matWorld);

			D3D10_TECHNIQUE_DESC dc;
			SHADER->GetTechnique()->GetDesc(&dc);
			for (UINT p = 0; p < dc.Passes; ++p)
			{
				SHADER->GetTechnique()->GetPassByIndex(p)->Apply(0);
				DX->GetDevice()->DrawIndexed(pSkinMesh->m_Mesh.m_pMaterial[i].m_pVerNum[j], 0, 0);
			}
		}
	}
}

//�J��
void CX_Skin::Release()
{
	//�C���X�^���X�j��
	PointerRelease(m_pInstance);
}