#include "calculate_method.h"
#include "QTSource.h"
#include "contable.h"
#include "DCMCompute.h"
#include "SortDcmFile.h"
#include <fftw3.h>

int calculate_method::DWI_Compute(string input,string output,DWIConstTable *DWIConst)
{
    const int MIN = 900;
    const Uint16 *pixel1;
    const Uint16 *pixel2;
    DcmFileFormat fileformat1;
    DcmFileFormat fileformat2;
    int sum=-1;
    string temInaddress = input;
    string temOutaddress = output;
    cout<<"after DWI sort"<<endl;
    for(int i=0;i<DWIConst->DSliceNum;++i)
    {

        int height,width;
        height=256;
        width=256;
            OFCondition os1 =fileformat2.loadFile((temInaddress+"IM0-"+itos(i)).c_str());//+"_"+itos(SL)
            OFCondition os2 =fileformat1.loadFile((temInaddress+"IM1-"+itos(i)).c_str());//+"_"+itos(SL)
            //cout<<image->getHeight()<<endl;
            if(os1.good() && os2.good()){
            }
            else{
                printf("wrong format\n");
                continue;
            }
            fileformat1.getDataset()->findAndGetUint16Array(DCM_PixelData, pixel1);
            fileformat2.getDataset()->findAndGetUint16Array(DCM_PixelData, pixel2);
            DcmDataset *ds=fileformat2.getDataset();
            sum=-1;
            for(int j=0;j<height;++j)
            {
                for(int k=0;k<width;++k)
                {
                    pixelBmp[j * 256 + k]=0;

                    if(pixel1[j*height+k] >= MIN)
                    {
                        pixel[j * 256 + k]=ADC_cal(1000,(_orig)pixel1[j*height+k],(_orig)pixel2[j*height+k]);
                    }
                    else
                    {
                        pixel[j * 256 + k]=0.0;
                    }
                    if(pixel[j * 256 + k]>= DWIConst->ADCMin)//)pixel[j][k]<=(2100*0.000001)  &&pixel[j][k]<=600*0.000001
                    {
                        if(pixel[j * 256 + k]<DWIConst->ADCMax)
                        {
                            pixelBmp[j * 256 + k]=(Uint16)(pixel[j * 256 + k]*1000000);
                        }else
                        {
                            pixelBmp[j * 256 + k]=(Uint16)(DWIConst->ADCMax*1000000);
                        }
                    }else
                    {
                        pixelBmp[j * 256 + k]=0;
                    }

                }
            }
            ds->putAndInsertString(DCM_WindowCenter,"1300");//1300
            ds->putAndInsertString(DCM_WindowWidth,"2100");//2100
            ds->putAndInsertUint16Array(DCM_PixelData,pixelBmp,256*256);
            fileformat2.saveFile((temOutaddress+"ADC_"+itos(i+1)).c_str(),EXS_LittleEndianExplicit);//+"_"+itos(SL)

    }
    //delete for new
    puts("DWI DONE");
    return 0;
}
int calculate_method::PWI_Compute(string input,string output,PWIConstTable *PWIconst)
{
    int S0Index;
    //PWI read
    DicomInfo PWIinfo;
     cout<<"after sort"<<endl;
    getDcm *PWIcommon=new getDcm(input);
    //读取PWI原始信息
    for(int i = 0 ;i < PWIconst->PSetNum ; i++)
    {
        for(int j = 0 ; j < PWIconst->PSliceNum ;j++)
        {
            PWIinfo.getInfo((PWIcommon->dir+"IM"+itos(i)+"-"+itos(j)).c_str());
            PWIArray[TWO_D(i,j,PWIconst->PSliceNum)] = PWIinfo;
        }
    }
    PWIArray[TWO_D(0,0,PWIconst->PSetNum)].dataset->findAndGetUint16(DcmTagKey(0x0028,0x0010),PWIcommon->height); //读取图片高度
    PWIArray[TWO_D(0,0,PWIconst->PSetNum)].dataset->findAndGetUint16(DcmTagKey(0x0028,0x0011),PWIcommon->width);  //读取图片宽度
    DicomInfo *s0 = new DicomInfo[PWIconst->PSliceNum];
    for(int i = 0; i < PWIconst->PSliceNum; ++i)
    {
        s0[i].allocMemory( PWIcommon->height * PWIcommon->width);
    }

    S0Index =  PWIconst->STDNum-1;
    //获取S0值
    for(int i = 0;i < PWIconst->PSliceNum ; ++i)
    {
        for(int j = 0;j < PWIcommon->height; ++j)
        {
            for(int k = 0;k < PWIcommon->width;++k)
            {
                s0[i].PixelData[TWO_D(j,k,PWIcommon->width)] = PWIArray[TWO_D(S0Index,i,PWIconst->PSliceNum)].PixelData[TWO_D(j,k,PWIcommon->width)];
            }
        }
    }



    //获取s0的均值
    int signalnum,s0Sum = 0;
    _signal avg=0;
    for(int i = 0;i < PWIconst->PSliceNum ; ++i )
    {
        signalnum=0;
        s0Sum = 0;
        for(int j = 0;j < PWIcommon->height; ++j)
        {
            for(int k = 0 ; k < PWIcommon->width; ++k)
            {
                if(s0[i].PixelData[TWO_D(j,k,PWIcommon->width)] != 0)
                {
                    s0Sum += PWIArray[TWO_D(S0Index,i,PWIconst->PSliceNum)].PixelData[TWO_D(j,k,PWIcommon->width)];
                    signalnum++;
                }
            }
        }
        avg = ((_signal)s0Sum)/signalnum * 0.5;
        //过滤掉小于50%的点   mask ADC 600-2100!!!!!
        for(int k = 0;k < PWIcommon->height; ++k)
            {
                for(int l = 0;l < PWIcommon->width; ++l)
                {
                    if(s0[i].PixelData[TWO_D(k,l,PWIcommon->width)]<avg)
                    {
                        s0[i].PixelData[TWO_D(k,l,PWIcommon->width)] = 0 ;
                    }
                }
            }

    }
    //计算R2
    for(int i = 0;i < PWIconst->PSetNum ; ++i)
    {
        for(int j = 0;j < PWIconst->PSliceNum; ++j)
        {
            for(int k = 0;k < PWIcommon->height; ++k)
            {
                for(int l = 0;l < PWIcommon->width; ++l)
                {
                    //判断是否为过滤掉的点
                    if(s0[j].PixelData[TWO_D(k,l,PWIcommon->width)] != 0 )
                    {
                        if(PWIArray[TWO_D(i,j,PWIconst->PSliceNum)].PixelData[TWO_D(k,l,PWIcommon->width)]!=0)
                        {
                            PWIArray[TWO_D(i,j,PWIconst->PSliceNum)].PixelData[TWO_D(k,l,PWIcommon->width)]=R2(s0[j].PixelData[TWO_D(k,l,PWIcommon->width)],PWIArray[TWO_D(i,j,PWIconst->PSliceNum)].PixelData[TWO_D(k,l,PWIcommon->width)],PWIconst->TE);
                        }else
                        {
                            //当St值为0时的处理方法
                            PWIArray[TWO_D(i,j,PWIconst->PSliceNum)].PixelData[TWO_D(k,l,PWIcommon->width)]=R2(s0[j].PixelData[TWO_D(k,l,PWIcommon->width)],1,PWIconst->TE);//测试第二个参数!!!!
                        }
                    }
                    else
                    {
                        //将过滤掉的点置为0
                        PWIArray[TWO_D(i,j,PWIconst->PSliceNum)].PixelData[TWO_D(k,l,PWIcommon->width)] = 0;
                    }


                }
            }

        }
    }
    //get Position of AIF

    for(int i = 0;i < PWIconst->PSetNum ; ++i)//分配内存
    {
        for(int j = 0;j <  PWIconst->PSliceNum; ++j)
        {

            PWICav[TWO_D(i,j,PWIconst->PSliceNum)].allocMemory(PWIcommon->height*PWIcommon->width);//分配空间
            PWICt[TWO_D(i,j,PWIconst->PSliceNum)].allocMemory(PWIcommon->height*PWIcommon->width);
            PWIRt[TWO_D(i,j,PWIconst->PSliceNum)].allocMemory(PWIcommon->height*PWIcommon->width);
            PWICav[TWO_D(i,j,PWIconst->PSliceNum)].time=PWIArray[TWO_D(i,j,PWIconst->PSliceNum)].time;
        }
    }
    int MRtype =1;// PWIconst->TType;//MRI类型 1.5T 或者 3.0T
    for(int i = 0 ;i < PWIconst->PSetNum; ++i)
    {
        for(int j = 0;j < PWIconst->PSliceNum;++j)
        {
            for(int k = 0;k < PWIcommon->height;++k)
            {
                for(int l = 0;l < PWIcommon->width;++l)
                {

                       PWICt[TWO_D(i,j,PWIconst->PSliceNum)].PixelData[TWO_D(k,l,PWIcommon->width)]=  Ct(PWIArray[TWO_D(i,j,PWIconst->PSliceNum)].PixelData[TWO_D(k,l,PWIcommon->width)],MRtype,PWIconst->x[MRtype][0]);
                }
            }
        }
    }



    //AIF Selection
    //get avg of Area
    DicomInfo * AreaAndCt = new DicomInfo[ PWIconst->PSliceNum ];
    for(int j = 0 ; j < PWIconst->PSliceNum ; ++j)
    {
        AreaAndCt[j].allocMemory(PWIcommon->height*PWIcommon->width);
        for(int i =0 ; i < PWIcommon->height*PWIcommon->width ; ++i)
        {
            AreaAndCt[j].PixelData[i] = 0;
        }
    }
    double temArea = 0;
    int AllCurve = 0;
    _signal Area;
    _signal SecondOrderThreshold;
    int AIFtestZ;
    int AIFtestX;
    int AIFtestY;
    double testC = 0;
    double temC = 0;
    vector<double> AreaSet;
    _signal * DsFirstOrder = new _signal [PWIconst->PSetNum];
    _signal * DsSecondOrder = new _signal [PWIconst->PSetNum];
    for(int j = 10;j < 20; ++j)
    {
        for(int k = 0;k < PWIcommon->height*2/3; ++k)
            {

                for(int l = 0;l < PWIcommon->width; ++l)
                {
                    if(s0[j].PixelData[TWO_D(k,l,PWIcommon->width)]==0)
                    {
                        continue;
                    }
                    AllCurve++;
                    for(int i = PWIconst->STDNum-1;i < PWIconst->PSetNum-1 ; ++i)
                    {
                        AreaAndCt[j].PixelData[TWO_D(k,l,PWIcommon->width)] += PWIArray[TWO_D(i,j,PWIconst->PSliceNum)].PixelData[TWO_D(k,l,PWIcommon->width)]+PWIArray[TWO_D(i+1,j,PWIconst->PSliceNum)].PixelData[TWO_D(k,l,PWIcommon->width)];
                    }
                    AreaSet.push_back(AreaAndCt[j].PixelData[TWO_D(k,l,PWIcommon->width)]);
                }
            }
    }
    sort(AreaSet.begin(),AreaSet.end());
    Area = AreaSet[(int)(AllCurve*0.9)];
    AreaSet.clear();
    //Ct daoshu
    AllCurve = 0;
    for(int j = 10;j < 20; ++j)
    {
        for(int k = 0;k < PWIcommon->height*2/3; ++k)
            {

                for(int l = 0;l < PWIcommon->width; ++l)
                {
                    if(s0[j].PixelData[TWO_D(k,l,PWIcommon->width)]==0||AreaAndCt[j].PixelData[TWO_D(k,l,PWIcommon->width)]<Area)
                    {
                        AreaAndCt[j].PixelData[TWO_D(k,l,PWIcommon->width)] = 0;
                        continue;
                    }
                    ++AllCurve;
                    for(int i = PWIconst->STDNum-1; i< PWIconst->PSetNum ; ++i)
                    {
                        DsSecondOrder[i] = PWIArray[TWO_D(i,j,PWIconst->PSliceNum)].PixelData[TWO_D(k,l,PWIcommon->width)];
                    }
                    for(int i = PWIconst->STDNum-1;i < PWIconst->PSetNum ; ++i)
                    {
                        if(i==PWIconst->STDNum-1)
                        {
                            DsFirstOrder[i] = (-3*DsSecondOrder[i] + 4 * DsSecondOrder[i+1] - DsSecondOrder[i+2]);

                        }else if(i == PWIconst->PSetNum-1)
                        {
                            DsFirstOrder[i] = (DsSecondOrder[i-2] - 4 * DsSecondOrder[i-1] + 3 * DsSecondOrder[i]);

                        }else
                        {
                            DsFirstOrder[i] = ( -1*DsSecondOrder[i-1]+DsSecondOrder[i+1]);

                        }
                    }
                    for(int i = PWIconst->STDNum-1;i < PWIconst->PSetNum ; ++i)
                    {
                        if(i==PWIconst->STDNum-1)
                        {
                            DsSecondOrder[i] = (-3*DsFirstOrder[i] + 4 * DsFirstOrder[i+1] - DsFirstOrder[i+2]);

                        }else if(i == PWIconst->PSetNum-1)
                        {
                            DsSecondOrder[i] = (DsFirstOrder[i-2] - 4 * DsFirstOrder[i-1] + 3 * DsFirstOrder[i]);

                        }else
                        {
                            DsSecondOrder[i] = ( -1*DsFirstOrder[i-1]+DsFirstOrder[i+1]);

                        }
                        DsSecondOrder[i]=DsSecondOrder[i]*DsSecondOrder[i];
                    }
                    _signal temSecondOrder = calcul(1.8,DsSecondOrder+PWIconst->STDNum-1,PWIconst->PSetNum-PWIconst->STDNum);
                    AreaAndCt[j].PixelData[TWO_D(k,l,PWIcommon->width)] = temSecondOrder;
                    AreaSet.push_back(temSecondOrder);

                }
            }
    }
    sort(AreaSet.begin(),AreaSet.end());
    SecondOrderThreshold = AreaSet[(int)(AllCurve*0.75)];
    AreaSet.clear();
    for(int j = 10;j < 20; ++j)
    {
        for(int k = 0;k < PWIcommon->height*2/3; ++k)
            {

                for(int l = 0;l < PWIcommon->width; ++l)
                {
                    if(AreaAndCt[j].PixelData[TWO_D(k,l,PWIcommon->width)]>SecondOrderThreshold)
                    {
                        AreaAndCt[j].PixelData[TWO_D(k,l,PWIcommon->width)] = 0;
                    }
                }
        }
    }
    /**SecondOrder version**/
    double avgTime,avgHeight,avgWidth;
    avgTime = 0;
    avgHeight = 0;
    avgWidth = 0;
    double minIndex = 0;
    double maxIndex = 0;
    double maxR2 = -100;
    for(int j = 10;j < 20; ++j)
    {
        for(int k = 0;k < PWIcommon->height*2/3; ++k)
        {

                for(int l = 0;l < PWIcommon->width; ++l)
                {
                    minIndex = PWIconst->PSetNum-1;
                    maxIndex = -1;
                    maxR2 = -100;
                    if(AreaAndCt[j].PixelData[TWO_D(k,l,PWIcommon->width)] == 0)
                    {
                        continue;
                    }
                    for(int i = PWIconst->STDNum-1; i< PWIconst->PSetNum ; ++i)
                    {

                        DsSecondOrder[i] = PWIArray[TWO_D(i,j,PWIconst->PSliceNum)].PixelData[TWO_D(k,l,PWIcommon->width)];
                        if(maxR2<DsSecondOrder[i])
                            maxR2 = DsSecondOrder[i];
                    }
                    for(int i = PWIconst->STDNum-1;i < PWIconst->PSetNum ; ++i)
                    {
                        if(i==PWIconst->STDNum-1)
                        {
                            DsFirstOrder[i] = (-3*DsSecondOrder[i] + 4 * DsSecondOrder[i+1] - DsSecondOrder[i+2]);

                        }else if(i == PWIconst->PSetNum-1)
                        {
                            DsFirstOrder[i] = (DsSecondOrder[i-2] - 4 * DsSecondOrder[i-1] + 3 * DsSecondOrder[i]);

                        }else
                        {
                            DsFirstOrder[i] = ( -1*DsSecondOrder[i-1]+DsSecondOrder[i+1]);

                        }
                    }
                    for(int i = PWIconst->STDNum+1;i < PWIconst->PSetNum ; ++i)
                    {
                        if(i==PWIconst->STDNum-1)
                        {
                            DsSecondOrder[i] = (-3*DsFirstOrder[i] + 4 * DsFirstOrder[i+1] - DsFirstOrder[i+2]);

                        }else if(i == PWIconst->PSetNum-1)
                        {
                            DsSecondOrder[i] = (DsFirstOrder[i-2] - 4 * DsFirstOrder[i-1] + 3 * DsFirstOrder[i]);

                        }else
                        {
                            DsSecondOrder[i] = ( -1*DsFirstOrder[i-1]+DsFirstOrder[i+1]);

                        }
                    }
                    for(int i = PWIconst->STDNum+1;i < PWIconst->PSetNum ; ++i)
                    {
                        if(DsSecondOrder[i] >0.008)
                        {
                            if(i<minIndex)
                            {
                                minIndex = i;
                            }
                            if(i>maxIndex)
                            {
                                maxIndex =i;
                            }
                        }
                    }
                    if(maxIndex == -1)
                    {
                        maxIndex =PWIconst->PSetNum-1;
                    }
                    if(minIndex == PWIconst->PSetNum-1)
                    {
                        minIndex =0;
                        maxIndex = PWIconst->PSetNum-1;
                    }
                    avgHeight += maxR2 * 100;
                    avgTime += minIndex* 1.8;
                    avgWidth += (maxIndex - minIndex)*1.8;

                }
        }
    }
    avgHeight/=AllCurve;
    avgWidth/=AllCurve;
    avgTime/=AllCurve;
    for(int j = 10;j < 20; ++j)
    {
        for(int k = 0;k < PWIcommon->height*2/3; ++k)
        {

                for(int l = 0;l < PWIcommon->width; ++l)
                {
                    minIndex = PWIconst->PSetNum-1;
                    maxIndex = -1;
                    maxR2 = -100;
                    if(AreaAndCt[j].PixelData[TWO_D(k,l,PWIcommon->width)] == 0)
                    {
                        continue;
                    }
                    for(int i = PWIconst->STDNum-1; i< PWIconst->PSetNum ; ++i)
                    {

                        DsSecondOrder[i] = PWIArray[TWO_D(i,j,PWIconst->PSliceNum)].PixelData[TWO_D(k,l,PWIcommon->width)];
                        if(maxR2<DsSecondOrder[i])
                            maxR2 = DsSecondOrder[i];
                    }
                    for(int i = PWIconst->STDNum-1;i < PWIconst->PSetNum ; ++i)
                    {
                        if(i==PWIconst->STDNum-1)
                        {
                            DsFirstOrder[i] = (-3*DsSecondOrder[i] + 4 * DsSecondOrder[i+1] - DsSecondOrder[i+2]);

                        }else if(i == PWIconst->PSetNum-1)
                        {
                            DsFirstOrder[i] = (DsSecondOrder[i-2] - 4 * DsSecondOrder[i-1] + 3 * DsSecondOrder[i]);

                        }else
                        {
                            DsFirstOrder[i] = ( -1*DsSecondOrder[i-1]+DsSecondOrder[i+1]);

                        }
                    }
                    for(int i = PWIconst->STDNum+1;i < PWIconst->PSetNum ; ++i)
                    {
                        if(i==PWIconst->STDNum-1)
                        {
                            DsSecondOrder[i] = (-3*DsFirstOrder[i] + 4 * DsFirstOrder[i+1] - DsFirstOrder[i+2]);

                        }else if(i == PWIconst->PSetNum-1)
                        {
                            DsSecondOrder[i] = (DsFirstOrder[i-2] - 4 * DsFirstOrder[i-1] + 3 * DsFirstOrder[i]);

                        }else
                        {
                            DsSecondOrder[i] = ( -1*DsFirstOrder[i-1]+DsFirstOrder[i+1]);

                        }
                    }
                    for(int i = PWIconst->STDNum+1;i < PWIconst->PSetNum ; ++i)
                    {
                        if(DsSecondOrder[i] >0.008)
                        {
                            if(i<minIndex)
                            {
                                minIndex = i;
                            }
                            if(i>maxIndex)
                                maxIndex =i;
                        }
                    }
                    if(maxIndex == -1)
                    {
                        maxIndex =PWIconst->PSetNum-1;
                    }
                    if(minIndex == PWIconst->PSetNum-1)
                    {
                        minIndex =0;
                        maxIndex = PWIconst->PSetNum-1;
                    }
                    if(minIndex == maxIndex)
                        maxIndex = PWIconst->PSetNum-1;

                        temC = PWIconst->ck1*(maxR2*100 - avgHeight) + PWIconst->ck2*(minIndex*1.8 - avgTime) + PWIconst->ck3 * ((maxIndex - minIndex)*1.8 - avgWidth);
                        if(maxR2*100>avgHeight && minIndex*1.8<avgTime && (maxIndex - minIndex)*1.8<avgWidth)
                        {
                            if(temC > testC)
                            {
                                testC = temC;
                                AIFtestZ = j;
                                AIFtestX = l;
                                AIFtestY = k;
                            }
                        }

                }
        }
    }
    delete  [] AreaAndCt;
    delete  [] DsFirstOrder;
    delete  [] DsSecondOrder;
    int AIFz = AIFtestZ;
    int AIFy = AIFtestY;
    int AIFx = AIFtestX;

    /*AIF end*/



    for(int i = 0 ;i < PWIconst->PSetNum; ++i)
    {
        for(int j = 0;j < PWIconst->PSliceNum;++j)
        {
            for(int k = 0;k < PWIcommon->height;++k)
            {
                for(int l = 0;l < PWIcommon->width;++l)
                {
                    //获取Cav的值
                        PWICav[TWO_D(i,j,PWIconst->PSliceNum)].PixelData[TWO_D(k,l,PWIcommon->width)]=Cav(PWIArray[TWO_D(i,j,PWIconst->PSliceNum)].PixelData[TWO_D(k,l,PWIcommon->width)],MRtype,PWIconst->x[MRtype][1],PWIconst->x[MRtype][2]);

                }
            }
        }
    }
    Uint16 temPixel[128*128];
       cout<<"Cav Ct OK"<<endl;
    //给FFT输入做赋值
    fftw_plan p;
    //计算卷积
    for(int i = 0 ;i < PWIconst->PSliceNum; ++i )
    {

            for(int j = 0; j < PWIcommon->height;++j)
            {
                for(int k = 0;k < PWIcommon->width; ++k)
                {
                    //zero padding!!!!!!!!!!!!!! 不是这样
                    for(int l = S0Index ; l < PWIconst->PSetNum;++l)
                    {   //15 52 44
                        TempInFFTCt[l-S0Index][0]=PWICt[TWO_D(l,i,PWIconst->PSliceNum)].PixelData[TWO_D(j,k,PWIcommon->width)];
                        TempInFFTCa[l-S0Index][0]=PWICav[TWO_D(l,AIFz,PWIconst->PSliceNum)].PixelData[TWO_D(AIFy,AIFx,PWIcommon->width)];
                        TempInFFTCt[l-S0Index][1] = 0;
                        TempInFFTCa[l-S0Index][1] = 0;
                    }
                    for(int l = PWIconst->PSetNum-S0Index ; l < 2*(PWIconst->PSetNum-S0Index);++l)
                    {
                        TempInFFTCt[l][0] = 0;
                        TempInFFTCa[l][0] = 0;
                        TempInFFTCt[l][1] = 0;
                        TempInFFTCa[l][1] = 0;
                    }
                    //计算FFT
                    p=fftw_plan_dft_1d( PWIconst->PSetNum-S0Index ,TempInFFTCt ,TempOutFFTCt , FFTW_FORWARD, FFTW_ESTIMATE);
                    fftw_execute(p);
                    fftw_destroy_plan(p);
                    p=fftw_plan_dft_1d(PWIconst->PSetNum-S0Index ,TempInFFTCa, TempOutFFTCa , FFTW_FORWARD, FFTW_ESTIMATE);
                    fftw_execute(p); //理解out
                    fftw_destroy_plan(p);
                    //FFT(TempInFFTCt,TempOutFFTCt,FFTLayer);
                    //FFT(TempInFFTCa,TempOutFFTCa,FFTLayer);
                    _signal maxFFTCa=-99999;
                    for(int l = 0 ; l < 2 * (PWIconst->PSetNum - S0Index); ++l)
                    {
                        int tempFFTCa = sqrt(TempOutFFTCa[l][0]*TempOutFFTCa[l][0]+TempOutFFTCa[l][1]*TempOutFFTCa[l][1]);
                        if(maxFFTCa < tempFFTCa)
                            maxFFTCa = tempFFTCa;
                    }

                    _signal TempN = maxFFTCa * PWIconst->pr/2;
                    _signal TempG;
                    for(int l = 0 ; l < 2 * (PWIconst->PSetNum - S0Index); ++l)
                    {
                        int tempFFTCa = sqrt(TempOutFFTCa[l][0]*TempOutFFTCa[l][0]+TempOutFFTCa[l][1]*TempOutFFTCa[l][1]);
                        if(tempFFTCa > TempN && l != 0)
                        {
                            TempG=(tempFFTCa*tempFFTCa-TempN*TempN)/(tempFFTCa*tempFFTCa);
                        }else if(l==0)
                        {
                            TempG=1;
                        }else
                        {
                            TempG=0;
                        }
                        if(TempG!=0)
                        {
                            TempInIFFT[l][0]=TempG * Divide_Real(TempOutFFTCt[l],TempOutFFTCa[l]);
                            TempInIFFT[l][1]=TempG * Divide_Img(TempOutFFTCt[l],TempOutFFTCa[l]);
                        }else
                        {
                            TempInIFFT[l][0]=0;
                            TempInIFFT[l][1]=0;
                        }
                        //cout<<TempInIFFT[l]<<endl;

                    }
                    //计算IFFT
                    p=fftw_plan_dft_1d(PWIconst->PSetNum-S0Index,TempInIFFT,TempOutIFFT, FFTW_BACKWARD, FFTW_ESTIMATE);
                    fftw_execute(p);
                    fftw_destroy_plan(p);

                    //求出Rt
                    for(int l = 0 ; l < PWIconst->PSetNum-S0Index; ++l)
                    {
                        PWIRt[TWO_D(l,i,PWIconst->PSliceNum)].PixelData[TWO_D(j,k,PWIcommon->width)]=1/(PWIconst->PSetNum*1.8)*TempOutIFFT[l][0];
                        //cout<<PWIRt[TWO_D(i,l,PWIconst->PSliceNum)].PixelData[TWO_D(j,k,PWIcommon->width)]<<endl;
                    }

                }

            }
    }

    //free fft



    //估算Kav
    int calVOF;
    int calAIF;
    _signal temVOF[62];
    _signal temAIF[62];
    calVOF = calcul(1800,temVOF,PWIconst->PSetNum);
    calAIF = calcul(1800,temAIF,PWIconst->PSetNum);

    cout<<"KAV:   AIF:"<<calAIF<<"   VOF:"<<calVOF<<endl;

    //计算CBV MTT
    for(int i = 0; i < PWIconst->PSliceNum; ++i)
    {
        PWICBV[i].allocMemory(PWIcommon->height*PWIcommon->width);
        PWIMTT[i].allocMemory(PWIcommon->height*PWIcommon->width);
        PWICBF[i].allocMemory(PWIcommon->height*PWIcommon->width);
        PWITmax[i].allocMemory(PWIcommon->height*PWIcommon->width);

    }

    _signal Integral_Ca=0;
    _signal Integral_Ct=0;
    _signal k0 = getK0(PWIconst->ro,PWIconst->HSV,PWIconst->HLV);
    int kav =1;

        for(int j = 0 ; j < PWIconst->PSliceNum; ++j)
        {
            for(int k = 0; k < PWIcommon->height; ++k)
            {
                for(int l = 0; l < PWIcommon->width; ++l)
                {
                    _signal maxRt = -10000;
                    _signal maxRtt = 0;
                    for(int i = 0; i < PWIconst->PSetNum-S0Index; ++i)
                    {
                        tempCt[i]=PWICt[TWO_D(i+S0Index,j,PWIconst->PSliceNum)].PixelData[TWO_D(k,l,PWIcommon->width)];
                        tempCa[i]=PWICav[TWO_D(i+S0Index,AIFz,PWIconst->PSliceNum)].PixelData[TWO_D(AIFy,AIFx,PWIcommon->width)];
                        //求出rt达到最大值时的t值
                        /*if(j==SelectSlice&&k==index_x&&l==index_y)
                        {
                            cout<<PWIRt[TWO_D(i,j,PWIconst->PSliceNum)].PixelData[TWO_D(k,l,PWIcommon->width)]<<" ";
                        }*/
                        //i<=(PWIconst->PSetNum-S0Index)/2+1 &&
                        if( i <= (PWIconst->PSetNum-S0Index)/2+1 && PWIRt[TWO_D(i,j,PWIconst->PSliceNum)].PixelData[TWO_D(k,l,PWIcommon->width)]>maxRt)
                        {
                            maxRt = PWIRt[TWO_D(i,j,PWIconst->PSliceNum)].PixelData[TWO_D(k,l,PWIcommon->width)];
                            maxRtt = i*1.8;

                        }
                    }

                    Integral_Ca=calcul(1800,tempCa,PWIconst->PSetNum-S0Index);
                    Integral_Ct=calcul(1800,tempCt,PWIconst->PSetNum-S0Index);
                    //if(s0[j].PixelData[TWO_D(k,l,PWIcommon->width)] != 0)//是否在mask内
                        PWICBV[j].PixelData[TWO_D(k,l,PWIcommon->width)]=Integral_Ct/Integral_Ca*k0*kav;
                    //else
                    //	PWICBV[j].PixelData[TWO_D(k,l,PWIcommon->width)] = 0;


                    PWICBF[j].PixelData[TWO_D(k,l,PWIcommon->width)]= maxRt*k0*kav*60;
                    PWIMTT[j].PixelData[TWO_D(k,l,PWIcommon->width)] = maxRtt;

                    /*if(j==SelectSlice&&l==index_x&&k==index_y)
                    {
                            cout<<"MaxRt:"<<maxRt<<" PWICBF:"<<PWICBF[j].PixelData[TWO_D(k,l,PWIcommon->width)]<<"  shu:"<<maxRt*k0*kav*60<<endl;
                    }*/

                }


            }

            string s_mtt_tem = output+"bmpMTT/"+itos(j)+"_MTT.bmp";
            string s_tmax_tem = output+"bmpTmax/"+itos(j)+"_Tmax.bmp";
            string s_cbf_tem = output+"bmpCBF/"+itos(j)+"_CBF.bmp";
            string s_cbv_tem = output+"bmpCBV/"+itos(j)+"_CBV.bmp";
            //计算MTT
            for(int i = 0 ; i < 128*128 ; ++i )
            {
                PWITmax[j].PixelData[i] = 60*PWICBV[j].PixelData[i]/PWICBF[j].PixelData[i];
            }
            //输出MTT
            for(int i = 0 ; i < 128*128 ; ++i )
            {
                temPixel[i] = (Uint16)(PWITmax[j].PixelData[i]*10);
            }
            PWIinfo.fileformat.getDataset()->putAndInsertUint16Array(DCM_PixelData,temPixel,128*128);
            PWIinfo.fileformat.saveFile((output+"Tmax/MTT_"+itos(j)).c_str(),EXS_LittleEndianExplicit);

            //输出Tmax
            for(int i = 0 ; i < 128*128 ; ++i )
            {
                temPixel[i] = (Uint16)(PWIMTT[j].PixelData[i]);
            }
            PWIinfo.fileformat.getDataset()->putAndInsertUint16Array(DCM_PixelData,temPixel,128*128);
            PWIinfo.fileformat.saveFile((output+"MTT/MTT_"+itos(j+1)).c_str(),EXS_LittleEndianExplicit);
            //输出CBF
            for(int i = 0 ; i < 128*128 ; ++i )
            {
                temPixel[i] = (Uint16)(PWICBF[j].PixelData[i]*10);
            }
            PWIinfo.fileformat.getDataset()->putAndInsertUint16Array(DCM_PixelData,temPixel,128*128);
            PWIinfo.fileformat.saveFile((output+"CBF/CBF_"+itos(j)).c_str(),EXS_LittleEndianExplicit);
                //输出CBV
            for(int i = 0 ; i < 128*128 ; ++i )
            {
                temPixel[i] = (Uint16)(PWICBV[j].PixelData[i]*100);
            }
            PWIinfo.fileformat.getDataset()->putAndInsertUint16Array(DCM_PixelData,temPixel,128*128);
            PWIinfo.fileformat.saveFile((output+"CBV/CBV_"+itos(j)).c_str(),EXS_LittleEndianExplicit);
            //write BMP
            for(int i = 0 ; i < 128*128 ; ++i )
            {
                temPixel[i] = (Uint16)((PWICBV[j].PixelData[i]<=0?0:PWICBV[j].PixelData[i])*100);
            }
            toBmp(128,128,temPixel,s_cbv_tem.c_str(),0,500,s0[j].PixelData);
            for(int i = 0 ; i < 128*128 ; ++i )
            {
                temPixel[i] = (Uint16)((PWICBF[j].PixelData[i]<=0?0:PWICBF[j].PixelData[i])*10);
            }
            toBmp(128,128,temPixel,s_cbf_tem.c_str(),0,400,s0[j].PixelData);
            for(int i = 0 ; i < 128*128 ; ++i )
            {
                temPixel[i] = (Uint16)((PWIMTT[j].PixelData[i]<=0?0:PWIMTT[j].PixelData[i])*10);
            }

            toBmp(128,128,temPixel,s_tmax_tem.c_str(),0,120,s0[j].PixelData);
            for(int i = 0 ; i < 128*128 ; ++i )
            {
                temPixel[i] = (Uint16)((PWITmax[j].PixelData[i]<=0?0:PWITmax[j].PixelData[i])*10);
            }
            toBmp(128,128,temPixel,s_mtt_tem.c_str(),0,160,s0[j].PixelData);
        }
    puts("PWI DONE");
    return 0;
}


    /*
     *how to run::
    void run(){

        QDir dir;
        string relative_dir = string(basedir) + "/data/" + pid;

        if(!dir.exists((relative_dir + "/result").c_str())){
            dir.mkdir((relative_dir + "/result").c_str());
        }
        if(!dir.exists((relative_dir + "/result/" + string(temp_filename)).c_str())){
            dir.mkdir((relative_dir + "/result/" + string(temp_filename)).c_str());
        }
        string  arr[9] = {"/ADC", "/MTT", "/bmpCBF", "/bmpMTT", "/bmpTmax", "/bmpCBV","/CBF", "/CBV","/Tmax"};
        for(int i = 0; i < 9; ++ i){
            if(!dir.exists((relative_dir + "/result/" + string(temp_filename) + "/" + arr[i]).c_str())){
                 dir.mkdir((relative_dir + "/result/" + string(temp_filename) + "/" + arr[i]).c_str());
            }
        }

        PWIConstTable *PWIconst = PWIConstTable::single();
        DWIConstTable *DWIconst = DWIConstTable::single();

        //PWI&DWI sort file
        SortDcmFile pwiSort;
        SortDcmFile dwiSort;
        pwiSort.sortFile((relative_dir + "/origin/PWI/").c_str());
        dwiSort.sortFile((relative_dir + "/origin/DWI/").c_str());
        PWIconst->PSetNum = pwiSort.PsetNum;
        PWIconst->PSliceNum = pwiSort.Pslice;
        init(PWIconst);
        PWI_Compute(relative_dir + "/origin/PWI/", relative_dir + "/result/" + string(temp_filename) + "/",PWIconst);
        DWI_Compute(relative_dir + "/origin/DWI/", relative_dir + "/result/" + string(temp_filename) + "/ADC/",DWIconst);
    }
    */
