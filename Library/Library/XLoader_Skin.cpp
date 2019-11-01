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

	MESH	Mesh;				//メッシュ
	int		BoneNum			= 0;//ボーン数
	int		SkinWeightNum	= 0;//スキンウェイトの数
	int		AnimeNum		= 0;//アニメーションの数

	BONE*			pBone		= nullptr;	//ボーンリスト
	SKIN_WEIGHT*	pSkinWeight	= nullptr;	//スキンウェイトリスト
	ANIMATION*		pAnime		= nullptr;	//アニメションリスト

	SKIN_MESH_HEADER SkinHeader;	//スキンメッシュヘッダー

	//メッシュの読み込み
	if (LoadMesh(fp, &Mesh,ReadStartPos) == false)
	{
		//メッシュ読み込み失敗
		return false;
	}
	
	//ボーン数取得
	BoneNum = GetBoneNum(fp, ReadStartPos);

	//スキンメッシュヘッダー読み込み
	LoadSkinMeshHeader(fp, &SkinHeader, ReadStartPos);

	//ボーンがあるときのみ
	if (BoneNum != 0)
	{
		//ボーンリストメモリ確保
		pBone = new BONE[BoneNum];

		//ボーン読み込み
		if (LoadBone(fp, pBone, ReadStartPos) == false)
		{
			//ボーン読み込み失敗
			return false;
		}
	}

	//スキンウェイトの数を数える
	SkinWeightNum = GetSkinWeightNum(fp, ReadStartPos);

	//スキンウェイトがあるときのみ
	if (SkinWeightNum != 0)
	{
		//スキンウェイトメモリ確保
		pSkinWeight = new SKIN_WEIGHT[SkinWeightNum];
		
		//スキン情報の読み込み
		if (LoadSkinWeight(fp, pSkinWeight, ReadStartPos) == false)
		{
			//スキン情報読み込み失敗
			return false;
		}
	}

	//アニメーションの数取得
	AnimeNum = GetAnimeNum(fp, ReadStartPos);

	//アニメーションがあるときのみ
	if (AnimeNum != 0)
	{
		//アニメーションメモリ確保
		pAnime = new ANIMATION[AnimeNum];

		//アニメーション読み込み
		if (LoadAnimation(fp, pAnime, ReadStartPos) == false)
		{
			//アニメーション読み込み失敗
			return false;
		}
	}

	//スキンメッシュにまとめる
	SkinMeshPutTogether(Mesh, pBone, BoneNum,pSkinWeight, SkinWeightNum, pAnime,AnimeNum, pSkinMesh,SkinHeader);
	
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
	
	//読み込み後そのまま使うもの
	FACE*		pFace		= nullptr;//面のリスト
	MATERIAL*	pMaterial	= nullptr;//マテリアルのリスト
	VERTEX*		pVertex		= nullptr;//頂点リスト

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
	pMesh->m_pVertex = pVertex;

	//一時保存は破棄
	PointerRelease(pvPos);
	PointerRelease(pvNormal);
	PointerRelease(pvTex);
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

//スキンメッシュヘッダー読み込み
void CX_Skin::LoadSkinMeshHeader(FILE* fp, SKIN_MESH_HEADER* pSkinHeader, long lStartPos)
{
	//ファイルの先頭にセット
	fseek(fp, lStartPos, SEEK_SET);

	//キーワード読み込み用
	char str[READ_ARRAY_SIZE];

	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//スキンメッシュヘッダー読み込み
		if (strcmp(str, "XSkinMeshHeader") == 0)
		{
			//{除去
			fscanf_s(fp, "%s", str, sizeof(str));

			//頂点の最大ウェイト数
			fscanf_s(fp, "%d;", &pSkinHeader->m_MaxVertex);
			
			//面の最大ウェイト数
			fscanf_s(fp, "%d;", &pSkinHeader->m_MaxFace);
			
			//ボーン数
			fscanf_s(fp, "%d;", &pSkinHeader->m_BoneNum);
			
			//}除去
			fscanf_s(fp, "%s", str, sizeof(str));
		}
	}
}


//ボーン数の取得
int CX_Skin::GetBoneNum(FILE* fp, long lStartPos)
{
	//ファイルの先頭にセット
	fseek(fp, lStartPos, SEEK_SET);

	int boneNum = 0;	//ボーンの数
	
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

	return boneNum;
}

//ボーン読み込み
bool CX_Skin::LoadBone(FILE* fp, BONE* pBone, long lStartPos)	
{
	//ファイルの先頭にセット
	fseek(fp, lStartPos, SEEK_SET);

	int boneNum=0;	//ボーンの数
		
	//キーワード読み込み用
	char str[READ_ARRAY_SIZE];
	
	int start_count = 0;//{を数える
	int end_count	= 0;//}を数える
		
	int boneIndex = 0;//インデックスカウンター

	//ボーン読み込み
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//ボーン
		if (strcmp(str, "Frame") == 0)
		{
			//ボーンをリストに保存
			BONE bone= LoadBoneInfo(fp, &boneIndex, pBone);
			pBone[bone.m_index]=bone;
		}
	}

	return true;
}

//ボーン情報の読み込み
BONE CX_Skin::LoadBoneInfo(FILE* fp, int* pBoneIndex, BONE* pBone)
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
			BONE read = LoadBoneInfo(fp, pBoneIndex, pBone);
			pBone[read.m_index] = read;
		}
	}

	return bone;
}

//スキンウェイトの数を取得
int CX_Skin::GetSkinWeightNum(FILE* fp, long lStartPos)
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

	return SkinWeightNum;
}

//スキンウェイトの読み込み
bool CX_Skin::LoadSkinWeight(FILE* fp, SKIN_WEIGHT* pSkinWeight, long lStartPos)
{
	//読み込み開始位置にセットする
	fseek(fp, lStartPos, SEEK_SET);

	//キーワード読み込み用
	char str[READ_ARRAY_SIZE];
		
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
			strcpy_s(pSkinWeight[count].m_BoneName, boneName);
						
			//ウェイトの数
			fscanf_s(fp, "%d;", &weightNum);		
			pSkinWeight[count].m_WeightNum = weightNum;

			//インデックスとウェイトのメモリ確保
			pSkinWeight[count].m_pIndex = new int[weightNum];
			pSkinWeight[count].m_pWeight = new float[weightNum];

			//インデックス読み込み
			for (int i = 0; i < weightNum; i++)
			{
				fscanf_s(fp, "%d", &pSkinWeight[count].m_pIndex[i]);

				//,または;の除去
				fgets(str, sizeof(str), fp);
			}

			//ウェイト読み込み
			for (int i = 0; i < weightNum; i++)
			{
				fscanf_s(fp, "%f" ,&pSkinWeight[count].m_pWeight[i]);

				//,または;の除去
				fgets(str, sizeof(str), fp);
			}
			//オフセット行列
			fscanf_s(fp,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f;;",
				&mat._11, &mat._12, &mat._13, &mat._14,
				&mat._21, &mat._22, &mat._23, &mat._24,
				&mat._31, &mat._32, &mat._33, &mat._34,
				&mat._41, &mat._42, &mat._43, &mat._44);
			pSkinWeight[count].m_matOffset = mat;

			//カウンター更新
			count++;
		}
	}
	return true;
}

//アニメーションの数を取得
int CX_Skin::GetAnimeNum(FILE* fp, long lStartPos)
{
	//読み込み開始位置にセットする
	fseek(fp, lStartPos, SEEK_SET);

	//アニメーションの数
	int animeNum = 0;

	//キーワード読み込み用
	char str[READ_ARRAY_SIZE];

	//アニメーションの数を数える
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		if (strcmp(str, "AnimationSet") == 0)
		{
			animeNum++;
		}
	}
	return animeNum;
}

//ボーン毎のキー情報の読み込み
BONE_KEY CX_Skin::LoadBoneKey(FILE* fp)
{
	
	BONE_KEY Out;

	//キーワード読み込み用
	char str[READ_ARRAY_SIZE];

	//対応ボーン名
	char boneName[NAME_ARRAY_SIZE];	

	//読み込み用初期化
	int keyNum = 0;
	int keyCount = 0;
	int valueNum = 0;

	//AnimationX 
	fscanf_s(fp, "%s ", str, sizeof(str));

	//{除去
	fgets(str, sizeof(str), fp);

	//対応ボーン名
	fgets(boneName, sizeof(boneName), fp);

	//{と}、空白、\nを除去
	ErasCharFromString(boneName, sizeof(boneName), '{');
	ErasCharFromString(boneName, sizeof(boneName), '}');
	ErasCharFromString(boneName, sizeof(boneName), ' ');
	ErasCharFromString(boneName, sizeof(boneName), '\n');

	//保存
	strcpy_s(Out.m_AffectBoneName, boneName);

	//1行飛ばす
	fgets(str, sizeof(str), fp);//AnimationKey{

	//キータイプ
	fscanf_s(fp, "%d;", &Out.m_KeyType);

	//キーの数
	fscanf_s(fp, "%d;", &keyNum);
	Out.m_KeyNum = keyNum;

	//キーのメモリ確保
	Out.m_pKey = new KEY[keyNum];

	//キーの読み込み
	for (int i = 0; i < keyNum; i++)
	{
		//コマ
		fscanf_s(fp, "%d;", &Out.m_pKey[i].m_Time);

		//データの数
		fscanf_s(fp, "%d;", &valueNum);
		Out.m_pKey[i].m_ValueNum = valueNum;

		//データのメモリ確保
		Out.m_pKey[i].m_pfValue = new float[valueNum];

		//カウンター初期化
		keyCount = 0;

		//最初の一つ
		fscanf_s(fp, "%f", &Out.m_pKey[i].m_pfValue[keyCount++]);

		//2つ目以降は,も含めて読む
		while (keyCount < Out.m_pKey[i].m_ValueNum)
		{
			fscanf_s(fp, ",%f",&Out.m_pKey[i].m_pfValue[keyCount++]);
		}

		//;;,を除去
		fgets(str, sizeof(str), fp);
	}	
	return Out;
}

//アニメーション読み込み
bool CX_Skin::LoadAnimation(FILE* fp, ANIMATION* pAnime, long lStartPos)
{
	//読み込み開始位置にセットする
	fseek(fp, lStartPos, SEEK_SET);

	//キーワード読み込み用
	char str[READ_ARRAY_SIZE];

	//読み込み用
	char animeName[NAME_ARRAY_SIZE];	//アニメーション名
	char boneName[NAME_ARRAY_SIZE];		//対応ボーン名
	int animeCount = 0;					//アニメーションカウンター
	
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//アニメーション
		if (strcmp(str, "AnimationSet") == 0)
		{
			//アニメーション名
			fscanf_s(fp, "%s ", animeName, sizeof(animeName));
			strcpy_s(pAnime[animeCount].m_Name, animeName);

			//ファイルの位置を保存
			long ReadStartPos = ftell(fp);

			int boneNum = 0;
			int start_count = 0;
			int end_count = 0;

			//先に対応ボーンの数を数える
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

				//対応ボーン
				if (strcmp(str, "Animation") == 0)
				{
					boneNum++;
				}
			}

			//対応ボーン数保存
			pAnime[animeCount].m_BoneKeyNum = boneNum;

			//メモリ確保
			pAnime[animeCount].m_pBoneKey = new BONE_KEY[boneNum];

			//アニメーション読み込み位置にセットする
			fseek(fp, ReadStartPos, SEEK_SET);

			//対応ボーン分読み込み
			int boneCount = 0;
			while(!feof(fp)&&(boneCount<boneNum))
			{
				fscanf_s(fp, "%s ", str, sizeof(str));

				if (strcmp(str, "Animation") == 0)
				{
					//ボーン毎のキー読み込み
					pAnime[animeCount].m_pBoneKey[boneCount] = LoadBoneKey(fp);
					boneCount++;
				}
			}
			//カウンター更新
			animeCount++;
		}
	}
	return true;
}

//スキンメッシュにまとめる
void CX_Skin::SkinMeshPutTogether(MESH Mesh, BONE* pBone, int BoneNum, SKIN_WEIGHT* pSkinWeight,int WeightNum ,ANIMATION* pAnimation, int AnimeNum, SKIN_MESH* pSkinMesh,SKIN_MESH_HEADER SkinHeader)
{
	pSkinMesh->m_Mesh		= Mesh;			//メッシュ
	pSkinMesh->m_BoneNum	= BoneNum;		//ボーン数
	pSkinMesh->m_pBone		= pBone;		//ボーン
	pSkinMesh->m_WeightNum	= WeightNum;	//ウェイト数	
	pSkinMesh->m_pWeight	= pSkinWeight;	//ウェイトリスト
	pSkinMesh->m_AnimeNum	= AnimeNum;		//アニメーションの数
	pSkinMesh->m_pAnimation	= pAnimation;	//アニメーション
	pSkinMesh->m_SkinHeader = SkinHeader;	//スキンヘッダー
}

//フレーム補完
KEY CX_Skin::FrameComplement(int NowFrame, BONE_KEY BoneKey)
{
	KEY out;

	int keyNum = BoneKey.m_KeyNum;
	int* pFrameDiff = new int[keyNum];

	bool bKey = false;

	//フレームの間隔差を保存
	for (int i = 0; i <keyNum; i++)
	{
		pFrameDiff[i] = BoneKey.m_pKey[i].m_Time - NowFrame;

		//現在のフレームがキーフレームの場合
		if (pFrameDiff[i] == 0)
		{
			bKey = true;
			out = BoneKey.m_pKey[i];
		}
	}

	//キーフレーム以外なら補完する
	if(bKey==false)
	{
		KEY before= BoneKey.m_pKey[0];
		KEY after= BoneKey.m_pKey[keyNum-1];
		
		//前フレームを探す
		for (int i = 0; i < keyNum; i++)
		{
			//差がマイナスの場合前フレーム
			if (pFrameDiff[i] < 0)
				before = BoneKey.m_pKey[i];
		}

		//後フレームを探す
		for (int i = keyNum - 1; i >= 0; i--)
		{
			//差がプラスなら後フレーム
			if (pFrameDiff[i] > 0)
				after = BoneKey.m_pKey[i];
		}

		//フレーム差を求める
		int FrameDiff = after.m_Time - before.m_Time;

		//変化の割合を求める
		float fPercent=	(float)(NowFrame - before.m_Time) / FrameDiff;

		//差分ポーズのメモリ確保
		float* pfPoseDiff = new float[before.m_ValueNum];

		//前後フレームでのポーズ差(前後フレームのデータ数は同じと仮定する)
		for (int i = 0; i < before.m_ValueNum; i++)
		{			
			//ポーズの差分を求める
			pfPoseDiff[i] = after.m_pfValue[i] - before.m_pfValue[i];
		}

		//値の数保存
		out.m_ValueNum = before.m_ValueNum;

		//メモリの確保
		out.m_pfValue = new float[out.m_ValueNum];

		//現在のポーズを求める
		for (int i = 0; i < out.m_ValueNum ; i++)
		{
			out.m_pfValue[i] = before.m_pfValue[i] + (pfPoseDiff[i] * fPercent);
		}

		//ポーズの差分破棄
		delete[] pfPoseDiff;
	}

	//現在フレーム保存
	out.m_Time = NowFrame;

	//フレーム差分破棄
	delete[] pFrameDiff;

	return out;
}

//アニメーション
void CX_Skin::Animation(int AnimeId,int NowFrame,SKIN_MESH* pSkinMesh)
{
	ANIMATION anime = pSkinMesh->m_pAnimation[AnimeId];

	//対応ボーン名保存用
	char affectName[NAME_ARRAY_SIZE];
	
	//そのフレームのポーズ
	KEY NowPose;
	D3DXMATRIX matNowPose;
	
	//対応ボーンごとボーンのポーズを求める
	for (int i = 0; i < anime.m_BoneKeyNum; i++)
	{
		//対応ボーン名
		strcpy_s(affectName, anime.m_pBoneKey[i].m_AffectBoneName);

		//フレーム補完
		NowPose = FrameComplement(NowFrame, anime.m_pBoneKey[i]);

		//ポーズを行列にする
		matNowPose = D3DXMATRIX(NowPose.m_pfValue);

		//対応ボーンを探す
		bool bFind = false;
		int boneID = -1;
		for (int i = 0; i < pSkinMesh->m_BoneNum && bFind == false; i++)
		{
			//対応ボーン発見
			if (strcmp(pSkinMesh->m_pBone[i].m_Name, affectName) == 0)
			{
				bFind = true;
				boneID = i;
			}
		}

		//ボーンにポーズを保存
		pSkinMesh->m_pBone[boneID].m_matNewPose = matNowPose;
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