#pragma once
#include <stdio.h>

constexpr int CHAR	{ 1 };	//Char
constexpr int SHORT	{ 2 };	//Short
constexpr int LONG	{ 4 };	//Long

//Str����Int��
int StrToInt(unsigned char* pStr, int Size);

//str����Long��
unsigned long StrToLong(unsigned char Str[4]);

//str����Short��
unsigned short StrToShort(unsigned char Str[2]);

//Str����float��
float StrToFloat(unsigned char* pStr);

//�w�蕶���𕶎��񂩂����
unsigned char* ErasCharFromString(unsigned char* pSource, int Size, char Erace);

//�e�L�X�g�ϊ�
wchar_t* CharToWchar_t(unsigned char* pStr, int StrSize);

//wchar_t������ǂݍ���
wchar_t* WcharStrRead(int StrSize, FILE* fp);