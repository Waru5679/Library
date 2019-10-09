#include "XLoader_Skin.h"
#include "Release.h"
#include "LibraryHeader.h"

//インスタンス
CX_Skin* CX_Skin::m_pInstance = nullptr;

//スキンメッシュの読み込み
bool CX_Skin::LoadSkinMesh(const char* FileName, SKIN_MESH* pSkinMesh)
{
	//ファイル読み込み
	FILE* fp = nullptr;
	fopen_s(&fp, FileName, "rt");
	if (fp == nullptr)
	{
		//ファイル読み込み失敗
		return false;
	}

	int verNum;	//頂点数
	int faceNum;//面の数
	int normNum;//法線数(頂点の数と同じ？)
	int uvNum;	//UV数

	FACE* pFace = nullptr;	//面のリスト

	D3DXVECTOR3* pvPos		= nullptr;//頂点座標
	D3DXVECTOR3* pvNormal	= nullptr;//法線
	D3DXVECTOR2* pvTex		= nullptr;//テクスチャ座標

	//読み込み用
	float x, y, z;

	int v1, v2, v3, v4;
	v1 = v2 = v3 = v4 = 0;

	int dataNum;
		
	//キーワード読み込み
	char str[200];
	
	while (!feof(fp))
	{
		//キーワード 読み込み
		fscanf_s(fp, "%s ", str, sizeof(str));

		//頂点
		if (strcmp(str, "Mesh") == 0)
		{
			fgets(str, sizeof(str), fp);//メッシュ名

			//頂点数
			fgets(str, sizeof(str), fp);
			verNum = atoi(str);
			
			//頂点座標メモリ確保
			pvPos = new D3DXVECTOR3[verNum];

			//頂点座標読み込み
			for (int i = 0; i < verNum; i++)
			{
				fscanf_s(fp, "%f;%f;%f;,", &x, &y, &z);
				pvPos[i].x = x;
				pvPos[i].y = y;
				pvPos[i].z = z;
			}

			fgets(str, sizeof(str), fp);	//;除去

			//面の数
			fgets(str, sizeof(str), fp);
			faceNum = atoi(str);

			//面のメモリ確保
			pFace = new FACE[faceNum];

			//面読み込み
			for (int i = 0; i < faceNum; i++)
			{
				//面を構成する頂点の数
				fscanf_s(fp, "%d;", &dataNum);
				pFace->m_FaceOfVer = dataNum;

				//面構成のインデックスリストのメモリ確保
				pFace->m_pIndex = new int[dataNum];

				//三角ポリゴン
				if (dataNum == 3)
				{
					fscanf_s(fp, "%d,%d,%d;,", &v1, &v2, &v3);
					pFace->m_pIndex[0] = v1;
					pFace->m_pIndex[1] = v2;
					pFace->m_pIndex[2] = v3;
				}
				//四角ポリゴン
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
		//法線
		if (strcmp(str, "MeshNormals") == 0)
		{
			fgets(str, sizeof(str), fp);//{除去

			//法線数
			fgets(str, sizeof(str), fp);
			normNum = atoi(str);

			//法線メモリ確保
			pvNormal = new D3DXVECTOR3[normNum];
			
			//法線読み込み
			for (int i = 0; i < normNum; i++)
			{
				fscanf_s(fp, "%f;%f;%f;,", &x, &y, &z);
				pvNormal[i].x = x;
				pvNormal[i].y = y;
				pvNormal[i].z = z;
			}
		}

		//テクスチャー座標 読み込み
		if (strcmp(str, "MeshTextureCoords") == 0)
		{
			fgets(str, sizeof(str), fp);//{除去

			//UVの数
			fgets(str, sizeof(str), fp);
			uvNum = atoi(str);

			//UVメモリ確保
			pvTex = new D3DXVECTOR2[uvNum];

			//UV読み込み
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

//メッシュ描画
void CX_Skin::DrawMesh(D3DMATRIX matWorld, SKIN_MESH* pMesh, CColorData* pColor)
{
	//バーテックスバッファーをセット（バーテックスバッファーは一つでいい）
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	DX->GetDevice()->IASetVertexBuffers(0, 1, &pMesh->m_pVertexBuffer, &stride, &offset);

	//マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
	for (int i = 0; i < pMesh->m_MaterialNum; i++)
	{
		//使用されていないマテリアル対策
		if (pMesh->m_pMaterial[i].m_FaceNum == 0)
		{
			continue;
		}
		//インデックスバッファーをセット
		stride = sizeof(int);
		offset = 0;
		DX->GetDevice()->IASetIndexBuffer(pMesh->m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

		//プリミティブ・トポロジーをセット
		DX->GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//シェーダセット
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

//開放
void CX_Skin::Release()
{
	//インスタンス破棄
	PointerRelease(m_pInstance);
}