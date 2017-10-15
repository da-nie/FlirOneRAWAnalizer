#ifndef C_VIEW_IMAGE_H
#define C_VIEW_IMAGE_H

#include "stdafx.h"
#include "cdocument_image.h"
#include "cframewnd_main.h"

//�������� ����������� �� X
#define OFFSET_X 1
//�������� ����������� �� Y
#define OFFSET_Y 1
//�������� ������� ������������ ����������� �� X
#define OFFSET_PALETTE_X 16
//�������� ������� ������������ ����������� �� Y
#define OFFSET_PALETTE_Y 0

//---------------------------------------------------------------------------
//����� �������������
//---------------------------------------------------------------------------
class CView_Image:public CView
{
 protected: 
  //-���������� ������-------------------------------------------------------
  LOGFONT LogFont_Text;//��� ������ ������
  //-������� ������----------------------------------------------------------
  afx_msg void OnInitialUpdate(void);//������������� ����
  CDocument_Image* GetDocument();//�������� ��������
  //-������------------------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CView_Image();
  //-���������� ������-------------------------------------------------------
  //-���������� ������-------------------------------------------------------
  //-������� ������----------------------------------------------------------
  void SaveImage(string filename);//��������� �����������
 protected:
  virtual void OnDraw(CDC* pDC);//��� ������� ���������� ���� (�.�. ����������� �� ������������ ������, �� ��� �����)
  void DrawImage(CDC *cDC_Ptr,unsigned long &width,unsigned long &height,bool draw);//���������� �����������
  //-������� ��������� ��������� ������--------------------------------------
  DECLARE_MESSAGE_MAP()
  afx_msg BOOL OnEraseBkgnd(CDC *pDC);//������� ����������� ����
  afx_msg void OnRButtonDown(UINT nFlags,CPoint point);//������� ������� ������ ������ ����
  afx_msg void OnRButtonUp(UINT nFlags,CPoint point);//������� ���������� ������ ������ ����
  afx_msg void OnLButtonDown(UINT nFlags,CPoint point);//������� ������� ����� ������ ����
  afx_msg void OnLButtonUp(UINT nFlags,CPoint point);//������� ���������� ����� ������ ����
  afx_msg void OnMouseMove(UINT nFlags,CPoint point);//������� ����������� �����
  afx_msg void OnTimer(UINT nIDEvent);//������� �������
  afx_msg void OnDestroy(void);//������� ����������� ����
  //-������------------------------------------------------------------------
  DECLARE_DYNCREATE(CView_Image) 
};

#endif