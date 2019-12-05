#pragma once

constexpr int CHAR	{ 1 };	//Char
constexpr int SHORT	{ 2 };	//Short
constexpr int LONG	{ 4 };	//Long

//Str‚©‚çInt‚Ö
int StrToInt(unsigned char* pStr, int Size);

//Str‚©‚çfloat‚Ö
float StrToFloat(unsigned char* pStr, int Size);

//str‚©‚çLong‚Ö
unsigned long StrToLong(unsigned char Str[4]);

//str‚©‚çShort‚Ö
unsigned short StrToShort(unsigned char Str[2]);

//w’è•¶š‚ğ•¶š—ñ‚©‚çÁ‚·
void ErasCharFromString(unsigned char* pSource, int Size, char Erace);