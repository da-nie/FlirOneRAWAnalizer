#ifndef C_FRAME_WND_MAIN_H
#define C_FRAME_WND_MAIN_H

#include "stdafx.h"
#include "cview_image.h"

#include <vector>
#include <string>

using namespace std;

class CFrameWnd_Main:public CFrameWnd
{
 protected:
  //-ѕеременные класса------------------------------------------------------- 
  CDialogBar cDialogBar_Control;//бокова€ панель настроек
  CDialogBar cDialogBar_RAWFileList;//список файлов, доступных дл€ просмотра
  vector<string> vector_PaletteFileName;//список палитр
  vector<string> vector_RAWFileName;//список raw-файлов
  string CurrentDirectory;//текущий каталог
  string SaveDirectory;//каталог, в который производитс€ сохранение файла

  //-‘ункции класса----------------------------------------------------------
  //-ѕрочее------------------------------------------------------------------
 public:
  //- онструктор класса------------------------------------------------------
  CFrameWnd_Main(void);
  //-ƒеструктор класса-------------------------------------------------------
  ~CFrameWnd_Main();
  //-ѕеременные класса-------------------------------------------------------
  //-‘ункции обработки сообщений класса--------------------------------------
  DECLARE_MESSAGE_MAP()
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);//событие создани€ окна  
  afx_msg void OnDestroy(void);//событие уничтожени€ окна
  afx_msg void OnTimer(UINT nIDEvent);//событие таймера
  afx_msg void OnMenu_Window_ShowControlPanel(void);//выбрана смена видимости панели управлени€
  afx_msg void OnMenu_Window_ShowRAWFileListPanel(void);//выбрана смена видимости панели списка RAW-файлов
  afx_msg void OnMenu_File_Load(void);//выбрана загрузка
  afx_msg void OnMenu_File_SaveImage(void);//выбрано сохранение изображени€
  afx_msg void OnMenu_File_Exit(void);//выбран выход из программы
  afx_msg void OnMenu_Image_UseHightContrastFilter(void);//использовать ли контрастоповышающий фильтр
  afx_msg void OnButton_ApplyPalette(void);//применить выбранную палитру
  afx_msg void OnButton_ClearList(void);//удалить все точки из списка
  afx_msg void OnButton_DeleteSelected(void);//удалить выбранные точки из списка
  afx_msg void OnButton_ApplyParameters(void);//применить новые параметры
  afx_msg void OnSelChange_List_PointList(void);//выбрали точку
  afx_msg void OnMessage_UpdatePointList(WPARAM wParam,LPARAM lParam);//обновить список точек
  afx_msg void OnSelChange_List_RAWFileList(void);
  //-ѕрочее------------------------------------------------------------------
  DECLARE_DYNCREATE(CFrameWnd_Main)
 private:
  void FindRAWFile(string StartDirectory);
 public:
  //-‘ункции класса----------------------------------------------------------
};

#endif