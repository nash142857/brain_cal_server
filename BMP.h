#ifndef BMP_H
#define BMP_H
#include<stdio.h>
#include <string>
#include "DCMHeaders.h"
#include "symbol.h"
typedef struct
{
	char a[3];
}XS;
int toInt(OFString str);
void toBmp(unsigned int width,unsigned int height,Uint16 *xs,const char url[],int sta,int end,_signal *flag);
#endif
