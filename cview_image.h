#ifndef C_VIEW_IMAGE_H
#define C_VIEW_IMAGE_H

#include "stdafx.h"
#include "cdocument_image.h"
#include "cframewnd_main.h"

//смещение изображения по X
#define OFFSET_X 1
//смещение изображения по Y
#define OFFSET_Y 1
//смещение палитры относительно изображения по X
#define OFFSET_PALETTE_X 16
//смещение палитры относительно изображения по Y
#define OFFSET_PALETTE_Y 0

//---------------------------------------------------------------------------
//класс представления
//---------------------------------------------------------------------------
class CView_Image:public CView
{
 protected: 
  //-Переменные класса-------------------------------------------------------
  LOGFONT LogFont_Text;//тип шрифта текста
  //-Функции класса----------------------------------------------------------
  afx_msg void OnInitialUpdate(void);//инициализация окна
  CDocument_Image* GetDocument();//получить документ
  //-Прочее------------------------------------------------------------------
 public:
  //-Конструктор класса------------------------------------------------------
  CView_Image();
  //-Деструктор класса-------------------------------------------------------
  //-Переменные класса-------------------------------------------------------
  //-Функции класса----------------------------------------------------------
  void SaveImage(string filename);//сохранить изображение
 protected:
  virtual void OnDraw(CDC* pDC);//это функция прорисовки вида (т.к. наследовали от абстрактного класса, то она нужна)
  void DrawImage(CDC *cDC_Ptr,unsigned long &width,unsigned long &height,bool draw);//отрисовать изображение
  //-Функции обработки сообщений класса--------------------------------------
  DECLARE_MESSAGE_MAP()
  afx_msg BOOL OnEraseBkgnd(CDC *pDC);//событие перерисовки фона
  afx_msg void OnRButtonDown(UINT nFlags,CPoint point);//событие нажатия правой кнопки мыши
  afx_msg void OnRButtonUp(UINT nFlags,CPoint point);//событие отпускания правой кнопки мыши
  afx_msg void OnLButtonDown(UINT nFlags,CPoint point);//событие нажатия левой кнопки мыши
  afx_msg void OnLButtonUp(UINT nFlags,CPoint point);//событие отпускания левой кнопки мыши
  afx_msg void OnMouseMove(UINT nFlags,CPoint point);//событие перемещение мышки
  afx_msg void OnTimer(UINT nIDEvent);//событие таймера
  afx_msg void OnDestroy(void);//событие уничтожения окна
  //-Прочее------------------------------------------------------------------
  DECLARE_DYNCREATE(CView_Image) 
};

#endif