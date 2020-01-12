#include "ModelLoader.h"
#include "LibraryHeader.h"

//インスタンス
CModelLoader* CModelLoader::m_pInstance = nullptr;

constexpr int ARRAY_SIZE{ 100 };			//配列サイズ

//読み込み
bool CModelLoader::Load(const char* FileName, MODEL_DATA* pModel)
{
	//ファイルオープン
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	//ファイル読み込み失敗
	if (fp == NULL)
	{
		return false;
	}
	
	//読み込み用
	char Key[ARRAY_SIZE];

	//モデル名
	fscanf_s(fp, "%s", Key, sizeof(Key));
	fgets(pModel->m_Name, sizeof(pModel->m_Name), fp);
	
	//テクスチャ読み込み
	if (TexLoad(fp, pModel) == false)
	{
		return false;
	}

	//頂点読み込み
	if (VertexLoad(fp, pModel) == false)
	{
		return false;
	}

	//バーテックスバッファ作成
	pModel->m_pVertexBuffer=DRAW->BufferCreate(pModel->m_pVertex, sizeof(VERTEX) * pModel->m_VerNum, D3D10_BIND_VERTEX_BUFFER);
	
	//面読み込み
	if (FaceLoad(fp, pModel) == false)
	{
		return false;
	}

	//マテリアル読み込み
	if (MateLoad(fp, pModel) == false)
	{
		return false;
	}

	return true;
}

//テクスチャ読み込み
bool CModelLoader::TexLoad(FILE* fp, MODEL_DATA* pModel)
{
	//読み込み用
	char Key[ARRAY_SIZE];

	//テクスチャ数
	fscanf_s(fp, "%s", Key, sizeof(Key));
	fscanf_s(fp, "%d", &pModel->m_TexNum);

	//テクスチャなし
	if (pModel->m_TexNum <= 0)
	{
		return false;
	}

	//メモリ確保
	pModel->m_ppTexture = new ID3D10ShaderResourceView * [pModel->m_TexNum];

	//テクスチャ読み込み
	for (int i = 0; i < pModel->m_TexNum; i++)
	{
		//テクスチャ名
		fscanf_s(fp, "%s", Key, sizeof(Key));

		//テクスチャーを作成
		if (FAILED(D3DX10CreateShaderResourceViewFromFileA(DX->GetDevice(), Key, NULL, NULL, &pModel->m_ppTexture[i], NULL)))
		{
			return false;
		}
	}

	return true;
}

//頂点読み込み
bool CModelLoader::VertexLoad(FILE* fp, MODEL_DATA* pModel)
{
	//読み込み用
	char Key[ARRAY_SIZE];
	float x, y, z, w;
	x = y = z = w = 0.0f;

	//頂点数
	fscanf_s(fp, "%s", Key, sizeof(Key));
	fscanf_s(fp, "%d", &pModel->m_VerNum);

	//頂点なし
	if (pModel->m_VerNum <= 0)
	{
		return false;
	}

	//頂点メモリ確保
	pModel->m_pVertex = new VERTEX[pModel->m_VerNum];

	//頂点情報読み込み
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

		//ウェイト数
		fscanf_s(fp, "%s", Key, sizeof(Key));
		//fscanf_s(fp, "%d", &pModel->m_pVertex[i].WeightNum);
		fscanf_s(fp, "%d", &WeightNum);

		////ボーンIDとウェイトのメモリ確保
		//pModel->m_pVertex[i].m_pBoneId = new int[pModel->m_pVertex[i].WeightNum];
		//pModel->m_pVertex[i].m_pfWeight = new float[pModel->m_pVertex[i].WeightNum];

		//ボーンID
		fscanf_s(fp, "%s", Key, sizeof(Key));
		/*for (int j = 0; j < pModel->m_pVertex[i].WeightNum; j++)
		{
			fscanf_s(fp, "%d,", &pModel->m_pVertex[i].m_pBoneId[j]);
		}*/

		for (int j = 0; j < WeightNum; j++)
		{
			fscanf_s(fp, "%d,", &dammy);
		}


		//ウェイト
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

//面読み込み
bool CModelLoader::FaceLoad(FILE* fp, MODEL_DATA* pModel)
{
	//読み込み用
	char Key[ARRAY_SIZE];
	int a, b, c;
	a = b = c = -1;

	//面数
	fscanf_s(fp, "%s", Key, sizeof(Key));
	fscanf_s(fp, "%d", &pModel->m_FaceNum);

	//面なし
	if (pModel->m_FaceNum <= 0)
	{
		return false;
	}

	//メモリ確保
	pModel->m_pFace = new FACE[pModel->m_FaceNum];

	//面読み込み
	for (int i = 0; i < pModel->m_FaceNum; i++)
	{
		fscanf_s(fp, "%d,%d,%d,", &a, &b, &c);
		pModel->m_pFace[i].m_VerId[0] = a;
		pModel->m_pFace[i].m_VerId[1] = b;
		pModel->m_pFace[i].m_VerId[2] = c;

		//インデックスバッファ作成
		pModel->m_pFace[i].m_pIndexBuffer=DRAW->BufferCreate(pModel->m_pFace[i].m_VerId, sizeof(int) * 3, D3D10_BIND_INDEX_BUFFER);
	}
}

//マテリアル読み込み
bool CModelLoader::MateLoad(FILE* fp, MODEL_DATA* pModel)
{
	//読み込み用
	char Key[ARRAY_SIZE];
	float r, g, b, a;
	r = g = b = a = 0.0f;

	//使用する面の数
	int UseFaceNum = 0;

	//マテリアル数
	fscanf_s(fp, "%s", Key, sizeof(Key));
	fscanf_s(fp, "%d", &pModel->m_MateNum);

	//マテリアルなし
	if (pModel->m_MateNum <= 0)
	{
		return false;
	}

	//メモリ確保
	pModel->m_pMaterial = new MATERIAL[pModel->m_MateNum];

	int FaceCount = 0;

	//マテリアル読み込み
	for (int i = 0; i < pModel->m_MateNum; i++)
	{
		//マテリアル名
		fscanf_s(fp, "%s", Key, sizeof(Key));
		fscanf_s(fp, "%s", pModel->m_pMaterial[i].m_Name, sizeof(pModel->m_pMaterial[i].m_Name));

		//色
		fscanf_s(fp, "%s", Key, sizeof(Key));
		fscanf_s(fp, "%f,%f,%f,%f,",&r,&g,&b,&a);
		pModel->m_pMaterial[i].m_vColor = D3DXVECTOR4(r, g, b, a);
		
		//ディフューズ
		fscanf_s(fp, "%s", Key, sizeof(Key));
		fscanf_s(fp, "%f,%f,%f,%f,", &r, &g, &b, &a);
		pModel->m_pMaterial[i].m_vDiffuse = D3DXVECTOR4(r, g, b, a);

		//スペキュラー
		fscanf_s(fp, "%s", Key, sizeof(Key));
		fscanf_s(fp, "%f,%f,%f,%f,", &r, &g, &b, &a);
		pModel->m_pMaterial[i].m_vSpecular = D3DXVECTOR4(r, g, b, a);

		//アンビエント
		fscanf_s(fp, "%s", Key, sizeof(Key));
		fscanf_s(fp, "%f,%f,%f,%f,", &r, &g, &b, &a);
		pModel->m_pMaterial[i].m_vAmbient = D3DXVECTOR4(r, g, b, a);

		//スペキュラパワー
		fscanf_s(fp, "%s", Key, sizeof(Key));
		fscanf_s(fp, "%f", &pModel->m_pMaterial[i].m_fSpePower);

		//テクスチャID
		fscanf_s(fp, "%s", Key, sizeof(Key));
		fscanf_s(fp, "%d", &pModel->m_pMaterial[i].m_TexId);

		//使用する面の数
		fscanf_s(fp, "%s", Key, sizeof(Key));
		fscanf_s(fp, "%d", &UseFaceNum);

		//面にマテリアル情報を渡す。
		for (int j = FaceCount; j < FaceCount+UseFaceNum; j++)
		{
			pModel->m_pFace[j].m_UseMateId = i;
		}

		//使用した面の数更新
		FaceCount += UseFaceNum;
	}
	return true;
}

//モデル描画
void CModelLoader::Draw(D3DMATRIX matWorld, MODEL_DATA* pModel, CColorData* pColor)
{
	
	//面ごとに描画
	for (unsigned int i = 0; i < pModel->m_FaceNum; i++)
	{
		//使用するマテリアル
		MATERIAL UseMaterial = pModel->m_pMaterial[pModel->m_pFace[i].m_UseMateId];

		if (pModel->m_pFace[i].m_UseMateId ==29 )
			int b = 0;

		//使用するテクスチャ
		ID3D10ShaderResourceView* pTex = pModel->m_ppTexture[UseMaterial.m_TexId];

		//シェーダーのセット
		SHADER->SetShader(pTex, NULL, pColor, matWorld);

		DRAW->DrawPolygon(3, pModel->m_pVertexBuffer, pModel->m_pFace[i].m_pIndexBuffer);
	}
}



//解放
void CModelLoader::Release()
{
	//インスタンス破棄
	delete m_pInstance;
	m_pInstance = nullptr;
}
