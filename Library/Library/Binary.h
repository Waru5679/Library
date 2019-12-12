#pragma once

constexpr int CHAR	{ 1 };	//Char
constexpr int SHORT	{ 2 };	//Short
constexpr int LONG	{ 4 };	//Long

//StrからIntへ
int StrToInt(unsigned char* pStr, int Size);

//Strからfloatへ
float StrToFloat(unsigned char* pStr, int Size);

//strからLongへ
unsigned long StrToLong(unsigned char Str[4]);

//strからShortへ
unsigned short StrToShort(unsigned char Str[2]);

//longからfloatへ
float StrToFloat(unsigned char* pStr);

//指定文字を文字列から消す
void ErasCharFromString(unsigned char* pSource, int Size, char Erace);