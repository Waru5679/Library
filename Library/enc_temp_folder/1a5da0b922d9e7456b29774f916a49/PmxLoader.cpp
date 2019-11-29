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
	unsigned char pData[4];

	//ファイルタイプ
	fread_s(pPmxData->m_Head.m_FileType, sizeof(pPmxData->m_Head.m_FileType), sizeof(pPmxData->m_Head.m_FileType), 1, fp);

	//バージョン
	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	pPmxData->m_Head.m_Ver = CtoL(pData);

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
	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	pPmxData->VerNum = CtoL(pData);

	//頂点メモリ確保
	pPmxData->m_pVertex = new PMX_VERTEX[pPmxData->VerNum];

	//頂点読み込み
	if (pPmxData->m_pVertex <= 0)
	{
		//頂点なし？
		return false;
	}
	
	//頂点読み込み
	VertexLoad(fp, pPmxData);
	
	//面の数
	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	pPmxData->FaceNum = CtoL(pData);

	//面データメモリ確保
	pPmxData->m_pFace = new PMX_FACE[pPmxData->FaceNum];

	return true;
}

//頂点読み込み
void CPmxLoader::VertexLoad(FILE* fp, PMX_DATA* pPmxData)
{
	//読み込み用
	unsigned char pData[4];

	//頂点の数だけ読み込む
	for (int i = 0; i < pPmxData->VerNum; i++)
	{
		//Pos_X
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pPmxData->m_pVertex[i].m_fPos[0] = CtoL(pData);

		//Pos_Y
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pPmxData->m_pVertex[i].m_fPos[1] = CtoL(pData);

		//Pos_Z
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pPmxData->m_pVertex[i].m_fPos[2] = CtoL(pData);

		//Norm_X
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pPmxData->m_pVertex[i].m_fNorm[0] = CtoL(pData);

		//Norm_Y
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pPmxData->m_pVertex[i].m_fNorm[1] = CtoL(pData);

		//Norm_Z
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pPmxData->m_pVertex[i].m_fNorm[2] = CtoL(pData);

		//UV_X
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pPmxData->m_pVertex[i].m_fUv[0] = CtoL(pData);

		//UV_Y
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pPmxData->m_pVertex[i].m_fUv[1] = CtoL(pData);

		//追加UVがある場合
		if (pPmxData->m_Head.m_pData[1] != 0)
		{
			//追加分のメモリ確保
			pPmxData->m_pVertex[i].m_pfAddUv = new float[pPmxData->m_Head.m_pData[1]];

			//追加分
			for (int j = 0; j < pPmxData->m_Head.m_pData[1]; j++)
			{
				fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
				pPmxData->m_pVertex[i].m_pfAddUv[j] = CtoL(pData);
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
			fread_s(pData, sizeof(pData), sizeof(char) * BoneIndexSize, 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef1.m_BoneID = CtoL(pData);

			break;
		}
		//BDFE2方式
		case 1:
		{
			//ボーンID_1
			fread_s(pData, sizeof(pData), sizeof(char) * BoneIndexSize, 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef2.m_BoneID[0] = CtoL(pData);

			//ボーンID_2
			fread_s(pData, sizeof(pData), sizeof(char) * BoneIndexSize, 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef2.m_BoneID[1] = CtoL(pData);

			//ウェイト
			fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef2.m_Weight = CtoL(pData);

			break;

		}
		//BDFE4方式
		case 2:
		{
			//ボーンID_1
			fread_s(pData, sizeof(pData), sizeof(char) * BoneIndexSize, 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_BoneID[0] = CtoL(pData);

			//ボーンID_2
			fread_s(pData, sizeof(pData), sizeof(char) * BoneIndexSize, 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_BoneID[1] = CtoL(pData);

			//ボーンID_3
			fread_s(pData, sizeof(pData), sizeof(char) * BoneIndexSize, 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_BoneID[2] = CtoL(pData);

			//ボーンID_4
			fread_s(pData, sizeof(pData), sizeof(char) * BoneIndexSize, 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_BoneID[3] = CtoL(pData);

			//ウェイト_1
			fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_Weight[0] = CtoL(pData);

			//ウェイト_2
			fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_Weight[1] = CtoL(pData);

			//ウェイト_3
			fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_Weight[2] = CtoL(pData);

			//ウェイト_4
			fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Bdef4.m_Weight[3] = CtoL(pData);

			break;
		}
		//SDEF方式
		case 3:
		{

			//ボーンID_1
			fread_s(pData, sizeof(pData), sizeof(char) * BoneIndexSize, 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_BoneID[0] = CtoL(pData);

			//ボーンID_2
			fread_s(pData, sizeof(pData), sizeof(char) * BoneIndexSize, 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_BoneID[1] = CtoL(pData);

			//ウェイト
			fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
			pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_Weight = CtoL(pData);

			//3x3マトリックス
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
					pPmxData->m_pVertex[i].m_WeightData.m_Sdef.m_Matrix[j][k] = CtoL(pData);
				}
			}

			break;
		}
		default:
			break;
		}

		//エッジ倍率
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pPmxData->m_pVertex[i].m_EdgeMagn = CtoL(pData);
	}
}