#include "Binary.h"
//#include <math.h>

//Str����int��
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

//Str����float��
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

//str����Long��
unsigned long StrToLong(unsigned char Str[4])
{
	unsigned long Out;

	Out = Str[0] | (Str[1] << 8) | (Str[2] << 16) | (Str[3] << 24);

	return Out;
}

//str����Short��
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
	//long Mant;//����
	//int Sign;//����
	//long Index;//�w��
	//int IndexSign;//�w������

	//Data = pStr[0] | (pStr[1]<<8) | (pStr[2] << 16) | (pStr[3] << 24);
	//
	//Index = (Data & 0x78000000)>>23;
	//
	////�����t����
	//Index -=0x80;

	//Sign = (Data&0x80000000)>>31;	

	//Mant = (Data & 0x007fffff);

	//Out=powf(-1, Sign)* Mant* powf(2, Index);
	//
	
	return Out;
}


//�w�蕶���𕶎��񂩂����
void ErasCharFromString(unsigned char* pSource, int Size, char Erace)
{
	int count = 0;//�����J�E���g

	for (int i = 0; i < Size; i++)
	{
		if (pSource[i] == Erace)
		{
			//���������J�E���g
			count++;
		}
		else
		{
			//�J�E���g�����l�߂ăR�s�[
			pSource[i - count] = pSource[i];
		}
	}

	//�������Ă���Ε�����̍Ō��\n�������
	if (count > 0)
	{
		pSource[Size - count] = '\0';
	}
}