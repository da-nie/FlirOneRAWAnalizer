#include "stdafx.h"
//------------------------------------------------------------------------------
#include "cdocument_image.h"
#include "cview_image.h"
#include "cframewnd_main.h"
//------------------------------------------------------------------------------
class CWinApp_Main:public CWinApp
{
 protected:
  //-���������� ������-------------------------------------------------------
  //-������� ������----------------------------------------------------------
  //-������------------------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CWinApp_Main(void);
  //-���������� ������-------------------------------------------------------
  ~CWinApp_Main();
  //-���������� ������-------------------------------------------------------
  //-������� ������----------------------------------------------------------
  BOOL InitInstance(void);
  //-������------------------------------------------------------------------
};
//-����������� ������--------------------------------------------------------
CWinApp_Main::CWinApp_Main(void)
{
}
//-���������� ������---------------------------------------------------------
CWinApp_Main::~CWinApp_Main()
{
}
//-������� ������------------------------------------------------------------
BOOL CWinApp_Main::InitInstance(void)
{
 CSingleDocTemplate* pDocTemplate;//����� ������ 
 pDocTemplate=new CSingleDocTemplate(IDR_MENU_MAIN,RUNTIME_CLASS(CDocument_Image),RUNTIME_CLASS(CFrameWnd_Main),RUNTIME_CLASS(CView_Image));
 //�����:
 //IDR_MENU_MAIN- ������������� ���� � ��������
 //CMyDocument-����� ��������� (������)
 //CMainFrameWnd-����� ����� ����
 //CMyView-����� ������������� ���������
 AddDocTemplate(pDocTemplate);//�������� ������
 CCommandLineInfo cmdInfo;//����� ������
 ParseCommandLine(cmdInfo);//������ ��������� ������
 if (!ProcessShellCommand(cmdInfo)) return(FALSE);// ������ ������ �� ����������
 m_pMainWnd->ShowWindow(SW_SHOW);//�������� ����
 m_pMainWnd->UpdateWindow();//��������� ���� ��������� ���������
 return(TRUE);
}

CWinApp_Main cWinApp_Main;


