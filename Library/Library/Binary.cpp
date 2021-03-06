#include "Binary.h"
#include <math.h>
#include <stdio.h>

//Strからintへ
int StrToInt(unsigned char* pStr, int Size)
{
	int Out;

	switch (Size)
	{
		case CHAR:
		{
			Out = pStr[0];
			break;
		}
		case SHORT:
		{
			Out = (int)StrToShort(pStr);
			break;
		}
		case LONG:
		{
			Out = (int)StrToLong(pStr);
			break;
		}
	}
	return Out;
}

//Strからfloatへ
float StrToFloat(unsigned char* pStr)
{
	float	Out;
	long	Data;	//Data
	long	Mant;	//仮数
	int		Sign;	//符号
	long	Index;	//指数

	Data = pStr[0] | (pStr[1] << 8) | (pStr[2] << 16) | (pStr[3] << 24);

	//符号
	Sign = (Data & 0x80000000) >> 31;

	//指数
	Index = (Data & 0x7f800000) >> 23;

	//符号付きに
	Index -= 0x7f;

	//仮数
	Mant = (Data & 0x007fffff);
	
	//仮数部の桁数を調べる
	int Count = 0;
	long MantCopy = Mant;
	while (1)
	{
		MantCopy /= 10;
		Count++;

		if (MantCopy < 10)
		{
			Count++;
			break;
		}
	}

	//仮数部を小数にする
	float fMant = Mant/(float)pow(10, Count);

	//floatにする
	Out = powf(-1, Sign) * powf(2, Index) * (1.0f + fMant);

	return Out;
}

//strからLongへ
unsigned long StrToLong(unsigned char Str[4])
{
	unsigned long Out;

	Out = Str[0] | (Str[1] << 8) | (Str[2] << 16) | (Str[3] << 24);

	return Out;
}

//strからShortへ
unsigned short StrToShort(unsigned char Str[2])
{
	unsigned short Out;

	Out = Str[0] | (Str[1] << 8);

	return Out;
}

//指定文字を文字列から消す
unsigned char* ErasCharFromString(unsigned char* pSource,int Size, char Erace)
{
	unsigned char* pOut=nullptr;

	int count = 0;//除去カウント

	//先に除去数をカウント
	for (int i = 0; i < Size; i++)
	{
		if (pSource[i] == Erace)
		{			
			count++;
		}
	}

	//新しいサイズ(元サイズ-除去数+\0)
	int NewSize = Size - count + 1;

	//メモリ確保
	pOut = new unsigned char[NewSize];

	//カウントリセット
	count = 0;

	//コピー
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
			pOut[i - count] = pSource[i];
		}
	}

	//最後に\0を入れる
	pOut[NewSize - 1] = '\0';

	//元データ破棄
	delete[] pSource;
	pSource = nullptr;
		
	return pOut;
}

//テキスト変換
wchar_t* CharToWchar_t(unsigned char* pStr,int StrSize)
{
	wchar_t* pOut = nullptr;
	
	//wcharは2文字で1文字+\0
	int wStrSize = StrSize / 2 + 1;

	//メモリ確保
	pOut = new wchar_t[wStrSize];

	int count = 0;
	for (int i = 0; i< StrSize; i += 2)
	{
		pOut[count++] = (pStr[i]) | (pStr[i+1] << 8);
	}

	//終わりに\0を入れる
	pOut[count] = '\0';

	return pOut;
}


//wchar_t文字列読み込み
wchar_t* WcharStrRead(int StrSize,FILE* fp)
{
	wchar_t* pOut = nullptr;

	//読み込み用メモリ確保
	unsigned char* pData = new unsigned char[StrSize];

	//文字列読み込み
	fread_s(pData, StrSize, StrSize, 1, fp);

	//wcharに変換
	pOut = CharToWchar_t(pData,StrSize);

	//読み込み用破棄
	delete[] pData;
	pData = nullptr;

	return pOut;
}
