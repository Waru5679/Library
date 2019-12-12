#include "Binary.h"
#include <math.h>

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

//float StrToFloat(unsigned char* pStr)
//{
//	float	Out;
//	long	Data;	//Data
//	long	Mant;	//仮数
//	int		Sign;	//符号
//	long	Index;	//指数
//	
//	Data = pStr[0] | (pStr[1]<<8) | (pStr[2] << 16) | (pStr[3] << 24);
//
//	//符号
//	Sign = (Data & 0x80000000) >> 31;
//
//	//指数
//	Index = (Data & 0x7f800000)>>23;
//	
//	//符号付きに
//	Index -=0x7f;
//
//	//仮数
//	Mant = (Data & 0x007fffff);
//
//	//floatにする
//	Out=powf(-1, Sign)* powf(2, Index)*(1.0f+Mant);
//	
//	return Out;
//}


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