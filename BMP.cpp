#include "BMP.h"
#include "QTSource.h"
int toInt(OFString str)
{	int size,sum=0,i,jin=1;
	const char *p=str.data();
	size=str.size();
	for(i=size-1;i>=0;i--)
	{
		sum+=jin*(p[i]-'0');
		jin*=10;
	}
	return sum;
}
void toBmp(unsigned int width,unsigned int height,Uint16 *xs,const char url[],int sta,int end,_signal *flag)
{
    int i,j,length,temp;
    unsigned char color;
    unsigned int size;
    unsigned int bl=0;
    unsigned int py=54;
    unsigned int bh=40;
    //width height
    short wm=1;
    size=3*height*width+54;
    length=width*height;
    XS xs1;
    QImage* image=new QImage(width,height,QImage::Format_RGB32);
    //int weight = (end -sta)*21/80;
    int section1 =112;
    int section2 =112+255;
    int section3 =112+255+255;
    int section4 =112+255+255+255;
    int section5 =112+255+255+255+123;
    double weight = ((double)(end -sta))/1000;
    //int hx = end -sta - weight;
    int fullcolor =255;
    //printf("BMP:%d\n",weight);

    for(i=width-1;i>=0;i--)
    {
        for(j=0;j<height;j++)
        {
            temp=(i*width)+j;
            if(temp<0)
                temp =0;
            //color=toColor(*(xs+temp)/2,Wwidth,Wcenter);
            if((*(flag+temp)) != 0)
            {

                if(((int)(*(xs+temp)))<=weight*section1)
                {

                        xs1.a[0]=143+(int)((*(xs+temp))/weight);
                        xs1.a[1]=0;
                        xs1.a[2]=0;

                }else if(((int)(*(xs+temp)))<=weight*section2)
                {
                        xs1.a[0]=fullcolor;
                        xs1.a[1]=(int)((*(xs+temp))/weight)-section1;
                        xs1.a[2]=0;

                }else if(((int)(*(xs+temp)))<=weight*section3)
                {
                        xs1.a[0]=fullcolor-((int)((*(xs+temp))/weight)-section2);
                        xs1.a[1]=fullcolor;
                        xs1.a[2]=((int)((*(xs+temp))/weight)-section2);
                }else if(((int)(*(xs+temp)))<=weight*section4)
                {
                        xs1.a[0]=0;
                        xs1.a[1]=fullcolor-((int)((*(xs+temp))/weight)-section3);
                        xs1.a[2]=fullcolor;
                }else if(((int)(*(xs+temp)))<=weight*section5)
                {
                        xs1.a[0]=0;
                        xs1.a[1]=0;
                        xs1.a[2]=((int)((*(xs+temp))/weight)-section4);
                }else
                {
                        xs1.a[0]=0;
                        xs1.a[1]=0;
                        xs1.a[2]=126;
                }
            }else
            {
                xs1.a[0]=0;
                xs1.a[1]=0;
                xs1.a[2]=0;
            }
            image -> setPixel(j,i,qRgb(xs1.a[2], xs1.a[1], xs1.a[0]));

        }
    }
    image -> save(url,"bmp");
    delete image;
}
