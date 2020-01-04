#pragma once
#include <stdio.h>

constexpr int CHAR	{ 1 };	//Char
constexpr int SHORT	{ 2 };	//Short
constexpr int LONG	{ 4 };	//Long

//StrからIntへ
int StrToInt(unsigned char* pStr, int Size);

//strからLongへ
unsigned long StrToLong(unsigned char Str[4]);

//strからShortへ
unsigned short StrToShort(unsigned char Str[2]);

//Strからfloatへ
float StrToFloat(unsigned char* pStr);

//指定文字を文字列から消す
unsigned char* ErasCharFromString(unsigned char* pSource, int Size, char Erace);

//テキスト変換
wchar_t* CharToWchar_t(unsigned char* pStr, int StrSize);

//wchar_t文字列読み込み
wchar_t* WcharStrRead(int StrSize, FILE* fp);