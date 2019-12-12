#include "PmxLoader.h"
#include "Binary.h"

//インスタンス
CPmxLoader* CPmxLoader::m_pInstance = nullptr;

//読み込み
bool CPmxLoader::Load(const char* FileName, PMX_DATA* pPmxData)
{
	//ファイルオープン
	FILE* fp = nullptr;
	fopen_s(&fp, FileName, "rb");

	//ファイルオープン失敗
	if (fp == nullptr)
		return false;

	//読み込み用
	unsigned char Data[4];

	//ファイルタイプ
	fread_s(pPmxData->m_Head.m_FileType, sizeof(pPmxData->m_Head.m_FileType), sizeof(pPmxData->m_Head.m_FileType), 1, fp);

	//バージョン
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_Head.m_Ver = StrToFloat(Data, sizeof(Data));

	//ファイルタイプ
	fread_s(&pPmxData->m_Head.m_Size, sizeof(pPmxData->m_Head.m_Size), sizeof(pPmxData->m_Head.m_Size), 1, fp);

	//バイト列サイズ
	int Size=pPmxData->m_Head.m_Size;
	pPmxData->m_Head.m_pData = new unsigned char[Size];
	
	//バイト列読み込み
	for (int i = 0; i < Size; i++)
	{
		//ファイルタイプ
		fread_s(&pPmxData->m_Head.m_pData[i], sizeof(unsigned char), sizeof(unsigned char), 1, fp);		
	}

	//モデル情報読み込み
	ModelInfoLoad(fp, pPmxData);

	//頂点数
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_VerNum = StrToInt(Data, sizeof(Data));

	//頂点なし
	if (pPmxData->m_VerNum <= 0)
	{
		return false;
	}

	//頂点メモリ確保
	pPmxData->m_pVertex = new PMX_VERTEX[pPmxData->m_VerNum];
		
	//頂点読み込み
	VertexLoad(fp, pPmxData);
	
	//面のサイズ
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_FaceNum = StrToInt(Data, sizeof(Data));

	//3頂点で1面
	pPmxData->m_FaceNum /= 3;

	//面なし
	if (pPmxData->m_FaceNum <= 0)
	{
		return false;
	}

	//面データメモリ確保
	pPmxData->m_pFace = new PMX_FACE[pPmxData->m_FaceNum];

	//面データ読み込み
	FaceLoad(fp, pPmxData);
		
	//テクスチャ数読み込み
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_TexNum = StrToInt(Data, sizeof(Data));

	//テクスチャなし
	if (pPmxData->m_TexNum <= 0)
	{
		return false;
	}

	//テクスチャデータメモリ確保
	pPmxData->m_pTex = new PMX_TEXTURE[pPmxData->m_TexNum];

	//テクスチャ読み込み
	TextureLoad(fp, pPmxData);

	//マテリアル数
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_MaterialNum = StrToInt(Data, sizeof(Data));

	//マテリアルなし
	if (pPmxData->m_MaterialNum <= 0)
	{
		return false;
	}

	//マテリアルメモリ確保
	pPmxData->m_pMaterial = new PMX_MATERIAL[pPmxData->m_MaterialNum];

	//マテリアル読み込み
	MaterialLoad(fp, pPmxData);

	return true;
}

//モデル情報読み込み
void CPmxLoader::ModelInfoLoad(FILE* fp, PMX_DATA* pPmxData)
{
	unsigned char Data[4];

	//モデル名(日)サイズ
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_ModelInfo.m_NameJpnSize = StrToInt(Data,sizeof(Data));

	//メモリ確保
	pPmxData->m_ModelInfo.m_pNameJap = new unsigned char[pPmxData->m_ModelInfo.m_NameJpnSize];

	//モデル名(日)読み込み
	fread_s(pPmxData->m_ModelInfo.m_pNameJap, pPmxData->m_ModelInfo.m_NameJpnSize, pPmxData->m_ModelInfo.m_NameJpnSize, 1, fp);
	
	//文字列から\0を消す
	ErasCharFromString(pPmxData->m_ModelInfo.m_pNameJap, pPmxData->m_ModelInfo.m_NameJpnSize, '\0');

	//モデル名(英)サイズ
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_ModelInfo.m_NameEngSize = StrToInt(Data, sizeof(Data));

	//メモリ確保
	pPmxData->m_ModelInfo.m_pNameEng = new unsigned char[pPmxData->m_ModelInfo.m_NameEngSize];

	//モデル名(英)読み込み
	fread_s(pPmxData->m_ModelInfo.m_pNameEng, pPmxData->m_ModelInfo.m_NameEngSize, pPmxData->m_ModelInfo.m_NameEngSize, 1, fp);

	//文字列から\0を消す
	ErasCharFromString(pPmxData->m_ModelInfo.m_pNameEng, pPmxData->m_ModelInfo.m_NameEngSize, '\0');

	//コメント(日)サイズ
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_ModelInfo.m_CommentJpnSize = StrToInt(Data,sizeof(Data));

	//メモリ確保
	pPmxData->m_ModelInfo.m_pCommentJap = new unsigned char[pPmxData->m_ModelInfo.m_CommentJpnSize];

	//コメント(日)読み込み
	fread_s(pPmxData->m_ModelInfo.m_pCommentJap, pPmxData->m_ModelInfo.m_CommentJpnSize, pPmxData->m_ModelInfo.m_CommentJpnSize, 1, fp);

	//文字列から\0を消す
	ErasCharFromString(pPmxData->m_ModelInfo.m_pCommentJap, pPmxData->m_ModelInfo.m_CommentJpnSize, '\0');

	//コメント(英)サイズ
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_ModelInfo.m_CommentEngSize = StrToInt(Data, sizeof(Data));

	//メモリ確保
	pPmxData->m_ModelInfo.m_pCommentEng = new unsigned char[pPmxData->m_ModelInfo.m_CommentEngSize];

	//コメント(英)読み込み
	fread_s(pPmxData->m_ModelInfo.m_pCommentEng, pPmxData->m_ModelInfo.m_CommentEngSize, pPmxData->m_ModelInfo.m_CommentEngSize, 1, fp);

	//文字列から\0を消す
	ErasCharFromString(pPmxData->m_ModelInfo.m_pCommentEng, pPmxData->m_ModelInfo.m_CommentEngSize, '\0');
}

//頂点読み込み
void CPmxLoader::VertexLoad(FILE* fp, PMX_DATA* pPmxData)
{
	//読み込み用
	unsigned char Data[4];

	//ボーンインデックスサイズ
	int BoneIndexSize = pPmxData->m_Head.m_pData[5];

	//ボーンインデックスデータの読み込み用
	unsigned char* pBoneIndex;
	pBoneIndex = new unsigned char[BoneIndexSize];

	//頂点の数だけ読み込む
	for (int i = 0; i < pPmxData->m_VerNum; i++)
	{
		//Pos
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pVertex[i].m_fPos[j] = StrToFloat(Data, sizeof(Data));
		}
		
		//Norm
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pVertex[i].m_fNorm[j] = StrToFloat(Data, sizeof(Data));
		}

		//UV
		for (int j = 0; j < 2; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pVertex[i].m_fUv[j] = StrToFloat(Data, sizeof(Data));
		}
		
		//追加UVがある場合
		if (pPmxData->m_Head.m_pData[1] != 0)
		{
			//追加分のメモリ確保
			pPmxData->m_pVertex[i].m_pfAddUv = new float[pPmxData->m_Head.m_pData[1]];

			//追加分
			for (int j = 0; j < pPmxData->m_Head.m_pData[1]; j++)
			{
				fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
				pPmxData->m_pVertex[i].m_pfAddUv[j] = StrToFloat(Data, sizeof(Data));
			}
		}

		//ウェイト変形方式
		fread_s(&pPmxData->m_pVertex[i].m_WeightData.m_WeightType, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

		//変形方式に応じて読み込み
		switch (pPmxData->m_pVertex[i].m_WeightData.m_WeightType)
		{
			//BDFE1方式
			case 0:
			{
				//ボーンID
				fread_s(pBoneIndex, BoneIndexSize, BoneIndexSize, 1, fp);
				pPmxData->m_pVertex[i].m_WeightData.m_Bdef1.m_BoneID = StrToInt(pBoneIndex, BoneIndexSize);

				break;
			}
			//BDFE2方式
			case 1:
			{
				//ボーンID*2
				for (int j = 0; j < 2; j++)
				{
					fread_s(pBoneIndex, BoneIndexSize, BoneIndexSize, 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_Bdef2.m_BoneID[j] = StrToInt(pBoneIndex, BoneIndexSize);
				}
			
				//ウェイト
				fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
				pPmxData->m_pVertex[i].m_WeightData.m_Bdef2.m_Weight = StrToFloat(Data, sizeof(Data));

				break;
			}
			//BDFE4方式
			case 2:
			{
				//ボーンID*4
				for (int j = 0; j < 4; j++)
				{		
					fread_s(pBoneIndex, BoneIndexSize, BoneIndexSize, 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_BoneID[j] = StrToInt(pBoneIndex, BoneIndexSize);
				}
	
				//ウェイト*4
				for (int j = 0; j < 4; j++)
				{
					//ウェイト
					fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_Weight[j] = StrToFloat(Data, sizeof(Data));
				}
				break;
			}
			//SDEF方式
			case 3:
			{
				//ボーンID*2
				for (int j = 0; j < 2; j++)
				{
					fread_s(pBoneIndex, BoneIndexSize, BoneIndexSize, 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_BoneID[j] = StrToInt(pBoneIndex, BoneIndexSize);
				}
			
				//ウェイト
				fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
				pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_Weight = StrToFloat(Data, sizeof(Data));

				//3x3マトリックス
				for (int j = 0; j < 3; j++)
				{
					for (int k = 0; k < 3; k++)
					{
						fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
						pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_Matrix[j][k] = StrToFloat(Data, sizeof(Data));
					}
				}
				break;
			}
			default:
				break;
		}

		//エッジ倍率
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pVertex[i].m_EdgeMagn = StrToFloat(Data, sizeof(Data));
	}
	delete[] pBoneIndex;
	pBoneIndex = nullptr;
}

//面読み込み
void CPmxLoader::FaceLoad(FILE* fp, PMX_DATA* pPmxData)
{
	//頂点インデックスサイズ
	int IndexSize = pPmxData->m_Head.m_pData[2];

	unsigned char* pData;
	pData = new unsigned char[IndexSize];
	
	//面の数回す
	for (int i = 0; i < pPmxData->m_FaceNum; i++)
	{
		//頂点インデックス読み込み
		for (int j = 0; j < 3; j++)
		{
			fread_s(pData,IndexSize, IndexSize, 1, fp);
			pPmxData->m_pFace[i].m_VerIndex[j] = StrToInt(pData, IndexSize);
		}
	}

	delete[] pData;
	pData = nullptr;
}


//テクスチャ読み込み
void CPmxLoader::TextureLoad(FILE* fp, PMX_DATA* pPmxData)
{
	unsigned char Data[4];
	int AfterByte;

	for (int i = 0; i < pPmxData->m_TexNum; i++)
	{
		//後続バイト数
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		AfterByte = StrToInt(Data, sizeof(Data));

		//テクスチャパスメモリ確保
		pPmxData->m_pTex[i].m_pPass = new unsigned char[AfterByte];

		//テクスチャパス読み込み
		fread_s(pPmxData->m_pTex[i].m_pPass, AfterByte, AfterByte, 1, fp);

		//文字列から\0を消す
		ErasCharFromString(pPmxData->m_pTex[i].m_pPass, AfterByte, '\0');
	}
}


//マテリアル読み込み
void CPmxLoader::MaterialLoad(FILE* fp, PMX_DATA* pPmxData)
{
	unsigned char Data[4];

	//材質名用サイズ
	int JapSize;
	int EngSize;

	//メモサイズ
	int MemoSize;

	//テクスチャインデックスサイズ
	int TexIndexSize = pPmxData->m_Head.m_pData[3];
	
	//テクスチャデータ読み込み用
	unsigned char* pTexData = nullptr;
	pTexData = new unsigned char[TexIndexSize];
		
	//読み込み
	for (int i = 0; i < pPmxData->m_MaterialNum; i++)
	{
		//材質名(日）サイズ
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		JapSize = StrToInt(Data, sizeof(Data));

		//メモリ確保
		pPmxData->m_pMaterial[i].m_pNameJap = new unsigned char[JapSize];

		//材質名(日）読み込み
		fread_s(pPmxData->m_pMaterial[i].m_pNameJap, JapSize, JapSize, 1, fp);

		//文字列から\0を消す
		ErasCharFromString(pPmxData->m_pMaterial[i].m_pNameJap, JapSize, '\0');

		//材質名(英）サイズ
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		EngSize = StrToInt(Data, sizeof(Data));

		//メモリ確保
		pPmxData->m_pMaterial[i].m_pNameEng = new unsigned char[EngSize];

		//材質名(英）読み込み
		fread_s(pPmxData->m_pMaterial[i].m_pNameEng, EngSize, EngSize, 1, fp);

		//文字列から\0を消す
		ErasCharFromString(pPmxData->m_pMaterial[i].m_pNameEng, EngSize, '\0');

		//ディフューズ
		for (int j = 0; j < 4; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pMaterial[i].m_Diffuse[j] = StrToFloat(Data, sizeof(Data));
		}

		//てすと！！！！
		StrToFloat(Data);

		//スペキュラー
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pMaterial[i].m_Specular[j] = StrToFloat(Data, sizeof(Data));
		}

		//スペキュラパワー
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pMaterial[i].m_SpePower = StrToFloat(Data, sizeof(Data));

		//アンビエント
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pMaterial[i].m_Ambient[j] = StrToFloat(Data, sizeof(Data));
		}

		//描画フラグ
		fread_s(&pPmxData->m_pMaterial[i].m_BitFlag, sizeof(pPmxData->m_pMaterial[i].m_BitFlag), sizeof(pPmxData->m_pMaterial[i].m_BitFlag), 1, fp);

		//エッジ色
		for (int j = 0; j < 4; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pMaterial[i].m_Edge[j] = StrToFloat(Data, sizeof(Data));
		}

		//エッジサイズ
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pMaterial[i].m_EdgeSize = StrToFloat(Data, sizeof(Data));

		//通常テクスチャ
		fread_s(pTexData, TexIndexSize, TexIndexSize, 1, fp);
		pPmxData->m_pMaterial[i].m_NormTex = StrToInt(pTexData, TexIndexSize);
		
		//スフィアテクスチャ
		fread_s(pTexData, TexIndexSize, TexIndexSize, 1, fp);
		pPmxData->m_pMaterial[i].m_SphereTex = StrToInt(pTexData, TexIndexSize);

		//スフィアモード
		fread_s(&pPmxData->m_pMaterial[i].m_SphereMode, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

		//共有Toonフラグ
		fread_s(&pPmxData->m_pMaterial[i].m_ToonFlag, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

		switch (pPmxData->m_pMaterial[i].m_ToonFlag)
		{
			//Toonテクスチャ
			case 0:
			{
				fread_s(pTexData, TexIndexSize, TexIndexSize, 1, fp);
				pPmxData->m_pMaterial[i].m_ToonTex = StrToInt(pTexData, TexIndexSize);
				break;
			}
			//共有Toonテクスチャ
			case 1:
			{
				unsigned char read;
				fread_s(&read, sizeof(unsigned char), sizeof(unsigned char), 1, fp);
				pPmxData->m_pMaterial[i].m_ToonTex = StrToInt(&read, sizeof(read));
				break;
			}
		}

		//メモサイズ
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		MemoSize = StrToInt(Data, sizeof(Data));

		//メモリ確保
		pPmxData->m_pMaterial[i].m_pMemo = new unsigned char[MemoSize];

		//メモ読み込み
		fread_s(pPmxData->m_pMaterial[i].m_pMemo, MemoSize, MemoSize, 1, fp);

		//文字列から\0を消す
		ErasCharFromString(pPmxData->m_pMaterial[i].m_pMemo, MemoSize, '\0');

		//使用する頂点数
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pMaterial[i].m_UseVerNum = StrToInt(Data, sizeof(Data));
	}

	//読み込み用破棄
	delete[] pTexData;
	pTexData = nullptr;
}

//書き出し
bool CPmxLoader::Write(const char* FileName, PMX_DATA* pPmxData)
{
	FILE* fp = nullptr;
	fopen_s(&fp, FileName, "w");

	//オープン失敗
	if (fp == nullptr)
		return false;

	fprintf_s(fp, "ファイルタイプ:%s\n", pPmxData->m_Head.m_FileType);
	fprintf_s(fp, "バージョン:%f\n", pPmxData->m_Head.m_Ver);

	fprintf_s(fp, "バイト列サイズ:%d\n", pPmxData->m_Head.m_Size);
	for (int i = 0; i < pPmxData->m_Head.m_Size; i++)
	{
		fprintf_s(fp, "%d,", pPmxData->m_Head.m_pData[i]);
	}
	fprintf_s(fp, "\n");

	fprintf_s(fp,"モデル名(日):%s\n", pPmxData->m_ModelInfo.m_pNameJap);
	fprintf_s(fp,"モデル名(英):%s\n", pPmxData->m_ModelInfo.m_pNameEng);
	fprintf_s(fp,"コメント(日):%s\n", pPmxData->m_ModelInfo.m_pCommentJap);
	fprintf_s(fp,"コメント(英):%s\n", pPmxData->m_ModelInfo.m_pCommentEng);
	fprintf_s(fp, "\n");

	fprintf_s(fp, "頂点数：%d\n", pPmxData->m_VerNum);

	for (int i = 0; i < pPmxData->m_VerNum; i++)
	{
		fprintf_s(fp, "頂点%8d\n", i);
	
		fprintf_s(fp, "Pos:");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pVertex[i].m_fPos[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "Norm:");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pVertex[i].m_fNorm[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "Uv:");
		for (int j = 0; j < 2; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pVertex[i].m_fUv[j]);
		}
		fprintf_s(fp, "\n");

		//追加頂点があれば
		if (pPmxData->m_pVertex->m_pfAddUv != nullptr)
		{
			fprintf_s(fp, "AddUv:");
			for (int j = 0; j < 4; j++)
			{
				fprintf_s(fp, "%f,", pPmxData->m_pVertex[i].m_pfAddUv[i]);
			}
			fprintf_s(fp, "\n");
		}

		fprintf_s(fp, "ウェイト変形方式:%d\n",pPmxData->m_pVertex[i].m_WeightData.m_WeightType);

		//変形方式に応じて吐き出す
		switch (pPmxData->m_pVertex[i].m_WeightData.m_WeightType)
		{
			//BDEF1
			case 0:
			{
				fprintf_s(fp, "ボーンID:");
				fprintf_s(fp, "%d", pPmxData->m_pVertex[i].m_WeightData.m_Bdef1.m_BoneID);
				break;
			}
			//BDEF2
			case 1:
			{

				fprintf_s(fp, "ボーンID:");
				for (int j = 0; j < 2; j++)
				{
					fprintf_s(fp, "%d,", pPmxData->m_pVertex[i].m_WeightData.m_Bdef2.m_BoneID[j]);
				}
				fprintf_s(fp,"\n");
				
				fprintf_s(fp, "ウェイト値:");
				fprintf_s(fp, "%f\n", pPmxData->m_pVertex[i].m_WeightData.m_Bdef2.m_Weight);
				break;
			}
			//BDEF4
			case 2:
			{
				fprintf_s(fp, "ボーンID:");
				for (int j = 0; j < 4; j++)
				{
					fprintf_s(fp, "%d,", pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_BoneID[j]);
				}
				fprintf_s(fp, "\n");

				fprintf_s(fp, "ウェイト値:");
				for (int j = 0; j < 4; j++)
				{
					fprintf_s(fp, "%f,", pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_Weight[j]);
				}
				fprintf_s(fp, "\n");
				break;
			}
			//BDEF2
			case 3:
			{
				fprintf_s(fp, "ボーンID:");
				for (int j = 0; j < 2; j++)
				{
					fprintf_s(fp, "%d,", pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_BoneID[j]);
				}
				fprintf_s(fp, "\n");

				fprintf_s(fp, "ウェイト値:");
				fprintf_s(fp, "%f\n", pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_Weight);

				fprintf_s(fp, "変形用行列3x3:");
				for (int j = 0; j < 3; j++)
				{
					for (int k = 0; k < 3; k++)
					{
						fprintf_s(fp, "%f", pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_Matrix[j][k]);
					}
					fprintf_s(fp, "\n");
				}
				fprintf_s(fp, "\n");
				break;
			}
			default:
				break;
		}
		
		fprintf_s(fp, "エッジ倍率：");
		fprintf_s(fp, "%f\n", pPmxData->m_pVertex->m_EdgeMagn);
		fprintf_s(fp, "\n");
	}

	fprintf_s(fp, "面の数：%d\n", pPmxData->m_FaceNum);

	for (int i = 0; i < pPmxData->m_FaceNum; i++)
	{
		fprintf_s(fp, "%5d:", i);
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%8d", pPmxData->m_pFace[i].m_VerIndex[j]);
		}
		fprintf_s(fp, "\n");
	}

	fprintf_s(fp, "テクスチャ数：%d\n", pPmxData->m_TexNum);

	for (int i = 0; i < pPmxData->m_TexNum; i++)
	{
		fprintf_s(fp, "%s\n", pPmxData->m_pTex[i].m_pPass);
	}
	
	fprintf_s(fp, "マテリアル数：%d\n", pPmxData->m_MaterialNum);

	for (int i = 0; i < pPmxData->m_MaterialNum; i++)
	{
		fprintf_s(fp, "マテリアル名(日):%s\n", pPmxData->m_pMaterial[i].m_pNameJap);
		fprintf_s(fp, "マテリアル名(英):%s\n", pPmxData->m_pMaterial[i].m_pNameEng);
		
		fprintf_s(fp, "Diffuse:");
		for (int j = 0; j < 4; j++)
		{
			fprintf_s(fp, "%f,",pPmxData->m_pMaterial[i].m_Diffuse[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "Specular:");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pMaterial[i].m_Specular[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "SpecularPower:%f\n", pPmxData->m_pMaterial[i].m_SpePower);

		fprintf_s(fp, "Ambient:");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pMaterial[i].m_Ambient[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "描画フラグ:%d\n", pPmxData->m_pMaterial[i].m_BitFlag);

		fprintf_s(fp, "Edge:");
		for (int j = 0; j < 4; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pMaterial[i].m_Edge[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "EdgeSize:%f\n", pPmxData->m_pMaterial[i].m_EdgeSize);

		fprintf_s(fp, "Texture:%d\n", pPmxData->m_pMaterial[i].m_NormTex);
		fprintf_s(fp, "SphereTexture:%d\n", pPmxData->m_pMaterial[i].m_SphereTex);
		fprintf_s(fp, "SphereMode:%d\n", pPmxData->m_pMaterial[i].m_SphereMode);
		fprintf_s(fp, "ToonTexture:%d\n", pPmxData->m_pMaterial[i].m_ToonTex);
		fprintf_s(fp, "ToonFlag:%d\n", pPmxData->m_pMaterial[i].m_ToonFlag);

		fprintf_s(fp, "メモ:%s\n", pPmxData->m_pMaterial[i].m_pMemo);
		fprintf_s(fp, "使用する頂点数:%d\n", pPmxData->m_pMaterial[i].m_UseVerNum);

		fprintf_s(fp, "\n");
	}

	return true;
}