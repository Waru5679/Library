#pragma once

constexpr int CHAR	{ 1 };	//Char
constexpr int SHORT	{ 2 };	//Short
constexpr int LONG	{ 4 };	//Long

//Str����Int��
int StrToInt(unsigned char* pStr, int Size);

//Str����float��
float StrToFloat(unsigned char* pStr, int Size);

//str����Long��
unsigned long StrToLong(unsigned char Str[4]);

//str����Short��
unsigned short StrToShort(unsigned char Str[2]);

//�w�蕶���𕶎��񂩂����
void ErasCharFromString(unsigned char* pSource, int Size, char Erace);