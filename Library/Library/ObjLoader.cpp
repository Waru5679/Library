#include "LibraryHeader.h"

//�C���X�^���X
CObjLoader* CObjLoader::m_pInstance = nullptr;

//���f���ǂݍ���
void CObjLoader::LoadMesh(int Id, const char* Name)
{
	MY_MESH Mesh;
	Mesh.Id = Id;
	
	//�ǂݍ���
	LoadObj(Name, &Mesh);

	//�x�N�^�[�o�^
	m_Mesh.push_back(Mesh);
}

//OBJ�t�@�C���̓ǂݍ���
HRESULT CObjLoader::LoadObj(const char* FileName, MY_MESH* pMesh)
{
	//�t�@�C���I�[�v��
	FILE* fp=NULL;
	fopen_s(&fp, FileName, "rt");

	//�t�@�C���ǂݍ��ݎ��s
	if (fp == NULL)
	{
		return E_FAIL;
	}

	//�L�[���[�h�ǂݍ��ݗp
	constexpr int ARRAY_SIZE{ 100 };
	char key[ARRAY_SIZE];

	float uv_x[4] = { 0.0f,1.0f,1.0f,0.0f };	
	float uv_y[4] = { 0.0f,0.0f,1.0f,1.0f };

	//���_���
	vector<D3DXVECTOR3> Pos;
	vector<D3DXVECTOR3> Nor;
	vector<D3DXVECTOR2> Tex;
	vector<MY_VERTEX> Vertex;
	
	int index_count = 0;
	
	pMesh->vMax = D3DXVECTOR3(-999.0f, -999.0f, -999.0f);
	pMesh->vMin	= D3DXVECTOR3(999.0f, 999.0f, 999.0f);
	
	int iFaceCount = -1;
	
	//�L�[���[�h�ǂݍ���
	fscanf_s(fp, "%s ", key, sizeof(key));

	//OBJ���̓ǂݍ���
	while (!feof(fp))
	{
		//�ꎞ�ۑ��p
		MY_MATERIAL Material;
		Material.pTexture = NULL;

		//�ʂ̒��_���ۑ��p
		vector<int> FaceOfVer;

		//�ǂݍ��ݗp
		float x, y, z;

		//�p�[�c���Ƃɓǂݍ���
		if (strcmp(key, "o") == 0)
		{
			//����o�܂Ń��[�v
			do
			{
				//�L�[���[�h�ǂݍ���
				fscanf_s(fp, "%s ", key, sizeof(key));
					
				//�ʒu
				if (strcmp(key, "v") == 0)
				{
					fscanf_s(fp, "%f %f %f", &x, &y, &z);
					
					//�ʒu���ۑ�
					D3DXVECTOR3 pos(x,y,z);
					Pos.push_back(pos);

					//�ŏ��ƍő�̕ۑ�
					MinAndMax(pos, pMesh);
				}
				//�@��
				if (strcmp(key, "vn") == 0)
				{
					fscanf_s(fp, "%f %f %f", &x, &y, &z);
				
					//�@�����o�^
					D3DXVECTOR3 nor(x, y, z);
					Nor.push_back(nor);
				}
				//UV
				if (strcmp(key, "vt") == 0)
				{
					fscanf_s(fp, "%f %f", &x, &y);

					//Uv���o�^
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
										
					//�󔒍��݂�1�s�ǂݍ���
					char str[ARRAY_SIZE];
					char* pStr = str;
					fgets(str, ARRAY_SIZE, fp);
							
					//�󔒂̐����璸�_�̐��𐔂���
					for (int j = 0; str[j] != '\0'; j++)
					{
						if (str[j] == ' ')
							iFaceNum++;
					}
											
					//���_�̐��͋󔒂��1����
					iFaceNum++;
					
					//�ʂ̒��_�̐���ۑ�
					FaceOfVer.push_back(iFaceNum);
						
					//�ǂݍ��ݗp
					int v, vt, vn;
					v = vt = vn = -1;

					//���_�̐�������
					for (int i = 0; i < iFaceNum; i++)
					{
						//�ǂݍ���
						if (sscanf_s(pStr, "%d/%d/%d", &v, &vt, &vn) != 3)
						{
							//UV��񂪂Ȃ��Ƃ��͓ǂݍ��݂Ȃ���
							if (sscanf_s(pStr, "%d//%d", &v, &vn) != 2)
								sscanf_s(pStr, "%d/%d", &v, &vn);
						}

						//�ꎞ�ۑ�
						MY_VERTEX vertex;

						//���_�\���̂ɑ��
						vertex.vPos = Pos[v - 1];

						//�@����񂪂��鎞
						if (Nor.size() != 0)
						{
							vertex.vNorm = Nor[vn - 1];
						}
						//�@����񂪂Ȃ���
						else
						{
							vertex.vNorm = D3DXVECTOR3(0.0f, 0.0f, 1.0f );
						}

						//UV��񂪂��鎞
						if (vt != -1)
						{
							vertex.vTex = Tex[vt - 1];
						}
						//UV��񂪂Ȃ���
						else
						{
							D3DXVECTOR2 tex;
							tex.x =uv_x[i];
							tex.y =uv_y[i];
							vertex.vTex = tex;
						}

						//���_���ۑ�
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

			MY_VERTEX*	pVertex = NULL;	//���_���̃|�C���^
			int*		pIndex	= NULL;	//�C���f�b�N�X���̃|�C���^

			ID3D10Buffer* IndexBuffer	= NULL;	//�C���f�b�N�X�o�b�t�@�̃|�C���^
			ID3D10Buffer* VertexBuffer	= NULL;	//�o�[�e�b�N�X�o�b�t�@�̃|�C���^

			//�ʂ��Ƃɉ�
			for (int i = 0; i <iFaceCount; i++)
			{				
				//���_��`
				pVertex = new MY_VERTEX[FaceOfVer[i]];
				for (int j = 0; j < FaceOfVer[i]; j++)
				{ 
					pVertex[j]=Vertex[index_count + j];
				}
				
				//�O�p�|���S��
				if (FaceOfVer[i] == 3)
				{
				}

				//�l�p�|���S��
				if (FaceOfVer[i] == 4)
				{
					//N����`���悤�ɏ��Ԃ����ւ���
					MY_VERTEX tmp = pVertex[2];
					pVertex[0] = pVertex[0];
					pVertex[1] = pVertex[1];
					pVertex[2] = pVertex[3];
					pVertex[3] = tmp;
				}
				
				//�ʂ̏���ۑ�
				FACE_INFO info;
				for (int VerNum = 0; VerNum < FaceOfVer[i]; VerNum++)
				{
					info.Vertex.push_back(pVertex[VerNum]);
				}
				Material.FaceInfo.push_back(info);

				//�o�[�e�b�N�X�o�b�t�@�[���쐬
				VertexBuffer = DRAW->BufferCreate(pVertex,sizeof(MY_VERTEX)*FaceOfVer[i], D3D10_BIND_VERTEX_BUFFER);

				//�o�[�e�b�N�X�o�b�t�@���o�^
				Material.pVertex.insert(Material.pVertex.begin() + i, VertexBuffer);

				//index���
				pIndex = new int[FaceOfVer[i]];
				for (int k = 0; k < FaceOfVer[i]; k++)
				{
					pIndex[k] = k;
				}

				//�C���f�b�N�X�o�b�t�@�[���쐬
				IndexBuffer = DRAW->BufferCreate(pIndex, sizeof(int)*FaceOfVer[i],D3D10_BIND_INDEX_BUFFER);
		
				//�C���f�b�N�X�o�b�t�@�o�^
				Material.pIndex.insert(Material.pIndex.begin() + i, IndexBuffer);

				//�g�������������炷
				index_count += FaceOfVer[i];
				
				//���
				delete[] pIndex;
				delete[] pVertex;
			}

			//�}�e���A������o�^
			pMesh->Material.push_back(Material);
			
			iFaceCount = 0;
		}
		else
		{
			//�L�[���[�h�ǂݍ���
			fscanf_s(fp, "%s ", key, sizeof(key));
		}

		//�ʂ̒��_���X�g���
		VectorRelease(FaceOfVer);
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
	
	//���_�����
	VectorRelease(Pos);
	VectorRelease(Nor);
	VectorRelease(Tex);
	VectorRelease(Vertex);
		
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

	//�t�@�C���ǂݍ��ݎ��s
	if (fp == NULL)
	{
		return E_FAIL;
	}

	//�}�e���A���J�E���g
	int mate_count = 0;

	constexpr int ARRAY_SIZE{ 100 };
	char key[ARRAY_SIZE] = { 0 };
	D3DXVECTOR3 v;

	//�L�[���[�h�ǂݍ���
	fscanf_s(fp, "%s ", key, sizeof(key));

	while (!feof(fp))
	{
		//�}�e���A����
		if (strcmp(key, "newmtl") == 0)
		{
			fscanf_s(fp, "%s ", key, sizeof(key));
		
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
					char texName[ARRAY_SIZE];
					fscanf_s(fp, "%s", texName, sizeof(texName));
					//�e�N�X�`���[���쐬
					if (FAILED(D3DX10CreateShaderResourceViewFromFileA(DX->GetDevice(), texName, NULL, NULL, &pMesh->Material[mate_count].pTexture, NULL)))
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
void CObjLoader::Draw(D3DMATRIX matWorld, MY_MESH* pMesh,ColorData* pColor)
{
	//�}�e���A���̐����ɕ`��
	for (unsigned int i = 0; i < pMesh->Material.size(); i++)
	{
		int size = pMesh->Material[i].pVertex.size();

		//�V�F�[�_�[�̃Z�b�g
		SHADER->SetShader(pMesh->Material[i].pTexture, NULL, pColor, matWorld);

		for (int j = 0; j < size; j++)
		{
			//�|���S���`��
			DRAW->DrawPolygon(pMesh->Material[i].FaceInfo[j].Vertex.size(), pMesh->Material[i].pVertex[j], pMesh->Material[i].pIndex[j]);
		}
	}
}

//Mesh�擾
MY_MESH* CObjLoader::GetMesh(int Id)
{
	for (unsigned int i = 0; i < m_Mesh.size(); i++)
	{
		if (m_Mesh[i].Id == Id)
		{
			return &m_Mesh[i];
		}
	}
	return nullptr;
}

//���
void CObjLoader::Release()
{
	VectorRelease(m_Mesh);

	//�C���X�^���X�j��
	PointerRelease(m_pInstance);
}