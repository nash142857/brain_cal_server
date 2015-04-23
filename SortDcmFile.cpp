#include "SortDcmFile.h"
bool cmp(const dicomInfo &a,const dicomInfo &b)
{
	if(a.imageID.size()!=b.imageID.size())
	{
		return  a.imageID.size()<b.imageID.size();
	}else
	{

		return a.imageID<b.imageID;

	}
}
void SortDcmFile::changName(const char url[])
{
	QDir *dir=new QDir(url);
	QStringList filter;
	QString FileAddress = QString(url);
	QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));
	for(int i = 2 ; i < fileInfo->count() ; ++i)
	{
		QString temName = fileInfo->at(i).fileName();
		if(temName[0]=='I'&&temName[1]=='M')
		{
			QFile::rename( FileAddress+temName ,FileAddress+"0"+temName);
		}
	}
}
void SortDcmFile::sortFile(const char url[])
{
	QDir *dir=new QDir(url);
	QStringList filter;
	DcmFileFormat fileformat;
	OFString tempinfo;
	QString FileAddress = QString(url);

	
	//如果重名
	SortDcmFile::changName(url);


	//获取目录下所有文件名（含两个目录名）
	QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));
	QList<dicomInfo> sortInfo;
	//获取dicom信息
	for(int i = 2 ; i < fileInfo->count() ; ++i)
	{
		dicomInfo temp;
		temp.filename = fileInfo->at(i).fileName();
		QByteArray ba = fileInfo->at(i).filePath().toLatin1();
		char *url = ba.data();
		OFCondition os=fileformat.loadFile(url);
		if(os.good())
		{
			if((fileformat.getDataset()->findAndGetOFString(DcmTagKey(0x0020,0x0013),tempinfo)).good())
			{
				
				temp.imageID=tempinfo;
				//cout<<temp.imageID<<endl;

			}else
			{
				cout<<"No Find"<<endl;
			}
			if(fileformat.getDataset()->findAndGetOFString(DcmTagKey(0x0020,0x1041),tempinfo).good())
			{
				
				temp.sliceLocation=OFtoSignal(tempinfo);

			}else
			{
				cout<<"No Find"<<endl;
			}
			sortInfo.push_back(temp);
			

		}
	}

	sort(sortInfo.begin(),sortInfo.end(),cmp);

	QList<dicomInfo>::iterator it;


	//分组
	double tempbef=9999;
	int group=0;
	int group_number=-1;
	//QList<dicomInfo>::iterator it;
	for(it=sortInfo.begin();it!=sortInfo.end();it++)
	{
		if(it->sliceLocation>tempbef)
		{
			group++;
			SortDcmFile::Pslice = group_number+1;
			group_number=0;
			tempbef=9999;
			
		}else
		{
			tempbef=it->sliceLocation;
			group_number++;
		}
        SortDcmFile::PsetNum = group+1;
		QFile::rename( FileAddress+it->filename ,FileAddress+"IM"+QString::fromStdString(itos(group))+"-"+QString::fromStdString(itos(group_number)));
	}
}
