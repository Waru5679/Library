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

	//���b�V���̓ǂݍ���
	if (LoadMesh(fp, &pSkinMesh->m_Mesh) == false)
	{
		//���b�V���ǂݍ��ݎ��s
		return false;
	}

	//�{�[���ǂݍ���
	if (LoadBone(fp, pSkinMesh) == false)
	{
		//�{�[���ǂݍ��ݎ��s
		return false;
	}
	
	//�X�L�����̓ǂݍ���
	if (LoadSkin(fp, pSkinMesh) == false)
	{
		//�X�L�����ǂݍ��ݎ��s
		return false;
	}

	return true;
}

//���b�V�����̓ǂݍ���
bool CX_Skin::LoadMesh(FILE* fp, MESH* pMesh)
{
	//�t�@�C���̐擪�ɃZ�b�g
	fseek(fp, 0, SEEK_SET);

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
	char str[200];

	while (!feof(fp))
	{
		//�L�[���[�h �ǂݍ���
		fscanf_s(fp, "%s ", str, sizeof(str));

		//���_
		if (strcmp(str, "Mesh") == 0)
		{
			fgets(str, sizeof(str), fp);//���b�V����

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

			fgets(str, sizeof(str), fp);	//;����

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
				if (faceOfVer == 3)
				{
					fscanf_s(fp, "%d,%d,%d;,", &v1, &v2, &v3);
					pFace[i].m_pIndex[0] = v1;
					pFace[i].m_pIndex[1] = v2;
					pFace[i].m_pIndex[2] = v3;
				}
				//�l�p�|���S��
				if (faceOfVer == 4)
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
			fgets(str, sizeof(str), fp);//{����

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
			fgets(str, sizeof(str), fp);//{����

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
			fgets(str, sizeof(str), fp);//{����

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

	//�t�@�C���̐擪�ɖ߂�
	fseek(fp, 0, SEEK_SET);

	while (!feof(fp))
	{
		//�L�[���[�h �ǂݍ���
		fscanf_s(fp, "%s ", str, sizeof(str));

		//�}�e���A���ǂݍ���
		for (int i = 0; i < mateNum; i++)
		{
			if (strcmp(str, "Material") == 0)
			{
				fgets(str, sizeof(str), fp);//{����

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
bool CX_Skin::LoadBone(FILE* fp, SKIN_MESH* pSkinMesh)
{
	//�t�@�C���̐擪�ɃZ�b�g
	fseek(fp, 0, SEEK_SET);

	int boneNum=0;	//�{�[���̐�
		
	//�L�[���[�h�ǂݍ��ݗp
	char str[200];

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

	int start_count = 0;	//{�J�E���g
	int end_count = 0;		//}�J�E���g
	int childNum = 0;		//�q�{�[���̐�

	//�ǂݍ��ݗp
	char str[200];

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
			fgets(str, sizeof(str), fp);//{����

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

			fgets(str, sizeof(str), fp);//}����
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
bool CX_Skin::LoadSkin(FILE* fp, SKIN_MESH* pSkinMesh)
{
	//�t�@�C���̐擪�ɖ߂�
	fseek(fp, 0, SEEK_SET);

	//�X�L���E�F�C�g�̐�
	int SkinWeightNum = 0;

	//�L�[���[�h�ǂݍ��ݗp
	char str[200];

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

	//�t�@�C���̐擪�ɖ߂�
	fseek(fp, 0, SEEK_SET);

	//�X�L���E�F�C�g�������m��
	pSkinMesh->m_pSkinWeight = new SKIN_WEIGHT[SkinWeightNum];

	////�ǂݍ���
	//while (!feof(fp))
	//{
	//	fscanf_s(fp, "%s ", str, sizeof(str));
	//	if (strcmp(str, "SkinWeights") == 0)
	//	{
	//		fgets(str, sizeof(str), fp);//{����
	//	}
	//}

	return true;
}

//���b�V���`��
void CX_Skin::DrawMesh(D3DMATRIX matWorld, SKIN_MESH* pSkinMesh, CColorData* pColor)
{
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g�i�o�[�e�b�N�X�o�b�t�@�[�͈�ł����j
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
			if (pSkinMesh->m_Mesh.m_pMaterial[i].m_pVerNum[j] == 3)
			{
				//�O�p�|���S��
				DX->GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			}
			if (pSkinMesh->m_Mesh.m_pMaterial[i].m_pVerNum[j] == 4)
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