#include "ModelLoader.h"
#include "LibraryHeader.h"

#include <stdio.h>

//インスタンス
CModelLoader* CModelLoader::m_pInstance = nullptr;

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
	char Key[100];
	float x, y, z, w;
	x = y = z = w = 0.0f;
	int a, b, c, d;
	a = b = c = d = -1;


	//読み込み
	while (!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp, "%s ", Key, sizeof(Key));

		//モデル名
		if (strcmp(Key, "ModelName") == 0)
		{
			fscanf_s(fp, "%s ", pModel->m_ModelName, sizeof(pModel->m_ModelName));
		}
		//テクスチャ
		if (strcmp(Key, "TexNum") == 0)
		{
			//テクスチャ数
			fscanf_s(fp, "%d ", &pModel->m_TexNum);

			//メモリ確保
			pModel->m_ppTex = new ID3D10ShaderResourceView * [pModel->m_TexNum];

			//テクスチャ読み込み
			for (int i = 0; i < pModel->m_TexNum; i++)
			{
				//テクスチャパス
				char TexPass[100];
				fscanf_s(fp, "%s ", TexPass, sizeof(TexPass));

				//テクスチャーを作成
				if (FAILED(D3DX10CreateShaderResourceViewFromFileA(DX->GetDevice(), TexPass, NULL, NULL, &pModel->m_ppTex[i], NULL)))
				{
					return false;
				}
			}
		}
		//頂点
		if (strcmp(Key, "VerNum") == 0)
		{
			//頂点数
			fscanf_s(fp, "%d ", &pModel->m_VerNum);

			//メモリ確保
			pModel->m_pVertex = new VERTEX[pModel->m_VerNum];

			//頂点データ読み込み
			for (int i = 0; i < pModel->m_VerNum; i++)
			{
				//位置
				fscanf_s(fp, "%s ", Key, sizeof(Key));
				fscanf_s(fp, "%f,%f,%f,", &x, &y, &z);
				pModel->m_pVertex[i].vPos = D3DXVECTOR3(x, y, z);
				
				//法線
				fscanf_s(fp, "%s ", Key, sizeof(Key));
				fscanf_s(fp, "%f,%f,%f,", &x, &y, &z);
				pModel->m_pVertex[i].vNorm = D3DXVECTOR3(x, y, z);
					
				//Uv
				fscanf_s(fp, "%s ", Key, sizeof(Key));
				fscanf_s(fp, "%f,%f,", &x, &y);
				pModel->m_pVertex[i].vUv = D3DXVECTOR2(x, y);
					
				//ウェイト数
				fscanf_s(fp, "%s ", Key, sizeof(Key));
				int dammy;
				fscanf_s(fp, "%d", &dammy);

//				fscanf_s(fp, "%d", &pModel->m_pVertex[i].WeightNum);

				//メモリ確保
	/*			pModel->m_pVertex[i].m_pBoneId = new int[pModel->m_pVertex[i].WeightNum];
				pModel->m_pVertex[i].m_pfWeight = new float[pModel->m_pVertex[i].WeightNum];
	*/				
				//ボーンID
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
				//ウェイト
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
			//頂点バッファ作成
			pModel->m_pVertexBuffer = DRAW->BufferCreate(pModel->m_pVertex, sizeof(VERTEX) * pModel->m_VerNum, D3D10_BIND_VERTEX_BUFFER);
		}
		//面
		if (strcmp(Key, "FaceNum") == 0)
		{
			//面の数
			fscanf_s(fp, "%d", &pModel->m_FaceNum);

			//メモリ確保
			pModel->m_pFace = new FACE[pModel->m_FaceNum];

			//面情報読み込み
			for (int i = 0; i < pModel->m_FaceNum; i++)
			{
				fscanf_s(fp, "%d,%d,%d,", &a, &b, &c);
				pModel->m_pFace[i].m_VerId[0] = a;
				pModel->m_pFace[i].m_VerId[1] = b;
				pModel->m_pFace[i].m_VerId[2] = c;

				//インデックスバッファーを作成
				pModel->m_pFace[i].m_pIndexBuffer = DRAW->BufferCreate(pModel->m_pFace[i].m_VerId, sizeof(int) * 3, D3D10_BIND_INDEX_BUFFER);
			}
		}
		//マテリアル
		if (strcmp(Key, "MateNum") == 0)
		{
			//マテリアルの数
			fscanf_s(fp, "%d", &pModel->m_MateNum);

			//面カウント
			int FaceCount = 0;

			//メモリ確保
			pModel->m_pMaterial = new MATERIAL[pModel->m_MateNum];

			//マテリアル読み込み
			for (int i = 0; i < pModel->m_MateNum; i++)
			{
				fscanf_s(fp, "%s", Key, sizeof(Key));

				//マテリアル名
				fscanf_s(fp, "%s", Key, sizeof(Key));
				fscanf_s(fp, "%s", pModel->m_pMaterial[i].m_Name, sizeof(pModel->m_pMaterial[i].m_Name));
				
				//色
				fscanf_s(fp, "%s", Key, sizeof(Key));
				fscanf_s(fp, "%f,%f,%f,%f,", &x, &y, &z, &w);
				pModel->m_pMaterial[i].m_vColor = D3DXVECTOR4(x, y, z, w);
				
				//ディフューズ
				fscanf_s(fp, "%s", Key, sizeof(Key));
				fscanf_s(fp, "%f,%f,%f,%f,", &x, &y, &z, &w);
				pModel->m_pMaterial[i].m_vDiffuse = D3DXVECTOR4(x, y, z, w);

				//スペキュラ
				fscanf_s(fp, "%s", Key, sizeof(Key));
				fscanf_s(fp, "%f,%f,%f,%f,", &x, &y, &z, &w);
				pModel->m_pMaterial[i].m_vSpecular = D3DXVECTOR4(x, y, z, w);
				
				//アンビエント
				fscanf_s(fp, "%s", Key, sizeof(Key));
				fscanf_s(fp, "%f,%f,%f,%f,", &x, &y, &z, &w);
				pModel->m_pMaterial[i].m_vAmbient = D3DXVECTOR4(x, y, z, w);
	
				//スペキュラパワー
				fscanf_s(fp, "%s", Key, sizeof(Key));
				fscanf_s(fp, "%f", &pModel->m_pMaterial[i].m_fSpePower);

				//テクスチャID
				fscanf_s(fp, "%s", Key, sizeof(Key));
				fscanf_s(fp, "%d", &pModel->m_pMaterial[i].m_TexId);
				
				//使用する面数
				int UseFaceNum = -1;
				fscanf_s(fp, "%s", Key, sizeof(Key));
				fscanf_s(fp, "%d", &UseFaceNum);

				//使用する面にマテリアル情報を渡す
				for (int j = FaceCount; j < FaceCount + UseFaceNum; j++)
				{
					pModel->m_pFace[j].m_UseMateId = i;
				}
				//カウンタ更新
				FaceCount += UseFaceNum;
			}
		}
	}
	return true;
}


//描画
void CModelLoader::Draw(D3DMATRIX matWorld, MODEL_DATA* pModel, CColorData* pColor)
{
	for (int i = 0; i < pModel->m_FaceNum; i++)
	{
		//使用するマテリアル
		MATERIAL UseMaterial = pModel->m_pMaterial[pModel->m_pFace[i].m_UseMateId];

		//使用するテクスチャ
		ID3D10ShaderResourceView* pTex  = pModel->m_ppTex[UseMaterial.m_TexId];

		//シェーダーのセット
		SHADER->SetShader(pTex,NULL, pColor, matWorld);

		//ポリゴン描画
		DRAW->DrawPolygon(3, pModel->m_pVertexBuffer,pModel->m_pFace[i].m_pIndexBuffer);
	}
}

//解放
void CModelLoader::Release()
{
	//インスタンス破棄
	delete m_pInstance;
	m_pInstance = nullptr;
}
