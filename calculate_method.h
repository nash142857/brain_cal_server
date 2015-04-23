#ifndef calculate_H
#define calculate_H

#include "QTSource.h"
#include "common.h"
#include "DicomInfo.h"
#include <fftw3.h>
#include "contable.h"
#include "QsLog.h"
/*
 * interface and usage:
 *  INIT: init(parameter const)
 *  CALCULATE: DWI_Compute PWI_Compute
 *  RELEASE: del
 */

class calculate_method{
public:
    Uint16 * pixelBmp;
    double * pixel;
    DicomInfo  *PWIArray;
    DicomInfo *PWICav;
    DicomInfo *PWICt;
    DicomInfo *PWIRt;
    _signal *tempCt;
    _signal *tempCa;
    DicomInfo * PWICBV, *PWIMTT, *PWICBF, *PWITmax;
    fftw_complex *TempInFFTCt, *TempOutFFTCt,*TempInFFTCa,*TempOutFFTCa,*TempInIFFT,*TempOutIFFT;
    //init space
    calculate_method(){
        pixelBmp = NULL;
        pixel = NULL;
        PWIArray = PWICav = PWICt = PWIRt = NULL;
        TempInFFTCt = TempOutFFTCt = TempInFFTCa = TempOutFFTCa = TempInIFFT = TempOutIFFT = NULL;
        tempCt = tempCa = NULL;
        PWICBV = PWIMTT = PWICBF = PWITmax = NULL;
    }

    void init(PWIConstTable *PWIconst){
        QLOG_DEBUG() << "begin init method";
        auto S0Index = PWIconst->STDNum-1;
        pixelBmp = new Uint16[256 * 256];
        pixel = new double[256 * 256];
        PWIArray = new DicomInfo[ PWIconst->PSetNum * PWIconst->PSliceNum ];
        PWICav=new DicomInfo [PWIconst->PSetNum*PWIconst->PSliceNum];
        PWICt = new DicomInfo [PWIconst->PSetNum*PWIconst->PSliceNum];
        PWIRt =new DicomInfo [PWIconst->PSetNum*PWIconst->PSliceNum];
        TempInFFTCt = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * 2 * (PWIconst->PSetNum-S0Index));
        TempOutFFTCt = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * 2 * (PWIconst->PSetNum-S0Index));
        TempInFFTCa = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * 2 * (PWIconst->PSetNum-S0Index));
        TempOutFFTCa = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * 2 * (PWIconst->PSetNum-S0Index));
        TempInIFFT = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * 2 * (PWIconst->PSetNum-S0Index));
        TempOutIFFT = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * 2* (PWIconst->PSetNum-S0Index));
        tempCt=new _signal [PWIconst->PSetNum];
        tempCa=new _signal [PWIconst->PSetNum];
        PWICBV = new DicomInfo[ PWIconst->PSliceNum ];
        PWIMTT = new DicomInfo[ PWIconst->PSliceNum ];
        PWICBF = new DicomInfo[ PWIconst->PSliceNum ];
        PWITmax = new DicomInfo[ PWIconst->PSliceNum ];
        QLOG_DEBUG() << "end init method";
    }
    int DWI_Compute(string input,string output,DWIConstTable *DWIConst);
    int PWI_Compute(string input,string output,PWIConstTable *PWIconst);


    //release space
    void del(){
        puts("begin del");
        //TODO MODIFY DELETE
        /*
        delete_arr_if_not_null(PWICBF);
        puts("end del");
        delete_arr_if_not_null(PWITmax);
        puts("end del");
        delete_arr_if_not_null(PWICav);
        puts("end del");
        delete_arr_if_not_null(PWICt);
        puts("end del");
        delete_arr_if_not_null(PWIRt);
        puts("end del");
        delete_arr_if_not_null(PWICBV);
        puts("end del");
        delete_arr_if_not_null(PWIMTT);
        puts("end del");
        delete_arr_if_not_null(PWIArray);
        puts("end del");
        delete_arr_if_not_null(tempCt);
        puts("end del");
        delete_arr_if_not_null(tempCa);
        if(TempInFFTCt != NULL){
            fftw_free(TempInFFTCt);
        }
        if(TempOutFFTCt != NULL){
            fftw_free(TempOutFFTCt);
        }
        if(TempInFFTCa != NULL){
            fftw_free(TempInFFTCa);
        }
        if(TempOutFFTCa != NULL){
            fftw_free(TempOutFFTCa);
        }
        if(TempInIFFT != NULL){
            fftw_free(TempInIFFT);
        }
        if(TempOutIFFT != NULL){
            fftw_free(TempOutIFFT);
        }
        delete_arr_if_not_null(pixel);
        delete_arr_if_not_null(pixelBmp);
        */
        puts("end del");
    }
    ~calculate_method(){
        del();
    }
};

#endif
