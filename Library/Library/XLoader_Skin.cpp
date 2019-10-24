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
		//ファイルオープン失敗
		return false;
	}

	//templateを省いたファイルの読み込み開始位置を保存
	long ReadStartPos = GetTemplateSkipStartPos(fp);

	//メッシュの読み込み
	if (LoadMesh(fp, &pSkinMesh->m_Mesh,ReadStartPos) == false)
	{
		//メッシュ読み込み失敗
		return false;
	}

	//ボーン読み込み
	if (LoadBone(fp, pSkinMesh, ReadStartPos) == false)
	{
		//ボーン読み込み失敗
		return false;
	}
	
	//スキン情報の読み込み
	if (LoadSkin(fp, pSkinMesh, ReadStartPos) == false)
	{
		//スキン情報読み込み失敗
		return false;
	}

	//アニメーション読み込み
	if (LoadAnimation(fp, pSkinMesh, ReadStartPos) == false)
	{
		//アニメーション読み込み失敗
		return false;
	}

	//ボーン情報とスキン情報をまとめる
	BoneWithSkin(pSkinMesh);

	return true;
}

//templateを飛ばした読み込み開始位置を取得する
long CX_Skin::GetTemplateSkipStartPos(FILE* fp)
{
	//ファイルの先頭にセット
	fseek(fp, 0, SEEK_SET);

	long pos;//ファイルの現在位置
	
	//キーワード読み込み
	char str[READ_ARRAY_SIZE];

	while (!feof(fp))
	{
		//ファイルの現在位置保存
		pos = ftell(fp);

		//キーワード 読み込み
		fscanf_s(fp, "%s ", str, sizeof(str));

		//Frameが出るまで飛ばす
		if (strcmp(str, "Frame") == 0)
		{
			return pos;
		}
	}
}

//メッシュ情報の読み込み
bool CX_Skin::LoadMesh(FILE* fp, MESH* pMesh,long lStartPos)
{
	//読み込みの開始位置にセット
	fseek(fp, lStartPos, SEEK_SET);

	int verNum	=0;//頂点数
	int faceNum	=0;//面の数
	int normNum	=0;//法線数
	int uvNum	=0;//UV数
	int mateNum	=0;//マテリアルの数
		
	//読み込み用（読み込み後破棄）
	int*		 pFaceOfMate= nullptr;//面の対応マテリアルのインデックス
	D3DXVECTOR3* pvPos		= nullptr;//頂点座標
	D3DXVECTOR3* pvNormal	= nullptr;//法線
	D3DXVECTOR2* pvTex		= nullptr;//テクスチャ座標
	VERTEX*		 pVertex	= nullptr;//頂点リスト

	//読み込み後そのまま使うもの
	FACE*		pFace		= pMesh->m_pFace;		//面のリスト
	MATERIAL*	pMaterial	= pMesh->m_pMaterial;	//マテリアルのリスト

	//読み込み用
	float x, y, z, w;
	int v1, v2, v3, v4;
	v1 = v2 = v3 = v4 = 0;

	//キーワード読み込み
	char str[READ_ARRAY_SIZE];

	while (!feof(fp))
	{
		//キーワード 読み込み
		fscanf_s(fp, "%s ", str, sizeof(str));

		//頂点
		if (strcmp(str, "Mesh") == 0)
		{
			//メッシュ名
			fgets(str, sizeof(str), fp);

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

			//;除去
			fgets(str, sizeof(str), fp);

			//面の数
			fgets(str, sizeof(str), fp);
			faceNum = atoi(str);
						
			//面のメモリ確保
			pFace = new FACE[faceNum];
		
			int faceOfVer = 0;//面を構成する頂点数

			//面読み込み
			for (int i = 0; i < faceNum; i++)
			{
				//面を構成する頂点の数
				fscanf_s(fp, "%d;", &faceOfVer);
				pFace[i].m_FaceOfVer = faceOfVer;

				//面構成のインデックスリストのメモリ確保
				pFace[i].m_pIndex = new int[faceOfVer];

				//三角ポリゴン
				if (faceOfVer == TRIANGLE_POLYGON)
				{
					fscanf_s(fp, "%d,%d,%d;,", &v1, &v2, &v3);
					pFace[i].m_pIndex[0] = v1;
					pFace[i].m_pIndex[1] = v2;
					pFace[i].m_pIndex[2] = v3;
				}
				//四角ポリゴン
				if (faceOfVer == QUAD_POLYGON)
				{
					fscanf_s(fp, "%d,%d,%d,%d;,", &v1, &v2, &v3, &v4);
					pFace[i].m_pIndex[0] = v1;
					pFace[i].m_pIndex[1] = v2;
					pFace[i].m_pIndex[2] = v3;
					pFace[i].m_pIndex[3] = v4;
				}
			}
		}
		//法線
		if (strcmp(str, "MeshNormals") == 0)
		{
			//{除去
			fgets(str, sizeof(str), fp);

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
			//{除去
			fgets(str, sizeof(str), fp);

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
			//{除去
			fgets(str, sizeof(str), fp);

			//マテリアルの数
			fgets(str, sizeof(str), fp);
			mateNum = atoi(str);

			//マテリアルメモリ確保
			pMaterial = new MATERIAL[mateNum];

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
	}

	//読み込みの開始位置に戻る
	fseek(fp, lStartPos, SEEK_SET);

	while (!feof(fp))
	{
		//キーワード 読み込み
		fscanf_s(fp, "%s ", str, sizeof(str));

		//マテリアル読み込み
		for (int i = 0; i < mateNum; i++)
		{
			if (strcmp(str, "Material") == 0)
			{
				//{除去
				fgets(str, sizeof(str), fp);

				//面の色
				fscanf_s(fp, "%f;%f;%f;%f;;", &x, &y, &z, &w);
				pMaterial[i].m_vFaceColor.x = x;
				pMaterial[i].m_vFaceColor.y = y;
				pMaterial[i].m_vFaceColor.z = z;
				pMaterial[i].m_vFaceColor.w = w;

				//スペキュラーのパワー
				fscanf_s(fp, "%f;", &pMaterial[i].m_fPower);

				//スペキュラー
				fscanf_s(fp, "%f;%f;%f;;", &x, &y, &z);
				pMaterial[i].m_vKs.x = x;
				pMaterial[i].m_vKs.y = y;
				pMaterial[i].m_vKs.z = z;

				//エミッシブ
				fscanf_s(fp, "%f;%f;%f;;", &x, &y, &z);
				pMaterial[i].m_vKe.x = x;
				pMaterial[i].m_vKe.y = y;
				pMaterial[i].m_vKe.z = z;

				//テクスチャ
				fscanf_s(fp, "%s ", str, sizeof(str));
				if (strcmp(str, "TextureFilename") == 0)
				{
					fgets(str, sizeof(str), fp);//{除去

					//読み込み最大サイズ
					int size = sizeof(pMaterial[i].m_TexName);

					//テクスチャ名
					fscanf_s(fp, "%s", pMaterial[i].m_TexName, size);

					//"と;を除去する
					ErasCharFromString(pMaterial->m_TexName,size, '\"');
					ErasCharFromString(pMaterial->m_TexName,size, ';');

					//テクスチャーを作成
					if (FAILED(D3DX10CreateShaderResourceViewFromFileA(DX->GetDevice(), pMaterial[i].m_TexName, NULL, NULL, &pMaterial[i].m_pTexture, NULL)))
					{
						//テクスチャ作成失敗
						return false;
					}
				}
			}
		}
	}

	//頂点構造体メモリ確保
	pVertex = new VERTEX[verNum];

	//頂点構造体にまとめる
	for (int i = 0; i < verNum; i++)
	{
		pVertex[i].m_vPos = pvPos[i];
		pVertex[i].m_vNorm = pvNormal[i];
		pVertex[i].m_vTex = pvTex[i];
	}

	//バーテックスバッファーを作成
	pMesh->m_pVertexBuffer = DRAW->BufferCreate(pVertex, sizeof(VERTEX) * verNum, D3D10_BIND_VERTEX_BUFFER);

	//マテリアル毎にインデックスバッファを作成
	for (int i = 0; i < mateNum;i++)
	{
		//そのマテリアルを使用する面の数を数える
		int UseFaceNum=0;
		for (int j = 0; j < faceNum; j++)
		{
			if (pFaceOfMate[j] == i)
			{
				UseFaceNum++;
			}
		}

		//面の数
		pMaterial[i].m_FaceNum = UseFaceNum;

		//面の数だけメモリ確保
		pMaterial[i].m_ppIndexBuffer = new ID3D10Buffer * [UseFaceNum];
		pMaterial[i].m_pVerNum = new int[UseFaceNum];

		//面ごとにインデックスバッファを作成
		for (int j = 0; j < faceNum; j++)
		{
			//このマテリアルを使用する面のみ
			if (pFaceOfMate[j] == i)
			{
				//インデックスバッファ作成
				pMaterial[i].m_ppIndexBuffer[j] = DRAW->BufferCreate(pFace[j].m_pIndex, sizeof(int) * pFace[j].m_FaceOfVer, D3D10_BIND_INDEX_BUFFER);

				//頂点数を保存
				pMaterial[i].m_pVerNum[j] = pFace[j].m_FaceOfVer;
			}
		}
	}
		
	//一時保存からメッシュポインタへデータを移す
	pMesh->m_MaterialNum = mateNum;
	pMesh->m_pMaterial = pMaterial;
	pMesh->m_FaceNum = faceNum;
	pMesh->m_pFace = pFace;

	//一時保存は破棄
	PointerRelease(pvPos);
	PointerRelease(pvNormal);
	PointerRelease(pvTex);
	PointerRelease(pVertex);
	PointerRelease(pFaceOfMate);

	return true;
}

//指定文字を文字列から消す
void CX_Skin::ErasCharFromString(char* pSource,int Size, char Erace)
{
	int count = 0;//除去カウント

	for (int i = 0; i < Size; i++)
	{
		if (pSource[i] == Erace)
		{
			//除去数をカウント
			count++;
		}
		else
		{
			//カウント分を詰めてコピー
			pSource[i - count] = pSource[i];
		}
	}
}

//ボーン読み込み
bool CX_Skin::LoadBone(FILE* fp, SKIN_MESH* pSkinMesh,long lStartPos)
{
	//ファイルの先頭にセット
	fseek(fp, lStartPos, SEEK_SET);

	int boneNum=0;	//ボーンの数
		
	//キーワード読み込み用
	char str[READ_ARRAY_SIZE];

	//ボーンの数を数える
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));
	
		if (strcmp(str, "Frame") == 0)
		{
			boneNum++;
		}
	}
	//ボーン数保存
	pSkinMesh->m_BoneNum = boneNum;

	if (boneNum == 0)
	{
		//ボーンなし
		return false;
	}
	
	//ファイルの先頭にセット
	fseek(fp, 0, SEEK_SET);

	int start_count = 0;//{を数える
	int end_count	= 0;//}を数える
		
	//ボーンリストメモリ確保
	pSkinMesh->m_pBone = new BONE[boneNum];

	int boneIndex = 0;//インデックスカウンター

	//ボーン読み込み
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//ボーン
		if (strcmp(str, "Frame") == 0)
		{
			//ボーンをリストに保存
			BONE bone= LoadBoneInfo(fp, &boneIndex,pSkinMesh);
			pSkinMesh->m_pBone[bone.m_index]=bone; 
		}
	}

	return true;
}

//ボーン情報の読み込み
BONE CX_Skin::LoadBoneInfo(FILE* fp, int* pBoneIndex,SKIN_MESH* pSkinMesh)
{
	//関数呼び出し時のファイルの位置を保存
	long ReadStartPos = ftell(fp);

	int start_count = 0;//{カウント
	int end_count	= 0;//}カウント
	int childNum	= 0;//子ボーンの数

	//読み込み用
	char str[READ_ARRAY_SIZE];

	//ボーン読み込み用
	BONE bone;

	//自身のインデックス
	bone.m_index = *(pBoneIndex);
	
	//先に子ボーンの数を数える
	while (start_count != end_count || start_count == 0 || end_count == 0)
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//{カウント
		if (strcmp(str, "{") == 0)
		{
			start_count++;
		}

		//}カウント
		if (strcmp(str, "}") == 0)
		{
			end_count++;
		}
		//子ボーン
		if (strcmp(str, "Frame") == 0)
		{
			//このボーンの直属の子
			if (start_count - end_count == 1)
			{
				childNum++;
			}
			else
			{
				;//孫ボーンなどはカウントしない
			}
		}
	}

	//この関数が呼ばれたと時の位置に戻す
	fseek(fp, ReadStartPos, SEEK_SET);

	//子の数を保存
	bone.m_ChildNum = childNum;

	//子ボーンのインデックスリストメモリ確保
	bone.m_pChildIndex = new int[childNum];

	//カウンタを初期化
	start_count = 0;
	end_count = 0;
	childNum = 0;

	//ボーン名
	fscanf_s(fp, "%s", bone.m_Name, sizeof(bone.m_Name));

	//本読み込み
	while (start_count != end_count || start_count == 0 || end_count == 0)
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//{カウント
		if (strcmp(str, "{") == 0)
		{
			start_count++;
		}

		//}カウント
		if (strcmp(str, "}") == 0)
		{
			end_count++;
		}

		//初期ポーズ
		if (strcmp(str, "FrameTransformMatrix") == 0)
		{
			//{除去
			fgets(str, sizeof(str), fp);

			//行列読み込み
			D3DXMATRIX mat;
			fgets(str, sizeof(str), fp);
			sscanf_s(str, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f;;",
				&mat._11, &mat._12, &mat._13, &mat._14,
				&mat._21, &mat._22, &mat._23, &mat._24,
				&mat._31, &mat._32, &mat._33, &mat._34,
				&mat._41, &mat._42, &mat._43, &mat._44);

			//保存
			bone.m_matBindPose = mat;

			//}除去
			fgets(str, sizeof(str), fp);
		}

		//子ボーン
		if (strcmp(str, "Frame") == 0)
		{
			//ボーンのインデックス更新
			*pBoneIndex = *(pBoneIndex)+1;

			//子ボーンのインデックスを保存
			bone.m_pChildIndex[childNum++] = *pBoneIndex;

			//ボーンをリストに保存
			BONE read = LoadBoneInfo(fp, pBoneIndex, pSkinMesh);
			pSkinMesh->m_pBone[read.m_index] = read;
		}
	}

	return bone;
}

//スキン情報の読み込み
bool CX_Skin::LoadSkin(FILE* fp, SKIN_MESH* pSkinMesh,long lStartPos)
{
	//読み込み開始位置にセットする
	fseek(fp, lStartPos, SEEK_SET);

	//スキンウェイトの数
	int SkinWeightNum = 0;

	//キーワード読み込み用
	char str[READ_ARRAY_SIZE];

	//スキンウェイトの数をカウントする
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		if (strcmp(str, "SkinWeights") == 0)
		{
			SkinWeightNum++;
		}
	}

	if (SkinWeightNum == 0)
	{
		//スキンウェイトなし
		return false;
	}

	//読み込み開始位置まで戻る
	fseek(fp, lStartPos, SEEK_SET);

	//ウェイト数
	pSkinMesh->m_WeightNum = SkinWeightNum;

	//スキンウェイトメモリ確保
	pSkinMesh->m_pSkinWeight = new SKIN_WEIGHT[SkinWeightNum];
		
	//読み込み用
	char boneName[NAME_ARRAY_SIZE];	//ボーン名
	int count		= 0;			//カウンター		
	int weightNum	= 0;			//ウェイト数
	D3DXMATRIX mat;					//行列

	//読み込み
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//スキンウェイト
		if (strcmp(str, "SkinWeights") == 0)
		{
			fgets(str, sizeof(str), fp);//{除去
			
			//ボーン名
			fscanf_s(fp, "%s", boneName,sizeof(boneName));

			//"と;を除去する
			ErasCharFromString(boneName, sizeof(boneName), '\"');
			ErasCharFromString(boneName, sizeof(boneName), ';');

			//保存
			strcpy_s(pSkinMesh->m_pSkinWeight[count].m_BoneName, boneName);
						
			//ウェイトの数
			fscanf_s(fp, "%d;", &weightNum);		
			pSkinMesh->m_pSkinWeight[count].m_WeightNum = weightNum;

			//インデックスとウェイトのメモリ確保
			pSkinMesh->m_pSkinWeight[count].m_pIndex = new int[weightNum];
			pSkinMesh->m_pSkinWeight[count].m_pWeight = new float[weightNum];

			//インデックス読み込み
			for (int i = 0; i < weightNum; i++)
			{
				fscanf_s(fp, "%d", &pSkinMesh->m_pSkinWeight[count].m_pIndex[i]);

				//,または;の除去
				fgets(str, sizeof(str), fp);
			}

			//ウェイト読み込み
			for (int i = 0; i < weightNum; i++)
			{
				fscanf_s(fp, "%f" ,&pSkinMesh->m_pSkinWeight[count].m_pWeight[i]);

				//,または;の除去
				fgets(str, sizeof(str), fp);
			}
			//オフセット行列
			fscanf_s(fp,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f;;",
				&mat._11, &mat._12, &mat._13, &mat._14,
				&mat._21, &mat._22, &mat._23, &mat._24,
				&mat._31, &mat._32, &mat._33, &mat._34,
				&mat._41, &mat._42, &mat._43, &mat._44);
			pSkinMesh->m_pSkinWeight[count].m_matOffset = mat;

			//カウンター更新
			count++;
		}
	}
	return true;
}

//アニメーション読み込み
bool CX_Skin::LoadAnimation(FILE* fp, SKIN_MESH* pSkinMesh, long lStartPos)
{
	//読み込み開始位置にセットする
	fseek(fp, lStartPos, SEEK_SET);

	//読み込み用
	char str[READ_ARRAY_SIZE];

	//アニメーションの数を数える
	int animeNum = 0;
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		if (strcmp(str, "Animation") == 0)
		{
			animeNum++;
		}
	}

	//アニメーションの数保存
	pSkinMesh->m_AnimeNum = animeNum;

	if (animeNum == 0)
	{
		//アニメーションなし
		return false;
	}

	//読み込み開始位置に戻す
	fseek(fp, lStartPos, SEEK_SET);

	//読み込み用
	char animeName[NAME_ARRAY_SIZE];	//アニメーション名
	char boneName[NAME_ARRAY_SIZE];		//対応ボーン名
	int keyNum		= 0;				//キーの数
	int animeCount	= 0;				//アニメーションカウンター
	int keyCount	= 0;				//キーカウンター
	KEY key;							//キー
	
	//アニメーションメモリ確保
	pSkinMesh->m_pAnimation = new ANIMATION[animeNum];

	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//アニメーション
		if (strcmp(str, "Animation") == 0)
		{
			//アニメーション名
			fscanf_s(fp, "%s ", animeName, sizeof(animeName));
			strcpy_s(pSkinMesh->m_pAnimation[animeCount].m_Name, animeName);

			//{除去
			fgets(str, sizeof(str), fp);
			
			//対応ボーン名
			fgets(boneName, sizeof(boneName), fp);

			//{と}、空白を除去
			ErasCharFromString(boneName, sizeof(boneName), '{');
			ErasCharFromString(boneName, sizeof(boneName), '}');
			ErasCharFromString(boneName, sizeof(boneName), ' ');
						
			//保存
			strcpy_s(pSkinMesh->m_pAnimation[animeCount].m_AffectBoneName,boneName);

			//1行飛ばす
			fgets(str, sizeof(str), fp);//AnimationKey{
			
			//キータイプ
			fscanf_s(fp, "%d;", &pSkinMesh->m_pAnimation[animeCount].m_KeyType);

			//キーの数
			fscanf_s(fp, "%d;", &keyNum);
			pSkinMesh->m_pAnimation[animeCount].m_KeyNum = keyNum;

			//キーのメモリ確保
			pSkinMesh->m_pAnimation[animeCount].m_pKey = new KEY[keyNum];

			//キーの読み込み
			for (int i = 0; i < keyNum; i++)
			{
				//コマ
				fscanf_s(fp, "%d;", &key.m_Time);
				
				//データの数
				fscanf_s(fp, "%d;", &key.m_ValueNum);
				
				//データのメモリ確保
				key.m_pValue = new float[key.m_ValueNum];

				//カウンター初期化
				keyCount = 0;

				//最初の一つ
				fscanf_s(fp, "%f", &key.m_pValue[keyCount++]);
				
				//2つ目以降は,も含めて読む
				while(keyCount<key.m_ValueNum)
				{
					fscanf_s(fp, ",%f", &key.m_pValue[keyCount++]);
				}

				//;;,を除去
				fgets(str, sizeof(str), fp);

				//キーの保存
				pSkinMesh->m_pAnimation[animeCount].m_pKey[i] = key;
			}

			//カウンター更新
			animeCount++;
		}
	}
}

//ボーンとスキン情報をまとめる
void CX_Skin::BoneWithSkin(SKIN_MESH* pSkinMesh)
{
	//ウェイト数
	int weightNum = pSkinMesh->m_WeightNum;	

	//ボーン数
	int boneNum = pSkinMesh->m_BoneNum;

	//メモリ確保
	pSkinMesh->m_pSkinBone = new SKIN_BONE[weightNum];

	//スキン情報から対応ボーンを探す
	for (int i = 0; i < weightNum; i++)
	{
		//スキン情報を持つべきボーン名
		char HaveBoneName[NAME_ARRAY_SIZE];
		strcpy_s(HaveBoneName, pSkinMesh->m_pSkinWeight[i].m_BoneName);

		bool bFind = false;
		for (int j = 0; j < boneNum && bFind == false; j++)
		{
			//対応ボーンを発見
			if (strcmp(HaveBoneName, pSkinMesh->m_pBone[j].m_Name) == 0)
			{
				bFind=true;
				BONE bone = pSkinMesh->m_pBone[j];								
				SKIN_WEIGHT skin_weight = pSkinMesh->m_pSkinWeight[i];

				//ボーン情報とスキン情報の統合を行う
				SKIN_BONE skin_bone;
				strcpy_s(skin_bone.m_Name, bone.m_Name);		//ボーン名
				skin_bone.m_index = bone.m_index;				//ボーン自身のインデックス
				skin_bone.m_ChildNum = bone.m_ChildNum;			//子の数
				skin_bone.m_pChildIndex = bone.m_pChildIndex;	//このインデックスリスト
				skin_bone.m_matBindPose = bone.m_matBindPose;	//初期ポーズ
				skin_bone.m_WeightNum = skin_weight.m_WeightNum;//ウェイト数
				skin_bone.m_pIndex = skin_weight.m_pIndex;		//ウェイトリスト
				skin_bone.m_pWeight = skin_weight.m_pWeight;	//影響する頂点のインデックスリスト
				skin_bone.m_matOffset = skin_weight.m_matOffset;//オフセット

				//保存する
				pSkinMesh->m_pSkinBone[i] = skin_bone;
				int a = 0;
			}
		}
	}	
}

//メッシュ描画
void CX_Skin::DrawMesh(D3DMATRIX matWorld, SKIN_MESH* pSkinMesh, CColorData* pColor)
{
	//バーテックスバッファーをセット）
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	DX->GetDevice()->IASetVertexBuffers(0, 1, &pSkinMesh->m_Mesh.m_pVertexBuffer, &stride, &offset);

	//マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
	for (int i = 0; i < pSkinMesh->m_Mesh.m_MaterialNum; i++)
	{
		//面ごとに描画
		for (int j = 0; j < pSkinMesh->m_Mesh.m_pMaterial[i].m_FaceNum; j++)
		{
			//インデックスバッファーをセット
			stride = sizeof(int);
			offset = 0;
			DX->GetDevice()->IASetIndexBuffer(pSkinMesh->m_Mesh.m_pMaterial[i].m_ppIndexBuffer[j], DXGI_FORMAT_R32_UINT, 0);

			//プリミティブ・トポロジーをセット
			if (pSkinMesh->m_Mesh.m_pMaterial[i].m_pVerNum[j] == TRIANGLE_POLYGON)
			{
				//三角ポリゴン
				DX->GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			}
			if (pSkinMesh->m_Mesh.m_pMaterial[i].m_pVerNum[j] == QUAD_POLYGON)
			{
				//四角ポリゴン
				DX->GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			}

			//シェーダセット
			SHADER->SetShader(pSkinMesh->m_Mesh.m_pMaterial[i].m_pTexture, NULL, pColor, matWorld);

			D3D10_TECHNIQUE_DESC dc;
			SHADER->GetTechnique()->GetDesc(&dc);
			for (UINT p = 0; p < dc.Passes; ++p)
			{
				SHADER->GetTechnique()->GetPassByIndex(p)->Apply(0);
				DX->GetDevice()->DrawIndexed(pSkinMesh->m_Mesh.m_pMaterial[i].m_pVerNum[j], 0, 0);
			}
		}
	}
}

//開放
void CX_Skin::Release()
{
	//インスタンス破棄
	PointerRelease(m_pInstance);
}