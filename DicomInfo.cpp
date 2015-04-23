#include "DicomInfo.h"
#include "DCMHeaders.h"
#include "UsefullTools.h"
void DicomInfo::allocMemory(int length)
{
	DicomInfo::PixelData=new _signal [length];
}
void DicomInfo::getInfo(const char address[])
{
	OFString UintStr;
	DicomInfo::fileformat.loadFile(address);
	DicomInfo::dataset=DicomInfo::fileformat.getDataset();

	dataset->findAndGetOFString(DcmTagKey(0x0028,0x0010),UintStr);
	DicomInfo::width=OFtoUint16(UintStr);
	dataset->findAndGetOFString(DcmTagKey(0x0028,0x0011),UintStr);
	DicomInfo::height=OFtoUint16(UintStr);
	dataset->findAndGetOFString(DcmTagKey(0x0018,0x1060),UintStr);
	DicomInfo::time=OFtoSignal(UintStr);

    const Uint16 *tempPoints;
    PixelData = new _signal [height*width];
	DicomInfo::dataset->findAndGetUint16Array(DCM_PixelData,tempPoints);
	for(int i=0;i<height;i++)
	{
		for(int j=0; j<width;j++)
		{
			PixelData[i*height+j]=(_signal)tempPoints[i*height+j];
		}
	}
}
DicomInfo::~DicomInfo()
{
    delete [] PixelData;
}
