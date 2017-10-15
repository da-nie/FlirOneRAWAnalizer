#ifndef CDECORATOR_HIGHT_CONTRAST_FILTER_H
#define CDECORATOR_HIGHT_CONTRAST_FILTER_H

#include "cdecorator_ciimage.h"

//��������� ��� ������������� �������������������� �������
class CDecorator_HightContrastFilter:public CDecorator_CIImage
{
 protected:
 public:
  CDecorator_HightContrastFilter(CIImage *cIImage_Set_Ptr);//�����������
  ~CDecorator_HightContrastFilter();//����������
  void GetRGBAImage(unsigned long &width,unsigned long &height,vector<unsigned long> &vector_image);//�������� RGBA �����������
  void SetRGBAImage(unsigned long &width,unsigned long &height,vector<unsigned long> &vector_image);//������ RGBA �����������
  void SetSize(unsigned long width,unsigned long height);//������ ������ ����������� � �������� ������
  void SetRGBAPixel(unsigned long x,unsigned long y,unsigned long color);//������ �����
  unsigned long GetRGBAPixel(unsigned long x,unsigned long y);//�������� �����
};

#endif
