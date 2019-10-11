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

	int verNum	=0;//頂点数
	int faceNum	=0;//面の数
	int normNum =0;//法線数(頂点の数と同じ？)
	int uvNum	=0;//UV数
	int mateNum	=0;//マテリアルの数

	FACE*		 pFace		= nullptr;//面のリスト
	int*		 pFaceOfMate= nullptr;//面の対応マテリアルのインデックス
	D3DXVECTOR3* pvPos		= nullptr;//頂点座標
	D3DXVECTOR3* pvNormal	= nullptr;//法線
	D3DXVECTOR2* pvTex		= nullptr;//テクスチャ座標
	MATERIAL*	 pMateial	= nullptr;//マテリアル
	VERTEX*		 pVertex	= nullptr;//頂点リスト

	//読み込み用
	float x, y, z, w;

	int v1, v2, v3, v4;
	v1 = v2 = v3 = v4 = 0;

	int faceOfVer=0;//面を構成する頂点数
		
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
				fscanf_s(fp, "%d;", &faceOfVer);
				pFace->m_FaceOfVer = faceOfVer;

				//面構成のインデックスリストのメモリ確保
				pFace->m_pIndex = new int[faceOfVer];

				//三角ポリゴン
				if (faceOfVer == 3)
				{
					fscanf_s(fp, "%d,%d,%d;,", &v1, &v2, &v3);
					pFace->m_pIndex[0] = v1;
					pFace->m_pIndex[1] = v2;
					pFace->m_pIndex[2] = v3;
				}
				//四角ポリゴン
				if (faceOfVer == 4)
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

		//マテリアルリスト読み込み
		if (strcmp(str, "MeshMaterialList") == 0)
		{
			fgets(str, sizeof(str), fp);//{除去

			//マテリアルの数
			fgets(str, sizeof(str), fp);
			mateNum = atoi(str);

			//マテリアルメモリ確保
			pMateial = new MATERIAL[mateNum];

			//面の数
			fgets(str, sizeof(str), fp);
			faceNum = atoi(str);

			//面の対応マテリアルメモリ確保
			pFaceOfMate = new int[faceNum];
			
			//面に何番のマテリアルを使うか
			for (int i = 0; i < faceNum; i++)
			{
				//読み込み
				fscanf_s(fp, "%d", &pFaceOfMate[i]);

				//,または;の除去
				fgets(str, sizeof(str), fp);
			}
		}
		//マテリアル読み込み(マテリアルリストを読むまでは通らない)
		for (int i = 0; i < mateNum; i++)
		{
			if (strcmp(str, "Material") == 0)
			{
				fgets(str, sizeof(str), fp);//{除去

				//面の色
				fscanf_s(fp, "%f;%f;%f;%f;;", &x, &y, &z, &w);
				pMateial[i].m_vFaceColor.x = x;
				pMateial[i].m_vFaceColor.y = y;
				pMateial[i].m_vFaceColor.z = z;
				pMateial[i].m_vFaceColor.w = w;

				//スペキュラーのパワー
				fscanf_s(fp, "%f;", &pMateial[i].m_fPower);

				//スペキュラー
				fscanf_s(fp, "%f;%f;%f;;", &x, &y, &z);
				pMateial[i].m_vKs.x = x;
				pMateial[i].m_vKs.y = y;
				pMateial[i].m_vKs.z = z;

				//エミッシブ
				fscanf_s(fp, "%f;%f;%f;;", &x, &y, &z);
				pMateial[i].m_vKe.x = x;
				pMateial[i].m_vKe.y = y;
				pMateial[i].m_vKe.z = z;

				//テクスチャ
				fscanf_s(fp, "%s ", str, sizeof(str));
				if (strcmp(str, "TextureFilename") == 0)
				{
					fgets(str, sizeof(str), fp);//{除去
					
					//読み込み最大サイズ
					int size = sizeof(pMateial[i].m_TexName);
					
					//テクスチャ名
					fscanf_s(fp, "%s", pMateial[i].m_TexName,size);
		
					//"と;を除去する
					int count = 0;//除去カウント
					for (int j = 0; j < size; j++)
					{
						if (pMateial[i].m_TexName[j] == '\"' || pMateial[i].m_TexName[j] == ';')
						{
							//除去数をカウント
							count++;
						}
						else
						{
							//カウント分を詰めてコピー
							pMateial[i].m_TexName[j - count] = pMateial[i].m_TexName[j];
						}
					}

					//テクスチャーを作成
					if (FAILED(D3DX10CreateShaderResourceViewFromFileA(DX->GetDevice(), pMateial[i].m_TexName, NULL, NULL, &pMateial[i].m_pTexture, NULL)))
					{
						//テクスチャ作成失敗
						return E_FAIL;
					}
				}
			}
		}

		////頂点構造体メモリ確保
		//pVertex = new VERTEX[verNum];

		////頂点構造体にまとめる
		//for (int i = 0; i < verNum; i++)
		//{
		//	pVertex[i].m_vPos	= pvPos[i];
		//	pVertex[i].m_vNorm	= pvNormal[i];
		//	pVertex[i].m_vTex	= pvTex[i];
		//}

		////バーテックスバッファーを作成
		//pSkinMesh->m_pVertexBuffer = DRAW->BufferCreate(pVertex, sizeof(VERTEX) * verNum, D3D10_BIND_VERTEX_BUFFER);

		//
		////マテリアルの数だけインデックスバッファ作成
		//for (int i = 0; i < mateNum; i++)
		//{
		//	pFace[i].
		//}
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
		SHADER->SetShader(pMesh->m_pMaterial[i].m_pTexture, NULL, pColor, matWorld);

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