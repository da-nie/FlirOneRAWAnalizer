#include "stdafx.h"
//------------------------------------------------------------------------------
#include "cdocument_image.h"
#include "cview_image.h"
#include "cframewnd_main.h"
//------------------------------------------------------------------------------
class CWinApp_Main:public CWinApp
{
 protected:
  //-Переменные класса-------------------------------------------------------
  //-Функции класса----------------------------------------------------------
  //-Прочее------------------------------------------------------------------
 public:
  //-Конструктор класса------------------------------------------------------
  CWinApp_Main(void);
  //-Деструктор класса-------------------------------------------------------
  ~CWinApp_Main();
  //-Переменные класса-------------------------------------------------------
  //-Функции класса----------------------------------------------------------
  BOOL InitInstance(void);
  //-Прочее------------------------------------------------------------------
};
//-Конструктор класса--------------------------------------------------------
CWinApp_Main::CWinApp_Main(void)
{
}
//-Деструктор класса---------------------------------------------------------
CWinApp_Main::~CWinApp_Main()
{
}
//-Функции класса------------------------------------------------------------
BOOL CWinApp_Main::InitInstance(void)
{
 CSingleDocTemplate* pDocTemplate;//новый шаблон 
 pDocTemplate=new CSingleDocTemplate(IDR_MENU_MAIN,RUNTIME_CLASS(CDocument_Image),RUNTIME_CLASS(CFrameWnd_Main),RUNTIME_CLASS(CView_Image));
 //здесь:
 //IDR_MENU_MAIN- идентификатор меню в ресурсах
 //CMyDocument-класс документа (данных)
 //CMainFrameWnd-класс рамки окна
 //CMyView-класс представления документа
 AddDocTemplate(pDocTemplate);//добавить шаблон
 CCommandLineInfo cmdInfo;//класс команд
 ParseCommandLine(cmdInfo);//разбор командной строки
 if (!ProcessShellCommand(cmdInfo)) return(FALSE);// запуск команд на выполнение
 m_pMainWnd->ShowWindow(SW_SHOW);//показать окно
 m_pMainWnd->UpdateWindow();//запустить цикл обработки сообщений
 return(TRUE);
}

CWinApp_Main cWinApp_Main;


