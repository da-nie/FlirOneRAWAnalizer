#ifndef CFACTORYMETHOD_CPICTURE_H
#define CFACTORYMETHOD_CPICTURE_H

#include "cifactorymethod_ciimage.h"
#include "cpicture.h"

//фабричный метод для CPicture
class CFactoryMethod_CPicture:public CIFactoryMethod_CIImage
{
 public:
  CIImage* Create_CIImage(void);//создать объект изображения
};

#endif