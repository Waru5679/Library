#include "Binary.h"
#include <math.h>
#include <stdio.h>

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

//�w�蕶���𕶎��񂩂����
unsigned char* ErasCharFromString(unsigned char* pSource,int Size, char Erace)
{
	unsigned char* pOut=nullptr;

	int count = 0;//�����J�E���g

	//��ɏ��������J�E���g
	for (int i = 0; i < Size; i++)
	{
		if (pSource[i] == Erace)
		{			
			count++;
		}
	}

	//�V�����T�C�Y(���T�C�Y-������+\0)
	int NewSize = Size - count + 1;

	//�������m��
	pOut = new unsigned char[NewSize];

	//�J�E���g���Z�b�g
	count = 0;

	//�R�s�[
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
			pOut[i - count] = pSource[i];
		}
	}

	//�Ō��\0������
	pOut[NewSize - 1] = '\0';

	//���f�[�^�j��
	delete[] pSource;
	pSource = nullptr;
		
	return pOut;
}

//�e�L�X�g�ϊ�
wchar_t* CharToWchar_t(unsigned char* pStr,int StrSize)
{
	wchar_t* pOut = nullptr;
	
	//wchar��2������1����+\0
	int wStrSize = StrSize / 2 + 1;

	//�������m��
	pOut = new wchar_t[wStrSize];

	int count = 0;
	for (int i = 0; i< StrSize; i += 2)
	{
		pOut[count++] = (pStr[i]) | (pStr[i+1] << 8);
	}

	//�I����\0������
	pOut[count] = '\0';

	return pOut;
}


//wchar_t������ǂݍ���
wchar_t* WcharStrRead(int StrSize,FILE* fp)
{
	wchar_t* pOut = nullptr;

	//�ǂݍ��ݗp�������m��
	unsigned char* pData = new unsigned char[StrSize];

	//������ǂݍ���
	fread_s(pData, StrSize, StrSize, 1, fp);

	//wchar�ɕϊ�
	pOut = CharToWchar_t(pData,StrSize);

	//�ǂݍ��ݗp�j��
	delete[] pData;
	pData = nullptr;

	return pOut;
}
