#include "ModelLoader.h"
#include "LibraryHeader.h"

#include <stdio.h>

//�C���X�^���X
CModelLoader* CModelLoader::m_pInstance = nullptr;

//�ǂݍ���
bool CModelLoader::Load(const char* FileName, MODEL_DATA* pModel)
{
	//�t�@�C���I�[�v��
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	//�t�@�C���ǂݍ��ݎ��s
	if (fp == NULL)
	{
		return false;
	}

	//�ǂݍ��ݗp
	char Key[100];
	float x, y, z, w;
	x = y = z = w = 0.0f;
	int a, b, c, d;
	a = b = c = d = -1;


	//�ǂݍ���
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s ", Key, sizeof(Key));

		//���f����
		if (strcmp(Key, "ModelName") == 0)
		{
			fscanf_s(fp, "%s ", pModel->m_ModelName, sizeof(pModel->m_ModelName));
		}
		//�e�N�X�`��
		if (strcmp(Key, "TexNum") == 0)
		{
			//�e�N�X�`����
			fscanf_s(fp, "%d ", &pModel->m_TexNum);

			//�������m��
			pModel->m_ppTex = new ID3D10ShaderResourceView * [pModel->m_TexNum];

			//�e�N�X�`���ǂݍ���
			for (int i = 0; i < pModel->m_TexNum; i++)
			{
				//�e�N�X�`���p�X
				char TexPass[100];
				fscanf_s(fp, "%s ", TexPass, sizeof(TexPass));

				//�e�N�X�`���[���쐬
				if (FAILED(D3DX10CreateShaderResourceViewFromFileA(DX->GetDevice(), TexPass, NULL, NULL, &pModel->m_ppTex[i], NULL)))
				{
					return false;
				}
			}
		}
		//���_
		if (strcmp(Key, "VerNum") == 0)
		{
			//���_��
			fscanf_s(fp, "%d ", &pModel->m_VerNum);

			//�������m��
			pModel->m_pVertex = new VERTEX[pModel->m_VerNum];

			//���_�f�[�^�ǂݍ���
			for (int i = 0; i < pModel->m_VerNum; i++)
			{
				//�ʒu
				fscanf_s(fp, "%s ", Key, sizeof(Key));
				fscanf_s(fp, "%f,%f,%f,", &x, &y, &z);
				pModel->m_pVertex[i].vPos = D3DXVECTOR3(x, y, z);
				
				//�@��
				fscanf_s(fp, "%s ", Key, sizeof(Key));
				fscanf_s(fp, "%f,%f,%f,", &x, &y, &z);
				pModel->m_pVertex[i].vNorm = D3DXVECTOR3(x, y, z);
					
				//Uv
				fscanf_s(fp, "%s ", Key, sizeof(Key));
				fscanf_s(fp, "%f,%f,", &x, &y);
				pModel->m_pVertex[i].vUv = D3DXVECTOR2(x, y);
					
				//�E�F�C�g��
				fscanf_s(fp, "%s ", Key, sizeof(Key));
				int dammy;
				fscanf_s(fp, "%d", &dammy);

//				fscanf_s(fp, "%d", &pModel->m_pVertex[i].WeightNum);

				//�������m��
	/*			pModel->m_pVertex[i].m_pBoneId = new int[pModel->m_pVertex[i].WeightNum];
				pModel->m_pVertex[i].m_pfWeight = new float[pModel->m_pVertex[i].WeightNum];
	*/				
				//�{�[��ID
				fscanf_s(fp, "%s ", Key, sizeof(Key));
				for (int j = 0; j < dammy; j++)
				{
					int dammy2;
					fscanf_s(fp, "%d,", &dammy2);
				}

				/*for (int j = 0; j < pModel->m_pVertex[i].WeightNum; j++)
				{
					fscanf_s(fp, "%d,", &pModel->m_pVertex[i].m_pBoneId[j]);
				}
					*/
				//�E�F�C�g
				fscanf_s(fp, "%s ", Key, sizeof(Key));
				for (int j = 0; j < dammy; j++)
				{
					float fdammy;
					fscanf_s(fp, "%f,", &fdammy);
				}			
				/*for (int j = 0; j < pModel->m_pVertex[i].WeightNum; j++)
				{
					fscanf_s(fp, "%f,", &pModel->m_pVertex[i].m_pfWeight[j]);
				}*/
			}
			//���_�o�b�t�@�쐬
			pModel->m_pVertexBuffer = DRAW->BufferCreate(pModel->m_pVertex, sizeof(VERTEX) * pModel->m_VerNum, D3D10_BIND_VERTEX_BUFFER);
		}
		//��
		if (strcmp(Key, "FaceNum") == 0)
		{
			//�ʂ̐�
			fscanf_s(fp, "%d", &pModel->m_FaceNum);

			//�������m��
			pModel->m_pFace = new FACE[pModel->m_FaceNum];

			//�ʏ��ǂݍ���
			for (int i = 0; i < pModel->m_FaceNum; i++)
			{
				fscanf_s(fp, "%d,%d,%d,", &a, &b, &c);
				pModel->m_pFace[i].m_VerId[0] = a;
				pModel->m_pFace[i].m_VerId[1] = b;
				pModel->m_pFace[i].m_VerId[2] = c;

				//�C���f�b�N�X�o�b�t�@�[���쐬
				pModel->m_pFace[i].m_pIndexBuffer = DRAW->BufferCreate(pModel->m_pFace[i].m_VerId, sizeof(int) * 3, D3D10_BIND_INDEX_BUFFER);
			}
		}
		//�}�e���A��
		if (strcmp(Key, "MateNum") == 0)
		{
			//�}�e���A���̐�
			fscanf_s(fp, "%d", &pModel->m_MateNum);

			//�ʃJ�E���g
			int FaceCount = 0;

			//�������m��
			pModel->m_pMaterial = new MATERIAL[pModel->m_MateNum];

			//�}�e���A���ǂݍ���
			for (int i = 0; i < pModel->m_MateNum; i++)
			{
				fscanf_s(fp, "%s", Key, sizeof(Key));

				//�}�e���A����
				fscanf_s(fp, "%s", Key, sizeof(Key));
				fscanf_s(fp, "%s", pModel->m_pMaterial[i].m_Name, sizeof(pModel->m_pMaterial[i].m_Name));
				
				//�F
				fscanf_s(fp, "%s", Key, sizeof(Key));
				fscanf_s(fp, "%f,%f,%f,%f,", &x, &y, &z, &w);
				pModel->m_pMaterial[i].m_vColor = D3DXVECTOR4(x, y, z, w);
				
				//�f�B�t���[�Y
				fscanf_s(fp, "%s", Key, sizeof(Key));
				fscanf_s(fp, "%f,%f,%f,%f,", &x, &y, &z, &w);
				pModel->m_pMaterial[i].m_vDiffuse = D3DXVECTOR4(x, y, z, w);

				//�X�y�L����
				fscanf_s(fp, "%s", Key, sizeof(Key));
				fscanf_s(fp, "%f,%f,%f,%f,", &x, &y, &z, &w);
				pModel->m_pMaterial[i].m_vSpecular = D3DXVECTOR4(x, y, z, w);
				
				//�A���r�G���g
				fscanf_s(fp, "%s", Key, sizeof(Key));
				fscanf_s(fp, "%f,%f,%f,%f,", &x, &y, &z, &w);
				pModel->m_pMaterial[i].m_vAmbient = D3DXVECTOR4(x, y, z, w);
	
				//�X�y�L�����p���[
				fscanf_s(fp, "%s", Key, sizeof(Key));
				fscanf_s(fp, "%f", &pModel->m_pMaterial[i].m_fSpePower);

				//�e�N�X�`��ID
				fscanf_s(fp, "%s", Key, sizeof(Key));
				fscanf_s(fp, "%d", &pModel->m_pMaterial[i].m_TexId);
				
				//�g�p����ʐ�
				int UseFaceNum = -1;
				fscanf_s(fp, "%s", Key, sizeof(Key));
				fscanf_s(fp, "%d", &UseFaceNum);

				//�g�p����ʂɃ}�e���A������n��
				for (int j = FaceCount; j < FaceCount + UseFaceNum; j++)
				{
					pModel->m_pFace[j].m_UseMateId = i;
				}
				//�J�E���^�X�V
				FaceCount += UseFaceNum;
			}
		}
	}
	return true;
}


//�`��
void CModelLoader::Draw(D3DMATRIX matWorld, MODEL_DATA* pModel, CColorData* pColor)
{
	for (int i = 0; i < pModel->m_FaceNum; i++)
	{
		//�g�p����}�e���A��
		MATERIAL UseMaterial = pModel->m_pMaterial[pModel->m_pFace[i].m_UseMateId];

		//�g�p����e�N�X�`��
		ID3D10ShaderResourceView* pTex  = pModel->m_ppTex[UseMaterial.m_TexId];

		//�V�F�[�_�[�̃Z�b�g
		SHADER->SetShader(pTex,NULL, pColor, matWorld);

		//�|���S���`��
		DRAW->DrawPolygon(3, pModel->m_pVertexBuffer,pModel->m_pFace[i].m_pIndexBuffer);
	}
}

//���
void CModelLoader::Release()
{
	//�C���X�^���X�j��
	delete m_pInstance;
	m_pInstance = nullptr;
}
