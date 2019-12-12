#include "Binary.h"
//#include <math.h>

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
float StrToFloat(unsigned char* pStr, int Size)
{
	float Out;

	switch (Size)
	{
		case CHAR:
		{
			Out = pStr[0];
			break;
		}
		case SHORT:
		{
			Out = (float)StrToShort(pStr);
			break;
		}
		case LONG:
		{
			Out = (float)StrToLong(pStr);
			break;
		}
	}
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

float StrToFloat(unsigned char* pStr)
{
	float Out=0.0f;
	//long Data;
	//long Mant;//仮数
	//int Sign;//符号
	//long Index;//指数
	//int IndexSign;//指数符号

	//Data = pStr[0] | (pStr[1]<<8) | (pStr[2] << 16) | (pStr[3] << 24);
	//
	//Index = (Data & 0x78000000)>>23;
	//
	////符号付きに
	//Index -=0x80;

	//Sign = (Data&0x80000000)>>31;	

	//Mant = (Data & 0x007fffff);

	//Out=powf(-1, Sign)* Mant* powf(2, Index);
	//
	
	return Out;
}


//指定文字を文字列から消す
void ErasCharFromString(unsigned char* pSource, int Size, char Erace)
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

	//除去していれば文字列の最後に\nをいれる
	if (count > 0)
	{
		pSource[Size - count] = '\0';
	}
}