#include "LibraryHeader.h"

//�C���X�^���X
CObjLoader* CObjLoader::m_pInstance = nullptr;

//OBJ�t�@�C���̓ǂݍ���
HRESULT CObjLoader::LoadMesh(const char* FileName, CModelData* pMesh)
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

	float uv_x[4] = { 1.0f,1.0f,0.0f,0.0f };	
	float uv_y[4] = { 0.0f,1.0f,1.0f,0.0f };

	//���_���
	vector<D3DXVECTOR3> Pos;
	vector<D3DXVECTOR3> Nor;
	vector<D3DXVECTOR2> Tex;
	vector<CVertex> Vertex;
	
	int index_count = 0;
	
	pMesh->m_vMax = D3DXVECTOR3(-999.0f, -999.0f, -999.0f);
	pMesh->m_vMin	= D3DXVECTOR3(999.0f, 999.0f, 999.0f);
	
	int iFaceCount = -1;
	
	//�L�[���[�h�ǂݍ���
	fscanf_s(fp, "%s ", key, sizeof(key));

	//OBJ���̓ǂݍ���
	while (!feof(fp))
	{
		//�ꎞ�ۑ��p
		CMaterialData Material;
		Material.m_pTexture = NULL;

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
						CVertex vertex;

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

			CVertex*	pVertex = NULL;	//���_���̃|�C���^
			int*		pIndex	= NULL;	//�C���f�b�N�X���̃|�C���^

			ID3D10Buffer* IndexBuffer	= NULL;	//�C���f�b�N�X�o�b�t�@�̃|�C���^
			ID3D10Buffer* VertexBuffer	= NULL;	//�o�[�e�b�N�X�o�b�t�@�̃|�C���^

			//�ʂ��Ƃɉ�
			for (int i = 0; i <iFaceCount; i++)
			{				
				//���_��`
				pVertex = new CVertex[FaceOfVer[i]];
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
					CVertex tmp = pVertex[2];
					pVertex[0] = pVertex[0];
					pVertex[1] = pVertex[1];
					pVertex[2] = pVertex[3];
					pVertex[3] = tmp;
				}
				
				//�ʂ̏���ۑ�
				CFaceData info;
				for (int VerNum = 0; VerNum < FaceOfVer[i]; VerNum++)
				{
					info.m_Vertex.push_back(pVertex[VerNum]);
				}
				Material.m_Face.push_back(info);

				//�o�[�e�b�N�X�o�b�t�@�[���쐬
				VertexBuffer = DRAW->BufferCreate(pVertex,sizeof(CVertex)*FaceOfVer[i], D3D10_BIND_VERTEX_BUFFER);

				//�o�[�e�b�N�X�o�b�t�@���o�^
				Material.m_pVertex.insert(Material.m_pVertex.begin() + i, VertexBuffer);

				//index���
				pIndex = new int[FaceOfVer[i]];
				for (int k = 0; k < FaceOfVer[i]; k++)
				{
					pIndex[k] = k;
				}

				//�C���f�b�N�X�o�b�t�@�[���쐬
				IndexBuffer = DRAW->BufferCreate(pIndex, sizeof(int)*FaceOfVer[i],D3D10_BIND_INDEX_BUFFER);
		
				//�C���f�b�N�X�o�b�t�@�o�^
				Material.m_pIndex.insert(Material.m_pIndex.begin() + i, IndexBuffer);

				//�g�������������炷
				index_count += FaceOfVer[i];
				
				//���
				delete[] pIndex;
				delete[] pVertex;
			}

			//�}�e���A������o�^
			pMesh->m_Material.push_back(Material);
			
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
	fseek(fp,0,SEEK_SET);

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

	for (unsigned int i = 0; i < Vertex.size(); i++)
	{
		Vertex[i].Release();
	}
	VectorRelease(Vertex);
		
	//�t�@�C���I���
	fclose(fp);

	return S_OK;
}

//�}�e���A���t�@�C���̓ǂݍ���
HRESULT CObjLoader::LoadMaterial(char* FileName, CModelData* pMesh)
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
	char key[ARRAY_SIZE];
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
					pMesh->m_Material[mate_count].Ka = v;
				}
				//Kd�@�f�B�t���[�Y
				if (strcmp(key, "Kd") == 0)
				{
					fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
					pMesh->m_Material[mate_count].Kd = v;
				}
				//Ks�@�X�y�L�����[
				if (strcmp(key, "Ks") == 0)
				{
					fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
					pMesh->m_Material[mate_count].Ks = v;
				}
				//map_Kd�@�e�N�X�`���[
				if (strcmp(key, "map_Kd") == 0)
				{
					char texName[ARRAY_SIZE];
					fscanf_s(fp, "%s", texName, sizeof(texName));
					//�e�N�X�`���[���쐬
					if (FAILED(D3DX10CreateShaderResourceViewFromFileA(DX->GetDevice(), texName, NULL, NULL, &pMesh->m_Material[mate_count].m_pTexture, NULL)))
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

//���f���`��
void CObjLoader::Draw(D3DMATRIX matWorld, CModelData* pMesh, CColorData* pColor)
{
	//�}�e���A���̐����ɕ`��
	for (unsigned int i = 0; i < pMesh->m_Material.size(); i++)
	{
		int size = pMesh->m_Material[i].m_pVertex.size();

		//�V�F�[�_�[�̃Z�b�g
		SHADER->SetShader(pMesh->m_Material[i].m_pTexture, NULL, pColor, matWorld);

		for (int j = 0; j < size; j++)
		{
			//�|���S���`��
			DRAW->DrawPolygon(pMesh->m_Material[i].m_Face[j].m_Vertex.size(), pMesh->m_Material[i].m_pVertex[j], pMesh->m_Material[i].m_pIndex[j]);
		}
	}
}

//�ő�ƍŏ��̃`�F�b�N
void CObjLoader::MinAndMax(D3DXVECTOR3 Pos, CModelData* pMesh)
{
	//�ő�l
	if (Pos.x > (pMesh->m_vMax.x))
		pMesh->m_vMax.x = Pos.x;

	if (Pos.y > (pMesh->m_vMax.y))
		pMesh->m_vMax.y = Pos.y;

	if (Pos.z > (pMesh->m_vMax.z))
		pMesh->m_vMax.z = Pos.z;

	//�ŏ��l
	if (Pos.x < (pMesh->m_vMin.x))
		pMesh->m_vMin.x = Pos.x;

	if (Pos.y < (pMesh->m_vMin.y))
		pMesh->m_vMin.y = Pos.y;

	if (Pos.z < (pMesh->m_vMin.z))
		pMesh->m_vMin.z = Pos.z;
}

//���
void CObjLoader::Release()
{
	//�C���X�^���X�j��
	PointerRelease(m_pInstance);
}