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
  //-���������� ������------------------------------------------------------- 
  CDialogBar cDialogBar_Control;//������� ������ ��������
  CDialogBar cDialogBar_RAWFileList;//������ ������, ��������� ��� ���������
  vector<string> vector_PaletteFileName;//������ ������
  vector<string> vector_RAWFileName;//������ raw-������
  string CurrentDirectory;//������� �������
  string SaveDirectory;//�������, � ������� ������������ ���������� �����

  //-������� ������----------------------------------------------------------
  //-������------------------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CFrameWnd_Main(void);
  //-���������� ������-------------------------------------------------------
  ~CFrameWnd_Main();
  //-���������� ������-------------------------------------------------------
  //-������� ��������� ��������� ������--------------------------------------
  DECLARE_MESSAGE_MAP()
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);//������� �������� ����  
  afx_msg void OnDestroy(void);//������� ����������� ����
  afx_msg void OnTimer(UINT nIDEvent);//������� �������
  afx_msg void OnMenu_Window_ShowControlPanel(void);//������� ����� ��������� ������ ����������
  afx_msg void OnMenu_Window_ShowRAWFileListPanel(void);//������� ����� ��������� ������ ������ RAW-������
  afx_msg void OnMenu_File_Load(void);//������� ��������
  afx_msg void OnMenu_File_SaveImage(void);//������� ���������� �����������
  afx_msg void OnMenu_File_Exit(void);//������ ����� �� ���������
  afx_msg void OnMenu_Image_UseHightContrastFilter(void);//������������ �� ������������������� ������
  afx_msg void OnButton_ApplyPalette(void);//��������� ��������� �������
  afx_msg void OnButton_ClearList(void);//������� ��� ����� �� ������
  afx_msg void OnButton_DeleteSelected(void);//������� ��������� ����� �� ������
  afx_msg void OnButton_ApplyParameters(void);//��������� ����� ���������
  afx_msg void OnSelChange_List_PointList(void);//������� �����
  afx_msg void OnMessage_UpdatePointList(WPARAM wParam,LPARAM lParam);//�������� ������ �����
  afx_msg void OnSelChange_List_RAWFileList(void);
  //-������------------------------------------------------------------------
  DECLARE_DYNCREATE(CFrameWnd_Main)
 private:
  void FindRAWFile(string StartDirectory);
 public:
  //-������� ������----------------------------------------------------------
};

#endif