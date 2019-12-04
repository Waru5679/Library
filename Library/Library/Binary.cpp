#include "Binary.h"

//Str‚©‚çint‚Ö
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

//Str‚©‚çfloat‚Ö
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


//str‚©‚çLong‚Ö
unsigned long StrToLong(unsigned char Str[4])
{
	unsigned long Out;

	Out = Str[0] | Str[1] << 8 | Str[2] << 16 | Str[3] << 24;

	return Out;
}

//str‚©‚çShort‚Ö
unsigned short StrToShort(unsigned char Str[2])
{
	unsigned short Out;

	Out = Str[0] | Str[1] << 8;

	return Out;
}