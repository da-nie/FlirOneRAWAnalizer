#include "cdecorator_hightcontrastfilter.h"

//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
CDecorator_HightContrastFilter::CDecorator_HightContrastFilter(CIImage *cIImage_Set_Ptr):CDecorator_CIImage(cIImage_Set_Ptr)
{ 
}
//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
CDecorator_HightContrastFilter::~CDecorator_HightContrastFilter()
{ 
}
//----------------------------------------------------------------------------------------------------
//получить RGBA изображение
//----------------------------------------------------------------------------------------------------
void CDecorator_HightContrastFilter::GetRGBAImage(unsigned long &width,unsigned long &height,vector<unsigned long> &vector_image)
{
 //запрашиваем изображение
 vector<unsigned long> vector_oldimage;
 CDecorator_CIImage::GetRGBAImage(width,height,vector_oldimage);
 //применяем к изображению контрастоповышающий фильтр
 vector_image.resize(width*height);

 unsigned long *i_ptr=&(vector_image[0]);
 unsigned long *oldi_ptr=&(vector_oldimage[0]);

 float MatrixF[3][3]={-1,-1,-1,-1,9,-1,-1,-1,-1};
 long x1,y1;
 for(long x=0;x<width;x++)
 {
  for(long y=0;y<height;y++)
  {
   //заполняем матрицу значениями изображения
   float value[4]={0,0,0,0};
   for(y1=0;y1<3;y1++)
   {
    for(x1=0;x1<3;x1++) 
	{
     long x_p=x1+x-1;
     long y_p=y1+y-1;
     if (x_p>=width) x_p=width-1;
     if (x_p<0) x_p=0;
     if (y_p>=height) y_p=height-1;
     if (y_p<0) y_p=0;
	 unsigned long p=oldi_ptr[y_p*width+x_p];
     unsigned long mask=0xFF;
     unsigned long offset=0;
     for(unsigned char k=0;k<4;k++,mask<<=8,offset+=8)
	 {
      unsigned long kp=(p&mask)>>offset;
      value[k]+=MatrixF[y1][x1]*static_cast<float>(kp);
	 }
 	}
   }
   unsigned long color=0;
   unsigned long offset=0;
   for(unsigned char k=0;k<4;k++,offset+=8)
   {
    long c=static_cast<long>(value[k]);
    if (c<0) c=0;
    if (c>0xFF) c=0xFF;
    color|=(c<<offset);
   }
   i_ptr[y*width+x]=color;
  }
 }
}
//----------------------------------------------------------------------------------------------------
//задать RGBA изображение
//----------------------------------------------------------------------------------------------------
void CDecorator_HightContrastFilter::SetRGBAImage(unsigned long &width,unsigned long &height,vector<unsigned long> &vector_image)
{
 CDecorator_CIImage::SetRGBAImage(width,height,vector_image);
}
//----------------------------------------------------------------------------------------------------
//задать размер изображения
//----------------------------------------------------------------------------------------------------
void CDecorator_HightContrastFilter::SetSize(unsigned long width,unsigned long height)
{
 CDecorator_CIImage::SetSize(width,height);
}
//----------------------------------------------------------------------------------------------------
//задать точку
//----------------------------------------------------------------------------------------------------
void CDecorator_HightContrastFilter::SetRGBAPixel(unsigned long x,unsigned long y,unsigned long color)
{
 CDecorator_CIImage::SetRGBAPixel(x,y,color);
}
//----------------------------------------------------------------------------------------------------
//получить точку
//----------------------------------------------------------------------------------------------------
unsigned long CDecorator_HightContrastFilter::GetRGBAPixel(unsigned long x,unsigned long y)
{
 return(CDecorator_CIImage::GetRGBAPixel(x,y));
}

