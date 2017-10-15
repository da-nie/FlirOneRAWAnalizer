#include "cview_image.h"

//-Функции обработки сообщений класса--------------------------------------
BEGIN_MESSAGE_MAP(CView_Image,CView)
 ON_WM_ERASEBKGND()
 ON_WM_RBUTTONDOWN()
 ON_WM_RBUTTONUP()
 ON_WM_LBUTTONDOWN()
 ON_WM_LBUTTONUP()
 ON_WM_MOUSEMOVE()
 ON_WM_TIMER()
 ON_WM_DESTROY() 
END_MESSAGE_MAP()
//-Конструктор класса--------------------------------------------------------
CView_Image::CView_Image() 
{
 //шрифты
 LogFont_Text.lfHeight=30; 
 LogFont_Text.lfWidth=0; 
 LogFont_Text.lfEscapement=0;
 LogFont_Text.lfOrientation=0; 
 LogFont_Text.lfWeight=FW_BOLD; 
 LogFont_Text.lfItalic=0;
 LogFont_Text.lfUnderline=0; 
 LogFont_Text.lfStrikeOut=0; 
 LogFont_Text.lfCharSet=DEFAULT_CHARSET; 
 LogFont_Text.lfOutPrecision=OUT_DEFAULT_PRECIS; 
 LogFont_Text.lfClipPrecision=CLIP_DEFAULT_PRECIS;
 LogFont_Text.lfQuality=DEFAULT_QUALITY; 
 LogFont_Text.lfPitchAndFamily=DEFAULT_PITCH;  
 strcpy(LogFont_Text.lfFaceName,"Times New Roman"); 
}
//-Деструктор класса---------------------------------------------------------
//-Функции класса------------------------------------------------------------

//---------------------------------------------------------------------------
//инициализация окна
//---------------------------------------------------------------------------
afx_msg void CView_Image::OnInitialUpdate(void)
{
 CView::OnInitialUpdate();
 SetTimer(IDR_TIMER_IMAGE,1,0);
}
//---------------------------------------------------------------------------
//получить документ
//---------------------------------------------------------------------------
CDocument_Image* CView_Image::GetDocument()
{
 ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDocument)));// проверить есть ли класс документа
 return( reinterpret_cast<CDocument_Image*>(m_pDocument));//возвратить указатель на документ
}
//---------------------------------------------------------------------------
//функция прорисовки вида
//---------------------------------------------------------------------------
void CView_Image::OnDraw(CDC* pDC)
{
 //готовим поле
 CDC cDC_Mem;
 cDC_Mem.CreateCompatibleDC(pDC);
 CRect cRect;
 GetClientRect(&cRect);
 CBitmap cBitmap;
 cDC_Mem.SetMapMode(MM_TEXT);
 cBitmap.CreateCompatibleBitmap(pDC,cRect.right,cRect.bottom);
 cDC_Mem.SelectObject(&cBitmap);
 //нарисуем первый раз, чтобы узнать размеры области
 unsigned long width;
 unsigned long height;
 DrawImage(&cDC_Mem,width,height,false);
 //очистим всю область
 CBrush cBrush;
 cBrush.CreateSolidBrush(RGB(192,192,192));
 cDC_Mem.FillRect(&cRect,&cBrush);
 cBrush.DeleteObject();
 //очистим внутреннюю область 
 CRect cRect_Image(0,0,width,height);
 cBrush.CreateSolidBrush(RGB(255,255,255));
 cDC_Mem.FillRect(&cRect_Image,&cBrush);
 cBrush.DeleteObject();
 //теперь можно рисовать
 DrawImage(&cDC_Mem,width,height,true);

 CDocument_Image* tempDocument=GetDocument();//получить класс документа
 CBitmap *cBitmapImage=tempDocument->GetBitmapImage();  
 if (cBitmapImage!=NULL)
 {
  //узнаем границы температурного диапазона
  double tmax;
  double tmin;
  tempDocument->GetMinMaxTemperature(tmin,tmax);

  //рисуем температуру в области мыши
  BITMAP BitmapImage;
  cBitmapImage->GetObject(sizeof(BITMAP),&BitmapImage);
  POINT MousePos;
  GetCursorPos(&MousePos);
  ScreenToClient(&MousePos);
  if (MousePos.x>=OFFSET_X && MousePos.x<BitmapImage.bmWidth+OFFSET_X)
  {
   if (MousePos.y>=OFFSET_Y && MousePos.y<BitmapImage.bmHeight+OFFSET_Y)
   {
    double tmp;
    if (tempDocument->GetTemperatureInPoint((MousePos.x-OFFSET_X)/IMAGE_SCALE_X,(MousePos.y-OFFSET_Y)/IMAGE_SCALE_Y,tmp)==true)
	{
     //используем обратную палитру
     unsigned char R[256];
     unsigned char G[256];
     unsigned char B[256];
     tempDocument->CopyColorMap(R,G,B,256);
     float delta=tmax-tmin;
	 if (fabs(delta)<=EPS) delta=1;
	 float scale=255.0/delta;
	 unsigned char index=static_cast<unsigned char>((tmp-tmin)*scale);
	 index=255-index;
     CFont ft_text;
     ft_text.CreateFontIndirect(&LogFont_Text);
     CFont* oldf=cDC_Mem.SelectObject(&ft_text);
     long oldbkmode=cDC_Mem.SetBkMode(TRANSPARENT);
     cDC_Mem.SetTextColor(RGB(R[index],G[index],B[index]));
     char string[255];
	 sprintf(string,"%.1fC",tmp);
     cDC_Mem.TextOut(MousePos.x+12,MousePos.y+12,string);
     cDC_Mem.SetBkMode(oldbkmode);
     cDC_Mem.SelectObject(oldf);
     ft_text.DeleteObject();
	}
   }
  }
 }
 //выводим область
 pDC->BitBlt(0,0,cRect.right,cRect.bottom,&cDC_Mem,0,0,SRCCOPY);
 cDC_Mem.DeleteDC(); 
}
//---------------------------------------------------------------------------
//отрисовать изображение
//---------------------------------------------------------------------------
void CView_Image::DrawImage(CDC *cDC_Ptr,unsigned long &width,unsigned long &height,bool draw)
{
 width=0;
 height=0;
 CDocument_Image* tempDocument=GetDocument();//получить класс документа
 //изображение
 long offsetx_image=OFFSET_X;
 long offsety_image=OFFSET_Y;
 long height_image=0;
 long offsetx_palette=offsetx_image+OFFSET_PALETTE_X;
 long offsety_palette=offsety_image+OFFSET_PALETTE_Y;

 CBitmap *cBitmapImage=tempDocument->GetBitmapImage(); 
 if (cBitmapImage!=NULL)
 {  
  BITMAP BitmapImage;
  cBitmapImage->GetObject(sizeof(BITMAP),&BitmapImage);
  if (draw==true)
  {   
   CDC cDC_Image;
   cDC_Image.CreateCompatibleDC(cDC_Ptr);
   cDC_Image.SelectObject(cBitmapImage);
   cDC_Ptr->SetMapMode(MM_TEXT);
   cDC_Ptr->StretchBlt(offsetx_image,offsety_image,BitmapImage.bmWidth,BitmapImage.bmHeight,&cDC_Image,0,0,BitmapImage.bmWidth,BitmapImage.bmHeight,SRCCOPY);
  }
  offsetx_palette+=BitmapImage.bmWidth;
  height_image=BitmapImage.bmHeight;
  //рисуем рамку вокруг
  if (draw==true)
  {
   CPen *cPen_Old_Ptr;
   CPen cPen_Border;
   cPen_Border.CreatePen(PS_SOLID,1,RGB(128,128,128));
   cPen_Old_Ptr=cDC_Ptr->SelectObject(&cPen_Border); 
   cDC_Ptr->MoveTo(CPoint(0,0));
   cDC_Ptr->LineTo(offsetx_image+BitmapImage.bmWidth,0);   
   cDC_Ptr->LineTo(offsetx_image+BitmapImage.bmWidth,offsety_image+BitmapImage.bmHeight);
   cDC_Ptr->LineTo(0,offsety_image+BitmapImage.bmHeight);
   cDC_Ptr->LineTo(0,0);
   cDC_Ptr->SelectObject(cPen_Old_Ptr);
  }  
  if (width<offsetx_image+BitmapImage.bmWidth+1) width=offsetx_image+BitmapImage.bmWidth+1;
  if (height<offsety_image+BitmapImage.bmHeight+1) height=offsety_image+BitmapImage.bmHeight+1;
 }

  //палитра
 CBitmap *cBitmapPalette=tempDocument->GetBitmapPalette();
 if (cBitmapPalette!=NULL && height_image>0)
 {
  BITMAP BitmapPalette;
  cBitmapPalette->GetObject(sizeof(BITMAP),&BitmapPalette);
  if (draw==true)
  {
   CDC cDC_Palette;
   cDC_Palette.CreateCompatibleDC(cDC_Ptr);
   cDC_Palette.SelectObject(cBitmapPalette);
   cDC_Ptr->StretchBlt(offsetx_palette,offsety_palette,BitmapPalette.bmWidth,height_image,&cDC_Palette,0,0,BitmapPalette.bmWidth,BitmapPalette.bmHeight,SRCCOPY);
   //рисуем рамку вокруг
   CPen *cPen_Old_Ptr;
   CPen cPen_Border;
   cPen_Border.CreatePen(PS_SOLID,1,RGB(128,128,128));
   cPen_Old_Ptr=cDC_Ptr->SelectObject(&cPen_Border); 
   cDC_Ptr->MoveTo(CPoint(offsetx_palette-1,offsety_palette-1));
   cDC_Ptr->LineTo(offsetx_palette+BitmapPalette.bmWidth,offsety_palette-1);   
   cDC_Ptr->LineTo(offsetx_palette+BitmapPalette.bmWidth,offsety_palette+height_image);
   cDC_Ptr->LineTo(offsetx_palette-1,offsety_palette+height_image);
   cDC_Ptr->LineTo(offsetx_palette-1,offsety_palette-1);
   cDC_Ptr->SelectObject(cPen_Old_Ptr);
  }
  if (width<offsetx_palette+BitmapPalette.bmWidth+1) width=offsetx_palette+BitmapPalette.bmWidth+1;
  if (height<offsety_palette+height_image+1) height=offsety_palette+height_image+1;

  //уровни температуры
  CFont ft_text;
  ft_text.CreateFontIndirect(&LogFont_Text);
  CFont* oldf=cDC_Ptr->SelectObject(&ft_text);
  long oldbkmode=cDC_Ptr->SetBkMode(TRANSPARENT);
  cDC_Ptr->SetTextColor(RGB(0,0,0));
  char string[255];
  double tmax;
  double tmin;
  tempDocument->GetMinMaxTemperature(tmin,tmax);
  SIZE Size;
  sprintf(string,"%.1fC",tmin);
  GetTextExtentPoint32(cDC_Ptr->m_hDC,string,strlen(string),&Size);
  long offsetx_text=offsetx_palette+BitmapPalette.bmWidth+4;
  long offsety_text=offsety_palette+height_image-Size.cy-1;
  if (draw==true) cDC_Ptr->TextOut(offsetx_text,offsety_text,string);

  if (width<offsetx_text+Size.cx) width=offsetx_text+Size.cx;
  if (height<offsety_text+Size.cy) height=offsety_text+Size.cy;

  sprintf(string,"%.1fC",tmax);
  GetTextExtentPoint32(cDC_Ptr->m_hDC,string,strlen(string),&Size);
  offsetx_text=offsetx_palette+BitmapPalette.bmWidth+4;
  offsety_text=offsety_palette;
  if (draw==true) cDC_Ptr->TextOut(offsetx_text,offsety_text,string);

  if (width<offsetx_text+Size.cx) width=offsetx_text+Size.cx;
  if (height<offsety_text+Size.cy) height=offsety_text+Size.cy;

  cDC_Ptr->SetBkMode(oldbkmode);
  cDC_Ptr->SelectObject(oldf);
  ft_text.DeleteObject();
 }
}

//-Функции обработки сообщений класса----------------------------------------
//---------------------------------------------------------------------------
//событие перерисовки фона
//---------------------------------------------------------------------------
BOOL CView_Image::OnEraseBkgnd(CDC *pDC)
{
 return(TRUE);
}
//---------------------------------------------------------------------------
//событие нажатия правой кнопки мыши
//---------------------------------------------------------------------------
void CView_Image::OnRButtonDown(UINT nFlags,CPoint point)
{
}
//---------------------------------------------------------------------------
//событие отпускания правой кнопки мыши
//---------------------------------------------------------------------------
void CView_Image::OnRButtonUp(UINT nFlags,CPoint point)
{
}
//---------------------------------------------------------------------------
//событие нажатия левой кнопки мыши
//---------------------------------------------------------------------------
void CView_Image::OnLButtonDown(UINT nFlags,CPoint point)
{
 CDocument_Image* tempDocument=GetDocument();//получить класс документа
 tempDocument->AddPoint(point.x-OFFSET_X,point.y-OFFSET_Y); 
 //изменяем список точек
 CFrameWnd_Main *cFrameWnd_Main=reinterpret_cast<CFrameWnd_Main*>(GetParentFrame());
 if (cFrameWnd_Main!=NULL) cFrameWnd_Main->SendMessage(MESSAGE_UPDATE_POINT_LIST,0,0);
}
//---------------------------------------------------------------------------
//событие отпускания левой кнопки мыши
//---------------------------------------------------------------------------
void CView_Image::OnLButtonUp(UINT nFlags,CPoint point)
{
}
//---------------------------------------------------------------------------
//событие перемещение мышки
//---------------------------------------------------------------------------
void CView_Image::OnMouseMove(UINT nFlags,CPoint point)
{
}
//---------------------------------------------------------------------------
//событие уничтожения окна
//---------------------------------------------------------------------------
afx_msg void CView_Image::OnDestroy(void)
{
 KillTimer(IDR_TIMER_IMAGE);
}
//---------------------------------------------------------------------------
//событие таймера
//---------------------------------------------------------------------------
afx_msg void CView_Image::OnTimer(UINT nIDEvent)
{
 if (nIDEvent!=IDR_TIMER_IMAGE) 
 {
  CView::OnTimer(nIDEvent);
  return;
 }
 InvalidateRect(NULL,FALSE);
}
//---------------------------------------------------------------------------
//сохранить изображение
//---------------------------------------------------------------------------
void CView_Image::SaveImage(string filename)
{
 CPaintDC cPaintDC(this);
 unsigned long width;
 unsigned long height;
 DrawImage(&cPaintDC,width,height,false);

 CMetaFileDC cMetaFileDC;
 cMetaFileDC.CreateEnhanced(NULL,filename.c_str(),NULL,NULL);
 if (cMetaFileDC.m_hDC==NULL) return;
 //рисуем всё, что требуется
 //очистим поле
 CBrush cBrush;
 cBrush.CreateSolidBrush(RGB(255,255,255));
 CRect cRect;
 cRect.left=0;
 cRect.right=cRect.left+width+1;
 cRect.top=0;
 cRect.bottom=cRect.top+height+1;
 cMetaFileDC.FillRect(&cRect,&cBrush);
 cMetaFileDC.SetMapMode(MM_TEXT);
 DrawImage(&cMetaFileDC,width,height,true);
 HENHMETAFILE hemf=cMetaFileDC.CloseEnhanced();
 DeleteEnhMetaFile(hemf);
}
//-Прочее--------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CView_Image,CView)

