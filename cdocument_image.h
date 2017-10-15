#ifndef C_DOCUMENT_IMAGE_H
#define C_DOCUMENT_IMAGE_H

#include "stdafx.h"
#include "cgraphics.h"
#include "cfactorymethod_cpicture.h"
#include "cdecorator_scale.h"
#include "cdecorator_hightcontrastfilter.h"
#include "tga.h"

#include <vector>
using namespace std;

//������ �������
#define PALETTE_IMAGE_WIDTH  32

//���������� �� X
#define IMAGE_SCALE_X 4
//���������� �� Y
#define IMAGE_SCALE_Y 4

//����� ���������
struct SPoint
{
 long X;//���������� �����
 long Y;
 bool Selected;//������� �� �����
};

//---------------------------------------------------------------------------
//����� ����������
//---------------------------------------------------------------------------
class CDocument_Image:public CDocument 
{
 protected:
  //-���������� ������-------------------------------------------------------  
  #pragma pack(1)
  struct SRAWHeader
  {
   unsigned long Width;//������
   unsigned long Height;//������
   //��������� ��� ������� �����������
   double PlanckR1;
   double PlanckB;
   double PlanckF;
   double PlanckO;
   double PlanckR2;
   double TempReflected;//����������� ����������
   double Emissivity;//����������� ���������
  } sRAWHeader;//��������� RAW-�����������
  #pragma pack()
  unsigned short *RAW_Image;//������ RAW-�����������
  double *Temperature_Image;//������ ����������� �� �����������

  CBitmap cBitmapImage;//������������ �������� 
  CBitmap cBitmapPalette;//������������ �������

  //������� ������������� �����������
  unsigned char ColorMap_R[256];
  unsigned char ColorMap_G[256];
  unsigned char ColorMap_B[256];

  double MaxTemperature;//������������ ����������� �� �����
  double MinTemperature;//����������� ����������� �� �����

  bool HightContrastFilterEnabled;//������������ �� ������������������� ������

  //������ �������� �����
  vector<SPoint> vector_SPoint;

  //��������� ����� ��� �������� �������� CIImage
  CFactoryMethod_CPicture cFactoryMethod_CPicture;
  //-������� ������----------------------------------------------------------
  //-������------------------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CDocument_Image(void); 
  //-���������� ������-------------------------------------------------------
  ~CDocument_Image();
  //-���������� ������-------------------------------------------------------
  //-������� ������----------------------------------------------------------
  void Load(FILE *file);//��������� �������
  void Save(FILE *file);//��������� �������
  CBitmap* GetBitmapImage(void);//�������� ��������� �� �����������
  CBitmap* GetBitmapPalette(void);//�������� ��������� �� �������� �������
  void GetMinMaxTemperature(double &tmin,double &tmax);//�������� ������������ � ����������� �����������
  bool GetTemperatureInPoint(long x,long y,double &tmp);//�������� ����������� � �����
  bool LoadColorMap(char *filename);//��������� ����� ������������� �����������
  void AddPoint(long x,long y);//�������� �����
  vector<SPoint>* GetVectorSPointPtr(void);//�������� ��������� �� ������ �����
  void ClearPointList(void);//������� ��� ����� �� ������
  void DeleteSelectedPoint(void);//������� ��������� ����� �� ������
  void RefreshSelectedList(void);//��������� ������ ��������� �����
  bool CopyColorMap(unsigned char R[256],unsigned char G[256],unsigned char B[256],unsigned long size);//����������� �������
  void SetEmissivity(double emissivity);//������ ����������� ���������
  double GetEmissivity(void);//�������� ����������� ���������
  void SetHightContrastFilterEnabled(bool enabled);//����������, ������������ �� ������������������� ������
 protected:
  //-������� ������----------------------------------------------------------
  void ReleaseData(void);//�������� ��� ������
  void CreateAllImage(void);//������� ��� �����������
  void CreateBitmapImage(void);//������� �����������
  void CreateBitmapPalette(void);//������� �������� �������
  void CreateTemperatureImage(void);//������� ����� ����������
  bool GetTemperature(unsigned short raw14,double &value);//��������� �����������
  //-������------------------------------------------------------------------
  DECLARE_DYNCREATE(CDocument_Image) 
};
#endif

