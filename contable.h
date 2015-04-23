#ifndef   CONSTABLE_H
#define   CONSTABLE_H
#include "symbol.h"
#include "common.h"
#include <sstream>
#include <exception>
/*
 * paras:
 * pwi_para=format_str&dwi_para=format_str
 */

class PWIConstTable
{
public:
    int PSetNum;
    int PSliceNum;
	int TE;
	int TR;
	int STDNum;
	_signal PMask;
	float ck1,ck2,ck3;
	int TType;
	double x[2][3];
	double pr;
	double ro,HSV,HLV;
	int Coef;
	int CBFC;
	int MTTC;
	int Tmax_th;
    int VP_th;
    PWIConstTable();
    bool load(string format_str){
        istringstream fin;
        try{
            fin.str(format_str);

        }
        catch(...){
            printf("init istringstream error %s", format_str.c_str());
            return false;
        }

        try{
            fin >> PSetNum >> PSliceNum >> TE >> TR >> STDNum >> PMask >> ck1 >> ck2 >> ck3 >> TType;
            for(int i = 0; i < 2; ++i)
                for(int j = 0; j < 3; ++j)
                    fin >> x[i][j];
            fin >> pr >> ro >> HSV >> HLV >> Coef >> CBFC >> MTTC >> Tmax_th >> VP_th;
        }
        catch(...){
            printf("fin error");
            return false;
        }
        return true;
    }
};
class DWIConstTable
{
public:
	int DSetNum;
	int DSliceNum;
	int b;
	double ADCMin;
	double ADCMax;
	double ADC_th;
    double VD_th;
    DWIConstTable();
    bool load(string format_str){
        istringstream fin;
        try{
            fin.str(format_str);

        }
        catch(...){
            printf("init istringstream error %s", format_str.c_str());
            return false;
        }

        try{
            fin >> DSetNum >> DSliceNum >> b >> ADCMin >> ADCMax >> ADC_th >> VD_th;
        }
        catch(...){
            printf("fin error");
            return false;
        }

        return true;
    }
};
#endif
