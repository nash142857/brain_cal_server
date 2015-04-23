#ifndef GETDCM_H
#define GETDCM_H
#include<string>
#include "DCMHeaders.h"
using namespace std;
class getDcm
{
public:
	string dir;
	Uint16 height;
	Uint16 width;
	getDcm(string dir);
};
#endif
