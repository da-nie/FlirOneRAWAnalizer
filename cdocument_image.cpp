#include "cdocument_image.h"

//-Функции обработки сообщений класса----------------------------------------
//-Конструктор класса--------------------------------------------------------
CDocument_Image::CDocument_Image(void)
{
 RAW_Image=NULL;
 Temperature_Image=NULL;
 //формируем палитру по-умолчанию
 for(long n=0;n<256;n++)
 {
  ColorMap_R[n]=(unsigned char)n;
  ColorMap_G[n]=(unsigned char)n;
  ColorMap_B[n]=(unsigned char)n;
 }
 MaxTemperature=0;
 MinTemperature=0;
 HightContrastFilterEnabled=false;
 CreateBitmapPalette();
}
//-Деструктор класса---------------------------------------------------------
CDocument_Image::~CDocument_Image()
{
 ReleaseData();
}
//-Функции класса------------------------------------------------------------

//---------------------------------------------------------------------------
//загрузить RAW-файл
//---------------------------------------------------------------------------
void CDocument_Image::Load(FILE *file)
{
 //читаем заголовок
 SRAWHeader sRAWHeader_Local;
 if (fread(&sRAWHeader_Local,sizeof(SRAWHeader),1,file)<=0) return;//ошибка чтения
 //читаем данные
 unsigned short *raw=new unsigned short[sRAWHeader_Local.Width*sRAWHeader_Local.Height];
 if (fread(raw,sizeof(unsigned short),sRAWHeader_Local.Width*sRAWHeader_Local.Height,file)<sRAWHeader_Local.Width*sRAWHeader_Local.Height)//ошибка чтения
 {
  delete[](raw);
  return;
 }
 //данные загружены
 ReleaseData();
 sRAWHeader=sRAWHeader_Local;
 RAW_Image=raw;
 Temperature_Image=new double[sRAWHeader_Local.Width*sRAWHeader_Local.Height];
 CreateAllImage();
}
//---------------------------------------------------------------------------
//сохранить файл
//---------------------------------------------------------------------------
void CDocument_Image::Save(FILE *file)
{
}

//----------------------------------------------------------------------------------------------------
//загрузить карту перекодировки изображения
//----------------------------------------------------------------------------------------------------
bool CDocument_Image::LoadColorMap(char *filename)
{ 
 FILE *file=fopen(filename,"rb");
 if (file==NULL) return(false);
 unsigned char colormap[768]; 
 if (fread(colormap,sizeof(unsigned char),768,file)<768)
 {
  fclose(file);
  return(false);
 }
 fclose(file);
 for(long n=0;n<256;n++)
 {
  ColorMap_R[n]=colormap[n*3+0];
  ColorMap_G[n]=colormap[n*3+1];
  ColorMap_B[n]=colormap[n*3+2];
 }
 CreateAllImage();
 return(true);
}
//---------------------------------------------------------------------------
//получить указатель на изображение
//---------------------------------------------------------------------------
CBitmap *CDocument_Image::GetBitmapImage(void) 
{
 if (RAW_Image==NULL) return(NULL);
 return(&cBitmapImage);
}
//---------------------------------------------------------------------------
//получить указатель на картинку палитры
//---------------------------------------------------------------------------
CBitmap *CDocument_Image::GetBitmapPalette(void) 
{
 return(&cBitmapPalette);
}
//---------------------------------------------------------------------------
//получить максимальную и минимальную температуру
//---------------------------------------------------------------------------
void CDocument_Image::GetMinMaxTemperature(double &tmin,double &tmax) 
{
 tmin=MinTemperature;
 tmax=MaxTemperature;
}
//---------------------------------------------------------------------------
//получить температуру в точке
//---------------------------------------------------------------------------
bool CDocument_Image::GetTemperatureInPoint(long x,long y,double &tmp)
{
 if (RAW_Image==NULL) return(false);
 if (Temperature_Image==NULL) return(false);
 if (x<0 || x>=sRAWHeader.Width) return(false);
 if (y<0 || y>=sRAWHeader.Height) return(false);
 long offset=x+y*sRAWHeader.Width;
 tmp=Temperature_Image[offset];
 return(true);
}
//---------------------------------------------------------------------------
//добавить точку
//---------------------------------------------------------------------------
void CDocument_Image::AddPoint(long x,long y)
{
 if (RAW_Image==NULL) return;
 if (x<0 || x>=sRAWHeader.Width*IMAGE_SCALE_X) return;
 if (y<0 || y>=sRAWHeader.Height*IMAGE_SCALE_Y) return;
 SPoint sPoint;
 sPoint.X=x;
 sPoint.Y=y;
 sPoint.Selected=false;
 vector_SPoint.push_back(sPoint);
 CreateAllImage(); 
}
//---------------------------------------------------------------------------
//получить указатель на список точек
//---------------------------------------------------------------------------
vector<SPoint>* CDocument_Image::GetVectorSPointPtr(void)
{
 return(&vector_SPoint);
}
//---------------------------------------------------------------------------
//удалить все точки из списка
//---------------------------------------------------------------------------
void CDocument_Image::ClearPointList(void)
{
 vector_SPoint.clear();
 CreateAllImage();
}
//---------------------------------------------------------------------------
//очистить все данные
//---------------------------------------------------------------------------
void CDocument_Image::ReleaseData(void)
{
 if (RAW_Image!=NULL) delete[](RAW_Image);
 RAW_Image=NULL;
 if (Temperature_Image!=NULL) delete[](Temperature_Image);
 Temperature_Image=NULL;
 cBitmapImage.DeleteObject();
 cBitmapPalette.DeleteObject();
}
//---------------------------------------------------------------------------
//создать все изображения
//---------------------------------------------------------------------------
void CDocument_Image::CreateAllImage(void)
{
 //создаём температурное изображение
 CreateTemperatureImage();
 //создаём изображение
 CreateBitmapImage();
 //меняем палитру
 CreateBitmapPalette();
 //обновляем все виды
 UpdateAllViews(NULL);
}
//---------------------------------------------------------------------------
//удалить выбранные точки из списка
//---------------------------------------------------------------------------
void CDocument_Image::DeleteSelectedPoint(void)
{
 vector<SPoint> vector_SPoint_New;
 long size=vector_SPoint.size();
 for(long n=0;n<size;n++)
 {
  if (vector_SPoint[n].Selected==true) continue;
  vector_SPoint_New.push_back(vector_SPoint[n]);
 }
 vector_SPoint=vector_SPoint_New;
 CreateAllImage();
}
//---------------------------------------------------------------------------
//обновился список выбранных точек
//---------------------------------------------------------------------------
void CDocument_Image::RefreshSelectedList(void)
{
 CreateAllImage();
}
//---------------------------------------------------------------------------
//скопировать палитру
//---------------------------------------------------------------------------
bool CDocument_Image::CopyColorMap(unsigned char R[256],unsigned char G[256],unsigned char B[256],unsigned long size)
{
 if (size<256) return(false);//буфер слишком мал
 for(long n=0;n<256;n++)
 {
  R[n]=ColorMap_R[n];
  G[n]=ColorMap_G[n];
  B[n]=ColorMap_B[n];
 }
 return(true);
}
//---------------------------------------------------------------------------
//задать коэффициент излучения
//---------------------------------------------------------------------------
void CDocument_Image::SetEmissivity(double emissivity)
{
 sRAWHeader.Emissivity=emissivity;
 CreateAllImage();
}
//---------------------------------------------------------------------------
//получить коэффициент излучения
//---------------------------------------------------------------------------
double CDocument_Image::GetEmissivity(void)
{
 return(sRAWHeader.Emissivity);
}
//---------------------------------------------------------------------------
//установить, использовать ли контрастоповышающий фильтр
//---------------------------------------------------------------------------
void CDocument_Image::SetHightContrastFilterEnabled(bool enabled)
{
 HightContrastFilterEnabled=enabled;
 CreateAllImage();
}
//---------------------------------------------------------------------------
//создать изображение
//---------------------------------------------------------------------------
void CDocument_Image::CreateBitmapImage(void)
{
 cBitmapImage.DeleteObject();
 if (RAW_Image==NULL) return;

 long x;
 long y;
 //найдём максимум и минимум для построения карты изображения
 unsigned short max=RAW_Image[0];
 unsigned short min=max;
 unsigned short *raw_ptr=RAW_Image;
 for(y=0;y<sRAWHeader.Height;y++)
 {
  for(x=0;x<sRAWHeader.Width;x++,raw_ptr++)
  {
   unsigned short v=*raw_ptr;
   if (v>max) max=v;
   if (v<min) min=v;
  }
 }
 long delta=max-min;
 if (delta==0) delta=1;
 float scale=255.0f/static_cast<float>(delta);

 CIImage *cIImage_Ptr=cFactoryMethod_CPicture.Create_CIImage();//создаём изображение
 cIImage_Ptr->SetSize(sRAWHeader.Width,sRAWHeader.Height);
 //создаём изображение
 for(y=0;y<sRAWHeader.Height;y++)
 {
  for(x=0;x<sRAWHeader.Width;x++)
  {
   unsigned short p=RAW_Image[y*sRAWHeader.Width+x];
   unsigned char index=static_cast<unsigned char>(static_cast<float>(p-min)*scale);
   unsigned long color=0;
   color|=0xff;
   color<<=8;
   color|=ColorMap_R[index];
   color<<=8;
   color|=ColorMap_G[index];
   color<<=8;
   color|=ColorMap_B[index];
   cIImage_Ptr->SetRGBAPixel(x,y,color);
  }
 }

 //подключим декоратор изменения размеров
 cIImage_Ptr=new CDecorator_Scale(cIImage_Ptr,sRAWHeader.Width*IMAGE_SCALE_X,sRAWHeader.Height*IMAGE_SCALE_Y);
 //подключим декоратор контрастоповышающего фильтра
 if (HightContrastFilterEnabled==true) cIImage_Ptr=new CDecorator_HightContrastFilter(cIImage_Ptr);

 unsigned long width;
 unsigned long height;
 vector<unsigned long> vector_image;
 cIImage_Ptr->GetRGBAImage(width,height,vector_image);
 unsigned long *BitmapImage=&(vector_image[0]);

 //добавляем точки температуры
 CGraphics cGraphics;
 cGraphics.Init(BitmapImage,width,height);
 long size=vector_SPoint.size();
 for(long n=0;n<size;n++)
 {
  long x=vector_SPoint[n].X;
  long y=vector_SPoint[n].Y;
  double t;
  if (GetTemperatureInPoint(x/IMAGE_SCALE_X,y/IMAGE_SCALE_Y,t)==false) continue;
  char string[255];
  sprintf(string,"%.1f!",t);
  if (vector_SPoint[n].Selected==true)//если точка выбрана, рисуем её в чёрном прямоугольнике
  {
   cGraphics.SolidFill(x-2,y-2,4+FONT_WIDTH*strlen(string),4+FONT_HEIGHT,0xFF000000);
  }
  //рисуем перекрестье
  cGraphics.DrawPointInversion(x,y-1);
  cGraphics.DrawPointInversion(x,y);
  cGraphics.DrawPointInversion(x,y+1);
  cGraphics.DrawPointInversion(x-1,y);
  cGraphics.DrawPointInversion(x+1,y);
  //выводим значение температуры
  cGraphics.PutStringInversion(x+1,y+1,string);
  //рисуем полочку
  long m;
  for(m=y+2;m<=y+FONT_HEIGHT;m++) cGraphics.DrawPointInversion(x,m);
  for(m=x;m<=x+FONT_WIDTH*strlen(string);m++) cGraphics.DrawPointInversion(m,y+FONT_HEIGHT+1);
 }

 cBitmapImage.CreateBitmap(width,height,1,32,BitmapImage);
 delete(cIImage_Ptr);
}
//---------------------------------------------------------------------------
//создать картинку палитры
//---------------------------------------------------------------------------
void CDocument_Image::CreateBitmapPalette(void)
{ 
 cBitmapPalette.DeleteObject();
 //создаём изображение PALETTE_IMAGE_WIDTHx256 пикселей
 unsigned long *BitmapPalette=new unsigned long[PALETTE_IMAGE_WIDTH*256];
 unsigned long *b_ptr=BitmapPalette;
 for(long index=255;index>=0;index--)
 {
  unsigned long color=0;
  color|=0xff;
  color<<=8;
  color|=ColorMap_R[index];
  color<<=8;
  color|=ColorMap_G[index];
  color<<=8;
  color|=ColorMap_B[index];
  for(long x=0;x<PALETTE_IMAGE_WIDTH;x++,b_ptr++) *b_ptr=color;
 }
 cBitmapPalette.CreateBitmap(PALETTE_IMAGE_WIDTH,256,1,32,BitmapPalette);
 delete[](BitmapPalette);
}
//---------------------------------------------------------------------------
//создать карту температур
//---------------------------------------------------------------------------
void CDocument_Image::CreateTemperatureImage(void)
{
 if (RAW_Image==NULL) return;
 if (Temperature_Image==NULL) return;
 long x;
 long y;
 //найдём максимум и минимум для построения карты изображения
 MaxTemperature=-100000;
 MinTemperature=100000;
 unsigned short *raw_ptr=RAW_Image;
 double *t_ptr=Temperature_Image;
 for(y=0;y<sRAWHeader.Height;y++)
 {
  for(x=0;x<sRAWHeader.Width;x++,raw_ptr++,t_ptr++)
  {
   unsigned short v=*raw_ptr;
   //вычисляем температуру
   double value;
   if (GetTemperature(v,value)==false) value=0;
   *t_ptr=value;
   if (MaxTemperature<value) MaxTemperature=value;
   if (MinTemperature>value) MinTemperature=value;
  }
 }
}
//---------------------------------------------------------------------------
//вычислить температуру
//---------------------------------------------------------------------------
bool CDocument_Image::GetTemperature(unsigned short raw14,double &value)
{
 if (raw14>0x10000) return(false);
 raw14*=4;    
 double zn=(sRAWHeader.PlanckR2*(exp(sRAWHeader.PlanckB/(sRAWHeader.TempReflected+273.15))-sRAWHeader.PlanckF));
 if (fabs(zn)<EPS) return(false);
 double RAWrefl=sRAWHeader.PlanckR1/zn-sRAWHeader.PlanckO;
 double RAWobj=(raw14-(1-sRAWHeader.Emissivity)*RAWrefl)/sRAWHeader.Emissivity;
 double lgr=sRAWHeader.PlanckR1/(sRAWHeader.PlanckR2*(RAWobj+sRAWHeader.PlanckO)+sRAWHeader.PlanckF);
 if (lgr<=EPS) return(false);
 value=sRAWHeader.PlanckB/log(lgr)-273.15; 
 return(true);
}

//-Функции обработки сообщений класса----------------------------------------
//-Прочее--------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CDocument_Image,CDocument)












