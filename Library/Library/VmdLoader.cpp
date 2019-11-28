#include "VmdLoader.h"
#include <stdio.h>
#include "Binary.h"

//インスタンス
CVmdLoader* CVmdLoader::m_pInstance = nullptr;

//読み込み
bool CVmdLoader::Load(const char* FileName, VMD_DATA* pVmdData)
{
	//ファイルオープン
	FILE* fp = nullptr;
	fopen_s(&fp, FileName, "rb");

	//ファイルオープン失敗
	if (fp == nullptr)
		return false;
	
	//ファイルシグネチャ
	fread_s(pVmdData->m_Head.m_Signature, sizeof(pVmdData->m_Head.m_Signature), sizeof(pVmdData->m_Head.m_Signature), 1, fp);

	//モデル名
	fread_s(pVmdData->m_Head.m_ModelName, sizeof(pVmdData->m_Head.m_ModelName), sizeof(pVmdData->m_Head.m_ModelName), 1, fp);

	//モーションレコード数
	unsigned char pData[4];
	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	pVmdData->m_MotionRecordNum = CtoL(pData);
	
	//モーション分のメモリ確保
	pVmdData->m_pMotion = new VMD_MOTION[pVmdData->m_MotionRecordNum];

	//モーションデータ読み込み
	for (unsigned int i = 0; i < pVmdData->m_MotionRecordNum; i++)
	{
		//ボーン名
		fread_s(pVmdData->m_pMotion[i].BoneName, sizeof(pVmdData->m_pMotion[i].BoneName), sizeof(pVmdData->m_pMotion[i].BoneName), 1, fp);
		
		//フレームナンバー
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pVmdData->m_pMotion[i].FrameNum = CtoL(pData);

		//Pos_X
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pVmdData->m_pMotion[i].Pos[0]= CtoL(pData);

		//Pos_Y
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pVmdData->m_pMotion[i].Pos[1] = CtoL(pData);
		
		//Pos_Z
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pVmdData->m_pMotion[i].Pos[2] = CtoL(pData);

		//Rot_X
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pVmdData->m_pMotion[i].Rot[0] = CtoL(pData);

		//Rot_Y
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pVmdData->m_pMotion[i].Rot[1] = CtoL(pData);

		//Rot_Z
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pVmdData->m_pMotion[i].Rot[2] = CtoL(pData);

		//Rot_W
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pVmdData->m_pMotion[i].Rot[3] = CtoL(pData);

		//補完データ
		fread_s(pVmdData->m_pMotion[i].Data, sizeof(pVmdData->m_pMotion[i].Data), sizeof(pVmdData->m_pMotion[i].Data), 1, fp);
	}

	//スキンレコード数
	fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
	pVmdData->m_SkinRecordNum = CtoL(pData);

	//スキン分のメモリ確保
	pVmdData->m_pSkin = new VMD_SKIN[pVmdData->m_SkinRecordNum];
		
	//スキン情報読み込み
	for (unsigned int i = 0; i < pVmdData->m_SkinRecordNum; i++)
	{
		//スキン名
		fread_s(pVmdData->m_pSkin[i].SkinName, sizeof(pVmdData->m_pSkin[i].SkinName), sizeof(pVmdData->m_pSkin[i].SkinName), 1, fp);

		//フレームナンバー
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pVmdData->m_pSkin[i].FlameNum = CtoL(pData);

		//ウェイト
		fread_s(pData, sizeof(pData), sizeof(pData), 1, fp);
		pVmdData->m_pSkin[i].Weight = CtoL(pData);
	}

	//ファイルクローズ
	fclose(fp);
	
	return true;
}

//書き出し
bool CVmdLoader::Write(const char* FileName, VMD_DATA* pVmdData)
{
	FILE* fp = nullptr;
	fopen_s(&fp, FileName, "w");

	//オープン失敗
	if (fp == nullptr)
		return false;

	fprintf_s(fp, "ファイルシグネチャ:%s\n", pVmdData->m_Head.m_Signature);
	fprintf_s(fp, "モデル名:%s\n", pVmdData->m_Head.m_ModelName);

	for (unsigned int i = 0; i < pVmdData->m_MotionRecordNum; i++)
	{
		fprintf_s(fp, "---モーション%d(始)---\n",i);

		fprintf_s(fp, "ボーン名:%s\n", pVmdData->m_pMotion[i].BoneName);
		
		fprintf_s(fp, "フレームNo:%d\n", pVmdData->m_pMotion[i].FrameNum);

		fprintf_s(fp, "位置\n");
		fprintf_s(fp, "X:%f,Y:%f,Z:%f\n", pVmdData->m_pMotion[i].Pos[0], pVmdData->m_pMotion[i].Pos[1], pVmdData->m_pMotion[i].Pos[2]);

		fprintf_s(fp, "回転\n");
		fprintf_s(fp, "X:%f,Y:%f,Z:%f,W:%f\n",
			pVmdData->m_pMotion[i].Rot[0], pVmdData->m_pMotion[i].Rot[1], pVmdData->m_pMotion[i].Rot[2], pVmdData->m_pMotion[i].Rot[3]);

		fprintf_s(fp, "補完パラメタ\n");
		fprintf_s(fp, "%s\n", pVmdData->m_pMotion[i].Data);
		
		fprintf_s(fp, "---モーション%d(終)---\n",i);
		fprintf_s(fp, "\n");
	}

	for (unsigned int i = 0; i < pVmdData->m_SkinRecordNum; i++)
	{
		fprintf_s(fp, "---スキン%d(始)---\n", i);

		fprintf_s(fp, "表情名:%s\n", pVmdData->m_pSkin[i].SkinName);

		fprintf_s(fp, "フレームNo:%d\n", pVmdData->m_pSkin[i].FlameNum);

		fprintf_s(fp, "ウェイト:%f\n", pVmdData->m_pSkin[i].Weight);

		fprintf_s(fp, "---スキン%d(終)---\n", i);
		fprintf_s(fp, "\n");
	}

	//ファイルクローズ
	fclose(fp);

	return true;
}