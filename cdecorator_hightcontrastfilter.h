#ifndef CDECORATOR_HIGHT_CONTRAST_FILTER_H
#define CDECORATOR_HIGHT_CONTRAST_FILTER_H

#include "cdecorator_ciimage.h"

//декоратор для использования контрастоповышающего фильтра
class CDecorator_HightContrastFilter:public CDecorator_CIImage
{
 protected:
 public:
  CDecorator_HightContrastFilter(CIImage *cIImage_Set_Ptr);//конструктор
  ~CDecorator_HightContrastFilter();//деструктор
  void GetRGBAImage(unsigned long &width,unsigned long &height,vector<unsigned long> &vector_image);//получить RGBA изображение
  void SetRGBAImage(unsigned long &width,unsigned long &height,vector<unsigned long> &vector_image);//задать RGBA изображение
  void SetSize(unsigned long width,unsigned long height);//задать размер изображения и выделить память
  void SetRGBAPixel(unsigned long x,unsigned long y,unsigned long color);//задать точку
  unsigned long GetRGBAPixel(unsigned long x,unsigned long y);//получить точку
};

#endif
