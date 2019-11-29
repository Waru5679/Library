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
	pPmxData->m_Head.m_Ver = CtoL(Data);

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

	//モデル名&コメントスキップ
	char test_1[16];
	fread_s(test_1, sizeof(test_1), sizeof(test_1), 1, fp);
	
	//頂点数
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->VerNum = CtoL(Data);

	//頂点なし
	if (pPmxData->m_pVertex <= 0)
	{
		return false;
	}

	//頂点メモリ確保
	pPmxData->m_pVertex = new PMX_VERTEX[pPmxData->VerNum];
		
	//頂点読み込み
	VertexLoad(fp, pPmxData);
	
	//面の数
	fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
	pPmxData->FaceNum = CtoL(Data);

	//面なし
	if (pPmxData->FaceNum <= 0)
	{
		return false;
	}

	//面データメモリ確保
	pPmxData->m_pFace = new PMX_FACE[pPmxData->FaceNum];

	//面データ読み込み
	FaceLoad(fp, pPmxData);

	return true;
}

//頂点読み込み
void CPmxLoader::VertexLoad(FILE* fp, PMX_DATA* pPmxData)
{
	//読み込み用
	unsigned char Data[4];

	//頂点の数だけ読み込む
	for (int i = 0; i < pPmxData->VerNum; i++)
	{
		//Pos
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pVertex[i].m_fPos[j] = CtoL(Data);
		}
		
		//Norm
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pVertex[i].m_fNorm[j] = CtoL(Data);
		}

		//UV
		for (int j = 0; j < 3; j++)
		{
			fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
			pPmxData->m_pVertex[i].m_fUv[j] = CtoL(Data);
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
				pPmxData->m_pVertex[i].m_pfAddUv[j] = CtoL(Data);
			}
		}

		//ウェイト変形方式
		fread_s(&pPmxData->m_pVertex[i].m_WeightData.m_WeightType, sizeof(unsigned char), sizeof(unsigned char), 1, fp);

		//ボーンインデックスサイズ
		int BoneIndexSize = pPmxData->m_Head.m_pData[5];

		//変形方式に応じてメモリ確保
		switch (pPmxData->m_pVertex[i].m_WeightData.m_WeightType)
		{
			//BDFE1方式
			case 0:
			{
				//ボーンID
				fread_s(Data, sizeof(Data), sizeof(char) * BoneIndexSize, 1, fp);
				pPmxData->m_pVertex[i].m_WeightData.m_Bdef1.m_BoneID = CtoL(Data);

				break;
			}
			//BDFE2方式
			case 1:
			{
				//ボーンID*2
				for (int j = 0; j < 2; j++)
				{
					fread_s(Data, sizeof(Data), sizeof(char) * BoneIndexSize, 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_Bdef2.m_BoneID[j] = CtoL(Data);
				}
			
				//ウェイト
				fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
				pPmxData->m_pVertex[i].m_WeightData.m_Bdef2.m_Weight = CtoL(Data);

				break;
			}
			//BDFE4方式
			case 2:
			{
				//ボーンID*4
				for (int j = 0; j < 4; j++)
				{		
					fread_s(Data, sizeof(Data), sizeof(char) * BoneIndexSize, 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_BoneID[j] = CtoL(Data);
				}
	
				//ウェイト*4
				for (int j = 0; j < 4; j++)
				{
					//ウェイト_1
					fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_Weight[j] = CtoL(Data);
				}
				break;
			}
			//SDEF方式
			case 3:
			{
				//ボーンID*2
				for (int j = 0; j < 2; j++)
				{
					fread_s(Data, sizeof(Data), sizeof(char) * BoneIndexSize, 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_BoneID[0] = CtoL(Data);
				}
			
				//ウェイト
				fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
				pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_Weight = CtoL(Data);

				//3x3マトリックス
				for (int j = 0; j < 3; j++)
				{
					for (int k = 0; k < 3; k++)
					{
						fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
						pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_Matrix[j][k] = CtoL(Data);
					}
				}

				break;
			}
			default:
				break;
		}

		//エッジ倍率
		fread_s(Data, sizeof(Data), sizeof(Data), 1, fp);
		pPmxData->m_pVertex[i].m_EdgeMagn = CtoL(Data);
	}
}

//面読み込み
void CPmxLoader::FaceLoad(FILE* fp, PMX_DATA* pPmxData)
{
	////頂点インデックスサイズ
	//int IndexSize = pPmxData->m_Head.m_pData[2];

	//unsigned char Data[4];

	////面の数回す
	//for (int i = 0; i < pPmxData->FaceNum; i++)
	//{
	//	//頂点インデックス読み込み
	//	for (int j = 0; j < 3; j++)
	//	{
	//		fread_s(Data, sizeof(Data), sizeof(char) * IndexSize, 1, fp);
	//		pPmxData->m_pFace[i].m_VerIndex[j] = CtoL(Data);
	//	}
	//}
}