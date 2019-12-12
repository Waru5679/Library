#include "Binary.h"
#include <math.h>

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
float StrToFloat(unsigned char* pStr)
{
	float	Out;
	long	Data;	//Data
	long	Mant;	//����
	int		Sign;	//����
	long	Index;	//�w��

	Data = pStr[0] | (pStr[1] << 8) | (pStr[2] << 16) | (pStr[3] << 24);

	//����
	Sign = (Data & 0x80000000) >> 31;

	//�w��
	Index = (Data & 0x7f800000) >> 23;

	//�����t����
	Index -= 0x7f;

	//����
	Mant = (Data & 0x007fffff);
	
	//�������̌����𒲂ׂ�
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

	//�������������ɂ���
	float fMant = Mant/(float)pow(10, Count);

	//float�ɂ���
	Out = powf(-1, Sign) * powf(2, Index) * (1.0f + fMant);

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

//float StrToFloat(unsigned char* pStr)
//{
//	float	Out;
//	long	Data;	//Data
//	long	Mant;	//����
//	int		Sign;	//����
//	long	Index;	//�w��
//	
//	Data = pStr[0] | (pStr[1]<<8) | (pStr[2] << 16) | (pStr[3] << 24);
//
//	//����
//	Sign = (Data & 0x80000000) >> 31;
//
//	//�w��
//	Index = (Data & 0x7f800000)>>23;
//	
//	//�����t����
//	Index -=0x7f;
//
//	//����
//	Mant = (Data & 0x007fffff);
//
//	//float�ɂ���
//	Out=powf(-1, Sign)* powf(2, Index)*(1.0f+Mant);
//	
//	return Out;
//}


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