#include "PmxLoader.h"
#include <stdio.h>
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

	//どっかずれる
	////頂点読み込み
	//for (int i = 0; i < pPmxData->VerNum; i++)
	//{
	//	//Pos_X
	//	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	//	pPmxData->m_pVertex[i].m_fPos[0] = CtoL(pData);

	//	//Pos_Y
	//	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	//	pPmxData->m_pVertex[i].m_fPos[1] = CtoL(pData);

	//	//Pos_Z
	//	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	//	pPmxData->m_pVertex[i].m_fPos[2] = CtoL(pData);

	//	//Norm_X
	//	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	//	pPmxData->m_pVertex[i].m_fNorm[0] = CtoL(pData);

	//	//Norm_Y
	//	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	//	pPmxData->m_pVertex[i].m_fNorm[1] = CtoL(pData);

	//	//Norm_Z
	//	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	//	pPmxData->m_pVertex[i].m_fNorm[2] = CtoL(pData);
	//	
	//	//UV_X
	//	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	//	pPmxData->m_pVertex[i].m_fUv[0] = CtoL(pData);

	//	//UV_Y
	//	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	//	pPmxData->m_pVertex[i].m_fUv[1] = CtoL(pData);

	//	//追加UVがある場合
	//	if (pPmxData->m_Head.m_pData[1] != 0)
	//	{
	//		//追加分のメモリ確保
	//		pPmxData->m_pVertex[i].m_pfAddUv = new float[pPmxData->m_Head.m_pData[1]];

	//		//追加分
	//		for (int j = 0; j < pPmxData->m_Head.m_pData[1]; j++)
	//		{
	//			fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	//			pPmxData->m_pVertex[i].m_pfAddUv[j] = CtoL(pData);				
	//		}
	//	}
	//	
	//	//ウェイト変形方式
	//	fread_s(&pPmxData->m_pVertex[i].WeightType, sizeof(unsigned char), sizeof(unsigned char), 1, fp);
	//	int a = 0;
	//}

	


	return true;
}