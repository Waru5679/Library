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

	MESH	Mesh;				//���b�V��
	int		BoneNum			= 0;//�{�[����
	int		SkinWeightNum	= 0;//�X�L���E�F�C�g�̐�
	int		AnimeNum		= 0;//�A�j���[�V�����̐�

	BONE*			pBone		= nullptr;	//�{�[�����X�g
	SKIN_WEIGHT*	pSkinWeight	= nullptr;	//�X�L���E�F�C�g���X�g
	ANIMATION*		pAnime		= nullptr;	//�A�j���V�������X�g

	SKIN_MESH_HEADER SkinHeader;	//�X�L�����b�V���w�b�_�[

	//���b�V���̓ǂݍ���
	if (LoadMesh(fp, &Mesh,ReadStartPos) == false)
	{
		//���b�V���ǂݍ��ݎ��s
		return false;
	}
	
	//�{�[�����擾
	BoneNum = GetBoneNum(fp, ReadStartPos);

	//�X�L�����b�V���w�b�_�[�ǂݍ���
	LoadSkinMeshHeader(fp, &SkinHeader, ReadStartPos);

	//�{�[��������Ƃ��̂�
	if (BoneNum != 0)
	{
		//�{�[�����X�g�������m��
		pBone = new BONE[BoneNum];

		//�{�[���ǂݍ���
		if (LoadBone(fp, pBone, ReadStartPos) == false)
		{
			//�{�[���ǂݍ��ݎ��s
			return false;
		}
	}

	//�X�L���E�F�C�g�̐��𐔂���
	SkinWeightNum = GetSkinWeightNum(fp, ReadStartPos);

	//�X�L���E�F�C�g������Ƃ��̂�
	if (SkinWeightNum != 0)
	{
		//�X�L���E�F�C�g�������m��
		pSkinWeight = new SKIN_WEIGHT[SkinWeightNum];
		
		//�X�L�����̓ǂݍ���
		if (LoadSkinWeight(fp, pSkinWeight, ReadStartPos) == false)
		{
			//�X�L�����ǂݍ��ݎ��s
			return false;
		}
	}

	//�A�j���[�V�����̐��擾
	AnimeNum = GetAnimeNum(fp, ReadStartPos);

	//�A�j���[�V����������Ƃ��̂�
	if (AnimeNum != 0)
	{
		//�A�j���[�V�����������m��
		pAnime = new ANIMATION[AnimeNum];

		//�A�j���[�V�����ǂݍ���
		if (LoadAnimation(fp, pAnime, ReadStartPos) == false)
		{
			//�A�j���[�V�����ǂݍ��ݎ��s
			return false;
		}
	}

	//�X�L�����b�V���ɂ܂Ƃ߂�
	SkinMeshPutTogether(Mesh, pBone, BoneNum,pSkinWeight, SkinWeightNum, pAnime,AnimeNum, pSkinMesh,SkinHeader);
	
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
	
	//�ǂݍ��݌セ�̂܂܎g������
	FACE*		pFace		= nullptr;//�ʂ̃��X�g
	MATERIAL*	pMaterial	= nullptr;//�}�e���A���̃��X�g
	VERTEX*		pVertex		= nullptr;//���_���X�g

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
	pMesh->m_pVertex = pVertex;

	//�ꎞ�ۑ��͔j��
	PointerRelease(pvPos);
	PointerRelease(pvNormal);
	PointerRelease(pvTex);
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

//�X�L�����b�V���w�b�_�[�ǂݍ���
void CX_Skin::LoadSkinMeshHeader(FILE* fp, SKIN_MESH_HEADER* pSkinHeader, long lStartPos)
{
	//�t�@�C���̐擪�ɃZ�b�g
	fseek(fp, lStartPos, SEEK_SET);

	//�L�[���[�h�ǂݍ��ݗp
	char str[READ_ARRAY_SIZE];

	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//�X�L�����b�V���w�b�_�[�ǂݍ���
		if (strcmp(str, "XSkinMeshHeader") == 0)
		{
			//{����
			fscanf_s(fp, "%s", str, sizeof(str));

			//���_�̍ő�E�F�C�g��
			fscanf_s(fp, "%d;", &pSkinHeader->m_MaxVertex);
			
			//�ʂ̍ő�E�F�C�g��
			fscanf_s(fp, "%d;", &pSkinHeader->m_MaxFace);
			
			//�{�[����
			fscanf_s(fp, "%d;", &pSkinHeader->m_BoneNum);
			
			//}����
			fscanf_s(fp, "%s", str, sizeof(str));
		}
	}
}


//�{�[�����̎擾
int CX_Skin::GetBoneNum(FILE* fp, long lStartPos)
{
	//�t�@�C���̐擪�ɃZ�b�g
	fseek(fp, lStartPos, SEEK_SET);

	int boneNum = 0;	//�{�[���̐�
	
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

	return boneNum;
}

//�{�[���ǂݍ���
bool CX_Skin::LoadBone(FILE* fp, BONE* pBone, long lStartPos)	
{
	//�t�@�C���̐擪�ɃZ�b�g
	fseek(fp, lStartPos, SEEK_SET);

	int boneNum=0;	//�{�[���̐�
		
	//�L�[���[�h�ǂݍ��ݗp
	char str[READ_ARRAY_SIZE];
	
	int start_count = 0;//{�𐔂���
	int end_count	= 0;//}�𐔂���
		
	int boneIndex = 0;//�C���f�b�N�X�J�E���^�[

	//�{�[���ǂݍ���
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//�{�[��
		if (strcmp(str, "Frame") == 0)
		{
			//�{�[�������X�g�ɕۑ�
			BONE bone= LoadBoneInfo(fp, &boneIndex, pBone);
			pBone[bone.m_index]=bone;
		}
	}

	return true;
}

//�{�[�����̓ǂݍ���
BONE CX_Skin::LoadBoneInfo(FILE* fp, int* pBoneIndex, BONE* pBone)
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
			BONE read = LoadBoneInfo(fp, pBoneIndex, pBone);
			pBone[read.m_index] = read;
		}
	}

	return bone;
}

//�X�L���E�F�C�g�̐����擾
int CX_Skin::GetSkinWeightNum(FILE* fp, long lStartPos)
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

	return SkinWeightNum;
}

//�X�L���E�F�C�g�̓ǂݍ���
bool CX_Skin::LoadSkinWeight(FILE* fp, SKIN_WEIGHT* pSkinWeight, long lStartPos)
{
	//�ǂݍ��݊J�n�ʒu�ɃZ�b�g����
	fseek(fp, lStartPos, SEEK_SET);

	//�L�[���[�h�ǂݍ��ݗp
	char str[READ_ARRAY_SIZE];
		
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
			strcpy_s(pSkinWeight[count].m_BoneName, boneName);
						
			//�E�F�C�g�̐�
			fscanf_s(fp, "%d;", &weightNum);		
			pSkinWeight[count].m_WeightNum = weightNum;

			//�C���f�b�N�X�ƃE�F�C�g�̃������m��
			pSkinWeight[count].m_pIndex = new int[weightNum];
			pSkinWeight[count].m_pWeight = new float[weightNum];

			//�C���f�b�N�X�ǂݍ���
			for (int i = 0; i < weightNum; i++)
			{
				fscanf_s(fp, "%d", &pSkinWeight[count].m_pIndex[i]);

				//,�܂���;�̏���
				fgets(str, sizeof(str), fp);
			}

			//�E�F�C�g�ǂݍ���
			for (int i = 0; i < weightNum; i++)
			{
				fscanf_s(fp, "%f" ,&pSkinWeight[count].m_pWeight[i]);

				//,�܂���;�̏���
				fgets(str, sizeof(str), fp);
			}
			//�I�t�Z�b�g�s��
			fscanf_s(fp,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f;;",
				&mat._11, &mat._12, &mat._13, &mat._14,
				&mat._21, &mat._22, &mat._23, &mat._24,
				&mat._31, &mat._32, &mat._33, &mat._34,
				&mat._41, &mat._42, &mat._43, &mat._44);
			pSkinWeight[count].m_matOffset = mat;

			//�J�E���^�[�X�V
			count++;
		}
	}
	return true;
}

//�A�j���[�V�����̐����擾
int CX_Skin::GetAnimeNum(FILE* fp, long lStartPos)
{
	//�ǂݍ��݊J�n�ʒu�ɃZ�b�g����
	fseek(fp, lStartPos, SEEK_SET);

	//�A�j���[�V�����̐�
	int animeNum = 0;

	//�L�[���[�h�ǂݍ��ݗp
	char str[READ_ARRAY_SIZE];

	//�A�j���[�V�����̐��𐔂���
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		if (strcmp(str, "AnimationSet") == 0)
		{
			animeNum++;
		}
	}
	return animeNum;
}

//�{�[�����̃L�[���̓ǂݍ���
BONE_KEY CX_Skin::LoadBoneKey(FILE* fp)
{
	
	BONE_KEY Out;

	//�L�[���[�h�ǂݍ��ݗp
	char str[READ_ARRAY_SIZE];

	//�Ή��{�[����
	char boneName[NAME_ARRAY_SIZE];	

	//�ǂݍ��ݗp������
	int keyNum = 0;
	int keyCount = 0;
	int valueNum = 0;

	//AnimationX 
	fscanf_s(fp, "%s ", str, sizeof(str));

	//{����
	fgets(str, sizeof(str), fp);

	//�Ή��{�[����
	fgets(boneName, sizeof(boneName), fp);

	//{��}�A�󔒁A\n������
	ErasCharFromString(boneName, sizeof(boneName), '{');
	ErasCharFromString(boneName, sizeof(boneName), '}');
	ErasCharFromString(boneName, sizeof(boneName), ' ');
	ErasCharFromString(boneName, sizeof(boneName), '\n');

	//�ۑ�
	strcpy_s(Out.m_AffectBoneName, boneName);

	//1�s��΂�
	fgets(str, sizeof(str), fp);//AnimationKey{

	//�L�[�^�C�v
	fscanf_s(fp, "%d;", &Out.m_KeyType);

	//�L�[�̐�
	fscanf_s(fp, "%d;", &keyNum);
	Out.m_KeyNum = keyNum;

	//�L�[�̃������m��
	Out.m_pKey = new KEY[keyNum];

	//�L�[�̓ǂݍ���
	for (int i = 0; i < keyNum; i++)
	{
		//�R�}
		fscanf_s(fp, "%d;", &Out.m_pKey[i].m_Time);

		//�f�[�^�̐�
		fscanf_s(fp, "%d;", &valueNum);
		Out.m_pKey[i].m_ValueNum = valueNum;

		//�f�[�^�̃������m��
		Out.m_pKey[i].m_pfValue = new float[valueNum];

		//�J�E���^�[������
		keyCount = 0;

		//�ŏ��̈��
		fscanf_s(fp, "%f", &Out.m_pKey[i].m_pfValue[keyCount++]);

		//2�ڈȍ~��,���܂߂ēǂ�
		while (keyCount < Out.m_pKey[i].m_ValueNum)
		{
			fscanf_s(fp, ",%f",&Out.m_pKey[i].m_pfValue[keyCount++]);
		}

		//;;,������
		fgets(str, sizeof(str), fp);
	}	
	return Out;
}

//�A�j���[�V�����ǂݍ���
bool CX_Skin::LoadAnimation(FILE* fp, ANIMATION* pAnime, long lStartPos)
{
	//�ǂݍ��݊J�n�ʒu�ɃZ�b�g����
	fseek(fp, lStartPos, SEEK_SET);

	//�L�[���[�h�ǂݍ��ݗp
	char str[READ_ARRAY_SIZE];

	//�ǂݍ��ݗp
	char animeName[NAME_ARRAY_SIZE];	//�A�j���[�V������
	char boneName[NAME_ARRAY_SIZE];		//�Ή��{�[����
	int animeCount = 0;					//�A�j���[�V�����J�E���^�[
	
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//�A�j���[�V����
		if (strcmp(str, "AnimationSet") == 0)
		{
			//�A�j���[�V������
			fscanf_s(fp, "%s ", animeName, sizeof(animeName));
			strcpy_s(pAnime[animeCount].m_Name, animeName);

			//�t�@�C���̈ʒu��ۑ�
			long ReadStartPos = ftell(fp);

			int boneNum = 0;
			int start_count = 0;
			int end_count = 0;

			//��ɑΉ��{�[���̐��𐔂���
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

				//�Ή��{�[��
				if (strcmp(str, "Animation") == 0)
				{
					boneNum++;
				}
			}

			//�Ή��{�[�����ۑ�
			pAnime[animeCount].m_BoneKeyNum = boneNum;

			//�������m��
			pAnime[animeCount].m_pBoneKey = new BONE_KEY[boneNum];

			//�A�j���[�V�����ǂݍ��݈ʒu�ɃZ�b�g����
			fseek(fp, ReadStartPos, SEEK_SET);

			//�Ή��{�[�����ǂݍ���
			int boneCount = 0;
			while(!feof(fp)&&(boneCount<boneNum))
			{
				fscanf_s(fp, "%s ", str, sizeof(str));

				if (strcmp(str, "Animation") == 0)
				{
					//�{�[�����̃L�[�ǂݍ���
					pAnime[animeCount].m_pBoneKey[boneCount] = LoadBoneKey(fp);
					boneCount++;
				}
			}
			//�J�E���^�[�X�V
			animeCount++;
		}
	}
	return true;
}

//�X�L�����b�V���ɂ܂Ƃ߂�
void CX_Skin::SkinMeshPutTogether(MESH Mesh, BONE* pBone, int BoneNum, SKIN_WEIGHT* pSkinWeight,int WeightNum ,ANIMATION* pAnimation, int AnimeNum, SKIN_MESH* pSkinMesh,SKIN_MESH_HEADER SkinHeader)
{
	pSkinMesh->m_Mesh		= Mesh;			//���b�V��
	pSkinMesh->m_BoneNum	= BoneNum;		//�{�[����
	pSkinMesh->m_pBone		= pBone;		//�{�[��
	pSkinMesh->m_WeightNum	= WeightNum;	//�E�F�C�g��	
	pSkinMesh->m_pWeight	= pSkinWeight;	//�E�F�C�g���X�g
	pSkinMesh->m_AnimeNum	= AnimeNum;		//�A�j���[�V�����̐�
	pSkinMesh->m_pAnimation	= pAnimation;	//�A�j���[�V����
	pSkinMesh->m_SkinHeader = SkinHeader;	//�X�L���w�b�_�[
}

//�t���[���⊮
KEY CX_Skin::FrameComplement(int NowFrame, BONE_KEY BoneKey)
{
	KEY out;

	int keyNum = BoneKey.m_KeyNum;
	int* pFrameDiff = new int[keyNum];

	bool bKey = false;

	//�t���[���̊Ԋu����ۑ�
	for (int i = 0; i <keyNum; i++)
	{
		pFrameDiff[i] = BoneKey.m_pKey[i].m_Time - NowFrame;

		//���݂̃t���[�����L�[�t���[���̏ꍇ
		if (pFrameDiff[i] == 0)
		{
			bKey = true;
			out = BoneKey.m_pKey[i];
		}
	}

	//�L�[�t���[���ȊO�Ȃ�⊮����
	if(bKey==false)
	{
		KEY before= BoneKey.m_pKey[0];
		KEY after= BoneKey.m_pKey[keyNum-1];
		
		//�O�t���[����T��
		for (int i = 0; i < keyNum; i++)
		{
			//�����}�C�i�X�̏ꍇ�O�t���[��
			if (pFrameDiff[i] < 0)
				before = BoneKey.m_pKey[i];
		}

		//��t���[����T��
		for (int i = keyNum - 1; i >= 0; i--)
		{
			//�����v���X�Ȃ��t���[��
			if (pFrameDiff[i] > 0)
				after = BoneKey.m_pKey[i];
		}

		//�t���[���������߂�
		int FrameDiff = after.m_Time - before.m_Time;

		//�ω��̊��������߂�
		float fPercent=	(float)(NowFrame - before.m_Time) / FrameDiff;

		//�����|�[�Y�̃������m��
		float* pfPoseDiff = new float[before.m_ValueNum];

		//�O��t���[���ł̃|�[�Y��(�O��t���[���̃f�[�^���͓����Ɖ��肷��)
		for (int i = 0; i < before.m_ValueNum; i++)
		{			
			//�|�[�Y�̍��������߂�
			pfPoseDiff[i] = after.m_pfValue[i] - before.m_pfValue[i];
		}

		//�l�̐��ۑ�
		out.m_ValueNum = before.m_ValueNum;

		//�������̊m��
		out.m_pfValue = new float[out.m_ValueNum];

		//���݂̃|�[�Y�����߂�
		for (int i = 0; i < out.m_ValueNum ; i++)
		{
			out.m_pfValue[i] = before.m_pfValue[i] + (pfPoseDiff[i] * fPercent);
		}

		//�|�[�Y�̍����j��
		delete[] pfPoseDiff;
	}

	//���݃t���[���ۑ�
	out.m_Time = NowFrame;

	//�t���[�������j��
	delete[] pFrameDiff;

	return out;
}

//�A�j���[�V����
void CX_Skin::Animation(int AnimeId,int NowFrame,SKIN_MESH* pSkinMesh)
{
	ANIMATION anime = pSkinMesh->m_pAnimation[AnimeId];

	//�Ή��{�[�����ۑ��p
	char affectName[NAME_ARRAY_SIZE];
	
	//���̃t���[���̃|�[�Y
	KEY NowPose;
	D3DXMATRIX matNowPose;
	
	//�Ή��{�[�����ƃ{�[���̃|�[�Y�����߂�
	for (int i = 0; i < anime.m_BoneKeyNum; i++)
	{
		//�Ή��{�[����
		strcpy_s(affectName, anime.m_pBoneKey[i].m_AffectBoneName);

		//�t���[���⊮
		NowPose = FrameComplement(NowFrame, anime.m_pBoneKey[i]);

		//�|�[�Y���s��ɂ���
		matNowPose = D3DXMATRIX(NowPose.m_pfValue);

		//�Ή��{�[����T��
		bool bFind = false;
		int boneID = -1;
		for (int i = 0; i < pSkinMesh->m_BoneNum && bFind == false; i++)
		{
			//�Ή��{�[������
			if (strcmp(pSkinMesh->m_pBone[i].m_Name, affectName) == 0)
			{
				bFind = true;
				boneID = i;
			}
		}

		//�{�[���Ƀ|�[�Y��ۑ�
		pSkinMesh->m_pBone[boneID].m_matNewPose = matNowPose;
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