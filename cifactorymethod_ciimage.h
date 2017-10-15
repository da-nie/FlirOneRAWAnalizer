#ifndef CIFACTORYMETHOD_CIIMAGE_H
#define CIFACTORYMETHOD_CIIMAGE_H

#include "ciimage.h"

//интерфейс к фабричному методу создания изображения
class CIFactoryMethod_CIImage
{
 public:
  virtual ~CIFactoryMethod_CIImage() {};//деструктор
  virtual CIImage* Create_CIImage(void)=0;//создать объект изображения
};


#endif