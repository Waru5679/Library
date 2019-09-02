#include "XLoader.h"
#include "LibraryHeader.h"

//Xファイルを読み込む関数
HRESULT CXLoader::LoadMeshFromX(const char* FileName, MY_MESH* pMesh)
{
	//デバイス取得
	ID3D10Device* pDevice = DX->GetDevice();

	float x, y, z;
	int v1 = 0, v2 = 0, v3 = 0;
	int vn1 = 0, vn2 = 0, vn3 = 0;
	int vt1 = 0, vt2 = 0, vt3 = 0;
	int* piMateriaList = NULL;
	int iCount = 0;

	char key[400] = { 0 };
	char str[400] = { 0 };
	
	//Xファイルを開いて内容を読み込む
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	//マテリアル読み込み
	D3DXVECTOR4 vV4;
	D3DXVECTOR3 vV3;

	//マテリアル数を調べる
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", key, sizeof(key));
		if (strcmp(key, "Material") == 0)
		{
			pMesh->dwNumMaterial++;
		}
	}

	//マテリアルのメモリー確保
	pMesh->pMaterial = new MY_MATERIAL[pMesh->dwNumMaterial];

	fseek(fp, SEEK_SET, 0);
	iCount = 0;
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", key, sizeof(key));
		if (strcmp(key, "Material") == 0)
		{
			//直後の行にあると推定　改行する
			fgets(str, sizeof(str), fp);

			//Kd　ディフューズ
			fscanf_s(fp, "%f;%f;%f;%f;;", &vV4.x, &vV4.y, &vV4.z, &vV4.w);
			pMesh->pMaterial[iCount].Kd = vV4;
			
			//パワー（スペキュラーのハイライト）		
			fscanf_s(fp, "%f;", &pMesh->pMaterial[iCount].Kp);
			
			//Ks　スペキュラー	
			fscanf_s(fp, "%f;%f;%f;;", &vV3.x, &vV3.y, &vV3.z);
			pMesh->pMaterial[iCount].Ks = vV3;
			
			//Ke　エミッシブ	
			fscanf_s(fp, "%f;%f;%f;;", &vV3.x, &vV3.y, &vV3.z);
			pMesh->pMaterial[iCount].Ke = vV3;

			//テクスチャー
			fscanf_s(fp, "%s ", key, sizeof(key));
			if (strcmp(key, "TextureFilename") == 0)
			{
				//直後の行にあると推定　改行する
				fgets(str, sizeof(str), fp);

				//map_Kd　テクスチャー	
				fscanf_s(fp, "%s", pMesh->pMaterial[iCount].szTextureName, sizeof(pMesh->pMaterial[iCount].szTextureName));
				
				//2重引用符を除去
				int k = 0;
				while (pMesh->pMaterial[iCount].szTextureName[++k] != '\"') pMesh->pMaterial[iCount].szTextureName[k - 1] = pMesh->pMaterial[iCount].szTextureName[k];
				pMesh->pMaterial[iCount].szTextureName[k - 1] = 0;
				
				//テクスチャーを作成
				if (FAILED(D3DX10CreateShaderResourceViewFromFileA(pDevice, pMesh->pMaterial[iCount].szTextureName, NULL, NULL, &pMesh->pMaterial[iCount].pTexture, NULL)))
				{
					return E_FAIL;
				}
			}
			iCount++;
		}
	}
	//マテリアルの数だけインデックスバッファーを作成
	pMesh->ppIndexBuffer = new ID3D10Buffer * [pMesh->dwNumMaterial];

	//事前に頂点数、ポリゴン数を調べる
	fseek(fp, SEEK_SET, 0);
	while (!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp, "%s ", key, sizeof(key));
		
		//頂点
		if (strcmp(key, "Mesh") == 0)
		{
			//直後の行にあると推定　改行する
			fgets(str, sizeof(str), fp);
			fgets(str, sizeof(str), fp);
			pMesh->dwNumVert = atoi(str);

			//頂点数だけ改行して、フェイス（頂点インデックス）のデータ部分まで進む
			for (DWORD i = 0; i < pMesh->dwNumVert; i++)
			{
				fgets(str, sizeof(str), fp);//改行
			}
			
			//フェイス（ポリゴン、頂点インデックス）
			fgets(str, sizeof(str), fp);
			pMesh->dwNumFace = atoi(str);
		}
	}

	//一時的なメモリ確保（頂点バッファとインデックスバッファ）
	MY_VERTEX* pvVertexBuffer = new MY_VERTEX[pMesh->dwNumVert];
	int* piFace = new int[pMesh->dwNumFace * 3];//3頂点なので
	D3DXVECTOR3* pvCoord = new D3DXVECTOR3[pMesh->dwNumVert];
	D3DXVECTOR3* pvNormal = new D3DXVECTOR3[pMesh->dwNumVert];
	D3DXVECTOR2* pvTexture = new D3DXVECTOR2[pMesh->dwNumVert];

	//本読み込み	
	fseek(fp, SEEK_SET, 0);

	while (!feof(fp))
	{
		//キーワード 読み込み
		ZeroMemory(key, sizeof(key));
		fscanf_s(fp, "%s ", key, sizeof(key));

		//頂点 読み込み
		if (strcmp(key, "Mesh") == 0)
		{
			fgets(str, sizeof(str), fp);//データは2行下にあると推定　改行する
			fgets(str, sizeof(str), fp);//データは2行下にあると推定　改行する

			for (DWORD i = 0; i < pMesh->dwNumVert; i++)
			{
				fscanf_s(fp, "%f;%f;%f;,", &x, &y, &z);
				pvCoord[i].x = x;
				pvCoord[i].y = y;
				pvCoord[i].z = z;
			}
			//フェイス読み込み		
			fgets(str, sizeof(str), fp);//データは2行下にあると推定　改行する
			fgets(str, sizeof(str), fp);//データは2行下にあると推定　改行する
			for (DWORD i = 0; i < pMesh->dwNumFace; i++)
			{
				int dammy;
				fscanf_s(fp, "%d;%d,%d,%d;,", &dammy, &v1, &v2, &v3);
				piFace[i * 3] = v1;
				piFace[i * 3 + 1] = v2;
				piFace[i * 3 + 2] = v3;
			}
		}

		//法線 読み込み
		if (strcmp(key, "MeshNormals") == 0)
		{
			fgets(str, sizeof(str), fp);//データは2行下にあると推定　改行する
			fgets(str, sizeof(str), fp);//データは2行下にあると推定　改行する

			for (DWORD i = 0; i < pMesh->dwNumVert; i++)
			{
				fscanf_s(fp, "%f;%f;%f;,", &x, &y, &z);
				pvNormal[i].x = x;
				pvNormal[i].y = y;
				pvNormal[i].z = z;
			}
		}

		//テクスチャー座標 読み込み
		if (strcmp(key, "MeshTextureCoords") == 0)
		{
			fgets(str, sizeof(str), fp);//データは2行下にあると推定　改行する
			fgets(str, sizeof(str), fp);//データは2行下にあると推定　改行する

			for (DWORD i = 0; i < pMesh->dwNumVert; i++)
			{
				fscanf_s(fp, "%f;%f;,", &x, &y);
				pvTexture[i].x = x;
				pvTexture[i].y = y;
			}
		}
	}

	//マテリアルリスト	
	fseek(fp, SEEK_SET, 0);
	while (!feof(fp))
	{
		ZeroMemory(key, sizeof(key));
		fscanf_s(fp, "%s ", key, sizeof(key));
		if (strcmp(key, "MeshMaterialList") == 0)
		{
			fgets(str, sizeof(str), fp);//空改行する
			fgets(str, sizeof(str), fp);//空改行する

			fgets(str, sizeof(str), fp);//リスト要素数
			int iNumFace = atoi(str);
			piMateriaList = new int[iNumFace];
			for (int i = 0; i < iNumFace; i++)
			{
				fgets(str, sizeof(str), fp);
				piMateriaList[i] = atoi(str);
			}
		}
	}
	//フェイス　読み込み　マテリアルごとに別のインデックスバッファーにする
	bool boFlag = false;
	for (DWORD i = 0; i < pMesh->dwNumMaterial; i++)
	{
		int* piFaceBuffer = new int[pMesh->dwNumFace * 3];//３頂点ポリゴンなので、1フェイス=3頂点(3インデックス)
		fseek(fp, SEEK_SET, 0);

		//同じマテリアルをもつポリゴンをまとめる		
		iCount = 0;
		for (DWORD k = 0; k < pMesh->dwNumFace; k++)
		{
			if (i == piMateriaList[k])//このk番目のポリゴンは、このi番マテリアルグループ
			{
				piFaceBuffer[iCount] = piFace[k * 3];
				piFaceBuffer[iCount + 1] = piFace[k * 3 + 1];
				piFaceBuffer[iCount + 2] = piFace[k * 3 + 2];
				pMesh->pMaterial[i].dwNumFace++;
				iCount += 3;
			}
		}
		if (iCount == 0) continue;

		//マテリアルの数だけインデックスバッファーを作成
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
	
	//頂点構造体にコピー
	for (DWORD i = 0; i < pMesh->dwNumVert; i++)
	{
		pvVertexBuffer[i].vPos = pvCoord[i];
		pvVertexBuffer[i].vNorm = pvNormal[i];
		pvVertexBuffer[i].vTex = pvTexture[i];
	}
	
	//バーテックスバッファーを作成
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

	//一時的な入れ物は、もはや不要
	delete piMateriaList;
	delete piFace;
	delete pvCoord;
	delete pvNormal;
	delete pvTexture;
	delete[] pvVertexBuffer;

	return S_OK;
}
