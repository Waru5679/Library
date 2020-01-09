﻿#include "PmxLoader.h"
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

	//ヘッダ情報読み込み
	HeadLoad(fp, pPmxData);	

	//モデル情報読み込み
	ModelInfoLoad(fp, pPmxData);

	//頂点読み込み
	if (VertexLoad(fp, pPmxData) == false)
	{
		return false;
	}
	
	//面データ読み込み
	if (FaceLoad(fp, pPmxData) == false)
	{
		return false;
	}
		
	//テクスチャ読み込み
	if (TextureLoad(fp, pPmxData) == false)
	{
		return false;
	}

	//マテリアル読み込み
	if (MaterialLoad(fp, pPmxData) == false)
	{
		return false;
	}

	//ボーン読み込み
	if (BoneLoad(fp, pPmxData) == false)
	{
		return false;
	}
	
	//モーフ読み込み
	if (MorphLoad(fp, pPmxData) == false)
	{
		return false;
	}

	//表示枠読み込み
	if (DisplayFrameLoad(fp,pPmxData) == false)
	{
		return false;
	}

	//剛体読み込み
	if (RigidBodyLoad(fp, pPmxData) == false)
	{
		return false;
	}

	//Joint読み込み
	if (JointLoad(fp, pPmxData) == false)
	{
		return false;
	}
	return true;
}


//ヘッダ情報読みこみ
void CPmxLoader::HeadLoad(FILE* fp, PMX_DATA* pPmxData)
{
	//読み込み用
	unsigned char Data[4];

	//ファイルタイプ
	fread_s(pPmxData->m_Head.m_FileType, sizeof(pPmxData->m_Head.m_FileType), sizeof(pPmxData->m_Head.m_FileType), 1, fp);

	//バージョン
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_Head.m_Ver = StrToFloat(Data);

	//ファイルタイプ
	fread_s(&pPmxData->m_Head.m_Size, sizeof(pPmxData->m_Head.m_Size), sizeof(pPmxData->m_Head.m_Size), 1, fp);

	//バイト列サイズ
	int Size = pPmxData->m_Head.m_Size;
	pPmxData->m_Head.m_pData = new unsigned char[Size];

	//バイト列読み込み
	for (int i = 0; i < Size; i++)
	{
		//ファイルタイプ
		fread_s(&pPmxData->m_Head.m_pData[i], sizeof(unsigned char), sizeof(unsigned char), 1, fp);
	}
}

//モデル情報読み込み
void CPmxLoader::ModelInfoLoad(FILE* fp, PMX_DATA* pPmxData)
{
	unsigned char Data[4];

	//サイズ
	int NameJapSize;
	int NameEngSize;
	int CommentJapSize;
	int CommentEngSize;
	
	//モデル名(日)サイズ
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	NameJapSize = StrToInt(Data,sizeof(Data));

	//サイズあるときのみ
	if (NameJapSize > 0)
	{
		//モデル名(日)読み込む
		pPmxData->m_ModelInfo.m_pNameJap=WcharStrRead(NameJapSize, fp);
	}

	//モデル名(英)サイズ
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	NameEngSize = StrToInt(Data, sizeof(Data));

	//サイズあるときのみ
	if (NameEngSize > 0)
	{
		//モデル名(英)読み込む
		pPmxData->m_ModelInfo.m_pNameEng=WcharStrRead(NameEngSize, fp);
	}

	//コメント(日)サイズ
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	CommentJapSize = StrToInt(Data,sizeof(Data));

	//サイズあるときのみ
	if (CommentJapSize > 0)
	{
		//コメント(日)読み込む
		pPmxData->m_ModelInfo.m_pCommentJap=WcharStrRead(CommentJapSize, fp);
	}

	//コメント(英)サイズ
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	CommentEngSize = StrToInt(Data, sizeof(Data));

	//サイズあるときのみ
	if (CommentEngSize > 0)
	{
		//コメント(英)読み込む
		pPmxData->m_ModelInfo.m_pCommentEng=WcharStrRead(CommentEngSize, fp);
	}
}

//頂点読み込み
bool CPmxLoader::VertexLoad(FILE* fp, PMX_DATA* pPmxData)
{
	unsigned char Data[4];
	
	//頂点数
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_VerNum = StrToInt(Data, sizeof(Data));

	//頂点なし
	if (pPmxData->m_VerNum <= 0)
	{
		return false;
	}

	//メモリ確保
	pPmxData->m_pVertex = new PMX_VERTEX[pPmxData->m_VerNum];
	
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
			pPmxData->m_pVertex[i].m_fPos[j] = StrToFloat(Data);
		}
		
		//Norm
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pVertex[i].m_fNorm[j] = StrToFloat(Data);
		}

		//UV
		for (int j = 0; j < 2; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pVertex[i].m_fUv[j] = StrToFloat(Data);
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
				pPmxData->m_pVertex[i].m_pfAddUv[j] = StrToFloat(Data);
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
				//メモリ確保
				pPmxData->m_pVertex[i].m_WeightData.m_pBdef1 = new BDEF1;

				//ボーンID
				fread_s(pBoneIndex, BoneIndexSize, BoneIndexSize, 1, fp);
				pPmxData->m_pVertex[i].m_WeightData.m_pBdef1->m_BoneID = StrToInt(pBoneIndex, BoneIndexSize);

				break;
			}
			//BDFE2方式
			case 1:
			{
				//メモリ確保
				pPmxData->m_pVertex[i].m_WeightData.m_pBdef2 = new BDEF2;

				//ボーンID*2
				for (int j = 0; j < 2; j++)
				{
					fread_s(pBoneIndex, BoneIndexSize, BoneIndexSize, 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_pBdef2->m_BoneID[j] = StrToInt(pBoneIndex, BoneIndexSize);
				}
			
				//ウェイト
				fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
				pPmxData->m_pVertex[i].m_WeightData.m_pBdef2->m_Weight = StrToFloat(Data);

				break;
			}
			//BDFE4方式
			case 2:
			{
				//メモリ確保
				pPmxData->m_pVertex[i].m_WeightData.m_pBdef4 = new BDEF4;

				//ボーンID*4
				for (int j = 0; j < 4; j++)
				{		
					fread_s(pBoneIndex, BoneIndexSize, BoneIndexSize, 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_pBdef4->m_BoneID[j] = StrToInt(pBoneIndex, BoneIndexSize);
				}
	
				//ウェイト*4
				for (int j = 0; j < 4; j++)
				{
					//ウェイト
					fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_pBdef4->m_Weight[j] = StrToFloat(Data);
				}
				break;
			}
			//SDEF方式
			case 3:
			{
				//メモリ確保
				pPmxData->m_pVertex[i].m_WeightData.m_pSdef = new SDEF;

				//ボーンID*2
				for (int j = 0; j < 2; j++)
				{
					fread_s(pBoneIndex, BoneIndexSize, BoneIndexSize, 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_pSdef->m_BoneID[j] = StrToInt(pBoneIndex, BoneIndexSize);
				}
			
				//ウェイト
				fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
				pPmxData->m_pVertex[i].m_WeightData.m_pSdef->m_Weight = StrToFloat(Data);

				//3x3マトリックス
				for (int j = 0; j < 3; j++)
				{
					for (int k = 0; k < 3; k++)
					{
						fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
						pPmxData->m_pVertex[i].m_WeightData.m_pSdef->m_Matrix[j][k] = StrToFloat(Data);
					}
				}
				break;
			}
			default:
				break;
		}

		//エッジ倍率
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pVertex[i].m_EdgeMagn = StrToFloat(Data);
	}

	//読み込み用破棄
	delete[] pBoneIndex;
	pBoneIndex = nullptr;

	return true;
}

//面読み込み
bool CPmxLoader::FaceLoad(FILE* fp, PMX_DATA* pPmxData)
{
	unsigned char Data[4];

	//面の数
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
	
	//頂点インデックスサイズ
	int IndexSize = pPmxData->m_Head.m_pData[2];

	//頂点インデックス読み込み用
	unsigned char* pFaceData;
	pFaceData = new unsigned char[IndexSize];
	
	//面の数回す
	for (int i = 0; i < pPmxData->m_FaceNum; i++)
	{
		//頂点インデックス読み込み
		for (int j = 0; j < 3; j++)
		{
			fread_s(pFaceData,IndexSize, IndexSize, 1, fp);
			pPmxData->m_pFace[i].m_VerIndex[j] = StrToInt(pFaceData, IndexSize);
		}
	}

	//読み込み用破棄
	delete[] pFaceData;
	pFaceData = nullptr;

	return true;
}

//テクスチャ読み込み
bool CPmxLoader::TextureLoad(FILE* fp, PMX_DATA* pPmxData)
{
	unsigned char Data[4];
	
	//テクスチャ数読み込み
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_TexNum = StrToInt(Data, sizeof(Data));

	//テクスチャなし
	if (pPmxData->m_TexNum <= 0)
	{
		return false;
	}

	//メモリ確保
	pPmxData->m_pTex = new PMX_TEXTURE[pPmxData->m_TexNum];

	//後続バイト数
	int AfterByte;

	for (int i = 0; i < pPmxData->m_TexNum; i++)
	{
		//後続バイト数
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		AfterByte = StrToInt(Data, sizeof(Data));

		//サイズあるときのみ
		if (AfterByte > 0)
		{
			//テクスチャ名読み込む
			pPmxData->m_pTex[i].m_pPass = WcharStrRead(AfterByte, fp);
		}
	}
	return true;
}


//マテリアル読み込み
bool CPmxLoader::MaterialLoad(FILE* fp, PMX_DATA* pPmxData)
{
	unsigned char Data[4];

	//マテリアル数
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_MaterialNum = StrToInt(Data, sizeof(Data));

	//マテリアルなし
	if (pPmxData->m_MaterialNum <= 0)
	{
		return false;
	}

	//メモリ確保
	pPmxData->m_pMaterial = new PMX_MATERIAL[pPmxData->m_MaterialNum];

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

		//サイズあるときのみ
		if (JapSize > 0)
		{
			//材質名(日)
			pPmxData->m_pMaterial[i].m_pNameJap=WcharStrRead(JapSize, fp);
		}

		//材質名(英）サイズ
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		EngSize = StrToInt(Data, sizeof(Data));

		//サイズあるときのみ
		if (EngSize > 0)
		{
			//材質名(英)
			pPmxData->m_pMaterial[i].m_pNameEng = WcharStrRead(EngSize, fp);
		}

		//ディフューズ
		for (int j = 0; j < 4; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pMaterial[i].m_Diffuse[j] = StrToFloat(Data);
		}

		//スペキュラー
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pMaterial[i].m_Specular[j] = StrToFloat(Data);
		}

		//スペキュラパワー
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pMaterial[i].m_SpePower = StrToFloat(Data);

		//アンビエント
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pMaterial[i].m_Ambient[j] = StrToFloat(Data);
		}

		//描画フラグ
		fread_s(&pPmxData->m_pMaterial[i].m_BitFlag, sizeof(pPmxData->m_pMaterial[i].m_BitFlag), sizeof(pPmxData->m_pMaterial[i].m_BitFlag), 1, fp);

		//エッジ色
		for (int j = 0; j < 4; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pMaterial[i].m_Edge[j] = StrToFloat(Data);
		}

		//エッジサイズ
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pMaterial[i].m_EdgeSize = StrToFloat(Data);

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
		
		//サイズあるときのみ
		if (MemoSize > 0)
		{
			//メモ
			pPmxData->m_pMaterial[i].m_pMemo = WcharStrRead(MemoSize, fp);
		}
	
		//使用する頂点数
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pMaterial[i].m_UseVerNum = StrToInt(Data, sizeof(Data));
	}

	//読み込み用破棄
	delete[] pTexData;
	pTexData = nullptr;

	return true;
}

//ボーン読み込み
bool CPmxLoader::BoneLoad(FILE* fp, PMX_DATA* pPmxData)
{
	unsigned char Data[4];

	//ボーン数
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_BoneNum = StrToInt(Data, sizeof(Data));

	//ボーンなし
	if (pPmxData->m_BoneNum <= 0)
	{
		return false;
	}

	//メモリ確保
	pPmxData->m_pBone = new PMX_BONE[pPmxData->m_BoneNum];

	//ボーン名用サイズ
	int JapSize;
	int EngSize;

	//ボーンインデックスサイズ
	int BoneIndexSize = pPmxData->m_Head.m_pData[5];

	//ボーンデータ読み込み用
	unsigned char* pBoneData = nullptr;
	pBoneData = new unsigned char[BoneIndexSize];
	
	//bitフラグ変換用
	int Flag;

	//読み込み
	for (int i = 0; i < pPmxData->m_BoneNum; i++)
	{
		//ボーン名(日)サイズ
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		JapSize = StrToInt(Data, sizeof(Data));

		//サイズあるときのみ
		if (JapSize > 0)
		{
			//ボーン名(日)
			pPmxData->m_pBone[i].m_pNameJap = WcharStrRead(JapSize, fp);
		}

		//ボーン名(英)サイズ
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		EngSize = StrToInt(Data, sizeof(Data));

		//サイズあるときのみ
		if (EngSize > 0)
		{
			//ボーン名(英)
			pPmxData->m_pBone[i].m_pNameEng = WcharStrRead(EngSize, fp);
		}

		//位置
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pBone[i].m_fPos[j] = StrToFloat(Data);
		}

		//親ボーンID
		fread_s(pBoneData, BoneIndexSize, BoneIndexSize, 1, fp);
		pPmxData->m_pBone[i].m_ParentId = StrToInt(pBoneData, BoneIndexSize);
		
		//変形階層
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pBone[i].m_Hierarchy = StrToInt(Data,sizeof(Data));

		//Bitフラグ
		for (int j = 0; j < 2; j++)
		{
			fread_s(&pPmxData->m_pBone[i].m_BitFlag[j], sizeof(unsigned char), sizeof(unsigned char), 1, fp);
		}

		//ビットフラグをintにする
		Flag=StrToInt(pPmxData->m_pBone[i].m_BitFlag, sizeof(pPmxData->m_pBone[i].m_BitFlag));
		
		//接続先0
		if ((Flag & 0x0001) == 0)
		{
			//オフセット
			for (int j = 0; j < 3; j++)
			{
				fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
				pPmxData->m_pBone[i].m_fOffset[j] = StrToFloat(Data);
			}
		}
		//接続先1
		else
		{
			//接続先ボーンID
			fread_s(pBoneData, BoneIndexSize, BoneIndexSize, 1, fp);
			pPmxData->m_pBone[i].m_ConnectId = StrToInt(pBoneData, BoneIndexSize);
		}

		//回転または移動の付与があるとき
		if ((Flag & 0x0100) != 0 || (Flag & 0x0200) != 0)
		{
			//付与親ボーンID
			fread_s(pBoneData, BoneIndexSize, BoneIndexSize, 1, fp);
			pPmxData->m_pBone[i].m_GrantId = StrToInt(pBoneData, BoneIndexSize);
						
			//付与率
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pBone[i].m_fGrantRate = StrToFloat(Data);
		}
	
		//軸固定
		if ((Flag & 0x0400) != 0)
		{
			for (int j = 0; j < 3; j++)
			{
				fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
				pPmxData->m_pBone[i].m_fFixedAxis[j] = StrToFloat(Data);
			}
		}

		//ローカル軸
		if ((Flag & 0x0800) != 0)
		{
			for (int j = 0; j < 3; j++)
			{
				fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
				pPmxData->m_pBone[i].m_fAxisX[j] = StrToFloat(Data);
			}
			for (int j = 0; j < 3; j++)
			{
				fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
				pPmxData->m_pBone[i].m_fAxisZ[j] = StrToFloat(Data);
			}
		}

		//外部親変形
		if ((Flag & 0x2000) != 0)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pBone[i].m_Key = StrToInt(Data,sizeof(Data));
		}

		//IK
		if ((Flag & 0x0020) != 0)
		{
			//ターゲットボーンID
			fread_s(pBoneData, BoneIndexSize, BoneIndexSize, 1, fp);
			pPmxData->m_pBone[i].m_Ik.m_TargetId = StrToInt(pBoneData, BoneIndexSize);

			//ループ回数
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pBone[i].m_Ik.m_RoopTime = StrToInt(Data, sizeof(Data));

			//回転角
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pBone[i].m_Ik.m_fRad = StrToFloat(Data);

			//IKリンク数
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pBone[i].m_Ik.m_LinkNum = StrToInt(Data, sizeof(Data));

			//メモリ確保
			pPmxData->m_pBone[i].m_Ik.m_pLink = new PMX_IK_LINK[pPmxData->m_pBone[i].m_Ik.m_LinkNum];

			//IKリンク読み込み
			for (int j = 0; j < pPmxData->m_pBone[i].m_Ik.m_LinkNum; j++)
			{
				//リンクボーンID
				fread_s(pBoneData, BoneIndexSize, BoneIndexSize, 1, fp);
				pPmxData->m_pBone[i].m_Ik.m_pLink[j].m_LinkBoneId = StrToInt(pBoneData, BoneIndexSize);
				
				//角度制限
				fread_s(&pPmxData->m_pBone[i].m_Ik.m_pLink[j].m_RadRest, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

				//角度制限ありの場合
				if (pPmxData->m_pBone[i].m_Ik.m_pLink[j].m_RadRest == 1)
				{
					//下限
					for (int k = 0; k < 3; k++)
					{
						fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
						pPmxData->m_pBone[i].m_Ik.m_pLink[j].m_fLowerRad[k] = StrToFloat(Data);
					}
					//上限
					for (int k = 0; k < 3; k++)
					{
						fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
						pPmxData->m_pBone[i].m_Ik.m_pLink[j].m_fUpperRad[k] = StrToFloat(Data);
					}
				}
			}
		}
	}
	
	//読み込み用破棄
	delete[] pBoneData;
	pBoneData = nullptr;

	return true;
}

//モーフ読み込み
bool CPmxLoader::MorphLoad(FILE* fp, PMX_DATA* pPmxData)
{
	unsigned char Data[4];

	//モーフ数
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_MorphNum = StrToInt(Data, sizeof(Data));

	//モーフ
	if (pPmxData->m_MorphNum <= 0)
	{
		return false;
	}

	//メモリ確保
	pPmxData->m_pMorph = new PMX_MORPH[pPmxData->m_MorphNum];

	//モーフ名サイズ
	int JapSize;
	int EngSize;

	//読み込み
	for (int i = 0; i < pPmxData->m_MorphNum; i++)
	{
		//モーフ名(日)サイズ
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		JapSize = StrToInt(Data, sizeof(Data));

		//サイズあるときのみ
		if (JapSize > 0)
		{
			//モーフ名(日)読み込み
			pPmxData->m_pMorph[i].m_pNameJap = WcharStrRead(JapSize, fp);
		}
			
		//モーフ名(英)サイズ
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		EngSize = StrToInt(Data, sizeof(Data));

		//サイズあるときのみ
		if (EngSize > 0)
		{
			//モーフ名(英)読み込み
			pPmxData->m_pMorph[i].m_pNameJap = WcharStrRead(EngSize, fp);
		}
		
		//PMDカテゴリ
		fread_s(&pPmxData->m_pMorph[i].m_PmdType, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

		//モーフタイプ
		fread_s(&pPmxData->m_pMorph[i].m_MorphType, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

		//データ数
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pMorph[i].m_DataNum = StrToInt(Data, sizeof(Data));

		switch (pPmxData->m_pMorph[i].m_MorphType)
		{
			//グループ
			case 0:
			{
				//メモリ確保
				pPmxData->m_pMorph[i].m_pGroupMorph = new PMX_GROUP_MORPH[pPmxData->m_pMorph[i].m_DataNum];

				//モーフインデックスサイズ
				int MorphIndexSize= pPmxData->m_Head.m_pData[6];

				//モーフインデックス読み込み用
				unsigned char* pMorph = nullptr;
				pMorph = new unsigned char[MorphIndexSize];

				//読み込み
				for (int j = 0; j < pPmxData->m_pMorph[i].m_DataNum; j++)
				{
					//モーフインデックス
					fread_s(pMorph, MorphIndexSize, MorphIndexSize, 1, fp);
					pPmxData->m_pMorph[i].m_pGroupMorph[j].m_MorphId = StrToInt(pMorph, MorphIndexSize);

					//モーフ率
					fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
					pPmxData->m_pMorph[i].m_pGroupMorph[j].m_fRate = StrToFloat(Data);
				}

				//読み込み用破棄
				delete[] pMorph;
				pMorph = nullptr;

				break;
			}
			//頂点
			case 1:
			{
				//メモリ確保
				pPmxData->m_pMorph[i].m_pVerMorph = new PMX_VER_MORPH[pPmxData->m_pMorph[i].m_DataNum];

				//頂点インデックスサイズ
				int VerIndexSize = pPmxData->m_Head.m_pData[2];

				//頂点インデックス読み込み用
				unsigned char* pVer = nullptr;
				pVer = new unsigned char[VerIndexSize];

				//読み込み
				for (int j = 0; j < pPmxData->m_pMorph[i].m_DataNum; j++)
				{
					//頂点ID
					fread_s(pVer, VerIndexSize, VerIndexSize, 1, fp);
					pPmxData->m_pMorph[i].m_pVerMorph[j].m_VerId=StrToInt(pVer,VerIndexSize);

					//オフセット値
					for (int k = 0; k < 3; k++)
					{
						fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
						pPmxData->m_pMorph[i].m_pVerMorph[j].m_fOffset[k] = StrToFloat(Data);
					}
				}

				//読み込み用破棄
				delete[] pVer;
				pVer = nullptr;

				break;
			}
			//ボーン
			case 2:
			{
				//メモリ確保
				pPmxData->m_pMorph[i].m_pBoneMorph = new PMX_BONE_MORPH[pPmxData->m_pMorph[i].m_DataNum];
				
				//ボーンインデックスサイズ
				int BoneIndexSize = pPmxData->m_Head.m_pData[5];

				//頂点インデックス読み込み用
				unsigned char* pBone = nullptr;
				pBone = new unsigned char[BoneIndexSize];

				//読み込み
				for (int j = 0; j < pPmxData->m_pMorph[i].m_DataNum; j++)
				{
					//ボーンID
					fread_s(pBone, BoneIndexSize, BoneIndexSize, 1, fp);
					pPmxData->m_pMorph[i].m_pBoneMorph[j].m_BoneId = StrToInt(pBone, BoneIndexSize);

					//移動量
					for (int k = 0; k < 3; k++)
					{
						fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
						pPmxData->m_pMorph[i].m_pBoneMorph[j].m_fMove[k] = StrToFloat(Data);
					}
					//回転量
					for (int k = 0; k < 4; k++)
					{
						fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
						pPmxData->m_pMorph[i].m_pBoneMorph[j].m_fRot[k] = StrToFloat(Data);
					}
				}

				//読み込み用破棄
				delete[] pBone;
				pBone = nullptr;

				break;
			}
			//材質
			case 8:
			{
				//メモリ確保
				pPmxData->m_pMorph[i].m_pMateMorph = new PMX_MATE_MORPH[pPmxData->m_pMorph[i].m_DataNum];

				//材質サイズ
				int MateIndexSize = pPmxData->m_Head.m_pData[4];

				//材質インデックス読み込み用
				unsigned char* pMate = nullptr;
				pMate = new unsigned char[MateIndexSize];

				//読み込み
				for (int j = 0; j < pPmxData->m_pMorph[i].m_DataNum; j++)
				{
					//材質インデックス
					fread_s(pMate, MateIndexSize, MateIndexSize, 1, fp);
					pPmxData->m_pMorph[i].m_pMateMorph[j].m_MateId = StrToInt(pMate, MateIndexSize);

					//オフセット演算形式
					fread_s(&pPmxData->m_pMorph[i].m_pMateMorph[j].m_Format, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

					//ディフューズ
					for (int k = 0; k < 4; k++)
					{
						fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
						pPmxData->m_pMorph[i].m_pMateMorph[j].m_fDiffuse[k] = StrToFloat(Data);
					}

					//スペキュラー
					for (int k = 0; k < 3; k++)
					{
						fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
						pPmxData->m_pMorph[i].m_pMateMorph[j].m_fSpecular[k] = StrToFloat(Data);
					}

					//スペキュラパワー
					fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
					pPmxData->m_pMorph[i].m_pMateMorph[j].m_fSpePower = StrToFloat(Data);
					
					//アンビエント
					for (int k = 0; k < 3; k++)
					{
						fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
						pPmxData->m_pMorph[i].m_pMateMorph[j].m_fAmbient[k] = StrToFloat(Data);
					}

					//エッジ色
					for (int k = 0; k < 4; k++)
					{
						fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
						pPmxData->m_pMorph[i].m_pMateMorph[j].m_fEdge[k] = StrToFloat(Data);
					}

					//エッジサイズ
					fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
					pPmxData->m_pMorph[i].m_pMateMorph[j].m_fEdgeSize = StrToFloat(Data);

					//テクスチャ係数
					for (int k = 0; k < 4; k++)
					{
						fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
						pPmxData->m_pMorph[i].m_pMateMorph[j].m_fTex[k] = StrToFloat(Data);
					}

					//スフィアテクスチャ係数
					for (int k = 0; k < 4; k++)
					{
						fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
						pPmxData->m_pMorph[i].m_pMateMorph[j].m_fSphereTex[k]= StrToFloat(Data);
					}

					//Toonテクスチャ
					for (int k = 0; k < 4; k++)
					{
						fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
						pPmxData->m_pMorph[i].m_pMateMorph[j].m_fToonTex[k] = StrToFloat(Data);
					}
				}

				//読み込み用破棄
				delete[] pMate;
				pMate = nullptr;

				break;
			}
			//UVor追加UV
			default:
			{
				//メモリ確保
				pPmxData->m_pMorph[i].m_pUvMorph= new PMX_UV_MORPH[pPmxData->m_pMorph[i].m_DataNum];
				
				//頂点インデックスサイズ
				int VerIndexSize = pPmxData->m_Head.m_pData[2];

				//頂点インデックス読み込み用
				unsigned char* pVer = nullptr;
				pVer = new unsigned char[VerIndexSize];

				//読み込み
				for (int j = 0; j < pPmxData->m_pMorph[i].m_DataNum; j++)
				{
					//頂点ID
					fread_s(pVer, VerIndexSize, VerIndexSize, 1, fp);
					pPmxData->m_pMorph[i].m_pUvMorph[j].m_VerId = StrToInt(pVer, VerIndexSize);

					//オフセット値
					for (int k = 0; k < 4; k++)
					{
						fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
						pPmxData->m_pMorph[i].m_pUvMorph[j].m_fOffset[k] = StrToFloat(Data);
					}
				}

				//読み込み用破棄
				delete[] pVer;
				pVer = nullptr;

				break;
			}
		}
	}
	return true;
}

//表示枠読み込み
bool CPmxLoader::DisplayFrameLoad(FILE* fp, PMX_DATA* pPmxData)
{
	unsigned char Data[4];

	//表示枠数
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_DisplayNum = StrToInt(Data, sizeof(Data));

	//表示枠なし
	if (pPmxData->m_DisplayNum <= 0)
	{
		return false;
	}

	//メモリ確保
	pPmxData->m_pDisplay = new PMX_DISPLAY[pPmxData->m_DisplayNum];

	//Indexサイズ
	int BoneIndexSize = pPmxData->m_Head.m_pData[5];
	int MorphIndexSize = pPmxData->m_Head.m_pData[6];

	//Index読み込み用
	unsigned char* pBoneIndex = new unsigned char[BoneIndexSize];
	unsigned char* pMorphIndex = new unsigned char[MorphIndexSize];

	//枠名サイズ
	int JapSize;
	int EngSize;

	//読み込み
	for (int i = 0; i < pPmxData->m_DisplayNum; i++)
	{
		//枠名(日)サイズ
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		JapSize = StrToInt(Data, sizeof(Data));

		//サイズあるときのみ
		if (JapSize > 0)
		{
			//枠名(日)読み込み
			pPmxData->m_pDisplay[i].m_pNameJap = WcharStrRead(JapSize, fp);
		}

		//枠名(英)サイズ
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		EngSize = StrToInt(Data, sizeof(Data));

		//サイズあるときのみ
		if (EngSize > 0)
		{
			//枠名(日)読み込み
			pPmxData->m_pDisplay[i].m_pNameEng = WcharStrRead(EngSize, fp);
		}

		//特殊枠フラグ
		fread_s(&pPmxData->m_pDisplay[i].m_SpecialFlag, sizeof(unsigned char), sizeof(unsigned char) , 1, fp);
		
		//後続の要素数
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pDisplay[i].m_ElementNum = StrToInt(Data, sizeof(Data));

		//メモリ確保
		pPmxData->m_pDisplay[i].m_pElement = new FRAME_ELEMENT[pPmxData->m_pDisplay[i].m_ElementNum];

		//枠内要素読み込み
		for (int j = 0; j < pPmxData->m_pDisplay[i].m_ElementNum; j++)
		{
			//フラグ
			fread_s(&pPmxData->m_pDisplay[i].m_pElement[j].m_Flag, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

			//フラグに合わせて読み込む
			switch (pPmxData->m_pDisplay[i].m_pElement[j].m_Flag)
			{
				//ボーン
				case 0:
				{
					fread_s(pBoneIndex, BoneIndexSize, BoneIndexSize, 1, fp);
					pPmxData->m_pDisplay[i].m_pElement[j].m_Index = StrToInt(pBoneIndex, BoneIndexSize);
					break;
				}
				//モーフ
				case 1:
				{
					fread_s(pMorphIndex, MorphIndexSize, MorphIndexSize, 1, fp);
					pPmxData->m_pDisplay[i].m_pElement[j].m_Index = StrToInt(pMorphIndex, MorphIndexSize);
					break;
				}
			default:
				break;
			}
		}
	}
	
	//読み込み用破棄
	delete[]pBoneIndex;
	pBoneIndex = nullptr;
	delete[] pMorphIndex;
	pMorphIndex = nullptr;

	return true;
}

//剛体読み込み
bool CPmxLoader::RigidBodyLoad(FILE* fp, PMX_DATA* pPmxData)
{
	unsigned char Data[4];

	//剛体数
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_RigidNum = StrToInt(Data, sizeof(Data));

	//剛体なし
	if (pPmxData->m_RigidNum <= 0)
	{
		return false;
	}

	//メモリ確保
	pPmxData->m_pRigidBody = new PMX_RIGIT_BODY[pPmxData->m_RigidNum];

	//剛体名サイズ
	int JapSize;
	int EngSize;

	//ボーンインデックスサイズ
	int BoneIndexSize = pPmxData->m_Head.m_pData[5];

	//ボーンインデックス読み込み用
	unsigned char* pBoneIndex = nullptr;
	pBoneIndex = new unsigned char[BoneIndexSize];


	for (int i = 0; i < pPmxData->m_RigidNum; i++)
	{
		//剛体名(日)サイズ
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		JapSize = StrToInt(Data, sizeof(Data));

		//サイズあるときのみ
		if (JapSize > 0)
		{
			//剛体名(日)読み込み
			pPmxData->m_pRigidBody[i].m_pNameJap=WcharStrRead(JapSize, fp);
		}

		//剛体名(英)サイズ
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		EngSize = StrToInt(Data, sizeof(Data));

		//サイズあるときのみ
		if (EngSize > 0)
		{
			//剛体名(英)読み込み
			pPmxData->m_pRigidBody[i].m_pNameEng = WcharStrRead(EngSize, fp);
		}

		//ボーンID
		fread_s(pBoneIndex, BoneIndexSize, BoneIndexSize, 1, fp);
		pPmxData->m_pRigidBody[i].m_BoneId = StrToInt(pBoneIndex, BoneIndexSize);

		//グループ
		fread_s(&pPmxData->m_pRigidBody[i].m_Group, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

		//非衝突グループ
		fread_s(&pPmxData->m_pRigidBody[i].m_NoCollision, sizeof(unsigned short), sizeof(unsigned short), 1, fp);

		//形状
		fread_s(&pPmxData->m_pRigidBody[i].m_Shape, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

		//サイズ
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pRigidBody[i].m_fSize[j] = StrToFloat(Data);
		}
		//位置
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pRigidBody[i].m_fPos[j] = StrToFloat(Data);
		}
		//回転
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pRigidBody[i].m_fRad[j] = StrToFloat(Data);
		}

		//質量
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pRigidBody[i].m_fMass = StrToFloat(Data);

		//移動減衰
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pRigidBody[i].m_fMoveDecay = StrToFloat(Data);

		//回転減衰
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pRigidBody[i].m_fRotDecay = StrToFloat(Data);

		//反発力
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pRigidBody[i].m_fRepulsive = StrToFloat(Data);

		//摩擦力
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pRigidBody[i].m_fFriction = StrToFloat(Data);

		//物理演算
		fread_s(&pPmxData->m_pRigidBody[i].m_Operation, sizeof(unsigned char), sizeof(unsigned char), 1, fp);
	}

	//読み込み用破棄
	delete[] pBoneIndex;
	pBoneIndex = nullptr;

	return true;
}

//Joint読み込み
bool CPmxLoader::JointLoad(FILE* fp, PMX_DATA* pPmxData)
{
	unsigned char Data[4];

	//Joint数
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->m_JointNum = StrToInt(Data, sizeof(Data));

	//Jointなし
	if (pPmxData->m_JointNum <= 0)
	{
		return false;
	}

	//メモリ確保
	pPmxData->m_pJoint = new PMX_JOINT[pPmxData->m_JointNum];

	//joint名サイズ
	int JapSize;
	int EngSize;

	//剛体インデックスサイズ
	int RigidIndexSize = pPmxData->m_Head.m_pData[7];
	
	//剛体インデックス読み込みよう
	unsigned char* pRigid = nullptr;
	pRigid = new unsigned char[RigidIndexSize];

	for (int i = 0; i < pPmxData->m_JointNum; i++)
	{
		//Joint名(日)サイズ
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		JapSize = StrToInt(Data, sizeof(Data));

		//サイズあるときのみ
		if (JapSize > 0)
		{
			//Joint名(日)読み込み
			pPmxData->m_pJoint[i].m_pNameJap = WcharStrRead(JapSize, fp);
		}

		//Joint名(英)サイズ
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		EngSize = StrToInt(Data, sizeof(Data));

		//サイズあるときのみ
		if (EngSize > 0)
		{
			//Joint名(英)読み込み
			pPmxData->m_pJoint[i].m_pNameEng = WcharStrRead(EngSize, fp);
		}

		//Jointタイプ
		fread_s(&pPmxData->m_pJoint[i].m_Type, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

		//剛体ID
		for (int j = 0; j < 2; j++)
		{
			fread_s(pRigid, RigidIndexSize, RigidIndexSize, 1, fp);
			pPmxData->m_pJoint[i].m_RigitId[j] = StrToInt(pRigid, RigidIndexSize);
		}

		//位置
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pJoint[i].m_fPos[j] = StrToFloat(Data);
		}
		//回転
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pJoint[i].m_fRad[j] = StrToFloat(Data);
		}
		//移動制限(下限）
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pJoint[i].m_fLowerMove[j] = StrToFloat(Data);
		}
		//移動制限(上限）
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pJoint[i].m_fUpperMove[j] = StrToFloat(Data);
		}
		//回転制限(下限）
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pJoint[i].m_fLowerRad[j] = StrToFloat(Data);
		}
		//回転制限(上限）
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pJoint[i].m_fUpperRad[j] = StrToFloat(Data);
		}
		//ばね定数_移動
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pJoint[i].m_fSpringMove[j] = StrToFloat(Data);
		}
		//ばね定数_回転
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pJoint[i].m_fSpringRad[j] = StrToFloat(Data);
		}
	}

	//読み込み用破棄
	delete[] pRigid;
	pRigid = nullptr;

	return true;
}

//データをまとめる
void CPmxLoader::DataPut(PMX_DATA* pPmxData)
{

}


//書き出し
bool CPmxLoader::Write(const char* FileName, PMX_DATA* pPmxData)
{
	FILE* fp = nullptr;
	fopen_s(&fp, FileName, "w");

	//オープン失敗
	if (fp == nullptr)
		return false;

	fprintf_s(fp, "FileType:%s\n", pPmxData->m_Head.m_FileType);
	fprintf_s(fp, "ver:%f\n", pPmxData->m_Head.m_Ver);

	fprintf_s(fp, "バイト列サイズ:%d\n", pPmxData->m_Head.m_Size);
	for (int i = 0; i < pPmxData->m_Head.m_Size; i++)
	{
		fprintf_s(fp, "%d,", pPmxData->m_Head.m_pData[i]);
	}
	fprintf_s(fp, "\n");

	fprintf_s(fp,"モデル名(日):%ls\n", pPmxData->m_ModelInfo.m_pNameJap);
	fprintf_s(fp,"モデル名(英):%ls\n", pPmxData->m_ModelInfo.m_pNameEng);
	fprintf_s(fp,"コメント(日):%ls\n", pPmxData->m_ModelInfo.m_pCommentJap);
	fprintf_s(fp,"コメント(英):%ls\n", pPmxData->m_ModelInfo.m_pCommentEng);
	fprintf_s(fp, "\n");

	fprintf_s(fp, "VerNum：%d\n", pPmxData->m_VerNum);

	for (int i = 0; i < pPmxData->m_VerNum; i++)
	{
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

		fprintf_s(fp, "WeightType:%d\n",pPmxData->m_pVertex[i].m_WeightData.m_WeightType);

		//変形方式に応じて吐き出す
		switch (pPmxData->m_pVertex[i].m_WeightData.m_WeightType)
		{
			//BDEF1
			case 0:
			{
				fprintf_s(fp, "BoneId:");
				fprintf_s(fp, "%d\n", pPmxData->m_pVertex[i].m_WeightData.m_pBdef1->m_BoneID);
				break;
			}
			//BDEF2
			case 1:
			{

				fprintf_s(fp, "BoneId:");
				for (int j = 0; j < 2; j++)
				{
					fprintf_s(fp, "%d,", pPmxData->m_pVertex[i].m_WeightData.m_pBdef2->m_BoneID[j]);
				}
				fprintf_s(fp,"\n");
				
				fprintf_s(fp, "Weight:");
				fprintf_s(fp, "%f\n", pPmxData->m_pVertex[i].m_WeightData.m_pBdef2->m_Weight);
				break;
			}
			//BDEF4
			case 2:
			{
				fprintf_s(fp, "BoneId:");
				for (int j = 0; j < 4; j++)
				{
					fprintf_s(fp, "%d,", pPmxData->m_pVertex[i].m_WeightData.m_pBdef4->m_BoneID[j]);
				}
				fprintf_s(fp, "\n");

				fprintf_s(fp, "Weight:");
				for (int j = 0; j < 4; j++)
				{
					fprintf_s(fp, "%f,", pPmxData->m_pVertex[i].m_WeightData.m_pBdef4->m_Weight[j]);
				}
				fprintf_s(fp, "\n");
				break;
			}
			//BDEF2
			case 3:
			{
				fprintf_s(fp, "BoneId:");
				for (int j = 0; j < 2; j++)
				{
					fprintf_s(fp, "%d,", pPmxData->m_pVertex[i].m_WeightData.m_pSdef->m_BoneID[j]);
				}
				fprintf_s(fp, "\n");

				fprintf_s(fp, "Weight:");
				fprintf_s(fp, "%f\n", pPmxData->m_pVertex[i].m_WeightData.m_pSdef->m_Weight);

				fprintf_s(fp, "Matrix3x3:");
				for (int j = 0; j < 3; j++)
				{
					for (int k = 0; k < 3; k++)
					{
						fprintf_s(fp, "%f", pPmxData->m_pVertex[i].m_WeightData.m_pSdef->m_Matrix[j][k]);
					}
					fprintf_s(fp, "\n");
				}
				fprintf_s(fp, "\n");
				break;
			}
			default:
				break;
		}
		fprintf_s(fp, "EdgeMagn：");
		fprintf_s(fp, "%f\n", pPmxData->m_pVertex->m_EdgeMagn);
		fprintf_s(fp, "\n");
	}

	fprintf_s(fp, "FaceNum：%d\n", pPmxData->m_FaceNum);
	
	for (int i = 0; i < pPmxData->m_FaceNum; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%d,", pPmxData->m_pFace[i].m_VerIndex[j]);
		}
		fprintf_s(fp, "\n");
	}

	fprintf_s(fp, "TexNum：%d\n", pPmxData->m_TexNum);

	for (int i = 0; i < pPmxData->m_TexNum; i++)
	{
		fprintf_s(fp, "%ls\n", pPmxData->m_pTex[i].m_pPass);
	}
	
	fprintf_s(fp, "MateNum：%d\n", pPmxData->m_MaterialNum);

	for (int i = 0; i < pPmxData->m_MaterialNum; i++)
	{
		fprintf_s(fp, "マテリアル名(日):%ls\n", pPmxData->m_pMaterial[i].m_pNameJap);
		fprintf_s(fp, "マテリアル名(英):%ls\n", pPmxData->m_pMaterial[i].m_pNameEng);
		
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
		fprintf_s(fp, "メモ:%ls\n", pPmxData->m_pMaterial[i].m_pMemo);
		fprintf_s(fp, "使用する頂点数:%d\n", pPmxData->m_pMaterial[i].m_UseVerNum);
		fprintf_s(fp, "\n");
	}

	fprintf_s(fp, "ボーン数：%d\n", pPmxData->m_BoneNum);

	for (int i = 0; i < pPmxData->m_BoneNum; i++)
	{
		fprintf_s(fp, "ボーン名(日):%ls\n", pPmxData->m_pBone[i].m_pNameJap);
		fprintf_s(fp, "ボーン名(英):%ls\n", pPmxData->m_pBone[i].m_pNameEng);

		fprintf_s(fp, "Pos:");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pBone[i].m_fPos[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "親ボーンID:%d\n", pPmxData->m_pBone[i].m_ParentId);
		fprintf_s(fp, "変形階層:%d\n", pPmxData->m_pBone[i].m_Hierarchy);
		fprintf_s(fp, "BitFlag:");
		for (int j = 0; j < 2; j++)
		{
			fprintf_s(fp, "%d,",pPmxData->m_pBone[i].m_BitFlag[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "Offset:");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pBone[i].m_fOffset[j]);
		}
		fprintf_s(fp, "\n");
		
		fprintf_s(fp, "接続先ボーン:%d\n",pPmxData->m_pBone[i].m_ConnectId);

		fprintf_s(fp, "付与親ボーン:%d\n", pPmxData->m_pBone[i].m_GrantId);
		fprintf_s(fp, "付与率:%f\n", pPmxData->m_pBone[i].m_fGrantRate);

		fprintf_s(fp, "軸固定:");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pBone[i].m_fFixedAxis[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "ローカル軸X:");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pBone[i].m_fAxisX[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "ローカル軸Z:");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pBone[i].m_fAxisZ[j]);
		}
		fprintf_s(fp, "\n");
		
		fprintf_s(fp, "Key:%d\n",pPmxData->m_pBone[i].m_Key);
			
		fprintf_s(fp, "ターゲットボーン:%d\n", pPmxData->m_pBone[i].m_Ik.m_TargetId);
		fprintf_s(fp, "ループ回数:%d\n", pPmxData->m_pBone[i].m_Ik.m_RoopTime);
		fprintf_s(fp, "回転角:%f\n", pPmxData->m_pBone[i].m_Ik.m_fRad);

		fprintf_s(fp, "リンク数:%d\n", pPmxData->m_pBone[i].m_Ik.m_LinkNum);
		
		for (int j = 0; j < pPmxData->m_pBone[i].m_Ik.m_LinkNum; j++)
		{
			fprintf_s(fp, "リンクボーン:%d\n", pPmxData->m_pBone[i].m_Ik.m_pLink[j].m_LinkBoneId);
			fprintf_s(fp, "角度制限:%d\n", pPmxData->m_pBone[i].m_Ik.m_pLink[j].m_RadRest);

			fprintf_s(fp, "下限角:");
			for (int k = 0; k < 3; k++)
			{
				fprintf_s(fp, "%f,", pPmxData->m_pBone[i].m_Ik.m_pLink[j].m_fLowerRad[k]);
			}
			fprintf_s(fp, "\n");

			fprintf_s(fp, "上限角:");
			for (int k = 0; k < 3; k++)
			{
				fprintf_s(fp, "%f,", pPmxData->m_pBone[i].m_Ik.m_pLink[j].m_fUpperRad[k]);
			}
			fprintf_s(fp, "\n");
		}
		fprintf_s(fp, "\n");
	}

	fprintf_s(fp, "モーフ数:%d\n", pPmxData->m_MorphNum);

	for (int i = 0; i < pPmxData->m_MorphNum; i++)
	{
		fprintf_s(fp, "モーフ名(日):%ls\n", pPmxData->m_pMorph[i].m_pNameJap);
		fprintf_s(fp, "モーフ名(英):%ls\n", pPmxData->m_pMorph[i].m_pNameEng);

		fprintf_s(fp, "PMDカテゴリ:%d\n", pPmxData->m_pMorph[i].m_PmdType);
		fprintf_s(fp, "モーフタイプ:%d\n", pPmxData->m_pMorph[i].m_MorphType);
		fprintf_s(fp, "データ数:%d\n", pPmxData->m_pMorph[i].m_DataNum);
		
		//タイプごとに出力する
		switch (pPmxData->m_pMorph[i].m_MorphType)
		{
			//グループモーフ
			case 0:
			{				
				for (int j = 0; j < pPmxData->m_pMorph[i].m_DataNum; j++)
				{
					fprintf_s(fp, "モーフID:%d\n", pPmxData->m_pMorph[i].m_pGroupMorph[j].m_MorphId);
					fprintf_s(fp, "モーフ率:%f\n", pPmxData->m_pMorph[i].m_pGroupMorph[j].m_fRate);
				}
				fprintf_s(fp, "\n");
				break;
			}
			//頂点モーフ
			case 1:
			{
				for (int j = 0; j < pPmxData->m_pMorph[i].m_DataNum; j++)
				{
					fprintf_s(fp, "頂点ID:%d\n", pPmxData->m_pMorph[i].m_pVerMorph[j].m_VerId);

					fprintf_s(fp, "座標オフセット:");
					for (int k = 0; k < 3; k++)
					{
						fprintf_s(fp, "%f,", pPmxData->m_pMorph[i].m_pVerMorph[j].m_fOffset[k]);
					}
					fprintf_s(fp, "\n");					
				}
				fprintf_s(fp, "\n");
				break;
			}
			//ボーン
			case 2:
			{
				for (int j = 0; j < pPmxData->m_pMorph[i].m_DataNum; j++)
				{
					fprintf_s(fp, "ボーンID:%d\n", pPmxData->m_pMorph[i].m_pBoneMorph[j].m_BoneId);

					fprintf_s(fp, "移動量:");
					for (int k = 0; k < 3; k++)
					{
						fprintf_s(fp, "%f,", pPmxData->m_pMorph[i].m_pBoneMorph[j].m_fMove[k]);
					}
					fprintf_s(fp, "\n");

					fprintf_s(fp, "回転量:");
					for (int k = 0; k < 4; k++)
					{
						fprintf_s(fp, "%f,", pPmxData->m_pMorph[i].m_pBoneMorph[j].m_fRot[k]);
					}
					fprintf_s(fp, "\n");
				}
				fprintf_s(fp, "\n");
				break;
			}
			//材質
			case 8:
			{
				for (int j = 0; j < pPmxData->m_pMorph[i].m_DataNum; j++)
				{
					fprintf_s(fp, "材質ID:%d\n", pPmxData->m_pMorph[i].m_pMateMorph[j].m_MateId);
					fprintf_s(fp, "オフセット演算形式:%d\n", pPmxData->m_pMorph[i].m_pMateMorph[j].m_Format);
					
					fprintf_s(fp, "Diffuse:");
					for (int k = 0; k < 4; k++)
					{
						fprintf_s(fp, "%f,", pPmxData->m_pMorph[i].m_pMateMorph[j].m_fDiffuse[k]);
					}
					fprintf_s(fp, "\n");
					
					fprintf_s(fp, "Specular:");
					for (int k = 0; k < 3; k++)
					{
						fprintf_s(fp, "%f,", pPmxData->m_pMorph[i].m_pMateMorph[j].m_fSpecular[k]);
					}
					fprintf_s(fp, "\n");

					fprintf_s(fp, "SpecularPower:%f\n", pPmxData->m_pMorph[i].m_pMateMorph[j].m_fSpePower);

					fprintf_s(fp, "Ambient:");
					for (int k = 0; k < 3; k++)
					{
						fprintf_s(fp, "%f,", pPmxData->m_pMorph[i].m_pMateMorph[j].m_fAmbient[k]);
					}
					fprintf_s(fp, "\n");

					fprintf_s(fp, "EdgeColor:");
					for (int k = 0; k < 4; k++)
					{
						fprintf_s(fp, "%f,", pPmxData->m_pMorph[i].m_pMateMorph[j].m_fAmbient[k]);
					}
					fprintf_s(fp, "\n");

					fprintf_s(fp, "EdgeSize:%f\n", pPmxData->m_pMorph[i].m_pMateMorph[j].m_fEdgeSize);

					fprintf_s(fp, "Tex:");
					for (int k = 0; k < 4; k++)
					{
						fprintf_s(fp, "%f,", pPmxData->m_pMorph[i].m_pMateMorph[j].m_fTex[k]);
					}
					fprintf_s(fp, "\n");
					
					fprintf_s(fp, "SphereTex:");
					for (int k = 0; k < 4; k++)
					{
						fprintf_s(fp, "%f,", pPmxData->m_pMorph[i].m_pMateMorph[j].m_fSphereTex[k]);
					}
					fprintf_s(fp, "\n");
					
					fprintf_s(fp, "ToonTex:");
					for (int k = 0; k < 4; k++)
					{
						fprintf_s(fp, "%f,", pPmxData->m_pMorph[i].m_pMateMorph[j].m_fToonTex[k]);
					}
					fprintf_s(fp, "\n");
				}
				fprintf_s(fp, "\n");
				break;
			}
			//UVor追加UV
			default:
			{
				for (int j = 0; j < pPmxData->m_pMorph[i].m_DataNum; j++)
				{
					fprintf_s(fp, "頂点ID:%d\n", pPmxData->m_pMorph[i].m_pUvMorph[j].m_VerId);

					fprintf_s(fp, "UVオフセット:");
					for (int k = 0; k < 4; k++)
					{
						fprintf_s(fp, "%f,", pPmxData->m_pMorph[i].m_pUvMorph[j].m_fOffset[k]);
					}
					fprintf_s(fp, "\n");
				}
				fprintf_s(fp, "\n");
				break;
			}
		}
		fprintf_s(fp, "\n");
	}

	fprintf_s(fp, "表示枠数:%d\n", pPmxData->m_DisplayNum);

	for (int i = 0; i < pPmxData->m_DisplayNum; i++)
	{
		fprintf_s(fp, "表示枠名(日):%ls\n", pPmxData->m_pDisplay[i].m_pNameJap);
		fprintf_s(fp, "表示枠名(英):%ls\n", pPmxData->m_pDisplay[i].m_pNameEng);
		fprintf_s(fp, "特殊枠:%d\n", pPmxData->m_pDisplay[i].m_SpecialFlag);
		fprintf_s(fp, "\n");

		fprintf_s(fp, "枠内要素数：%d\n", pPmxData->m_pDisplay[i].m_ElementNum);
		for (int j = 0; j < pPmxData->m_pDisplay[i].m_ElementNum; j++)
		{
			fprintf_s(fp, "フラグ:%d\n", pPmxData->m_pDisplay[i].m_pElement[j].m_Flag);
			fprintf_s(fp, "Index:%d\n", pPmxData->m_pDisplay[i].m_pElement[j].m_Index);
			fprintf_s(fp, "\n");
		}
		fprintf_s(fp, "\n");
	}

	fprintf_s(fp, "剛体数:%d\n", pPmxData->m_RigidNum);

	for (int i = 0; i < pPmxData->m_RigidNum; i++)
	{
		fprintf_s(fp, "剛体名(日):%ls\n", pPmxData->m_pRigidBody[i].m_pNameJap);
		fprintf_s(fp, "剛体名(英):%ls\n", pPmxData->m_pRigidBody[i].m_pNameEng);
		fprintf_s(fp, "関連ボーンID:%d\n", pPmxData->m_pRigidBody[i].m_BoneId);
		fprintf_s(fp, "グループ:%d\n", pPmxData->m_pRigidBody[i].m_Group);
		fprintf_s(fp, "非衝突グループフラグ:%d\n", pPmxData->m_pRigidBody[i].m_NoCollision);
		fprintf_s(fp, "形状:%d\n", pPmxData->m_pRigidBody[i].m_Shape);

		fprintf_s(fp, "サイズ:");
		for(int j=0;j<3;j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pRigidBody[i].m_fSize[j]);
		}
		fprintf_s(fp, "\n");


		fprintf_s(fp, "位置");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pRigidBody[i].m_fPos[j]);
		}
		fprintf_s(fp, "\n");


		fprintf_s(fp, "回転:");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pRigidBody[i].m_fRad[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "質量:%f\n", pPmxData->m_pRigidBody[i].m_fMass);
		fprintf_s(fp, "移動減衰:%f\n", pPmxData->m_pRigidBody[i].m_fMoveDecay);
		fprintf_s(fp, "回転減衰:%f\n", pPmxData->m_pRigidBody[i].m_fRotDecay);
		fprintf_s(fp, "反発力:%f\n", pPmxData->m_pRigidBody[i].m_fRepulsive);
		fprintf_s(fp, "摩擦力:%f\n", pPmxData->m_pRigidBody[i].m_fFriction);
		fprintf_s(fp, "物理演算:%d\n", pPmxData->m_pRigidBody[i].m_Operation);
		fprintf_s(fp, "\n");
	}

	fprintf_s(fp, "Joint数:%d\n", pPmxData->m_JointNum);

	for (int i = 0; i < pPmxData->m_JointNum; i++)
	{
		fprintf_s(fp,"Joint名(日):%ls\n", pPmxData->m_pJoint[i].m_pNameJap);
		fprintf_s(fp,"Joint名(英):%ls\n", pPmxData->m_pJoint[i].m_pNameEng);
		fprintf_s(fp,"JointType:%d\n", pPmxData->m_pJoint[i].m_Type);
		
		fprintf_s(fp, "剛体Index:");
		for (int j = 0; j < 2; j++)
		{
			fprintf_s(fp, "%d,", pPmxData->m_pJoint[i].m_RigitId[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "Pos:");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pJoint[i].m_fPos[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "Rad:");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pJoint[i].m_fRad[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "移動制限(上限):");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pJoint[i].m_fUpperMove[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "移動制限(下限):");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pJoint[i].m_fLowerMove[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "回転制限(上限):");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pJoint[i].m_fUpperRad[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "回転制限(下限):");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pJoint[i].m_fLowerRad[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "ばね定数(移動):");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pJoint[i].m_fSpringMove[j]);
		}
		fprintf_s(fp, "\n");
		
		fprintf_s(fp, "ばね定数(回転):");
		for (int j = 0; j < 3; j++)
		{
			fprintf_s(fp, "%f,", pPmxData->m_pJoint[i].m_fSpringRad[j]);
		}
		fprintf_s(fp, "\n");

		fprintf_s(fp, "\n");
	}
	

	return true;
}