#ifndef CIFACTORYMETHOD_CIIMAGE_H
#define CIFACTORYMETHOD_CIIMAGE_H

#include "ciimage.h"

//��������� � ���������� ������ �������� �����������
class CIFactoryMethod_CIImage
{
 public:
  virtual ~CIFactoryMethod_CIImage() {};//����������
  virtual CIImage* Create_CIImage(void)=0;//������� ������ �����������
};


#endif