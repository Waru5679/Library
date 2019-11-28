#include "Binary.h"

//char‚©‚çLong‚Ö
unsigned long CtoL(unsigned char str[4])
{
	unsigned long out;

	out = str[0] | str[1] << 8 | str[2] << 16 | str[3] << 24;

	return out;
}