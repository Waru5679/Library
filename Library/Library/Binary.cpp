#include "Binary.h"

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

	Out = Str[0] | Str[1] << 8 | Str[2] << 16 | Str[3] << 24;

	return Out;
}

//str����Short��
unsigned short StrToShort(unsigned char Str[2])
{
	unsigned short Out;

	Out = Str[0] | Str[1] << 8;

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
}