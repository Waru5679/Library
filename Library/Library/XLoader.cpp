#include "XLoader.h"
#include "LibraryHeader.h"

//X�t�@�C����ǂݍ��ފ֐�
HRESULT CXLoader::LoadMeshFromX(const char* FileName, MY_MESH* pMesh)
{
	//�f�o�C�X�擾
	ID3D10Device* pDevice = DX->GetDevice();

	float x, y, z;
	int v1 = 0, v2 = 0, v3 = 0;
	int vn1 = 0, vn2 = 0, vn3 = 0;
	int vt1 = 0, vt2 = 0, vt3 = 0;
	int* piMateriaList = NULL;
	int iCount = 0;

	char key[400] = { 0 };
	char str[400] = { 0 };
	
	//X�t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	//�}�e���A���ǂݍ���
	D3DXVECTOR4 vV4;
	D3DXVECTOR3 vV3;

	//�}�e���A�����𒲂ׂ�
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", key, sizeof(key));
		if (strcmp(key, "Material") == 0)
		{
			pMesh->dwNumMaterial++;
		}
	}

	//�}�e���A���̃������[�m��
	pMesh->pMaterial = new MY_MATERIAL[pMesh->dwNumMaterial];

	fseek(fp, SEEK_SET, 0);
	iCount = 0;
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", key, sizeof(key));
		if (strcmp(key, "Material") == 0)
		{
			//����̍s�ɂ���Ɛ���@���s����
			fgets(str, sizeof(str), fp);

			//Kd�@�f�B�t���[�Y
			fscanf_s(fp, "%f;%f;%f;%f;;", &vV4.x, &vV4.y, &vV4.z, &vV4.w);
			pMesh->pMaterial[iCount].Kd = vV4;
			
			//�p���[�i�X�y�L�����[�̃n�C���C�g�j		
			fscanf_s(fp, "%f;", &pMesh->pMaterial[iCount].Kp);
			
			//Ks�@�X�y�L�����[	
			fscanf_s(fp, "%f;%f;%f;;", &vV3.x, &vV3.y, &vV3.z);
			pMesh->pMaterial[iCount].Ks = vV3;
			
			//Ke�@�G�~�b�V�u	
			fscanf_s(fp, "%f;%f;%f;;", &vV3.x, &vV3.y, &vV3.z);
			pMesh->pMaterial[iCount].Ke = vV3;

			//�e�N�X�`���[
			fscanf_s(fp, "%s ", key, sizeof(key));
			if (strcmp(key, "TextureFilename") == 0)
			{
				//����̍s�ɂ���Ɛ���@���s����
				fgets(str, sizeof(str), fp);

				//map_Kd�@�e�N�X�`���[	
				fscanf_s(fp, "%s", pMesh->pMaterial[iCount].szTextureName, sizeof(pMesh->pMaterial[iCount].szTextureName));
				
				//2�d���p��������
				int k = 0;
				while (pMesh->pMaterial[iCount].szTextureName[++k] != '\"') pMesh->pMaterial[iCount].szTextureName[k - 1] = pMesh->pMaterial[iCount].szTextureName[k];
				pMesh->pMaterial[iCount].szTextureName[k - 1] = 0;
				
				//�e�N�X�`���[���쐬
				if (FAILED(D3DX10CreateShaderResourceViewFromFileA(pDevice, pMesh->pMaterial[iCount].szTextureName, NULL, NULL, &pMesh->pMaterial[iCount].pTexture, NULL)))
				{
					return E_FAIL;
				}
			}
			iCount++;
		}
	}
	//�}�e���A���̐������C���f�b�N�X�o�b�t�@�[���쐬
	pMesh->ppIndexBuffer = new ID3D10Buffer * [pMesh->dwNumMaterial];

	//���O�ɒ��_���A�|���S�����𒲂ׂ�
	fseek(fp, SEEK_SET, 0);
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s ", key, sizeof(key));
		
		//���_
		if (strcmp(key, "Mesh") == 0)
		{
			//����̍s�ɂ���Ɛ���@���s����
			fgets(str, sizeof(str), fp);
			fgets(str, sizeof(str), fp);
			pMesh->dwNumVert = atoi(str);

			//���_���������s���āA�t�F�C�X�i���_�C���f�b�N�X�j�̃f�[�^�����܂Ői��
			for (DWORD i = 0; i < pMesh->dwNumVert; i++)
			{
				fgets(str, sizeof(str), fp);//���s
			}
			
			//�t�F�C�X�i�|���S���A���_�C���f�b�N�X�j
			fgets(str, sizeof(str), fp);
			pMesh->dwNumFace = atoi(str);
		}
	}

	//�ꎞ�I�ȃ������m�ہi���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�j
	MY_VERTEX* pvVertexBuffer = new MY_VERTEX[pMesh->dwNumVert];
	int* piFace = new int[pMesh->dwNumFace * 3];//3���_�Ȃ̂�
	D3DXVECTOR3* pvCoord = new D3DXVECTOR3[pMesh->dwNumVert];
	D3DXVECTOR3* pvNormal = new D3DXVECTOR3[pMesh->dwNumVert];
	D3DXVECTOR2* pvTexture = new D3DXVECTOR2[pMesh->dwNumVert];

	//�{�ǂݍ���	
	fseek(fp, SEEK_SET, 0);

	while (!feof(fp))
	{
		//�L�[���[�h �ǂݍ���
		ZeroMemory(key, sizeof(key));
		fscanf_s(fp, "%s ", key, sizeof(key));

		//���_ �ǂݍ���
		if (strcmp(key, "Mesh") == 0)
		{
			fgets(str, sizeof(str), fp);//�f�[�^��2�s���ɂ���Ɛ���@���s����
			fgets(str, sizeof(str), fp);//�f�[�^��2�s���ɂ���Ɛ���@���s����

			for (DWORD i = 0; i < pMesh->dwNumVert; i++)
			{
				fscanf_s(fp, "%f;%f;%f;,", &x, &y, &z);
				pvCoord[i].x = x;
				pvCoord[i].y = y;
				pvCoord[i].z = z;
			}
			//�t�F�C�X�ǂݍ���		
			fgets(str, sizeof(str), fp);//�f�[�^��2�s���ɂ���Ɛ���@���s����
			fgets(str, sizeof(str), fp);//�f�[�^��2�s���ɂ���Ɛ���@���s����
			for (DWORD i = 0; i < pMesh->dwNumFace; i++)
			{
				int dammy;
				fscanf_s(fp, "%d;%d,%d,%d;,", &dammy, &v1, &v2, &v3);
				piFace[i * 3] = v1;
				piFace[i * 3 + 1] = v2;
				piFace[i * 3 + 2] = v3;
			}
		}

		//�@�� �ǂݍ���
		if (strcmp(key, "MeshNormals") == 0)
		{
			fgets(str, sizeof(str), fp);//�f�[�^��2�s���ɂ���Ɛ���@���s����
			fgets(str, sizeof(str), fp);//�f�[�^��2�s���ɂ���Ɛ���@���s����

			for (DWORD i = 0; i < pMesh->dwNumVert; i++)
			{
				fscanf_s(fp, "%f;%f;%f;,", &x, &y, &z);
				pvNormal[i].x = x;
				pvNormal[i].y = y;
				pvNormal[i].z = z;
			}
		}

		//�e�N�X�`���[���W �ǂݍ���
		if (strcmp(key, "MeshTextureCoords") == 0)
		{
			fgets(str, sizeof(str), fp);//�f�[�^��2�s���ɂ���Ɛ���@���s����
			fgets(str, sizeof(str), fp);//�f�[�^��2�s���ɂ���Ɛ���@���s����

			for (DWORD i = 0; i < pMesh->dwNumVert; i++)
			{
				fscanf_s(fp, "%f;%f;,", &x, &y);
				pvTexture[i].x = x;
				pvTexture[i].y = y;
			}
		}
	}

	//�}�e���A�����X�g	
	fseek(fp, SEEK_SET, 0);
	while (!feof(fp))
	{
		ZeroMemory(key, sizeof(key));
		fscanf_s(fp, "%s ", key, sizeof(key));
		if (strcmp(key, "MeshMaterialList") == 0)
		{
			fgets(str, sizeof(str), fp);//����s����
			fgets(str, sizeof(str), fp);//����s����

			fgets(str, sizeof(str), fp);//���X�g�v�f��
			int iNumFace = atoi(str);
			piMateriaList = new int[iNumFace];
			for (int i = 0; i < iNumFace; i++)
			{
				fgets(str, sizeof(str), fp);
				piMateriaList[i] = atoi(str);
			}
		}
	}
	//�t�F�C�X�@�ǂݍ��݁@�}�e���A�����Ƃɕʂ̃C���f�b�N�X�o�b�t�@�[�ɂ���
	bool boFlag = false;
	for (DWORD i = 0; i < pMesh->dwNumMaterial; i++)
	{
		int* piFaceBuffer = new int[pMesh->dwNumFace * 3];//�R���_�|���S���Ȃ̂ŁA1�t�F�C�X=3���_(3�C���f�b�N�X)
		fseek(fp, SEEK_SET, 0);

		//�����}�e���A�������|���S�����܂Ƃ߂�		
		iCount = 0;
		for (DWORD k = 0; k < pMesh->dwNumFace; k++)
		{
			if (i == piMateriaList[k])//����k�Ԗڂ̃|���S���́A����i�ԃ}�e���A���O���[�v
			{
				piFaceBuffer[iCount] = piFace[k * 3];
				piFaceBuffer[iCount + 1] = piFace[k * 3 + 1];
				piFaceBuffer[iCount + 2] = piFace[k * 3 + 2];
				pMesh->pMaterial[i].dwNumFace++;
				iCount += 3;
			}
		}
		if (iCount == 0) continue;

		//�}�e���A���̐������C���f�b�N�X�o�b�t�@�[���쐬
		D3D10_BUFFER_DESC bd;
		bd.Usage = D3D10_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * pMesh->pMaterial[i].dwNumFace * 3;
		bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D10_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = piFaceBuffer;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &pMesh->ppIndexBuffer[i])))
			return FALSE;

		delete[] piFaceBuffer;
	}
	fclose(fp);
	
	//���_�\���̂ɃR�s�[
	for (DWORD i = 0; i < pMesh->dwNumVert; i++)
	{
		pvVertexBuffer[i].vPos = pvCoord[i];
		pvVertexBuffer[i].vNorm = pvNormal[i];
		pvVertexBuffer[i].vTex = pvTexture[i];
	}
	
	//�o�[�e�b�N�X�o�b�t�@�[���쐬
	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MY_VERTEX) * pMesh->dwNumVert;
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pvVertexBuffer;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &pMesh->pVertexBuffer)))
		return FALSE;

	//�ꎞ�I�ȓ��ꕨ�́A���͂�s�v
	delete piMateriaList;
	delete piFace;
	delete pvCoord;
	delete pvNormal;
	delete pvTexture;
	delete[] pvVertexBuffer;

	return S_OK;
}
