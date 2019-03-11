#include <stdio.h>
#include "Csv.h"

//csvデータ読み込み
int* CCsv::LoadCsv(const char* Name, int Size_x, int Size_y)
{

	//ファイルを開く
	FILE* fp;
	fopen_s(&fp, Name, "r");

	if (fp == NULL)
	{
		//読み込み失敗
		return NULL;
	}

	//サイズ分のメモリ確保
	int *pData = new int[Size_x*Size_y];

	//読み込み用
	int c;
	int Count = 0;

	//ファイル読み込み
	while (!(feof(fp)))
	{
		c = fgetc(fp);

		//0～9の時だけ読み込む
		if ('0' <= c && c <= '9')
		{
			//charをintに変換
			*(pData + Count) = c - '0';
			Count++;
		}
	}

	return pData;
}
