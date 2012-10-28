// SaccadeSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SaccadeSearch.h"
#include "SaccadeSearchDlg.h"
#include "funcs.h"
#include "Conandata.h"
#include "Eegraph.h"
#include "Saccade.h"
#include <vector>
using namespace std;
typedef vector <Saccade*> SaccadeVec;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

 Eegraph* Win;
 ConanData* Conan;
 CSaccadeSearchDlg* dialog;
// CAboutDlg dialog used for App About

void SetDropDownHeight(CComboBox* pMyComboBox, int itemsToShow)
{
  //Get rectangles
  CRect rctComboBox, rctDropDown;
  pMyComboBox->GetClientRect(&rctComboBox); //Combo rect
  pMyComboBox->GetDroppedControlRect(&rctDropDown); //DropDownList rect

  int itemHeight = pMyComboBox->GetItemHeight(-1); //Get Item height

  pMyComboBox->GetParent()->ScreenToClient(&rctDropDown); //Converts coordinates
  rctDropDown.bottom = rctDropDown.top + rctComboBox.Height() + itemHeight*itemsToShow; //Set height
  pMyComboBox->MoveWindow(&rctDropDown); //enable changes
}


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSaccadeSearchDlg dialog




CSaccadeSearchDlg::CSaccadeSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaccadeSearchDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  Win=NULL;
  Conan=NULL;
  CurSaccade=0;
  init=0;
}

CSaccadeSearchDlg::~CSaccadeSearchDlg()
{
  if(Win!=NULL)
  delete(Win);
  if(Conan!=NULL)
    delete(Conan);
}

void CSaccadeSearchDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_EDIT1, log);
  //DDX_Control(pDX, IDC_BUTTON1, LoadBtn);
  DDX_Control(pDX, IDC_EDIT2, xscale);
  DDX_Control(pDX, IDC_EDIT3, yscale);
  DDX_Control(pDX, IDC_COMBO1, chan);
  DDX_Control(pDX, IDC_EDIT4, xfreq);
  DDX_Control(pDX, IDC_EDIT5, xoffset);
  DDX_Control(pDX, IDC_EDIT6, yoffset);
  DDX_Control(pDX, IDC_COMBO2, rec);
  DDX_Control(pDX, IDC_CHECK1, AutoDefineYOffset);
  DDX_Control(pDX, IDC_CHECK2, GridEnabled);
  DDX_Control(pDX, IDC_CHECK3, showpoints);


  DDX_Control(pDX, IDC_RADIO2, mouse_move);
  DDX_Control(pDX, IDC_RADIO1, mouse_zoom);
  DDX_Control(pDX, IDC_RADIO3, mouse_ruler);
  DDX_Control(pDX, IDC_EDIT8, rulerx);
  DDX_Control(pDX, IDC_EDIT9, rulery);
  DDX_Control(pDX, IDC_EDIT10, MinYSpeedT);
  DDX_Control(pDX, IDC_EDIT11, MinSpeedPointsT);
  DDX_Control(pDX, IDC_EDIT12, MinYLengthT);
  DDX_Control(pDX, IDC_EDIT13, MinXLengthT);
  DDX_Control(pDX, IDC_EDIT14, MinTimeOffsetT);
  DDX_Control(pDX, IDC_EDIT15, StimulCodesT);
  DDX_Control(pDX, IDC_CHECK4, ClearOther);
  DDX_Control(pDX, IDC_EDIT7, CurSaccadeT);
  DDX_Control(pDX, IDC_EDIT16, SacChanT);
  DDX_Control(pDX, IDC_EDIT17, SacRecT);
  DDX_Control(pDX, IDC_EDIT18, SacXBeginT);
  DDX_Control(pDX, IDC_EDIT19, SacYBeginT);
  DDX_Control(pDX, IDC_EDIT20, SacXEndT);
  DDX_Control(pDX, IDC_EDIT21, SacYEndT);
  DDX_Control(pDX, IDC_EDIT22, SacAmplitudeT);
  DDX_Control(pDX, IDC_EDIT23, SacStimulCodeT);
  DDX_Control(pDX, IDC_EDIT24, SacLatentTimeT);
}

BEGIN_MESSAGE_MAP(CSaccadeSearchDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
//  ON_BN_CLICKED(IDC_BUTTON1, &CSaccadeSearchDlg::OnBnClickedButton1)
  ON_CBN_SELCHANGE(IDC_COMBO1, &CSaccadeSearchDlg::OnCbnSelchangeCombo1)
  ON_EN_CHANGE(IDC_EDIT2, &CSaccadeSearchDlg::OnEnChangeEdit2)
  ON_EN_CHANGE(IDC_EDIT3, &CSaccadeSearchDlg::OnEnChangeEdit3)
  ON_EN_UPDATE(IDC_EDIT6, &CSaccadeSearchDlg::OnEnUpdateEdit6)
  ON_EN_UPDATE(IDC_EDIT5, &CSaccadeSearchDlg::OnEnUpdateEdit5)
  ON_WM_SIZE()
  ON_CBN_SELCHANGE(IDC_COMBO2, &CSaccadeSearchDlg::OnCbnSelchangeCombo2)
  ON_BN_CLICKED(IDC_CHECK1, &CSaccadeSearchDlg::OnBnClickedCheck1)
  ON_BN_CLICKED(IDC_BUTTON2, &CSaccadeSearchDlg::OnBnClickedButton2)
  ON_BN_CLICKED(IDC_BUTTON3, &CSaccadeSearchDlg::OnBnClickedButton3)
  ON_BN_CLICKED(IDC_BUTTON4, &CSaccadeSearchDlg::OnBnClickedButton4)
  ON_BN_CLICKED(IDC_BUTTON5, &CSaccadeSearchDlg::OnBnClickedButton5)
  ON_BN_CLICKED(IDC_CHECK2, &CSaccadeSearchDlg::OnBnClickedCheck2)
  ON_WM_NCLBUTTONDOWN()
  ON_WM_LBUTTONDOWN()
  ON_WM_RBUTTONDOWN()
  ON_BN_CLICKED(IDC_CHECK3, &CSaccadeSearchDlg::OnBnClickedCheck3)
  ON_BN_CLICKED(IDOK, &CSaccadeSearchDlg::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL, &CSaccadeSearchDlg::OnBnClickedCancel)
  ON_WM_CLOSE()
  ON_CBN_SELCHANGE(IDC_COMBO3, &CSaccadeSearchDlg::OnCbnSelchangeCombo3)
//  ON_WM_SETCURSOR()
ON_BN_CLICKED(IDC_RADIO2, &CSaccadeSearchDlg::OnBnClickedRadio2)
ON_BN_CLICKED(IDC_RADIO1, &CSaccadeSearchDlg::OnBnClickedRadio1)
ON_BN_CLICKED(IDC_RADIO3, &CSaccadeSearchDlg::OnBnClickedRadio3)
ON_BN_CLICKED(IDC_BUTTON9, &CSaccadeSearchDlg::OnBnClickedButton9)
ON_BN_CLICKED(IDC_BUTTON11, &CSaccadeSearchDlg::OnBnClickedButton11)
ON_BN_CLICKED(IDC_BUTTON8, &CSaccadeSearchDlg::OnBnClickedButton8)
//ON_BN_CLICKED(IDC_BUTTON10, &CSaccadeSearchDlg::OnBnClickedButton10)
ON_BN_CLICKED(IDC_BUTTON12, &CSaccadeSearchDlg::OnBnClickedButton12)
ON_BN_CLICKED(IDC_BUTTON13, &CSaccadeSearchDlg::OnBnClickedButton13)
ON_BN_CLICKED(IDC_BUTTON7, &CSaccadeSearchDlg::OnBnClickedButton7)
ON_BN_CLICKED(IDC_BUTTON14, &CSaccadeSearchDlg::OnBnClickedButton14)
ON_BN_CLICKED(IDC_BUTTON1, &CSaccadeSearchDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON6, &CSaccadeSearchDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CSaccadeSearchDlg message handlers

BOOL CSaccadeSearchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//this->OnBnClickedButton7();//load data

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CSaccadeSearchDlg::RecreateEeGraph()
{
  if(Win!=NULL)
  {
    Eegraph* Win2=new Eegraph();
    Win2->grid=Win->grid;
    Win2->MouseAction=Win->MouseAction;
    Win2->points=Win->points;
    Win2->RulerPointX=Win->RulerPointX;
    Win2->RulerPointY=Win->RulerPointY;
    delete(Win);
    Win=Win2;
  }
  else
  {
    delete(Win);
    Win=new Eegraph();
  }
	RECT DRect;
	RECT WRect;
	RECT ORect;
	GetClientRect(&DRect);			//Получить координаты клиентской части диалога
	//log.GetWindowRect(&ORect);	//Получить координаты кнопки
	ScreenToClient(&ORect);			//Преобразовать координаты кнопки из экранных в клиентские
	WRect.top=DRect.top+280;			//Задать координаты окна
	WRect.left=DRect.left+120;
  WRect.right =DRect.right-5;
	WRect.bottom=DRect.bottom-5;

	Win->Create(NULL,NULL,//	WS_CAPTION |//WS_THICKFRAME |//WS_SYSMENU |
    WS_THICKFRAME|WS_BORDER |WS_VISIBLE, WRect, this,0);
  
}


void CSaccadeSearchDlg::OnSysCommand(UINT nID, LPARAM lParam)
{

	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSaccadeSearchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
  if(this->init==0)
  {
    this->init=1;
	  this->OnBnClickedButton7();//load data
    this->init=2;
  }
  else if(this->init==2)
    Win->WinPaint();
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSaccadeSearchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//void CSaccadeSearchDlg::OnBnClickedButton1()
//{
//  Win->changed=1;
//  Win->WinPaint();
//}

void CSaccadeSearchDlg::OnCbnSelchangeCombo1()
{
  Win->hold();
  Conan->CurChannel=chan.GetCurSel();
  this->AutoSetXYOffset();
  Win->changed=1;
  Win->unhold();
  //Win->WinPaint();
}

void CSaccadeSearchDlg::OnEnChangeEdit2()
{
  ChangeParam();
}
void CSaccadeSearchDlg::ChangeParam()
{
  CString tmp;
  float sc;

  xscale.GetWindowTextA(tmp);
  sc=atof(tmp.GetBuffer());
  if(sc!=0)
    Conan->ZoomX=sc;

  yscale.GetWindowTextA(tmp);
  sc=atof(tmp.GetBuffer());
  if(sc!=0)
    Conan->ZoomY=sc;

  yoffset.GetWindowTextA(tmp);
  sc=atof(tmp.GetBuffer());
  Conan->YOffset=-sc;

  xoffset.GetWindowTextA(tmp);
  sc=atof(tmp.GetBuffer());
  Conan->XOffset=-sc;

  Win->changed=1;
  Win->WinPaint();
}
void CSaccadeSearchDlg::OnEnChangeEdit3()
{
  ChangeParam();
}

void CSaccadeSearchDlg::OnEnUpdateEdit6()
{
  ChangeParam();
}

void CSaccadeSearchDlg::OnEnUpdateEdit5()
{
  ChangeParam();
}

void CSaccadeSearchDlg::OnSize(UINT nType, int cx, int cy)
{
  CDialog::OnSize(nType, cx, cy);
  if(Conan==NULL)
    return;
  RecreateEeGraph();
  Win->changed=1;
}

void CSaccadeSearchDlg::OnCbnSelchangeCombo2()
{
  
  Win->hold();
  Conan->CurRec=rec.GetCurSel();
  this->AutoSetXYOffset();
  Win->changed=1;
  Win->unhold();
  //Win->WinPaint();
}

void CSaccadeSearchDlg::OnBnClickedCheck1()
{
  this->AutoSetXYOffset();
}

void CSaccadeSearchDlg::AutoSetXYOffset()
{
  if(!AutoDefineYOffset.GetCheck())
    return;

  //Win->hold();
  int ichan=Conan->CurChannel;
  int irec=Conan->CurRec;
  float y,x;
  float miny=Conan->Eeg[irec][ichan][0];
  float w=Win->RealGraphWidth();
  for(int n=0;n<Conan->NDataReal[irec];n++)
  {
    y=Conan->Eeg[irec][ichan][n];
    x=Win->XToRealCoordsFromPoint(n);
    if(x>w)
      break;
    if(y<miny)
      miny=y;
  }
  miny-=Win->ScreenGraphHeight()/3.0;
  CString tmp;
  tmp.Format("%f",miny);
  yoffset.SetWindowTextA(tmp);
  tmp="0";
  xoffset.SetWindowTextA(tmp);
  //Win->unhold();
}

void CSaccadeSearchDlg::OnBnClickedButton2()
{
  Move(-1,0);
}
void CSaccadeSearchDlg::OnBnClickedButton3()
{
  Move(0,1);
}

void CSaccadeSearchDlg::OnBnClickedButton4()
{
  Move(1,0);
}

void CSaccadeSearchDlg::OnBnClickedButton5()
{
  Move(0,-1);
}

void CSaccadeSearchDlg::Move(int x,int y)
{
  CString tmp;
  float tmp2;
  Conan->XOffset-=Win->RealGraphWidth()/10.0*x;
  tmp2=Conan->YOffset-Win->RealGraphHeight()/10.0*y;
  //Win->changed=1;
  //Win->WinPaint();
  Win->hold();
  tmp.Format("%f",-Conan->XOffset);
  xoffset.SetWindowTextA(tmp);
  tmp.Format("%f",-tmp2);
  yoffset.SetWindowTextA(tmp);
  Win->unhold();
}
void CSaccadeSearchDlg::OnBnClickedCheck2()
{
    Win->grid=GridEnabled.GetCheck();
    Win->changed=1;
    Win->WinPaint();
}

void CSaccadeSearchDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
  // TODO: Add your message handler code here and/or call default

  CDialog::OnNcLButtonDown(nHitTest, point);
}

void CSaccadeSearchDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
  Win->hold();
  if(nFlags==666)//zoom IN message from graph control
  {
    //make it into real coords
    float x=Win->XToRealCoords(point.x);
    float y=Win->YToRealCoords(point.y);
    
    //to 2x zoom, we need to
    float NewXOffset=x-Win->RealGraphWidth()/4.0;
    float NewYOffset=y-Win->RealGraphHeight()/4.0;
    CString tmp;
    tmp.Format("%f",NewXOffset);
    xoffset.SetWindowTextA(tmp);
    tmp.Format("%f",NewYOffset);
    yoffset.SetWindowTextA(tmp);


    xscale.GetWindowTextA(tmp);
    float tmpf=atof(tmp.GetBuffer())*2;
    tmp.Format("%f",tmpf);
    xscale.SetWindowTextA(tmp);

    
    yscale.GetWindowTextA(tmp);
    tmpf=atof(tmp.GetBuffer())*2;
    tmp.Format("%f",tmpf);
    yscale.SetWindowTextA(tmp);
  }
  Win->unhold();
  //Win->WinPaint();
  CDialog::OnLButtonDown(nFlags, point);
}

void CSaccadeSearchDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
  
  Win->hold();
  if(nFlags==666)//zoom OUT message from graph control
  {
    //make it into real coords
    float x=Win->XToRealCoords(point.x);
    float y=Win->YToRealCoords(point.y);
    
    //to 2x zoom, we need to
    float NewXOffset=x-Win->RealGraphWidth();
    float NewYOffset=y-Win->RealGraphHeight();
    CString tmp;
    tmp.Format("%f",NewXOffset);
    xoffset.SetWindowTextA(tmp);
    tmp.Format("%f",NewYOffset);
    yoffset.SetWindowTextA(tmp);


    xscale.GetWindowTextA(tmp);
    float tmpf=atof(tmp.GetBuffer())/2.0;
    tmp.Format("%f",tmpf);
    xscale.SetWindowTextA(tmp);

    
    yscale.GetWindowTextA(tmp);
    tmpf=atof(tmp.GetBuffer())/2.0;
    tmp.Format("%f",tmpf);
    yscale.SetWindowTextA(tmp);
  }
  else if(nFlags==667)//move message
  {
    float xmove=(float)point.x/Conan->ZoomX-Conan->XOffset;
    float ymove=-(float)point.y/Conan->ZoomY-Conan->YOffset;
    CString tmp;
    tmp.Format("%f",xmove);
    xoffset.SetWindowTextA(tmp);
    tmp.Format("%f",ymove);
    yoffset.SetWindowTextA(tmp);
  }

  else if(nFlags==668)
  {
    CString tmp;
    tmp.Format("%f",Win->RulerPointX);
    rulerx.SetWindowTextA(tmp);
    tmp.Format("%f",Win->RulerPointY);
    rulery.SetWindowTextA(tmp);
  }
  Win->unhold();
  //Win->WinPaint();
  CDialog::OnRButtonDown(nFlags, point);
}

void CSaccadeSearchDlg::OnBnClickedCheck3()
{
    Win->points=showpoints.GetCheck();
    Win->changed=1;
    Win->WinPaint();
}

void CSaccadeSearchDlg::OnBnClickedOk()
{
  //OnOK();
}

void CSaccadeSearchDlg::OnBnClickedCancel()
{
  //OnCancel();
}

void CSaccadeSearchDlg::OnClose()
{
  OnCancel();
}

void CSaccadeSearchDlg::OnCbnSelchangeCombo3()
{
}


void CSaccadeSearchDlg::OnBnClickedRadio2()
{
  mouse_move.SetCheck(1);
  mouse_zoom.SetCheck(0);
  mouse_ruler.SetCheck(0);
  Win->MouseAction=0;
}

void CSaccadeSearchDlg::OnBnClickedRadio1()
{
  mouse_move.SetCheck(0);
  mouse_zoom.SetCheck(1);
  mouse_ruler.SetCheck(0);
  Win->MouseAction=1;
}

void CSaccadeSearchDlg::OnBnClickedRadio3()
{
  mouse_move.SetCheck(0);
  mouse_zoom.SetCheck(0);
  mouse_ruler.SetCheck(1);
  Win->MouseAction=2;
}

void CSaccadeSearchDlg::OutputSaccades()
{
  CString tmp;
  tmp.Format("%d/%d",this->CurSaccade+1,Conan->Saccades.size());
  CurSaccadeT.SetWindowTextA(tmp);
  if(Conan->Saccades.size()==0)
    return;
  Saccade* s=Conan->Saccades.at(this->CurSaccade);
  tmp.Format("%d",s->chan+1);
  this->SacChanT.SetWindowTextA(tmp);
  tmp.Format("%d",s->rec+1);
  this->SacRecT.SetWindowTextA(tmp);
  tmp.Format("%1.1f",s->BeginX);
  this->SacXBeginT.SetWindowTextA(tmp);
  tmp.Format("%1.4f",s->BeginY);
  this->SacYBeginT.SetWindowTextA(tmp);
  tmp.Format("%1.1f",s->EndX);
  this->SacXEndT.SetWindowTextA(tmp);
  tmp.Format("%1.4f",s->EndY);
  this->SacYEndT.SetWindowTextA(tmp);


  tmp.Format("%1.4f",s->AmplitudeY());
  this->SacAmplitudeT.SetWindowTextA(tmp);
  tmp.Format("%x",s->StimulCode);
  this->SacStimulCodeT.SetWindowTextA(tmp);
  tmp.Format("%1.1f",s->LatentTime);
  this->SacLatentTimeT.SetWindowTextA(tmp);
  Win->hold();
  Conan->CurChannel=s->chan;
  Conan->CurRec=s->rec;
  chan.SetCurSel(s->chan);
  rec.SetCurSel(s->rec);
  Win->unhold();
  FocusToSaccade();
}

void CSaccadeSearchDlg::OnBnClickedButton9()
{
  if((ClearOther.GetCheck())&&(Conan->Saccades.size()!=0))
  {
    int c=MessageBox("Are you sure that you want to perform a new search? All currently found values will be lost!","Confirmation",MB_OKCANCEL);
    if(c==IDCANCEL)
      return;
  }
  SaccadeVec Saccades;

  //get search params
  float MinYSpeed,MinSpeedPoints,MinYLength,MinXLength,MinTimeOffset;
  CString StimulCodes,tmp;

  MinYSpeedT.GetWindowTextA(tmp);
  MinYSpeed=atof(tmp);
  MinSpeedPointsT.GetWindowTextA(tmp);
  MinSpeedPoints=atof(tmp);
  MinYLengthT.GetWindowTextA(tmp);
  MinYLength=atof(tmp);
  MinXLengthT.GetWindowTextA(tmp);
  MinXLength=atof(tmp);
  MinTimeOffsetT.GetWindowTextA(tmp);
  MinTimeOffset=atof(tmp);

  StimulCodesT.GetWindowTextA(StimulCodes);

  //init search vars
  int YPoints=0;
  float x=0;
  float y=0;
  int rec=Conan->CurRec;
  int chan=Conan->CurChannel;
  float y1;
  bool sign;
  int method=0;
  if(method==0)//simple search
  {
    if(Conan->Eeg[rec][chan][1]-Conan->Eeg[rec][chan][0]>0)
      sign=1;
    else
      sign=0;
    for(int n=0;n<Conan->NDataReal[rec]-1;n++)
    {
        x=Win->XToRealCoordsFromPoint(n);
        if(x<MinTimeOffset)//not still time...
          continue;
        y=Conan->Eeg[rec][chan][n];
        y1=Conan->Eeg[rec][chan][n+1];
        bool sign2;//check for speed sign
        if(y1-y>0)
          sign2=1;
        else
          sign2=0;
        if(abs(y1-y)>=MinYSpeed)//probably, saccade point found
        {
          if(sign2!=sign)
            YPoints=1;//the first speed point after sign change
          else
            YPoints++;
        }
        else
          YPoints=0;
        sign=sign2;
        if(YPoints>=MinSpeedPoints)
        {
          //probably, saccade found :)
          //going to it's last point
          bool sign2=sign;
          while((abs(y1-y)>=MinYSpeed)&&(sign2==sign))
          {
            n++;
            YPoints++;
            y=Conan->Eeg[rec][chan][n];
            y1=Conan->Eeg[rec][chan][n+1];
            if(y1-y>0)
              sign2=1;
            else
              sign2=0;
          }
          n--;//last point was passed in extra circle
          YPoints--;
          sign=sign2;
          //y=Conan->Eeg[rec][chan][n];
          //y1=Conan->Eeg[rec][chan][n+1];
          
          Saccade* sac=new Saccade();
          sac->chan=Conan->CurChannel;
          sac->rec=Conan->CurRec;
          sac->Points=YPoints;
          sac->EndX=Win->XToRealCoordsFromPoint(n+1);
          sac->EndY=Conan->Eeg[rec][chan][n+1];
          //sac->Sign=sign;
          sac->BeginX=Win->XToRealCoordsFromPoint(n+1-YPoints);
          sac->BeginY=Conan->Eeg[rec][chan][n+1-YPoints];
          
          if((sac->AmplitudeX()<MinXLength)||(abs(sac->AmplitudeY())<MinYLength))
          {
            delete(sac);
          }
          else//saccade is fine too
          {
            Saccades.push_back(sac);
          }
          YPoints=0;
        }
        //x+=(float)Conan->Header->freq/1000;
    }
  }

  if(ClearOther.GetCheck())
  {
    this->CurSaccade=0;
    Conan->ResetSaccades();
  }
  int s=Saccades.size();
  for(int i=0;i<s;i++)//found some, woohoo. copy it to conan
  {
    Conan->Saccades.push_back(Saccades.at(i));
  }
  this->CurSaccade=Conan->Saccades.size()-s;//the first from the new
  FocusToSaccade();
  OutputSaccades();
}

void CSaccadeSearchDlg::OnBnClickedButton11()
{
  //next saccade
  int z=Conan->Saccades.size();
  if(z==0)
    return;
  if(this->CurSaccade+2>z)
    this->CurSaccade=0;
  else
    this->CurSaccade++;
  OutputSaccades();
  Conan->CurChannel=Conan->Saccades.at(this->CurSaccade)->chan;
  Conan->CurRec=Conan->Saccades.at(this->CurSaccade)->rec;
  FocusToSaccade();
}

void CSaccadeSearchDlg::OnBnClickedButton8()
{
  //prev saccade
  int z=Conan->Saccades.size();
  if(z==0)
    return;

  if(this->CurSaccade==0)
    this->CurSaccade=z-1;
  else
    this->CurSaccade--;
  OutputSaccades();
  Conan->CurChannel=Conan->Saccades.at(this->CurSaccade)->chan;
  Conan->CurRec=Conan->Saccades.at(this->CurSaccade)->rec;
  FocusToSaccade();
}

//void CSaccadeSearchDlg::OnBnClickedButton10()
//{
//  Win->changed=1;
//  Win->WinPaint();
//}
void CSaccadeSearchDlg::FocusToSaccade()
{
  if(Conan->Saccades.size()==0)
    return;
  float x=Conan->Saccades.at(this->CurSaccade)->BeginX-Win->RealGraphWidth()/2;
  float y=Conan->Saccades.at(this->CurSaccade)->BeginY-Win->RealGraphHeight()/2;
  CString tmp;
  Win->hold();
  tmp.Format("%f",x);
  xoffset.SetWindowTextA(tmp);
  tmp.Format("%f",y);
  yoffset.SetWindowTextA(tmp);
  Win->unhold();
}

void CSaccadeSearchDlg::OnBnClickedButton12()
{
  //save saccade
  if(Conan->Saccades.size()==0)
  {
    Saccade* s=new Saccade();
    Conan->Saccades.push_back(s);
    this->CurSaccade=0;
  }
  CString tmp;
  Saccade* s=Conan->Saccades.at(this->CurSaccade);
  this->SacChanT.GetWindowTextA(tmp);
  s->chan=atoi(tmp)-1;
  if(s->chan<0)
    s->chan=0;
  this->SacRecT.GetWindowTextA(tmp);
  s->rec=atoi(tmp)-1;
  if(s->rec<0)
    s->rec=0;
  this->SacXBeginT.GetWindowTextA(tmp);
  s->BeginX=atof(tmp);
  this->SacYBeginT.GetWindowTextA(tmp);
  s->BeginY=atof(tmp);
  this->SacXEndT.GetWindowTextA(tmp);
  s->EndX=atof(tmp);
  this->SacYEndT.GetWindowTextA(tmp);
  s->EndY=atof(tmp);


  tmp.Format("%1.4f",s->AmplitudeY());
  this->SacAmplitudeT.SetWindowTextA(tmp);
  this->SacStimulCodeT.GetWindowTextA(tmp);
  s->StimulCode=atoi(tmp);
  this->SacLatentTimeT.GetWindowTextA(tmp);
  s->LatentTime=atof(tmp);
  OutputSaccades();
}

void CSaccadeSearchDlg::OnBnClickedButton13()
{
  Saccade* s=new Saccade();
  s->chan=Conan->CurChannel;
  s->rec=Conan->CurRec;
  Conan->Saccades.push_back(s);
  this->CurSaccade=Conan->Saccades.size()-1;
  OutputSaccades();
}

void CSaccadeSearchDlg::OnBnClickedButton7()
{
  //this->DoModal();
  CString path;
  CFileDialog dlg(TRUE/*Open=TRUE Save=False*/,"cad"/*Filename Extension*/,""/*Initial Filename*/,OFN_ENABLESIZING|OFN_EXPLORER|OFN_FILEMUSTEXIST/*Flags*/,"Conan File Format(*.cad)|*.cad||"/*Filetype Filter*/,this/*parent Window*/);
	int z=dlg.DoModal();
  if (z == IDOK)
		path=dlg.GetPathName();
  else
    this->OnCancel();//exit program
  if(z!=IDOK)
    return;

  if(Conan!=NULL)
    delete(Conan);
  Conan=ReadConanFile(path.GetBuffer(),&log);
  if(Conan==NULL)
    return;
  CString chnum,chname,channels;
  channels.Append(Conan->Header->chNames);
  //int pos=0;
  //int pos2=0;
  for(int i=0;i<Conan->Header->nChan;i++)
  {
    //pos2=channels.Find(" ",pos);
    chname=channels.Mid(i*4,4);
    chnum.Format("%d ",i+1);
    chnum.Append(chname);
    chan.AddString(chnum.GetBuffer());
    //pos=pos2+1;
  }
  SetDropDownHeight(&chan,Conan->Header->nChan);
  chan.SetCurSel(0);

  CString RecNum;
  for(int i=0;i<Conan->Header->nRec;i++)
  {
    RecNum.Format("%d ",i+1);
    rec.AddString(RecNum.GetBuffer());
    //pos=pos2+1;
  }
  SetDropDownHeight(&rec,20);
  rec.SetCurSel(0);



  CString tmp;
  tmp.Format("%d",Conan->Header->freq);
  xfreq.SetWindowTextA(tmp);


	this->RecreateEeGraph();	
  Win->hold();
  xscale.SetWindowTextA("10");
  yscale.SetWindowTextA("1");
  xoffset.SetWindowTextA("0");
  yoffset.SetWindowTextA("0");

  AutoDefineYOffset.SetCheck(1);
  OnBnClickedRadio2();
  Win->unhold();
  this->AutoSetXYOffset();


  
  MinYSpeedT.SetWindowTextA("1");
  MinSpeedPointsT.SetWindowTextA("1");
  MinYLengthT.SetWindowTextA("1");
  MinXLengthT.SetWindowTextA("1");
  MinTimeOffsetT.SetWindowTextA("0");
}

void CSaccadeSearchDlg::OnBnClickedButton14()
{
  if(Conan->Saccades.size()==0)//nothing to erase
    return;
  delete(Conan->Saccades.at(this->CurSaccade));
  Conan->Saccades.erase(Conan->Saccades.begin()+this->CurSaccade);
  if(this->CurSaccade>0)
    this->CurSaccade--;
  else if(this->CurSaccade==0 && Conan->Saccades.size()>0)
    ;//CurSaccade++;
  else if(Conan->Saccades.size()==0)
  {
    this->SacChanT.SetWindowTextA("");
    this->SacRecT.SetWindowTextA("");
    this->SacXBeginT.SetWindowTextA("");
    this->SacYBeginT.SetWindowTextA("");
    this->SacXEndT.SetWindowTextA("");
    this->SacYEndT.SetWindowTextA("");
    this->SacAmplitudeT.SetWindowTextA("");
    this->SacStimulCodeT.SetWindowTextA("");
    this->SacLatentTimeT.SetWindowTextA("");
  }
  OutputSaccades();
  // TODO: Add your control notification handler code here
}

void CSaccadeSearchDlg::OnBnClickedButton1()
{
  CString save,tmp;
  for(int i=0;i<Conan->Saccades.size();i++)
  {
    Saccade* s=Conan->Saccades.at(i);
    tmp.Format("%d",s->chan+1);
    save.Append(tmp);
    save.Append(" ");
    tmp.Format("%d",s->rec+1);
    save.Append(tmp);
    save.Append(" ");
    tmp.Format("%1.1f",s->BeginX);
    save.Append(tmp);
    save.Append(" ");
    tmp.Format("%1.4f",s->BeginY);
    save.Append(tmp);
    save.Append(" ");
    tmp.Format("%1.1f",s->EndX);
    save.Append(tmp);
    save.Append(" ");
    tmp.Format("%1.4f",s->EndY);
    save.Append(tmp);
    save.Append(" ");
    tmp.Format("%1.4f",s->AmplitudeY());
    save.Append(tmp);
    save.Append(" ");
    tmp.Format("%x",s->StimulCode);
    save.Append(tmp);
    save.Append(" ");
    tmp.Format("%1.1f",s->LatentTime);
    save.Append(tmp);
    save.Append("\n");
  }
  CString path;
  CFileDialog dlg(FALSE/*Open=TRUE Save=False*/,"sac"/*Filename Extension*/,""/*Initial Filename*/,OFN_ENABLESIZING|OFN_EXPLORER/*Flags*/,"Saccade file(*.sac)|*.sac||"/*Filetype Filter*/,this/*parent Window*/);
	int z=dlg.DoModal();
  if (z == IDOK)
  {
		path=dlg.GetPathName();
    FILE * pFile;
    fopen_s(&pFile, path, "w");
    fwrite(save,1,save.GetLength(),pFile);
    fclose(pFile);
  }
}

void CSaccadeSearchDlg::OnBnClickedButton6()
{
  bool erase=0;
  if(Conan->Saccades.size())
  {
    int c=MessageBox("Do you want to erase currently found values?","Confirmation",MB_YESNO);
    if(c==IDYES)
      erase=1;
  }
  CString path;
  CFileDialog dlg(TRUE/*Open=TRUE Save=False*/,"sac"/*Filename Extension*/,""/*Initial Filename*/,OFN_ENABLESIZING|OFN_EXPLORER|OFN_FILEMUSTEXIST/*Flags*/,"Saccade file(*.sac)|*.sac||"/*Filetype Filter*/,this/*parent Window*/);
	int z=dlg.DoModal();
  if (z == IDOK)
  {
		path=dlg.GetPathName();
    FILE * pFile;
    long lSize;
    char * buffer;
    size_t result;
    fopen_s(&pFile, path.GetBuffer(), "r");
    if (pFile==NULL)
    {
	    MessageBox("Не могу прочитать файл!","Ошибка",MB_OK);
	    return;
    }
    //erase
    if(erase)
    {
      this->CurSaccade=0;
      Conan->ResetSaccades();
    }

    int r;
    float amp;
    while(true)
    {
      Saccade* s=new Saccade();
      r=fscanf(pFile,"%d %d %f %f %f %f %f %x %f\n",&s->chan,&s->rec,&s->BeginX,&s->BeginY,&s->EndX,&s->EndY,&amp,&s->StimulCode,&s->LatentTime);
      if(r==EOF)
      {
        delete(s);
        break;
      }
      s->chan--;
      s->rec--;
      Conan->Saccades.push_back(s);
    }
    fclose(pFile);
    OutputSaccades();
  }
      return;
}
