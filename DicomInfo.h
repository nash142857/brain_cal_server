#ifndef   DICOMINFO_H
#define   DICOMINFO_H
#include "symbol.h"
#include "DCMHeaders.h"
#include "string"
using namespace std;
class DicomInfo
{
public:
	Uint16 height;
	Uint16 width;
	_signal time;
	_signal *PixelData;
	DcmDataset *dataset;
	DcmFileFormat fileformat;
	void allocMemory(int length);
	void getInfo(const char address[]);
    ~DicomInfo();
};
#endif
