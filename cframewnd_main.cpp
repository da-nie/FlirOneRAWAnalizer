#include "cframewnd_main.h"

//-������� ��������� ��������� ������----------------------------------------
BEGIN_MESSAGE_MAP(CFrameWnd_Main,CFrameWnd)
 ON_WM_CREATE()
 ON_WM_DESTROY()
 ON_WM_TIMER()
 ON_COMMAND(ID_MENU_WINDOW_CONTROL_PANEL,OnMenu_Window_ShowControlPanel)
 ON_COMMAND(ID_MENU_WINDOW_RAW_FILE_LIST_PANEL,OnMenu_Window_ShowRAWFileListPanel)
 ON_COMMAND(ID_MENU_FILE_LOAD,OnMenu_File_Load)
 ON_COMMAND(ID_MENU_FILE_SAVE_IMAGE,OnMenu_File_SaveImage)
 ON_COMMAND(ID_MENU_FILE_EXIT,OnMenu_File_Exit)
 ON_COMMAND(ID_MENU_IMAGE_USE_HIGHT_CONTRAST_FILTER,OnMenu_Image_UseHightContrastFilter)
 ON_COMMAND(IDC_BUTTON_DIALOGBAR_CONTROL_APPLY_PALETTE,OnButton_ApplyPalette)
 ON_COMMAND(IDC_BUTTON_DIALOGBAR_CONTROL_CLEAR_LIST,OnButton_ClearList)
 ON_COMMAND(IDC_BUTTON_DIALOGBAR_CONTROL_DELETE_SELECTED,OnButton_DeleteSelected)
 ON_COMMAND(IDC_BUTTON_DIALOGBAR_CONTROL_APPLY_PARAMETERS,OnButton_ApplyParameters)
 ON_LBN_SELCHANGE(IDC_LIST_DIALOGBAR_CONTROL_POINT_LIST,OnSelChange_List_PointList) 
 ON_MESSAGE(MESSAGE_UPDATE_POINT_LIST,OnMessage_UpdatePointList)
 ON_LBN_SELCHANGE(IDC_LIST_DIALOGBAR_RAW_FILE_LIST_LIST,OnSelChange_List_RAWFileList) 
END_MESSAGE_MAP()
//-����������� ������--------------------------------------------------------
CFrameWnd_Main::CFrameWnd_Main(void)
{
}
//-���������� ������---------------------------------------------------------
CFrameWnd_Main::~CFrameWnd_Main()
{
}
//-������� ������------------------------------------------------------------
//-������� ��������� ��������� ������----------------------------------------

//---------------------------------------------------------------------------
//������� �������� ����
//---------------------------------------------------------------------------
int CFrameWnd_Main::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
 EnableDocking(CBRS_ALIGN_ANY);
 //�������� ���������� ������
 cDialogBar_Control.Create(this,IDD_DIALOGBAR_CONTROL,WS_VISIBLE|CBRS_ALIGN_LEFT|CBRS_GRIPPER,IDD_DIALOGBAR_CONTROL);
 cDialogBar_Control.SetWindowText("��������������� ������ ����������");
 cDialogBar_Control.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);
 DockControlBar(&cDialogBar_Control,AFX_IDW_DOCKBAR_LEFT);
 //��������� �������� � ����� ������
 ((CEdit *)cDialogBar_Control.GetDlgItem(IDC_EDIT_DIALOGBAR_CONTROL_EMISSIVITY))->SetLimitText(10);
 ((CEdit *)cDialogBar_Control.GetDlgItem(IDC_EDIT_DIALOGBAR_CONTROL_EMISSIVITY))->SetWindowText("0.95"); 

 //�������� ���������� ������
 cDialogBar_RAWFileList.Create(this,IDD_DIALOGBAR_RAW_FILE_LIST,WS_VISIBLE|CBRS_ALIGN_RIGHT|CBRS_GRIPPER,IDD_DIALOGBAR_RAW_FILE_LIST);
 cDialogBar_RAWFileList.SetWindowText("������ ������");
 cDialogBar_RAWFileList.EnableDocking(CBRS_ALIGN_RIGHT|CBRS_ALIGN_LEFT);
 DockControlBar(&cDialogBar_RAWFileList,AFX_IDW_DOCKBAR_RIGHT);
 //���� �������
 ((CComboBox *)cDialogBar_Control.GetDlgItem(IDC_COMBO_DIALOGBAR_CONTROL_PALETTE))->ResetContent();
 vector_PaletteFileName.clear();
 WIN32_FIND_DATA wfd;
 //������� ���� �����
 char path[MAX_PATH];
 GetCurrentDirectory(MAX_PATH,path);
 HANDLE Handle_Find=FindFirstFile("palette\\*.pal",&wfd);
 if (Handle_Find!=INVALID_HANDLE_VALUE)
 {
  while(1)
  {
   //��� ����
   if (wfd.cFileName[0]!='.' && !(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
   {
    //��������� � ������
    string filename=path;
	filename+="\\palette\\";
	filename+=wfd.cFileName;
	((CComboBox *)cDialogBar_Control.GetDlgItem(IDC_COMBO_DIALOGBAR_CONTROL_PALETTE))->AddString(wfd.cFileName);
	vector_PaletteFileName.push_back(filename);
   }
   if (FindNextFile(Handle_Find,&wfd)==FALSE) break;
  }
  FindClose(Handle_Find);
 }
 ((CComboBox *)cDialogBar_Control.GetDlgItem(IDC_COMBO_DIALOGBAR_CONTROL_PALETTE))->SetCurSel(0);
 CurrentDirectory=path;
 SaveDirectory=path;
 //�������� ������ ������� ������
 //��������� ����� � ��������
 FindRAWFile(CurrentDirectory);
 //���������� ������
 SetTimer(IDR_TIMER_MAIN,100,0);
 return(CFrameWnd::OnCreate(lpCreateStruct));
}
//---------------------------------------------------------------------------
//������� ����������� ����
//---------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnDestroy(void)
{
 KillTimer(IDR_TIMER_MAIN);
}
//---------------------------------------------------------------------------
//������� �������
//---------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnTimer(UINT nIDEvent)
{
 if (nIDEvent==IDR_TIMER_MAIN)
 {
  CMenu *cMenu_Main=GetMenu();
  if (cMenu_Main!=NULL)
  {
   long bVisible_ControlPanel=((cDialogBar_Control.GetStyle()&WS_VISIBLE)!=0);
   long bVisible_RAWFileListPanel=((cDialogBar_RAWFileList.GetStyle()&WS_VISIBLE)!=0);

   long state_ControlPanel=cMenu_Main->GetMenuState(ID_MENU_WINDOW_CONTROL_PANEL,MF_BYCOMMAND);
   long state_RAWFileListPanel=cMenu_Main->GetMenuState(ID_MENU_WINDOW_RAW_FILE_LIST_PANEL,MF_BYCOMMAND);

   if (state_ControlPanel==MF_CHECKED && bVisible_ControlPanel==0) cMenu_Main->CheckMenuItem(ID_MENU_WINDOW_CONTROL_PANEL,MF_UNCHECKED);
   if (state_RAWFileListPanel==MF_CHECKED && bVisible_RAWFileListPanel==0) cMenu_Main->CheckMenuItem(ID_MENU_WINDOW_RAW_FILE_LIST_PANEL,MF_UNCHECKED);
  }
 }
 CFrameWnd::OnTimer(nIDEvent);
}
//---------------------------------------------------------------------------
//������� ����� ��������� ������ ����������
//---------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnMenu_Window_ShowControlPanel(void)
{
 CMenu *cMenu_Main=GetMenu();
 long state=cMenu_Main->GetMenuState(ID_MENU_WINDOW_CONTROL_PANEL,MF_BYCOMMAND);
 if (state==MF_CHECKED)//���������� ������
 {
  cMenu_Main->CheckMenuItem(ID_MENU_WINDOW_CONTROL_PANEL,MF_UNCHECKED);
  ShowControlBar(&cDialogBar_Control,FALSE,FALSE);
 }
 if (state==MF_UNCHECKED)//������� ������
 {
  cMenu_Main->CheckMenuItem(ID_MENU_WINDOW_CONTROL_PANEL,MF_CHECKED);
  ShowControlBar(&cDialogBar_Control,TRUE,FALSE);
 }
}
//---------------------------------------------------------------------------
//������� ����� ��������� ������ ������ RAW-������
//---------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnMenu_Window_ShowRAWFileListPanel(void)
{
 CMenu *cMenu_Main=GetMenu();
 long state=cMenu_Main->GetMenuState(ID_MENU_WINDOW_RAW_FILE_LIST_PANEL,MF_BYCOMMAND);
 if (state==MF_CHECKED)//���������� ������
 {
  cMenu_Main->CheckMenuItem(ID_MENU_WINDOW_RAW_FILE_LIST_PANEL,MF_UNCHECKED);
  ShowControlBar(&cDialogBar_RAWFileList,FALSE,FALSE);
 }
 if (state==MF_UNCHECKED)//������� ������
 {
  cMenu_Main->CheckMenuItem(ID_MENU_WINDOW_RAW_FILE_LIST_PANEL,MF_CHECKED);
  ShowControlBar(&cDialogBar_RAWFileList,TRUE,FALSE);
 }
}
//---------------------------------------------------------------------------
//������� �������� RAW
//---------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnMenu_File_Load(void)
{	
 CFileDialog cFileDialog(TRUE,"","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"*.raw|*.raw||",this);
 char Title[256];
 strcpy(Title,"����� �������� ������ RAW-�����������");
 cFileDialog.m_ofn.lpstrTitle=Title;
 if (cFileDialog.DoModal()!=IDOK) return;
 char path[MAX_PATH];
 GetCurrentDirectory(MAX_PATH,path);
 //�������� ������ ������� ������
 CurrentDirectory=path;
 //��������� ����� � ��������
 FindRAWFile(CurrentDirectory);
}
//---------------------------------------------------------------------------
//������� ���������� �����������
//---------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnMenu_File_SaveImage(void)
{
 CFileDialog cFileDialog(FALSE,"","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"*.emf|*.emf||",this);
 cFileDialog.m_ofn.lpstrInitialDir=SaveDirectory.c_str();
 char Title[256];
 strcpy(Title,"���������� �����������");
 cFileDialog.m_ofn.lpstrTitle=Title;
 if (cFileDialog.DoModal()!=IDOK) return;
 string file_name=cFileDialog.GetFileName();
 //������� ��������� �� �������������
 CView_Image* cView=reinterpret_cast<CView_Image*>(GetActiveView());
 cView->SaveImage(file_name);
 char path[MAX_PATH];
 GetCurrentDirectory(MAX_PATH,path); 
 SaveDirectory=path;
 SetCurrentDirectory(CurrentDirectory.c_str());
}
//---------------------------------------------------------------------------
//������ ����� �� ���������
//---------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnMenu_File_Exit(void)
{
 if (MessageBox("��������� ������ ���������?","�������������",MB_YESNO|MB_DEFBUTTON2)!=IDYES) return;
 DestroyWindow();
}
//---------------------------------------------------------------------------
//������������ �� ������������������� ������
//---------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnMenu_Image_UseHightContrastFilter(void)
{
 CMenu *cMenu_Main=GetMenu();
 long state=cMenu_Main->GetMenuState(ID_MENU_IMAGE_USE_HIGHT_CONTRAST_FILTER,MF_BYCOMMAND);
 bool filter_state=false;
 if (state==MF_CHECKED) cMenu_Main->CheckMenuItem(ID_MENU_IMAGE_USE_HIGHT_CONTRAST_FILTER,MF_UNCHECKED);
 else 
 {
  filter_state=true;
  cMenu_Main->CheckMenuItem(ID_MENU_IMAGE_USE_HIGHT_CONTRAST_FILTER,MF_CHECKED);
 }
 //������� ��������� �� ��������
 CDocument_Image* tempDocument=reinterpret_cast<CDocument_Image*>(GetActiveDocument());
 tempDocument->SetHightContrastFilterEnabled(filter_state);
}
//---------------------------------------------------------------------------
//��������� ��������� �������
//---------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnButton_ApplyPalette(void)
{
 long index=((CComboBox *)cDialogBar_Control.GetDlgItem(IDC_COMBO_DIALOGBAR_CONTROL_PALETTE))->GetCurSel();
 if (vector_PaletteFileName.size()<=index) return;
  //������� ��������� �� ��������
 CDocument_Image* tempDocument=reinterpret_cast<CDocument_Image*>(GetActiveDocument());
 tempDocument->LoadColorMap(const_cast<char*>(vector_PaletteFileName[index].c_str()));
}
//---------------------------------------------------------------------------
//������� ��� ����� �� ������
//---------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnButton_ClearList(void)
{
 if (MessageBox("�������� ���� ������?","�������������",MB_YESNO|MB_DEFBUTTON2)!=IDYES) return;
 //������� ��������� �� ��������
 CDocument_Image* tempDocument=reinterpret_cast<CDocument_Image*>(GetActiveDocument());
 tempDocument->ClearPointList();
 ((CListBox *)cDialogBar_Control.GetDlgItem(IDC_LIST_DIALOGBAR_CONTROL_POINT_LIST))->ResetContent();
}
//---------------------------------------------------------------------------
//������� ��������� ����� �� ������
//---------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnButton_DeleteSelected(void)
{
 //������� ��������� �� ��������
 CDocument_Image* tempDocument=reinterpret_cast<CDocument_Image*>(GetActiveDocument());
 tempDocument->DeleteSelectedPoint();
 OnMessage_UpdatePointList(0,0);
}
//---------------------------------------------------------------------------
//��������� ����� ���������
//---------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnButton_ApplyParameters(void)
{
 //������� ��������� �� �������� � ������� ����������� ���������
 CDocument_Image* tempDocument=reinterpret_cast<CDocument_Image*>(GetActiveDocument());
 char string[255];
 ((CEdit *)cDialogBar_Control.GetDlgItem(IDC_EDIT_DIALOGBAR_CONTROL_EMISSIVITY))->GetWindowText(string,255); 
 double emissivity=atof(string);
 if (emissivity<0 || emissivity>1)
 {
  double emissivity=tempDocument->GetEmissivity();
  sprintf(string,"%.2f",emissivity);
  ((CEdit *)cDialogBar_Control.GetDlgItem(IDC_EDIT_DIALOGBAR_CONTROL_EMISSIVITY))->SetWindowText(string); 
  MessageBox("����������� ��������� ������ ���� �� 0 �� 1!","������",MB_OK);
  return;
 }
 tempDocument->SetEmissivity(emissivity);
}

//---------------------------------------------------------------------------
//������� �����
//---------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnSelChange_List_PointList(void)
{ 
 long n;
 //������� ��������� �� ��������
 CDocument_Image* tempDocument=reinterpret_cast<CDocument_Image*>(GetActiveDocument());
 vector<SPoint> *vector_SPoint_Ptr=tempDocument->GetVectorSPointPtr();
 long size=vector_SPoint_Ptr->size();
 int *SelectedIndex=new int[size+1];//��������� ������� �����
 long selected=((CListBox *)cDialogBar_Control.GetDlgItem(IDC_LIST_DIALOGBAR_CONTROL_POINT_LIST))->GetSelItems(size+1,SelectedIndex);
 if (selected==LB_ERR) selected=0;//������ �� �������
 for(n=0;n<size;n++) (*vector_SPoint_Ptr)[n].Selected=false;
 if (selected==0) MessageBeep(1);
 for(n=0;n<selected;n++) (*vector_SPoint_Ptr)[SelectedIndex[n]].Selected=true;
 delete[](SelectedIndex);
 tempDocument->RefreshSelectedList(); 
}
//---------------------------------------------------------------------------
//�������� ������ �����
//---------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnMessage_UpdatePointList(WPARAM wParam,LPARAM lParam)
{
 //������� ��������� �� ��������
 CDocument_Image* tempDocument=reinterpret_cast<CDocument_Image*>(GetActiveDocument());
 vector<SPoint>* vector_SPoint_Ptr=tempDocument->GetVectorSPointPtr();
 long size=vector_SPoint_Ptr->size();
 ((CListBox *)cDialogBar_Control.GetDlgItem(IDC_LIST_DIALOGBAR_CONTROL_POINT_LIST))->ResetContent();
 char string[255];
 for(long n=0;n<size;n++)
 {
  sprintf(string,"�����:%i �������:[%i,%i]",n+1,(*vector_SPoint_Ptr)[n].X,(*vector_SPoint_Ptr)[n].Y);
  ((CListBox *)cDialogBar_Control.GetDlgItem(IDC_LIST_DIALOGBAR_CONTROL_POINT_LIST))->AddString(string);
  if ((*vector_SPoint_Ptr)[n].Selected==true) ((CListBox *)cDialogBar_Control.GetDlgItem(IDC_LIST_DIALOGBAR_CONTROL_POINT_LIST))->SetSel(n,true);
 }
}
//---------------------------------------------------------------------------
//������� ����� �� ������
//---------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnSelChange_List_RAWFileList(void)
{ 
 int selected=((CListBox *)cDialogBar_RAWFileList.GetDlgItem(IDC_LIST_DIALOGBAR_RAW_FILE_LIST_LIST))->GetCurSel();
 if (selected==LB_ERR) return;//������ �� �������
 //�������� ���� � ��������� ������ 
 string file_name=vector_RAWFileName[selected];
 FILE *file=fopen(file_name.c_str(),"rb");
 if (file==NULL) return;
  //������� ��������� �� ��������
 CDocument_Image* tempDocument=reinterpret_cast<CDocument_Image*>(GetActiveDocument());
 tempDocument->Load(file);
 tempDocument->SetTitle(file_name.c_str());
 fclose(file);
 //�������� ����������� ���������
 double emissivity=tempDocument->GetEmissivity();
 char string[255];
 sprintf(string,"%.2f",emissivity);
 ((CEdit *)cDialogBar_Control.GetDlgItem(IDC_EDIT_DIALOGBAR_CONTROL_EMISSIVITY))->SetWindowText(string);
}

//-������� ������----------------------------------------------------------

//---------------------------------------------------------------------------
//����� ������ �������
//---------------------------------------------------------------------------
void CFrameWnd_Main::FindRAWFile(string StartDirectory)
{
 vector_RAWFileName.clear();
 //������� ������ ������
 ((CListBox*)cDialogBar_RAWFileList.GetDlgItem(IDC_LIST_DIALOGBAR_RAW_FILE_LIST_LIST))->ResetContent();
 SetCurrentDirectory(StartDirectory.c_str());
 WIN32_FIND_DATA wfd;
 //���� �����
 HANDLE Handle_Find=FindFirstFile("*.raw",&wfd);
 if (Handle_Find!=INVALID_HANDLE_VALUE)
 {
  while(1)
  {
   //��� ����
   if (wfd.cFileName[0]!='.' && !(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
   {
    //��������� � ������ ��������� ������
    ((CListBox*)cDialogBar_RAWFileList.GetDlgItem(IDC_LIST_DIALOGBAR_RAW_FILE_LIST_LIST))->AddString(wfd.cFileName);
    vector_RAWFileName.push_back(wfd.cFileName);
   }
   if (FindNextFile(Handle_Find,&wfd)==FALSE) break;
  }
  FindClose(Handle_Find);
 }
}

//-������--------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CFrameWnd_Main,CFrameWnd)
//-������� ������----------------------------------------------------------

