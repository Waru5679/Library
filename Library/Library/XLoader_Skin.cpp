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
		//�t�@�C���ǂݍ��ݎ��s
		return false;
	}

	int verNum	=0;//���_��
	int faceNum	=0;//�ʂ̐�
	int normNum =0;//�@����(���_�̐��Ɠ����H)
	int uvNum	=0;//UV��
	int mateNum	=0;//�}�e���A���̐�

	FACE*		 pFace		= nullptr;//�ʂ̃��X�g
	int*		 pFaceOfMate= nullptr;//�ʂ̑Ή��}�e���A���̃C���f�b�N�X
	D3DXVECTOR3* pvPos		= nullptr;//���_���W
	D3DXVECTOR3* pvNormal	= nullptr;//�@��
	D3DXVECTOR2* pvTex		= nullptr;//�e�N�X�`�����W
	MATERIAL*	 pMateial	= nullptr;//�}�e���A��
	VERTEX*		 pVertex	= nullptr;//���_���X�g

	//�ǂݍ��ݗp
	float x, y, z, w;

	int v1, v2, v3, v4;
	v1 = v2 = v3 = v4 = 0;

	int faceOfVer=0;//�ʂ��\�����钸�_��
		
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
		
			//�ʓǂݍ���
			for (int i = 0; i < faceNum; i++)
			{
				//�ʂ��\�����钸�_�̐�
				fscanf_s(fp, "%d;", &faceOfVer);
				pFace->m_FaceOfVer = faceOfVer;

				//�ʍ\���̃C���f�b�N�X���X�g�̃������m��
				pFace->m_pIndex = new int[faceOfVer];

				//�O�p�|���S��
				if (faceOfVer == 3)
				{
					fscanf_s(fp, "%d,%d,%d;,", &v1, &v2, &v3);
					pFace->m_pIndex[0] = v1;
					pFace->m_pIndex[1] = v2;
					pFace->m_pIndex[2] = v3;
				}
				//�l�p�|���S��
				if (faceOfVer == 4)
				{
					fscanf_s(fp, "%d,%d,%d,%d;,", &v1, &v2, &v3, &v4);
					pFace->m_pIndex[0] = v1;
					pFace->m_pIndex[1] = v2;
					pFace->m_pIndex[2] = v3;
					pFace->m_pIndex[3] = v4;
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
			pMateial = new MATERIAL[mateNum];

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
		//�}�e���A���ǂݍ���(�}�e���A�����X�g��ǂނ܂ł͒ʂ�Ȃ�)
		for (int i = 0; i < mateNum; i++)
		{
			if (strcmp(str, "Material") == 0)
			{
				fgets(str, sizeof(str), fp);//{����

				//�ʂ̐F
				fscanf_s(fp, "%f;%f;%f;%f;;", &x, &y, &z, &w);
				pMateial[i].m_vFaceColor.x = x;
				pMateial[i].m_vFaceColor.y = y;
				pMateial[i].m_vFaceColor.z = z;
				pMateial[i].m_vFaceColor.w = w;

				//�X�y�L�����[�̃p���[
				fscanf_s(fp, "%f;", &pMateial[i].m_fPower);

				//�X�y�L�����[
				fscanf_s(fp, "%f;%f;%f;;", &x, &y, &z);
				pMateial[i].m_vKs.x = x;
				pMateial[i].m_vKs.y = y;
				pMateial[i].m_vKs.z = z;

				//�G�~�b�V�u
				fscanf_s(fp, "%f;%f;%f;;", &x, &y, &z);
				pMateial[i].m_vKe.x = x;
				pMateial[i].m_vKe.y = y;
				pMateial[i].m_vKe.z = z;

				//�e�N�X�`��
				fscanf_s(fp, "%s ", str, sizeof(str));
				if (strcmp(str, "TextureFilename") == 0)
				{
					fgets(str, sizeof(str), fp);//{����
					
					//�ǂݍ��ݍő�T�C�Y
					int size = sizeof(pMateial[i].m_TexName);
					
					//�e�N�X�`����
					fscanf_s(fp, "%s", pMateial[i].m_TexName,size);
		
					//"��;����������
					int count = 0;//�����J�E���g
					for (int j = 0; j < size; j++)
					{
						if (pMateial[i].m_TexName[j] == '\"' || pMateial[i].m_TexName[j] == ';')
						{
							//���������J�E���g
							count++;
						}
						else
						{
							//�J�E���g�����l�߂ăR�s�[
							pMateial[i].m_TexName[j - count] = pMateial[i].m_TexName[j];
						}
					}

					//�e�N�X�`���[���쐬
					if (FAILED(D3DX10CreateShaderResourceViewFromFileA(DX->GetDevice(), pMateial[i].m_TexName, NULL, NULL, &pMateial[i].m_pTexture, NULL)))
					{
						//�e�N�X�`���쐬���s
						return E_FAIL;
					}
				}
			}
		}

		////���_�\���̃������m��
		//pVertex = new VERTEX[verNum];

		////���_�\���̂ɂ܂Ƃ߂�
		//for (int i = 0; i < verNum; i++)
		//{
		//	pVertex[i].m_vPos	= pvPos[i];
		//	pVertex[i].m_vNorm	= pvNormal[i];
		//	pVertex[i].m_vTex	= pvTex[i];
		//}

		////�o�[�e�b�N�X�o�b�t�@�[���쐬
		//pSkinMesh->m_pVertexBuffer = DRAW->BufferCreate(pVertex, sizeof(VERTEX) * verNum, D3D10_BIND_VERTEX_BUFFER);

		//
		////�}�e���A���̐������C���f�b�N�X�o�b�t�@�쐬
		//for (int i = 0; i < mateNum; i++)
		//{
		//	pFace[i].
		//}
	}

	return true;
}

//���b�V���`��
void CX_Skin::DrawMesh(D3DMATRIX matWorld, SKIN_MESH* pMesh, CColorData* pColor)
{
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g�i�o�[�e�b�N�X�o�b�t�@�[�͈�ł����j
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	DX->GetDevice()->IASetVertexBuffers(0, 1, &pMesh->m_pVertexBuffer, &stride, &offset);

	//�}�e���A���̐������A���ꂼ��̃}�e���A���̃C���f�b�N�X�o�b�t�@�|��`��
	for (int i = 0; i < pMesh->m_MaterialNum; i++)
	{
		//�g�p����Ă��Ȃ��}�e���A���΍�
		if (pMesh->m_pMaterial[i].m_FaceNum == 0)
		{
			continue;
		}
		//�C���f�b�N�X�o�b�t�@�[���Z�b�g
		stride = sizeof(int);
		offset = 0;
		DX->GetDevice()->IASetIndexBuffer(pMesh->m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

		//�v���~�e�B�u�E�g�|���W�[���Z�b�g
		DX->GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�V�F�[�_�Z�b�g
		SHADER->SetShader(pMesh->m_pMaterial[i].m_pTexture, NULL, pColor, matWorld);

		D3D10_TECHNIQUE_DESC dc;
		SHADER->GetTechnique()->GetDesc(&dc);

		for (UINT p = 0; p < dc.Passes; ++p)
		{
			SHADER->GetTechnique()->GetPassByIndex(p)->Apply(0);
			DX->GetDevice()->DrawIndexed(pMesh->m_pMaterial[i].m_FaceNum * 3, 0, 0);
		}
	}
}

//�J��
void CX_Skin::Release()
{
	//�C���X�^���X�j��
	PointerRelease(m_pInstance);
}