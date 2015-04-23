#include <string>
#include "DCMHeaders.h"
#include "UsefullTools.h"
#include <QDir>
#include <QStringList>
#include <Qlist.h>
#include <algorithm>
typedef struct 
{
	QString filename;
	double sliceLocation;
	OFString imageID;
}dicomInfo;
using namespace std;
bool cmp(const dicomInfo &a,const dicomInfo &b);

class SortDcmFile
{	
public:
    void sortFile(const char url[]);
    void changName(const char url[]);
    int Pslice;
    int PsetNum;

};
