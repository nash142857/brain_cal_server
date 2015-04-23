#ifndef USEFULLTOOLS_H
#define USEFULLTOOLS_H
#include <string>
#include "DCMHeaders.h"
#include "symbol.h"
#include <complex>
#define PI 3.14159265358979323846
using namespace std;
string itos(int x);
Uint16 OFtoUint16(OFString s);
_signal OFtoSignal(OFString s);
void FFT(complex<_signal>*in,complex<_signal>*out ,int r);
void IFFT(complex<_signal> * pCFData, complex<_signal> * pCTData, int nLevel);
#endif
