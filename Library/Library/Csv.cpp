#include <stdio.h>
#include "Csv.h"

//csv�f�[�^�ǂݍ���
int* CCsv::LoadCsv(const char* Name, int Size_x, int Size_y)
{

	//�t�@�C�����J��
	FILE* fp;
	fopen_s(&fp, Name, "r");

	if (fp == NULL)
	{
		//�ǂݍ��ݎ��s
		return NULL;
	}

	//�T�C�Y���̃������m��
	int *pData = new int[Size_x*Size_y];

	//�ǂݍ��ݗp
	int c;
	int Count = 0;

	//�t�@�C���ǂݍ���
	while (!(feof(fp)))
	{
		c = fgetc(fp);

		//0�`9�̎������ǂݍ���
		if ('0' <= c && c <= '9')
		{
			//char��int�ɕϊ�
			*(pData + Count) = c - '0';
			Count++;
		}
	}

	return pData;
}
