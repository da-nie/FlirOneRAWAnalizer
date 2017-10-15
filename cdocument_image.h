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

//ширина палитры
#define PALETTE_IMAGE_WIDTH  32

//увеличение по X
#define IMAGE_SCALE_X 4
//увеличение по Y
#define IMAGE_SCALE_Y 4

//точки измерений
struct SPoint
{
 long X;//координаты точки
 long Y;
 bool Selected;//выбрана ли точка
};

//---------------------------------------------------------------------------
//класс документов
//---------------------------------------------------------------------------
class CDocument_Image:public CDocument 
{
 protected:
  //-Переменные класса-------------------------------------------------------  
  #pragma pack(1)
  struct SRAWHeader
  {
   unsigned long Width;//ширина
   unsigned long Height;//высота
   //параметры для расчёта температуры
   double PlanckR1;
   double PlanckB;
   double PlanckF;
   double PlanckO;
   double PlanckR2;
   double TempReflected;//температура болометров
   double Emissivity;//коэффициент излучения
  } sRAWHeader;//параметры RAW-изображения
  #pragma pack()
  unsigned short *RAW_Image;//данные RAW-изображения
  double *Temperature_Image;//данные изображения по температуре

  CBitmap cBitmapImage;//отображаемая картинка 
  CBitmap cBitmapPalette;//отображаемая палитра

  //палитра перекодировки изображения
  unsigned char ColorMap_R[256];
  unsigned char ColorMap_G[256];
  unsigned char ColorMap_B[256];

  double MaxTemperature;//максимальная температура по кадру
  double MinTemperature;//минимальная температура по кадру

  bool HightContrastFilterEnabled;//используется ли контрастоповышающий фильтр

  //список хранимых точек
  vector<SPoint> vector_SPoint;

  //фабричный метод для создания объектор CIImage
  CFactoryMethod_CPicture cFactoryMethod_CPicture;
  //-Функции класса----------------------------------------------------------
  //-Прочее------------------------------------------------------------------
 public:
  //-Конструктор класса------------------------------------------------------
  CDocument_Image(void); 
  //-Деструктор класса-------------------------------------------------------
  ~CDocument_Image();
  //-Переменные класса-------------------------------------------------------
  //-Функции класса----------------------------------------------------------
  void Load(FILE *file);//загрузить уровень
  void Save(FILE *file);//сохранить уровень
  CBitmap* GetBitmapImage(void);//получить указатель на изображение
  CBitmap* GetBitmapPalette(void);//получить указатель на картинку палитру
  void GetMinMaxTemperature(double &tmin,double &tmax);//получить максимальную и минимальную температуру
  bool GetTemperatureInPoint(long x,long y,double &tmp);//получить температуру в точке
  bool LoadColorMap(char *filename);//загрузить карту перекодировки изображения
  void AddPoint(long x,long y);//добавить точку
  vector<SPoint>* GetVectorSPointPtr(void);//получить указатель на список точек
  void ClearPointList(void);//удалить все точки из списка
  void DeleteSelectedPoint(void);//удалить выбранные точки из списка
  void RefreshSelectedList(void);//обновился список выбранных точек
  bool CopyColorMap(unsigned char R[256],unsigned char G[256],unsigned char B[256],unsigned long size);//скопировать палитру
  void SetEmissivity(double emissivity);//задать коэффициент излучения
  double GetEmissivity(void);//получить коэффициент излучения
  void SetHightContrastFilterEnabled(bool enabled);//установить, использовать ли контрастоповышающий фильтр
 protected:
  //-Функции класса----------------------------------------------------------
  void ReleaseData(void);//очистить все данные
  void CreateAllImage(void);//создать все изображения
  void CreateBitmapImage(void);//создать изображение
  void CreateBitmapPalette(void);//создать картинку палитры
  void CreateTemperatureImage(void);//создать карту температур
  bool GetTemperature(unsigned short raw14,double &value);//вычислить температуру
  //-Прочее------------------------------------------------------------------
  DECLARE_DYNCREATE(CDocument_Image) 
};
#endif

