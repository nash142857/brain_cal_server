#include "contable.h"

/*
 * 默认参数
 */
PWIConstTable::PWIConstTable()
{
	PSetNum=60;
	PSliceNum=20;
	STDNum=4;
	PMask=0.5;
	TE = 30;
	ck1=1.0;
	ck2=-3.5;
	ck3=-1.0;
	TType=1;
	x[0][0]=22.7;
	x[0][1]=0.0076;
	x[0][2]=0.000574;
	x[1][0]=11.5;
	x[1][1]=0.000493;
	x[1][2]=0.00262;
	pr=0.15;
	ro=1.04;
	HSV=0.25;
	HLV=0.45;
	Coef=100;
	CBFC=60;
	MTTC=60;
	Tmax_th=6;
	VP_th=3;
}
DWIConstTable::DWIConstTable()
{
	DSetNum=2;
	DSliceNum=20;
	b=1000;
	ADCMin=0.0001;
	ADCMax=0.0021;
	ADC_th=0.0006;
	VD_th=1;
}
