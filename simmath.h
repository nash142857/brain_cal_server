#ifndef SIMMATH_H
#define SIMMATH_H
#include "symbol.h"
#include <cmath>
#include "contable.h"
#include <fftw3.h>
/*
*每个函数算一个点的值
*/


/*计算R2*/
inline _signal R2(_signal s0,_signal st,_signal TE)
{
	return -1.0/TE*log((_signal)st/s0);
}
/*计算对剂浓度*/
inline _signal Ct(_signal r2,int i,_signal x1)
{
	return  x1*r2;
}
/*AIF/VOF曲线*/
inline bool getCavPart(_signal r2,int i,_signal x2,_signal x3)
{
	if(x2*x2+4*x3*r2>=0)
		return true;
	else
		return false;

}
inline _signal Cav(_signal r2,int i,_signal x2,_signal x3)//i是TType
{
	
	_signal temp;
	if(r2>=0)
		temp =(-1*x2+sqrt(x2*x2+4*x3 * r2))/(2*x3);
	else
        temp=(x2-sqrt((x2*x2+4*x3 * r2)>=0?(x2*x2+4*x3 * r2):0))/(2*x3);
	return temp;

}
/*计算经验常数k0*/
inline _signal getK0(_signal ro ,_signal HSV,_signal HLV)
{
	return 100/ro*((1.0-HSV)/(1.0-HLV));
}
/*计算积分*/
inline _signal calcul(_signal jg,_signal a[],int n)//jg时间间隔 n数组大小  梯形公式
{
	_signal res=0;
	int i=1;
	while(i<n)
	{

		res+=(a[i]+a[i-1])*jg/2;
		++i;
	}
	return res;
}

/*计算ADC值*/

inline _signal ADC_cal(int b,_signal s0,_signal s1000)
{
	if(s0==0||s1000==0)
		return 0;
	else
		return -1.0*log(1.0*s1000/s0)/b;
}
/*复数除法*/
inline _signal Divide_Real(fftw_complex a,fftw_complex b)
{
	return (a[0]*b[0]+a[1]*b[1])/(b[0]*b[0]+b[1]*b[1]);
}
inline _signal Divide_Img(fftw_complex a,fftw_complex b)
{
	return (a[1]*b[0]- a[0]*b[1])/(b[0]*b[0]+b[1]*b[1]);
}
/*一维转二维*/
inline int  TWO_D(int i,int j,int length)
{
	return i*length+j;
}
#endif
