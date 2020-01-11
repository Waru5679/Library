#include "ModelLoader.h"
#include <stdio.h>

//インスタンス
CModelLoader* CModelLoader::m_pInstance = nullptr;

//読み込み
bool CModelLoader::Load(const char* FileName, MODEL_DATA* pModel)
{
	//ファイルオープン
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");

	//ファイル読み込み失敗
	if (fp == NULL)
	{
		return false;
	}

	return true;
}

//解放
void CModelLoader::Release()
{
	//インスタンス破棄
	delete m_pInstance;
	m_pInstance = nullptr;
}
