#include "ModelLoader.h"
#include "LibraryHeader.h"

//�C���X�^���X
CModelLoader* CModelLoader::m_pInstance = nullptr;

constexpr int ARRAY_SIZE{ 100 };			//�z��T�C�Y

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
	char Key[ARRAY_SIZE];

	//���f����
	fscanf_s(fp, "%s", Key, sizeof(Key));
	fgets(pModel->m_Name, sizeof(pModel->m_Name), fp);
	
	//�e�N�X�`���ǂݍ���
	if (TexLoad(fp, pModel) == false)
	{
		return false;
	}

	//���_�ǂݍ���
	if (VertexLoad(fp, pModel) == false)
	{
		return false;
	}

	//�o�[�e�b�N�X�o�b�t�@�쐬
	pModel->m_pVertexBuffer=DRAW->BufferCreate(pModel->m_pVertex, sizeof(VERTEX) * pModel->m_VerNum, D3D10_BIND_VERTEX_BUFFER);
	
	//�ʓǂݍ���
	if (FaceLoad(fp, pModel) == false)
	{
		return false;
	}

	//�}�e���A���ǂݍ���
	if (MateLoad(fp, pModel) == false)
	{
		return false;
	}

	return true;
}

//�e�N�X�`���ǂݍ���
bool CModelLoader::TexLoad(FILE* fp, MODEL_DATA* pModel)
{
	//�ǂݍ��ݗp
	char Key[ARRAY_SIZE];

	//�e�N�X�`����
	fscanf_s(fp, "%s", Key, sizeof(Key));
	fscanf_s(fp, "%d", &pModel->m_TexNum);

	//�e�N�X�`���Ȃ�
	if (pModel->m_TexNum <= 0)
	{
		return false;
	}

	//�������m��
	pModel->m_ppTexture = new ID3D10ShaderResourceView * [pModel->m_TexNum];

	//�e�N�X�`���ǂݍ���
	for (int i = 0; i < pModel->m_TexNum; i++)
	{
		//�e�N�X�`����
		fscanf_s(fp, "%s", Key, sizeof(Key));

		//�e�N�X�`���[���쐬
		if (FAILED(D3DX10CreateShaderResourceViewFromFileA(DX->GetDevice(), Key, NULL, NULL, &pModel->m_ppTexture[i], NULL)))
		{
			return false;
		}
	}

	return true;
}

//���_�ǂݍ���
bool CModelLoader::VertexLoad(FILE* fp, MODEL_DATA* pModel)
{
	//�ǂݍ��ݗp
	char Key[ARRAY_SIZE];
	float x, y, z, w;
	x = y = z = w = 0.0f;

	//���_��
	fscanf_s(fp, "%s", Key, sizeof(Key));
	fscanf_s(fp, "%d", &pModel->m_VerNum);

	//���_�Ȃ�
	if (pModel->m_VerNum <= 0)
	{
		return false;
	}

	//���_�������m��
	pModel->m_pVertex = new VERTEX[pModel->m_VerNum];

	//���_���ǂݍ���
	for (int i = 0; i < pModel->m_VerNum; i++)
	{
		//Pos
		fscanf_s(fp, "%s", Key, sizeof(Key));
		fscanf_s(fp, "%f,%f,%f,", &x, &y, &z);
		pModel->m_pVertex[i].vPos = D3DXVECTOR3(x, y, z);

		//Norm
		fscanf_s(fp, "%s", Key, sizeof(Key));
		fscanf_s(fp, "%f,%f,%f,", &x, &y, &z);
		pModel->m_pVertex[i].vNorm = D3DXVECTOR3(x, y, z);

		//Uv
		fscanf_s(fp, "%s", Key, sizeof(Key));
		fscanf_s(fp, "%f,%f,", &x, &y);
		pModel->m_pVertex[i].vUv = D3DXVECTOR2(x, y);

		int WeightNum;
		int dammy;
		float fdammy;

		//�E�F�C�g��
		fscanf_s(fp, "%s", Key, sizeof(Key));
		//fscanf_s(fp, "%d", &pModel->m_pVertex[i].WeightNum);
		fscanf_s(fp, "%d", &WeightNum);

		////�{�[��ID�ƃE�F�C�g�̃������m��
		//pModel->m_pVertex[i].m_pBoneId = new int[pModel->m_pVertex[i].WeightNum];
		//pModel->m_pVertex[i].m_pfWeight = new float[pModel->m_pVertex[i].WeightNum];

		//�{�[��ID
		fscanf_s(fp, "%s", Key, sizeof(Key));
		/*for (int j = 0; j < pModel->m_pVertex[i].WeightNum; j++)
		{
			fscanf_s(fp, "%d,", &pModel->m_pVertex[i].m_pBoneId[j]);
		}*/

		for (int j = 0; j < WeightNum; j++)
		{
			fscanf_s(fp, "%d,", &dammy);
		}


		//�E�F�C�g
		/*fscanf_s(fp, "%s", Key, sizeof(Key));
		for (int j = 0; j < pModel->m_pVertex[i].WeightNum; j++)
		{
			fscanf_s(fp, "%f,", &pModel->m_pVertex[i].m_pfWeight[j]);
		}*/

		fscanf_s(fp, "%s", Key, sizeof(Key));
		for (int j = 0; j < WeightNum; j++)
		{
			fscanf_s(fp, "%f,", &fdammy);
		}
	}
	return true;
}

//�ʓǂݍ���
bool CModelLoader::FaceLoad(FILE* fp, MODEL_DATA* pModel)
{
	//�ǂݍ��ݗp
	char Key[ARRAY_SIZE];
	int a, b, c;
	a = b = c = -1;

	//�ʐ�
	fscanf_s(fp, "%s", Key, sizeof(Key));
	fscanf_s(fp, "%d", &pModel->m_FaceNum);

	//�ʂȂ�
	if (pModel->m_FaceNum <= 0)
	{
		return false;
	}

	//�������m��
	pModel->m_pFace = new FACE[pModel->m_FaceNum];

	//�ʓǂݍ���
	for (int i = 0; i < pModel->m_FaceNum; i++)
	{
		fscanf_s(fp, "%d,%d,%d,", &a, &b, &c);
		pModel->m_pFace[i].m_VerId[0] = a;
		pModel->m_pFace[i].m_VerId[1] = b;
		pModel->m_pFace[i].m_VerId[2] = c;

		//�C���f�b�N�X�o�b�t�@�쐬
		pModel->m_pFace[i].m_pIndexBuffer=DRAW->BufferCreate(pModel->m_pFace[i].m_VerId, sizeof(int) * 3, D3D10_BIND_INDEX_BUFFER);
	}
}

//�}�e���A���ǂݍ���
bool CModelLoader::MateLoad(FILE* fp, MODEL_DATA* pModel)
{
	//�ǂݍ��ݗp
	char Key[ARRAY_SIZE];
	float r, g, b, a;
	r = g = b = a = 0.0f;

	//�g�p����ʂ̐�
	int UseFaceNum = 0;

	//�}�e���A����
	fscanf_s(fp, "%s", Key, sizeof(Key));
	fscanf_s(fp, "%d", &pModel->m_MateNum);

	//�}�e���A���Ȃ�
	if (pModel->m_MateNum <= 0)
	{
		return false;
	}

	//�������m��
	pModel->m_pMaterial = new MATERIAL[pModel->m_MateNum];

	int FaceCount = 0;

	//�}�e���A���ǂݍ���
	for (int i = 0; i < pModel->m_MateNum; i++)
	{
		//�}�e���A����
		fscanf_s(fp, "%s", Key, sizeof(Key));
		fscanf_s(fp, "%s", pModel->m_pMaterial[i].m_Name, sizeof(pModel->m_pMaterial[i].m_Name));

		//�F
		fscanf_s(fp, "%s", Key, sizeof(Key));
		fscanf_s(fp, "%f,%f,%f,%f,",&r,&g,&b,&a);
		pModel->m_pMaterial[i].m_vColor = D3DXVECTOR4(r, g, b, a);
		
		//�f�B�t���[�Y
		fscanf_s(fp, "%s", Key, sizeof(Key));
		fscanf_s(fp, "%f,%f,%f,%f,", &r, &g, &b, &a);
		pModel->m_pMaterial[i].m_vDiffuse = D3DXVECTOR4(r, g, b, a);

		//�X�y�L�����[
		fscanf_s(fp, "%s", Key, sizeof(Key));
		fscanf_s(fp, "%f,%f,%f,%f,", &r, &g, &b, &a);
		pModel->m_pMaterial[i].m_vSpecular = D3DXVECTOR4(r, g, b, a);

		//�A���r�G���g
		fscanf_s(fp, "%s", Key, sizeof(Key));
		fscanf_s(fp, "%f,%f,%f,%f,", &r, &g, &b, &a);
		pModel->m_pMaterial[i].m_vAmbient = D3DXVECTOR4(r, g, b, a);

		//�X�y�L�����p���[
		fscanf_s(fp, "%s", Key, sizeof(Key));
		fscanf_s(fp, "%f", &pModel->m_pMaterial[i].m_fSpePower);

		//�e�N�X�`��ID
		fscanf_s(fp, "%s", Key, sizeof(Key));
		fscanf_s(fp, "%d", &pModel->m_pMaterial[i].m_TexId);

		//�g�p����ʂ̐�
		fscanf_s(fp, "%s", Key, sizeof(Key));
		fscanf_s(fp, "%d", &UseFaceNum);

		//�ʂɃ}�e���A������n���B
		for (int j = FaceCount; j < FaceCount+UseFaceNum; j++)
		{
			pModel->m_pFace[j].m_UseMateId = i;
		}

		//�g�p�����ʂ̐��X�V
		FaceCount += UseFaceNum;
	}
	return true;
}

//���f���`��
void CModelLoader::Draw(D3DMATRIX matWorld, MODEL_DATA* pModel, CColorData* pColor)
{
	
	//�ʂ��Ƃɕ`��
	for (unsigned int i = 0; i < pModel->m_FaceNum; i++)
	{
		//�g�p����}�e���A��
		MATERIAL UseMaterial = pModel->m_pMaterial[pModel->m_pFace[i].m_UseMateId];

		if (pModel->m_pFace[i].m_UseMateId ==29 )
			int b = 0;

		//�g�p����e�N�X�`��
		ID3D10ShaderResourceView* pTex = pModel->m_ppTexture[UseMaterial.m_TexId];

		//�V�F�[�_�[�̃Z�b�g
		SHADER->SetShader(pTex, NULL, pColor, matWorld);

		DRAW->DrawPolygon(3, pModel->m_pVertexBuffer, pModel->m_pFace[i].m_pIndexBuffer);
	}
}



//���
void CModelLoader::Release()
{
	//�C���X�^���X�j��
	delete m_pInstance;
	m_pInstance = nullptr;
}
