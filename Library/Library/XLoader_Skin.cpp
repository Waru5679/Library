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

	int verNum;	//���_��
	int faceNum;//�ʂ̐�
	int normNum;//�@����(���_�̐��Ɠ����H)
	int uvNum;	//UV��

	FACE* pFace = nullptr;	//�ʂ̃��X�g

	D3DXVECTOR3* pvPos		= nullptr;//���_���W
	D3DXVECTOR3* pvNormal	= nullptr;//�@��
	D3DXVECTOR2* pvTex		= nullptr;//�e�N�X�`�����W

	//�ǂݍ��ݗp
	float x, y, z;

	int v1, v2, v3, v4;
	v1 = v2 = v3 = v4 = 0;

	int dataNum;
		
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
				fscanf_s(fp, "%d;", &dataNum);
				pFace->m_FaceOfVer = dataNum;

				//�ʍ\���̃C���f�b�N�X���X�g�̃������m��
				pFace->m_pIndex = new int[dataNum];

				//�O�p�|���S��
				if (dataNum == 3)
				{
					fscanf_s(fp, "%d,%d,%d;,", &v1, &v2, &v3);
					pFace->m_pIndex[0] = v1;
					pFace->m_pIndex[1] = v2;
					pFace->m_pIndex[2] = v3;
				}
				//�l�p�|���S��
				if (dataNum == 4)
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
		SHADER->SetShader(pMesh->m_pMaterial[i].pTexture, NULL, pColor, matWorld);

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