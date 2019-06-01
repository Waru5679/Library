#include "ObjLoader.h"
#include "DirectX.h"
#include "Shader.h"
#include "Task.h"
#include "Math.h"

CObjLoader g_Loader;

//モデル読み込み
void CObjLoader::LoadMesh(int Id, const char* Name)
{
	MY_MESH Mesh;
	Mesh.Id = Id;
	
	//読み込み
	LoadObj(Name, &Mesh);

	//ベクター登録
	m_Mesh.push_back(Mesh);
}

//OBJファイルの読み込み
HRESULT CObjLoader::LoadObj(const char* FileName, MY_MESH* pMesh)
{
	//ファイルオープン
	FILE* fp=NULL;
	fopen_s(&fp, FileName, "rt");

	//ファイル読み込み失敗
	if (fp == NULL)
	{
		return E_FAIL;
	}

	//キーワード読み込み用
	char key[200];

	float uv_x[4] = { 0.0f,-1.0f,-1.0f,0.0f };	
	float uv_y[4] = { 0.0f,0.0f,1.0f,1.0f };

	//頂点情報
	vector<D3DXVECTOR3> Pos;
	vector<D3DXVECTOR3> Nor;
	vector<D3DXVECTOR2> Tex;
	vector<MY_VERTEX> Vertex;
	
	int index_count = 0;

	pMesh->vMax = D3DXVECTOR3(-999.0f, -999.0f, -999.0f);
	pMesh->vMin	= D3DXVECTOR3(999.0f, 999.0f, 999.0f);
	
	int iFaceCount = -1;
	
	//キーワード読み込み
	fscanf_s(fp, "%s ", key, sizeof(key));

	//OBJ情報の読み込み
	while (!feof(fp))
	{
		//一時保存用
		MY_MATERIAL Material;
		Material.pTexture = NULL;

		//面の頂点数保存用
		vector<int> FaceOfVer;

		//読み込み用
		float x, y, z;

		//パーツごとに読み込む
		if (strcmp(key, "o") == 0)
		{
			//次のoまでループ
			do
			{
				//キーワード読み込み
				fscanf_s(fp, "%s ", key, sizeof(key));
					
				//位置
				if (strcmp(key, "v") == 0)
				{
					fscanf_s(fp, "%f %f %f", &x, &y, &z);
					
					//位置情報保存
					D3DXVECTOR3 pos(x,y,z);
					Pos.push_back(pos);

					//最小と最大の保存
					MinAndMax(pos, pMesh);
				}
				//法線
				if (strcmp(key, "vn") == 0)
				{
					fscanf_s(fp, "%f %f %f", &x, &y, &z);
				
					//法線情報登録
					D3DXVECTOR3 nor(x, y, z);
					Nor.push_back(nor);
				}
				//UV
				if (strcmp(key, "vt") == 0)
				{
					fscanf_s(fp, "%f %f", &x, &y);

					//Uv情報登録
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
					FaceOfVer.push_back(iFaceNum);
						
					//読み込み用
					int v, vt, vn;
					v = vt = vn = -1;

					//頂点の数だけ回す
					for (int i = 0; i < iFaceNum; i++)
					{
						//読み込み
						if (sscanf_s(pStr, "%d/%d/%d", &v, &vt, &vn) != 3)
						{
							//UV情報がないときは読み込みなおす
							if (sscanf_s(pStr, "%d//%d", &v, &vn) != 2)
								sscanf_s(pStr, "%d/%d", &v, &vn);
						}

						//一時保存
						MY_VERTEX vertex;

						//頂点構造体に代入
						vertex.vPos = Pos[v - 1];

						//法線情報がある時
						if (Nor.size() != 0)
						{
							vertex.vNorm = Nor[vn - 1];
						}
						//法線情報がない時
						else
						{
							vertex.vNorm = D3DXVECTOR3(0.0f, 0.0f, 1.0f );
						}

						//UV情報がある時
						if (vt != -1)
						{
							vertex.vTex = Tex[vt - 1];
						}
						//UV情報がない時
						else
						{
							D3DXVECTOR2 tex;
							tex.x = uv_x[i];
							tex.y = uv_y[i];
							vertex.vTex = tex;
						}

						//頂点情報保存
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

			MY_VERTEX*	pVertex = NULL;	//頂点情報のポインタ
			int*		pIndex	= NULL;	//インデックス情報のポインタ

			ID3D10Buffer* IndexBuffer	= NULL;	//インデックスバッファのポインタ
			ID3D10Buffer* VertexBuffer	= NULL;	//バーテックスバッファのポインタ

			//面ごとに回す
			for (int i = 0; i <iFaceCount; i++)
			{				
				//頂点定義
				pVertex = new MY_VERTEX[FaceOfVer[i]];
				for (int j = 0; j < FaceOfVer[i]; j++)
				{ 
					pVertex[j]=Vertex[index_count + j];
				}
				
				//四角ポリゴン
				if (FaceOfVer[i] == 4)
				{
					//N字を描くように順番を入れ替える
					MY_VERTEX tmp = pVertex[2];
					pVertex[0] = pVertex[0];
					pVertex[1] = pVertex[1];
					pVertex[2] = pVertex[3];
					pVertex[3] = tmp;
				}
				
				//面の情報を保存
				FACE_INFO info;
				for (int VerNum = 0; VerNum < FaceOfVer[i]; VerNum++)
				{
					info.Vertex.push_back(pVertex[VerNum]);
				}
				Material.FaceInfo.push_back(info);

				//バーテックスバッファーを作成
				VertexBuffer = g_Draw.BufferCreate(pVertex,sizeof(MY_VERTEX)*FaceOfVer[i], D3D10_BIND_VERTEX_BUFFER);

				//バーテックスバッファ情報登録
				Material.pVertex.insert(Material.pVertex.begin() + i, VertexBuffer);

				//index情報
				pIndex = new int[FaceOfVer[i]];
				for (int k = 0; k < FaceOfVer[i]; k++)
				{
					pIndex[k] = k;
				}

				//インデックスバッファーを作成
				IndexBuffer = g_Draw.BufferCreate(pIndex, sizeof(int)*FaceOfVer[i],D3D10_BIND_INDEX_BUFFER);
		
				//インデックスバッファ登録
				Material.pIndex.insert(Material.pIndex.begin() + i, IndexBuffer);

				//使った数だけずらす
				index_count += FaceOfVer[i];
				
				//解放
				delete[] pIndex;
				delete[] pVertex;
			}

			//マテリアル情報を登録
			pMesh->Material.push_back(Material);
			
			iFaceCount = 0;
		}
		else
		{
			//キーワード読み込み
			fscanf_s(fp, "%s ", key, sizeof(key));
		}

		//面の頂点リスト解放
		FaceOfVer.clear();
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
	
	//頂点情報解放
	Pos.clear();
	Nor.clear();
	Tex.clear();
	Vertex.clear();
	
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

	//ファイル読み込み失敗
	if (fp == NULL)
	{
		return E_FAIL;
	}

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

//メッシュ描画(描画色指定なし)
void CObjLoader::Draw(D3DMATRIX matWorld,MY_MESH* pMesh)
{
	//描画色
	float Color[4] = { 1.0f,1.0f,1.0f,1.0f };
	
	Draw(matWorld, pMesh, Color);
}

//メッシュ描画(描画色指定あり)
void CObjLoader::Draw(D3DMATRIX matWorld, MY_MESH* pMesh, float fColor[4])
{
	//テクスチャ切り取り位置
	float Src[4] = { 0.0f,0.0f,1.0f,1.0f };

	//マテリアルの数毎に描画
	for (unsigned int i = 0; i < pMesh->Material.size(); i++)
	{
		int size = pMesh->Material[i].pVertex.size();

		//シェーダーのセット
		g_Shader.SetShader(pMesh->Material[i].pTexture, Src, fColor, matWorld);

		for (int j = 0; j < size; j++)
		{
			//ポリゴン描画
			g_Draw.DrawPolygon(pMesh->Material[i].FaceInfo[j].Vertex.size() , pMesh->Material[i].pVertex[j], pMesh->Material[i].pIndex[j]);
		}
	}
}

//テクスチャを指定してメッシュ描画
void CObjLoader::Draw(int TexId, D3DMATRIX matWorld, MY_MESH* pMesh)
{
	//描画色
	float Color[4] = { 1.0f,1.0f,1.0f,1.0f };

	//テクスチャ切り取り位置
	float Src[4] = { 0.0f,0.0f,1.0f,1.0f };

	//テクスチャ
	ID3D10ShaderResourceView*pTex =	g_Task.GetTex(TexId)->m_pTex;

	//マテリアルの数毎に描画
	for (unsigned int i = 0; i < pMesh->Material.size(); i++)
	{
		int size = pMesh->Material[i].pVertex.size();

		//シェーダーのセット
		g_Shader.SetShader(pMesh->Material[i].pTexture, Src, Color, matWorld);

		for (int j = 0; j < size; j++)
		{
			//ポリゴン描画
			g_Draw.DrawPolygon(pMesh->Material[i].FaceInfo[j].Vertex.size(), pMesh->Material[i].pVertex[j], pMesh->Material[i].pIndex[j]);
		}
	}
}

//Mesh取得
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

//解放
void CObjLoader::Release()
{
	m_Mesh.erase(m_Mesh.begin(), m_Mesh.end());
}