#include "ObjLoader.h"
#include "DirectX.h"
#include "Shader.h"
#include "Task.h"

CObjLoader g_Loader;

//モデル読み込み
void CObjLoader::LoadMesh(int Id, const char* Name)
{
	MY_MESH Mesh;
	
	LoadObj(Name, &Mesh);
	g_Task.Insert(Mesh, Id);
}

//OBJファイルの読み込み
HRESULT CObjLoader::LoadObj(const char* FileName, MY_MESH* pMesh)
{
	//ファイルオープン
	FILE* fp=NULL;
	fopen_s(&fp, FileName, "rt");

	if (fp == NULL)
	{
		return E_FAIL;
	}

	//Obj読み込み
	char key[200];

	float x, y, z;
	x = y = z = -1.0f;

	//マテリアルカウンタ
	pMesh->MaterialNum= 0;
	int v, vt, vn;
	v = vt = vn = -1;

	float uv_x[4] = { 0.0f,-1.0f,-1.0f,0.0f };	
	float uv_y[4] = { 0.0f,0.0f,1.0f,1.0f };

	//頂点情報
	vector<D3DXVECTOR3> Pos;
	vector<D3DXVECTOR3> Nor;
	vector<D3DXVECTOR2> Tex;
	
	int index_count = 0;

	pMesh->vMax = D3DXVECTOR3(-999.0f, -999.0f, -999.0f);
	pMesh->vMin	= D3DXVECTOR3(999.0f, 999.0f, 999.0f);
	
	//頂点情報
	vector<MY_VERTEX> Vertex;

	int iFaceCount = -1;

	//キーワード読み込み
	fscanf_s(fp, "%s ", key, sizeof(key));

	//OBJ情報の読み込み
	while (!feof(fp))
	{
		//一時保存用
		MY_MATERIAL Material;
		Material.pTexture = NULL;

		//パーツごとに読み込む
		if (strcmp(key, "o") == 0)
		{
			//次のoまでループ
			do
			{
				//キーワード読み込み
				fscanf_s(fp, "%s ", key, sizeof(key));
			
				//頂点
				if (strcmp(key, "v") == 0)
				{
					fscanf_s(fp, "%f %f %f", &x, &y, &z);
					
					D3DXVECTOR3 pos;
					pos.x = -x;//OBJは右手、Direct3Dは左手座標系。
					pos.y = y;
					pos.z = z;

					//最小と最大の保存
					MinAndMax(pos, pMesh);
				
					Pos.push_back(pos);
				}
				//法線
				if (strcmp(key, "vn") == 0)
				{
			
					fscanf_s(fp, "%f %f %f", &x, &y, &z);
					
					D3DXVECTOR3 nor;
					nor.x = -x;//OBJは右手、Direct3Dは左手座標系。
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
					tex.y = -y;//OBJファイルはY成分が逆なので合わせる
					
					Tex.push_back(tex);
				}
				//フェイス(ポリゴン)
				if (strcmp(key, "f") == 0)
				{

					iFaceCount++;

					//面ごとの頂点の数
					int iFaceNum = 0;

					//一時保存
					MY_VERTEX vertex;
					
					//空白込みで1行読み込む
					char str[200];
					char* pStr = str;
					fgets(str, 200, fp);
							
					//空白の数から頂点の数を数える
					for (int j = 0; str[j] != '\0'; j++)
					{
						if (str[j] == ' ')
							iFaceNum++;
					}
											
					//頂点の数は空白より1つ多い
					iFaceNum++;
					
					//面の頂点の数を保存
					Material.FaceOfVer.push_back(iFaceNum);
						
					for (int i = 0; i < iFaceNum; i++)
					{
						//読み込み
						if (sscanf_s(pStr, "%d/%d/%d", &v, &vt, &vn) != 3)
						{
							//UV情報がないときは読み込みなおす
							if (sscanf_s(pStr, "%d//%d", &v, &vn) != 2)
								sscanf_s(pStr, "%d/%d", &v, &vn);
						}

						Material.iFaceBuffer.push_back(v-1);
					
						//頂点構造体に代入
						vertex.vPos = Pos[v - 1];

						//法線情報がある時「
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
							//UV情報があるときのみ
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
			
						//空白が出るまでポインタをずらす
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

			//面ごとに回す
			for (int i = 0; i <iFaceCount; i++)
			{
				int id = 0;
							
				//頂点定義
				MY_VERTEX* vartices = new MY_VERTEX[Material.FaceOfVer[i]];
				for (int j = 0; j < Material.FaceOfVer[i]; j++)
				{ 
					vartices[j]=Vertex[index_count + j];
				}
				
				if (Material.FaceOfVer[i] == 4)
				{
				//	4頂点の時に順番を入れ替える
				//	N字を描くように
					MY_VERTEX _0 = vartices[3];
					MY_VERTEX _1 = vartices[2];
					MY_VERTEX _2 = vartices[0];
					MY_VERTEX _3 = vartices[1];

					vartices[0] = _0;
					vartices[1] = _1;
					vartices[2] = _2;
					vartices[3] = _3;
				}

				//バーテックスバッファーを作成
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

				//index情報
				int *index;
				index = new int[Material.FaceOfVer[i]];
				for (int k = 0; k < Material.FaceOfVer[i]; k++)
				{
					index[k] = k;
				}
				//インデックスバッファーを作成
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

				//使った数だけずらす
				index_count += Material.FaceOfVer[i];
			}

			//マテリアル情報を登録
			pMesh->Material.push_back(Material);

			pMesh->MaterialNum++;

			iFaceCount = 0;
		}
		else
		{
			//キーワード読み込み
			fscanf_s(fp, "%s ", key, sizeof(key));
		}
	}

	//ファイルポインタを先頭に戻す
	fseek(fp, SEEK_SET, 0);

	//マテリアルファイルを読み込む
	while (!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp, "%s ", key, sizeof(key));

		//マテリアルファイル読み込み
		if (strcmp(key, "mtllib") == 0)
		{
			//キーワード読み込み
			fscanf_s(fp, "%s ", key, sizeof(key));

			if (FAILED(LoadMaterial(key,pMesh)))
			{
				return E_FAIL;
			}
		}
	}
	
	//ファイル終わり
	fclose(fp);

	return S_OK;
}

//マテリアルファイルの読み込み
HRESULT CObjLoader::LoadMaterial(char* FileName,MY_MESH* pMesh)
{
	//マテリアルファイルを開いて内容を読み込む
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	int mate_count = 0;
	char key[100] = { 0 };
	D3DXVECTOR3 v;

	//キーワード読み込み
	fscanf_s(fp, "%s ", key, sizeof(key));

	while (!feof(fp))
	{
		//マテリアル名
		if (strcmp(key, "newmtl") == 0)
		{
			fscanf_s(fp, "%s ", key, sizeof(key));
			strcpy_s(pMesh->Material[mate_count].szName, key);

			//次のマテリアルまで読み込む
			do
			{
				fscanf_s(fp, "%s ", key, sizeof(key));

				//Ka　アンビエント
				if (strcmp(key, "Ka") == 0)
				{
					fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
					pMesh->Material[mate_count].Ka = v;
				}
				//Kd　ディフューズ
				if (strcmp(key, "Kd") == 0)
				{
					fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
					pMesh->Material[mate_count].Kd = v;
				}
				//Ks　スペキュラー
				if (strcmp(key, "Ks") == 0)
				{
					fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
					pMesh->Material[mate_count].Ks = v;
				}
				//map_Kd　テクスチャー
				if (strcmp(key, "map_Kd") == 0)
				{
					char texName[100];
					fscanf_s(fp, "%s", texName, sizeof(texName));
					//テクスチャーを作成
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
			//キーワード読み込み
			fscanf_s(fp, "%s ", key, sizeof(key));
		}
	}
	fclose(fp);

	return S_OK;
}


//最大と最小のチェック
void CObjLoader::MinAndMax(D3DXVECTOR3 Pos,MY_MESH* pMesh)
{
	//最大値
	if (Pos.x > (pMesh->vMax.x))
		pMesh->vMax.x = Pos.x;

	if (Pos.y > (pMesh->vMax.y))
		pMesh->vMax.y = Pos.y;

	if (Pos.z > (pMesh->vMax.z))
		pMesh->vMax.z = Pos.z;

	//最小値
	if (Pos.x < (pMesh->vMin.x))
		pMesh->vMin.x = Pos.x;

	if (Pos.y < (pMesh->vMin.y))
		pMesh->vMin.y = Pos.y;

	if (Pos.z < (pMesh->vMin.z))
		pMesh->vMin.z = Pos.z;
}

//メッシュ描画
void CObjLoader::Draw(D3DMATRIX matWorld,MY_MESH* pMesh)
{
	//マテリアルの数毎に描画
	for (int i = 0; i < pMesh->MaterialNum; i++)
	{
		int size = pMesh->Material[i].pVertex.size();

		//シェーダーのセット
		g_Shader.SetShader(matWorld, pMesh->Material[i]);

		for (int j = 0; j <size; j++)
		{
			//ポリゴン描画
			DrawMesh(pMesh->Material[i].FaceOfVer[j], pMesh->Material[i].pVertex[j], pMesh->Material[i].pIndex[j]);
		}
	}
}

//ポリゴン描画
void CObjLoader::DrawMesh(int ver_num,ID3D10Buffer* VertexBuffer,ID3D10Buffer* IndexBuffer)
{
	//バーテックスバッファーをセット
	UINT stride = sizeof(MY_VERTEX);
	UINT offset = 0;
	dx.m_pDevice->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

	//インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	dx.m_pDevice->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//プリミティブ・トポロジーをセット
	if (ver_num == 3)
	{
		//三角形
		dx.m_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	if (ver_num == 4)
	{
		//四角形
		dx.m_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}

	//プリミティブをレンダリング
	D3D10_TECHNIQUE_DESC dc;
	g_Shader.m_pTechnique->GetDesc(&dc);
	for (UINT p = 0; p < dc.Passes; ++p)
	{
		g_Shader.m_pTechnique->GetPassByIndex(p)->Apply(0);
		dx.m_pDevice->DrawIndexed(ver_num, 0, 0);
	}
}
