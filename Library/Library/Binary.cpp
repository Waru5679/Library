#include "Binary.h"

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

	Out = Str[0] | Str[1] << 8 | Str[2] << 16 | Str[3] << 24;

	return Out;
}

//strからShortへ
unsigned short StrToShort(unsigned char Str[2])
{
	unsigned short Out;

	Out = Str[0] | Str[1] << 8;

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
}