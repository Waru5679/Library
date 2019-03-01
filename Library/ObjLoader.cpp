#include "ObjLoader.h"
#include "DirectX.h"
#include "Shader.h"
#include "Task.h"

CObjLoader g_Loader;

//���f���ǂݍ���
void CObjLoader::LoadMesh(int Id, const char* Name)
{
	MY_MESH Mesh;
	
	LoadObj(Name, &Mesh);
	g_Task.Insert(Mesh, Id);
}

//OBJ�t�@�C���̓ǂݍ���
HRESULT CObjLoader::LoadObj(const char* FileName, MY_MESH* pMesh)
{
	//�t�@�C���I�[�v��
	FILE* fp=NULL;
	fopen_s(&fp, FileName, "rt");

	if (fp == NULL)
	{
		return E_FAIL;
	}

	//Obj�ǂݍ���
	char key[200];

	float x, y, z;
	x = y = z = -1.0f;

	//�}�e���A���J�E���^
	pMesh->MaterialNum= 0;
	int v, vt, vn;
	v = vt = vn = -1;

	float uv_x[4] = { 0.0f,-1.0f,-1.0f,0.0f };	
	float uv_y[4] = { 0.0f,0.0f,1.0f,1.0f };

	//���_���
	vector<D3DXVECTOR3> Pos;
	vector<D3DXVECTOR3> Nor;
	vector<D3DXVECTOR2> Tex;
	
	int index_count = 0;

	pMesh->vMax = D3DXVECTOR3(-999.0f, -999.0f, -999.0f);
	pMesh->vMin	= D3DXVECTOR3(999.0f, 999.0f, 999.0f);
	
	//���_���
	vector<MY_VERTEX> Vertex;

	int iFaceCount = -1;

	//�L�[���[�h�ǂݍ���
	fscanf_s(fp, "%s ", key, sizeof(key));

	//OBJ���̓ǂݍ���
	while (!feof(fp))
	{
		//�ꎞ�ۑ��p
		MY_MATERIAL Material;
		Material.pTexture = NULL;

		//�p�[�c���Ƃɓǂݍ���
		if (strcmp(key, "o") == 0)
		{
			//����o�܂Ń��[�v
			do
			{
				//�L�[���[�h�ǂݍ���
				fscanf_s(fp, "%s ", key, sizeof(key));
			
				//���_
				if (strcmp(key, "v") == 0)
				{
					fscanf_s(fp, "%f %f %f", &x, &y, &z);
					
					D3DXVECTOR3 pos;
					pos.x = -x;//OBJ�͉E��ADirect3D�͍�����W�n�B
					pos.y = y;
					pos.z = z;

					//�ŏ��ƍő�̕ۑ�
					MinAndMax(pos, pMesh);
				
					Pos.push_back(pos);
				}
				//�@��
				if (strcmp(key, "vn") == 0)
				{
			
					fscanf_s(fp, "%f %f %f", &x, &y, &z);
					
					D3DXVECTOR3 nor;
					nor.x = -x;//OBJ�͉E��ADirect3D�͍�����W�n�B
					nor.y = y;
					nor.z = z;
					
					Nor.push_back(nor);
				}
				//UV
				if (strcmp(key, "vt") == 0)
				{
					fscanf_s(fp, "%f %f", &x, &y);

					D3DXVECTOR2 tex;
					tex.x = x;
					tex.y = -y;//OBJ�t�@�C����Y�������t�Ȃ̂ō��킹��
					
					Tex.push_back(tex);
				}
				//�t�F�C�X(�|���S��)
				if (strcmp(key, "f") == 0)
				{

					iFaceCount++;

					//�ʂ��Ƃ̒��_�̐�
					int iFaceNum = 0;

					//�ꎞ�ۑ�
					MY_VERTEX vertex;
					
					//�󔒍��݂�1�s�ǂݍ���
					char str[200];
					char* pStr = str;
					fgets(str, 200, fp);
							
					//�󔒂̐����璸�_�̐��𐔂���
					for (int j = 0; str[j] != '\0'; j++)
					{
						if (str[j] == ' ')
							iFaceNum++;
					}
											
					//���_�̐��͋󔒂��1����
					iFaceNum++;
					
					//�ʂ̒��_�̐���ۑ�
					Material.FaceOfVer.push_back(iFaceNum);
						
					for (int i = 0; i < iFaceNum; i++)
					{
						//�ǂݍ���
						if (sscanf_s(pStr, "%d/%d/%d", &v, &vt, &vn) != 3)
						{
							//UV��񂪂Ȃ��Ƃ��͓ǂݍ��݂Ȃ���
							if (sscanf_s(pStr, "%d//%d", &v, &vn) != 2)
								sscanf_s(pStr, "%d/%d", &v, &vn);
						}

						Material.iFaceBuffer.push_back(v-1);
					
						//���_�\���̂ɑ��
						vertex.vPos = Pos[v - 1];

						//�@����񂪂��鎞�u
						if (Nor.size() != 0)
						{
							vertex.vNorm = Nor[vn - 1];
						}
						else
						{
							vertex.vNorm = D3DXVECTOR3(0.0f, 0.0f, 1.0f );
						}

						if (vt != -1)
						{
							//UV��񂪂���Ƃ��̂�
							vertex.vTex = Tex[vt - 1];
						}
						else
						{
							D3DXVECTOR2 tex;
							tex.x = uv_x[i];
							tex.y = uv_y[i];

							vertex.vTex = tex;
						}

						Vertex.push_back(vertex);
			
						//�󔒂��o��܂Ń|�C���^�����炷
						while (1)
						{
							if (*pStr == ' '||*pStr =='\0')
							{
								pStr++;
								break;
							}
							pStr++;
						}
					}				
				}
			} while (strcmp(key, "o") !=  0&& !feof(fp));

			ID3D10Buffer* index_buffer = NULL;
			ID3D10Buffer* vertex_buffer = NULL;

			//�ʂ��Ƃɉ�
			for (int i = 0; i <iFaceCount; i++)
			{
				int id = 0;
							
				//���_��`
				MY_VERTEX* vartices = new MY_VERTEX[Material.FaceOfVer[i]];
				for (int j = 0; j < Material.FaceOfVer[i]; j++)
				{ 
					vartices[j]=Vertex[index_count + j];
				}
				
				if (Material.FaceOfVer[i] == 4)
				{
				//	4���_�̎��ɏ��Ԃ����ւ���
				//	N����`���悤��
					MY_VERTEX _0 = vartices[3];
					MY_VERTEX _1 = vartices[2];
					MY_VERTEX _2 = vartices[0];
					MY_VERTEX _3 = vartices[1];

					vartices[0] = _0;
					vartices[1] = _1;
					vartices[2] = _2;
					vartices[3] = _3;
				}

				//�o�[�e�b�N�X�o�b�t�@�[���쐬
				D3D10_BUFFER_DESC bd;
				bd.Usage = D3D10_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(MY_VERTEX) * Material.FaceOfVer[i];
				bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
				bd.CPUAccessFlags = 0;
				bd.MiscFlags = 0;
				D3D10_SUBRESOURCE_DATA InitData;
				InitData.pSysMem = vartices;

				if (FAILED(dx.m_pDevice->CreateBuffer(&bd, &InitData, &vertex_buffer)))
					return FALSE;

				Material.pVertex.insert(Material.pVertex.begin() + i, vertex_buffer);

				//index���
				int *index;
				index = new int[Material.FaceOfVer[i]];
				for (int k = 0; k < Material.FaceOfVer[i]; k++)
				{
					index[k] = k;
				}
				//�C���f�b�N�X�o�b�t�@�[���쐬
				bd.Usage = D3D10_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(int) * Material.FaceOfVer[i];
				bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
				bd.CPUAccessFlags = 0;
				bd.MiscFlags = 0;
				InitData.pSysMem = index;
				InitData.SysMemPitch = 0;
				InitData.SysMemSlicePitch = 0;
				if (FAILED(dx.m_pDevice->CreateBuffer(&bd, &InitData, &index_buffer)))
					return FALSE;

				Material.pIndex.insert(Material.pIndex.begin() + i, index_buffer);

				//�g�������������炷
				index_count += Material.FaceOfVer[i];
			}

			//�}�e���A������o�^
			pMesh->Material.push_back(Material);

			pMesh->MaterialNum++;

			iFaceCount = 0;
		}
		else
		{
			//�L�[���[�h�ǂݍ���
			fscanf_s(fp, "%s ", key, sizeof(key));
		}
	}

	//�t�@�C���|�C���^��擪�ɖ߂�
	fseek(fp, SEEK_SET, 0);

	//�}�e���A���t�@�C����ǂݍ���
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s ", key, sizeof(key));

		//�}�e���A���t�@�C���ǂݍ���
		if (strcmp(key, "mtllib") == 0)
		{
			//�L�[���[�h�ǂݍ���
			fscanf_s(fp, "%s ", key, sizeof(key));

			if (FAILED(LoadMaterial(key,pMesh)))
			{
				return E_FAIL;
			}
		}
	}
	
	//�t�@�C���I���
	fclose(fp);

	return S_OK;
}

//�}�e���A���t�@�C���̓ǂݍ���
HRESULT CObjLoader::LoadMaterial(char* FileName,MY_MESH* pMesh)
{
	//�}�e���A���t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	int mate_count = 0;
	char key[100] = { 0 };
	D3DXVECTOR3 v;

	//�L�[���[�h�ǂݍ���
	fscanf_s(fp, "%s ", key, sizeof(key));

	while (!feof(fp))
	{
		//�}�e���A����
		if (strcmp(key, "newmtl") == 0)
		{
			fscanf_s(fp, "%s ", key, sizeof(key));
			strcpy_s(pMesh->Material[mate_count].szName, key);

			//���̃}�e���A���܂œǂݍ���
			do
			{
				fscanf_s(fp, "%s ", key, sizeof(key));

				//Ka�@�A���r�G���g
				if (strcmp(key, "Ka") == 0)
				{
					fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
					pMesh->Material[mate_count].Ka = v;
				}
				//Kd�@�f�B�t���[�Y
				if (strcmp(key, "Kd") == 0)
				{
					fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
					pMesh->Material[mate_count].Kd = v;
				}
				//Ks�@�X�y�L�����[
				if (strcmp(key, "Ks") == 0)
				{
					fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
					pMesh->Material[mate_count].Ks = v;
				}
				//map_Kd�@�e�N�X�`���[
				if (strcmp(key, "map_Kd") == 0)
				{
					char texName[100];
					fscanf_s(fp, "%s", texName, sizeof(texName));
					//�e�N�X�`���[���쐬
					if (FAILED(D3DX10CreateShaderResourceViewFromFileA(dx.m_pDevice, texName, NULL, NULL, &pMesh->Material[mate_count].pTexture, NULL)))
					{
						return E_FAIL;
					}
				}
			} while (strcmp(key, "newmtl") != 0 && !feof(fp));
			
			mate_count++;
		}
		else
		{
			//�L�[���[�h�ǂݍ���
			fscanf_s(fp, "%s ", key, sizeof(key));
		}
	}
	fclose(fp);

	return S_OK;
}


//�ő�ƍŏ��̃`�F�b�N
void CObjLoader::MinAndMax(D3DXVECTOR3 Pos,MY_MESH* pMesh)
{
	//�ő�l
	if (Pos.x > (pMesh->vMax.x))
		pMesh->vMax.x = Pos.x;

	if (Pos.y > (pMesh->vMax.y))
		pMesh->vMax.y = Pos.y;

	if (Pos.z > (pMesh->vMax.z))
		pMesh->vMax.z = Pos.z;

	//�ŏ��l
	if (Pos.x < (pMesh->vMin.x))
		pMesh->vMin.x = Pos.x;

	if (Pos.y < (pMesh->vMin.y))
		pMesh->vMin.y = Pos.y;

	if (Pos.z < (pMesh->vMin.z))
		pMesh->vMin.z = Pos.z;
}

//���b�V���`��
void CObjLoader::Draw(D3DMATRIX matWorld,MY_MESH* pMesh)
{
	//�}�e���A���̐����ɕ`��
	for (int i = 0; i < pMesh->MaterialNum; i++)
	{
		int size = pMesh->Material[i].pVertex.size();

		//�V�F�[�_�[�̃Z�b�g
		g_Shader.SetShader(matWorld, pMesh->Material[i]);

		for (int j = 0; j <size; j++)
		{
			//�|���S���`��
			DrawMesh(pMesh->Material[i].FaceOfVer[j], pMesh->Material[i].pVertex[j], pMesh->Material[i].pIndex[j]);
		}
	}
}

//�|���S���`��
void CObjLoader::DrawMesh(int ver_num,ID3D10Buffer* VertexBuffer,ID3D10Buffer* IndexBuffer)
{
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(MY_VERTEX);
	UINT offset = 0;
	dx.m_pDevice->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	dx.m_pDevice->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	if (ver_num == 3)
	{
		//�O�p�`
		dx.m_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	if (ver_num == 4)
	{
		//�l�p�`
		dx.m_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}

	//�v���~�e�B�u�������_�����O
	D3D10_TECHNIQUE_DESC dc;
	g_Shader.m_pTechnique->GetDesc(&dc);
	for (UINT p = 0; p < dc.Passes; ++p)
	{
		g_Shader.m_pTechnique->GetPassByIndex(p)->Apply(0);
		dx.m_pDevice->DrawIndexed(ver_num, 0, 0);
	}
}
