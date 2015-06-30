// SaccadeSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SaccadeSearch.h"
#include "SaccadeSearchDlg.h"
#include "funcs.h"
#include "ConanData.h"
#include "Eegraph.h"
//#include "Saccade.h"
#include "windows.h"
//#include <vector>


//using namespace std;
typedef std::vector <Saccade*> SaccadeVec;
typedef std::vector <__int8> StimulVec;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


  int MinTimeBeforeStimul=0;//must be defined later
  int MaxTimeAfterStimul=1500;

 Eegraph* Win;
 ConanData* Conan;
 CSaccadeSearchDlg* dialog;
 StimulVec StimulNegative;
 StimulVec StimulPositive;
 bool FirstLoad=true;
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
	DDX_Control(pDX, IDC_EDIT15, NegativeStimulCodesT);
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
	DDX_Control(pDX, IDC_EDIT24, SacTimeFromCalT);
	DDX_Control(pDX, IDC_EDIT25, SacTimeFromStimulT);
	DDX_Control(pDX, IDC_EDIT26, ApproxPrec);
	DDX_Control(pDX, IDC_EDIT27, AprIterationsT);
	DDX_Control(pDX, IDC_EDIT28, MinExtremPointsT);
	DDX_Control(pDX, IDC_CHECK5, UseApproximation);
	DDX_Control(pDX, IDC_CHECK6, PreprocessAllRecords);
	DDX_Control(pDX, IDC_COMBO3, SearchMethod);
	DDX_Control(pDX, IDC_EDIT29, PositiveStimulCodesT);
	DDX_Control(pDX, IDC_CHECK7, CreateVirtualSaccades);
	DDX_Control(pDX, IDC_EDIT30, AllSaccades);
	DDX_Control(pDX, IDC_CHECK8, SearchForCal);
	DDX_Control(pDX, IDC_EDIT31, OpenedFile);
	DDX_Control(pDX, IDC_EDIT32, Notice);
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
ON_EN_CHANGE(IDC_EDIT26, &CSaccadeSearchDlg::OnEnChangeEdit26)
ON_EN_CHANGE(IDC_EDIT27, &CSaccadeSearchDlg::OnEnChangeEdit27)
ON_EN_CHANGE(IDC_EDIT28, &CSaccadeSearchDlg::OnEnChangeEdit28)
ON_BN_CLICKED(IDC_CHECK5, &CSaccadeSearchDlg::OnBnClickedCheck5)
ON_EN_CHANGE(IDC_EDIT7, &CSaccadeSearchDlg::OnEnChangeEdit7)
ON_EN_CHANGE(IDC_EDIT29, &CSaccadeSearchDlg::OnEnChangeEdit29)
ON_EN_CHANGE(IDC_EDIT15, &CSaccadeSearchDlg::OnEnChangeEdit15)
ON_EN_CHANGE(IDC_EDIT8, &CSaccadeSearchDlg::OnEnChangeEdit8)
ON_EN_CHANGE(IDC_EDIT18, &CSaccadeSearchDlg::OnEnChangeEdit18)
ON_EN_CHANGE(IDC_EDIT20, &CSaccadeSearchDlg::OnEnChangeEdit20)
ON_BN_CLICKED(IDC_BUTTON10, &CSaccadeSearchDlg::OnBnClickedButton10)
ON_BN_CLICKED(IDC_BUTTON15, &CSaccadeSearchDlg::OnBnClickedButton15)
ON_BN_CLICKED(IDC_BUTTON17, &CSaccadeSearchDlg::OnBnClickedButton17)
ON_BN_CLICKED(IDC_BUTTON16, &CSaccadeSearchDlg::OnBnClickedButton16)
ON_BN_CLICKED(IDC_BUTTON18, &CSaccadeSearchDlg::OnBnClickedButton18)
//ON_BN_CLICKED(IDC_CHECK7, &CSaccadeSearchDlg::OnBnClickedCheck7)
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

	SearchMethod.AddString(_T("free search"));
	SearchMethod.AddString(_T("Stimul based"));
	SearchMethod.AddString(_T("2x Stimul based"));
  SetDropDownHeight(&SearchMethod,3);
  SearchMethod.SetCurSel(0);
  OnCbnSelchangeCombo3();

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
    Win2->CurSaccade=Win->CurSaccade;
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
	WRect.top=DRect.top+370;			//Задать координаты окна
	WRect.left=DRect.left+140;
  WRect.right =DRect.right-5;
	WRect.bottom=DRect.bottom-5;

	Win->Create(NULL,NULL,//	WS_CAPTION |//WS_THICKFRAME |//WS_SYSMENU |
    WS_BORDER |WS_VISIBLE, WRect, this,0);
  
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


void CSaccadeSearchDlg::OnCbnSelchangeCombo1()
{
  Win->hold();
  Conan->CurChannel=chan.GetCurSel();
  this->AutoSetXYOffset();
  Approximate(Conan->CurChannel,Conan->CurRec);
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

  xscale.GetWindowText(tmp);
  sc=_wtof(tmp.GetBuffer());
  if(sc!=0)
    Conan->ZoomX=sc;

  yscale.GetWindowText(tmp);
  sc=_wtof(tmp.GetBuffer());
  if(sc!=0)
    Conan->ZoomY=sc;

  yoffset.GetWindowText(tmp);
  sc=_wtof(tmp.GetBuffer());
  Conan->YOffset=-sc;

  xoffset.GetWindowText(tmp);
  sc=_wtof(tmp.GetBuffer());
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
  Approximate(Conan->CurChannel,Conan->CurRec);
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

  Win->hold();
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
  miny-=Win->RealGraphHeight()/2.0;
  CString tmp;
  tmp.Format(_T("%f"), miny);
  yoffset.SetWindowText(tmp);
  tmp="0";
  xoffset.SetWindowText(tmp);
  Win->unhold();
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
  tmp.Format( _T("%f"), -Conan->XOffset);
  xoffset.SetWindowText(tmp);
  tmp.Format(_T("%f"), -tmp2);
  yoffset.SetWindowText(tmp);
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
	tmp.Format(_T("%f"), NewXOffset);
    xoffset.SetWindowText(tmp);
	tmp.Format(_T("%f"), NewYOffset);
    yoffset.SetWindowText(tmp);


    xscale.GetWindowText(tmp);
    float tmpf=_wtof(tmp.GetBuffer())*2;
	tmp.Format(_T("%f"), tmpf);
    xscale.SetWindowText(tmp);

    
    yscale.GetWindowText(tmp);
    tmpf=_wtof(tmp.GetBuffer())*2;
	tmp.Format(_T("%f"), tmpf);
    yscale.SetWindowText(tmp);
    Win->changed=1;
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
	tmp.Format(_T("%f"), NewXOffset);
    xoffset.SetWindowText(tmp);
	tmp.Format(_T("%f"), NewYOffset);
    yoffset.SetWindowText(tmp);


    xscale.GetWindowText(tmp);
    float tmpf=_wtof(tmp.GetBuffer())/2.0;
	tmp.Format(_T("%f"), tmpf);
    xscale.SetWindowText(tmp);

    
    yscale.GetWindowText(tmp);
    tmpf=_wtof(tmp.GetBuffer())/2.0;
	tmp.Format(_T("%f"), tmpf);
    yscale.SetWindowText(tmp);
  }
  else if(nFlags==667)//move message
  {
    float xmove=(float)point.x/Conan->ZoomX-Conan->XOffset;
    float ymove=-(float)point.y/Conan->ZoomY-Conan->YOffset;
    CString tmp;
	tmp.Format(_T("%f"), xmove);
    xoffset.SetWindowText(tmp);
	tmp.Format(_T("%f"), ymove);
    yoffset.SetWindowText(tmp);
  }

  else if(nFlags==668)
  {
    CString tmp;
	tmp.Format(_T("%1.0f"), Win->RulerPointX);
    rulerx.SetWindowText(tmp);
    //tmp.Format("%f",Win->RulerPointY);
    //rulery.SetWindowText(tmp);
  }
  Win->changed=1;
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
  int z=SearchMethod.GetCurSel();
  if(z==0)//simple search
  {
    PositiveStimulCodesT.EnableWindow(0);
    NegativeStimulCodesT.EnableWindow(0);
    CreateVirtualSaccades.EnableWindow(0);
    SearchForCal.EnableWindow(0);
  }
  else if(z==1)
  {
    PositiveStimulCodesT.EnableWindow(1);
    NegativeStimulCodesT.EnableWindow(1);
    CreateVirtualSaccades.EnableWindow(1);
    SearchForCal.EnableWindow(1);
  }
  else if(z==2)
  {
    PositiveStimulCodesT.EnableWindow(1);
    NegativeStimulCodesT.EnableWindow(1);
    CreateVirtualSaccades.EnableWindow(1);
    SearchForCal.EnableWindow(1);
  }
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
  CurSaccadeT.GetWindowText(tmp);
  if(_wtoi(tmp)!=Win->CurSaccade+1)
  {
	  tmp.Format(_T("%d"), Win->CurSaccade + 1);
    CurSaccadeT.SetWindowText(tmp);
  }
  tmp.Format(_T("%d"), Conan->Saccades.size());
  AllSaccades.SetWindowText(tmp);  
  if(Conan->Saccades.size()==0)
  {
  this->SacChanT.Clear();
  this->SacRecT.Clear();
  this->SacXBeginT.Clear();
  this->SacYBeginT.Clear();
  this->SacXEndT.Clear();
  this->SacYEndT.Clear();
  this->SacAmplitudeT.Clear();
  this->SacStimulCodeT.Clear();
  this->SacTimeFromCalT.Clear();
  this->SacTimeFromStimulT.Clear();
  this->Notice.Clear();
    return;
  }
  Saccade* s=Conan->Saccades.at(Win->CurSaccade);
  tmp.Format(_T("%d"), s->chan + 1);
  this->SacChanT.SetWindowText(tmp);
  tmp.Format(_T("%d"), s->rec + 1);
  this->SacRecT.SetWindowText(tmp);
  tmp.Format(_T("%1.0f"), s->BeginX);
  this->SacXBeginT.SetWindowText(tmp);
  tmp.Format(_T("%1.4f"), s->BeginY);
  this->SacYBeginT.SetWindowText(tmp);
  tmp.Format(_T("%1.0f"), s->EndX);
  this->SacXEndT.SetWindowText(tmp);
  tmp.Format(_T("%1.4f"), s->EndY);
  this->SacYEndT.SetWindowText(tmp);
  tmp.Format(_T("%1.4f"), s->AmplitudeY());
  this->SacAmplitudeT.SetWindowText(tmp);
  tmp.Format(_T("%x"), s->StimulCode);
  this->SacStimulCodeT.SetWindowText(tmp);
  tmp.Format(_T("%1.0f"), s->TimeFromCal);
  this->SacTimeFromCalT.SetWindowText(tmp);
  tmp.Format(_T("%1.0f"), s->TimeFromStimul);
  this->SacTimeFromStimulT.SetWindowText(tmp);
  this->Notice.SetWindowText(s->Notice);

  Win->hold();
  Conan->CurChannel=s->chan;
  Conan->CurRec=s->rec;
  chan.SetCurSel(s->chan);
  rec.SetCurSel(s->rec);
  //FocusToSaccade();
  Win->unhold();
}

int CSaccadeSearchDlg::SetStimul(int rec,int n)
{
  //check for stimul value
  Stimul=Conan->Discr[rec][n].Elder;
  if(Stimul!=2 && Stimul!=LastStimulCode)
  {
    float x=Win->XToRealCoordsFromPoint(n);
    if(Stimul==128)//calibrating,80 in hex
    {
      LastCalStimulTime=x;
      return 0;
    }
    else
    {
      LastStimulTime=x;
      LastStimulCode=Stimul;
      return Stimul;
    }
  }
  else return 0;
}

int CSaccadeSearchDlg::GetStimulSign(__int8 stimul)
{
  for(int i=0;i<StimulNegative.size();i++)
    if (StimulNegative.at(i)==stimul)
      return -1;
  for(int i=0;i<StimulPositive.size();i++)
    if (StimulPositive.at(i)==stimul)
      return 1;

  
  /*CString tmp;
  tmp.Format("Stimul code '%x' not found as positive or negative!",stimul);
  MessageBox(tmp,"ERROR",MB_OK);*/
  return 0;
  
}
void CSaccadeSearchDlg::OnBnClickedButton9()
{
  if((ClearOther.GetCheck())&&(Conan->Saccades.size()!=0))
  {
    int c=MessageBox(_T("Are you sure that you want to clear all currently found results?"),_T("Confirmation"),MB_OKCANCEL);
    if(c==IDCANCEL)
      return;
  }
  SaccadeVec Saccades;
  //set stimul params
  LastStimulCode=0;
  Stimul=0;
  LastStimulTime=0;
  LastCalStimulTime=0;
  //get search params
  float MinYSpeed,MinSpeedPoints,MinYLength,MinXLength,MinTimeOffset;
  CString tmp;

  MinYSpeedT.GetWindowText(tmp);
  MinYSpeed=_wtof(tmp);
  MinSpeedPointsT.GetWindowText(tmp);
  MinSpeedPoints=_wtof(tmp);
  MinYLengthT.GetWindowText(tmp);
  MinYLength=_wtof(tmp);
  MinXLengthT.GetWindowText(tmp);
  MinXLength=_wtof(tmp);
  MinTimeOffsetT.GetWindowText(tmp);
  MinTimeOffset=_wtof(tmp);


  bool CreateVirtual=0;
  if(CreateVirtualSaccades.GetCheck())
    CreateVirtual=1;

  //init search vars
  int YPoints=0;
  float x=0;
  float y=0;
  int chan=Conan->CurChannel;
  float y1;
  bool sign;
  float* YCoords;
  float* YAppr;
  int recfrom,recto;
  
  if(PreprocessAllRecords.GetCheck())//for all records
  {
    recfrom=0;
    recto=Conan->Header->nRec-1;
  }
  else//only for selected
  {
    recfrom=Conan->CurRec;
    recto=Conan->CurRec;
  }
  int method=SearchMethod.GetCurSel();

  for(int rec=recfrom;rec<=recto;rec++)
  {
    
    if(Conan->Approximate)
    {
      YAppr=Conan->EegApproximated[rec][chan];
      YCoords=Conan->Eeg[rec][chan];
      Approximate(chan,rec);
    }
    else
    {
      YAppr=Conan->Eeg[rec][chan];
      YCoords=Conan->Eeg[rec][chan];
    }

    if(YCoords[1]-YCoords[0]>=0)//sign of the first change
      sign=1;
    else
      sign=0;

    if(method==0)//simple search
    {
      for(int n=0;n<Conan->NDataReal[rec]-1;n++)
      {
          x=Win->XToRealCoordsFromPoint(n);
          if(x<MinTimeOffset)//not still time...
            continue;
          y=YAppr[n];
          y1=YAppr[n+1];
          bool sign2;//check for speed sign
          if(y1-y>=0)
            sign2=1;
          else
            sign2=0;
          SetStimul(rec,n);
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
              SetStimul(rec,n);
              n++;
              YPoints++;
              y=YAppr[n];
              y1=YAppr[n+1];
              if(y1-y>=0)
                sign2=1;
              else
                sign2=0;
            }
            n--;//last point was passed in extra circle
            YPoints--;
            sign=sign2;
            
            Saccade* sac=new Saccade();
            sac->chan=chan;
            sac->rec=rec;
            sac->Points=YPoints;
            sac->EndX=Win->XToRealCoordsFromPoint(n+1);
            sac->EndY=YCoords[n+1];
            sac->BeginX=Win->XToRealCoordsFromPoint(n+1-YPoints);
            sac->BeginY=YCoords[n+1-YPoints];
            sac->StimulCode=LastStimulCode;
            sac->TimeFromCal=sac->BeginX-LastCalStimulTime;
            sac->TimeFromStimul=sac->BeginX-LastStimulTime;

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
      }
    }
    else//method = 2 || 3. stimul based search
    {
    
      for(int n=0;n<Conan->NDataReal[rec]-1;n++)
      {
          x=Win->XToRealCoordsFromPoint(n);
          y=YCoords[n];
          if(x<MinTimeOffset)//not still time...
            continue;
          unsigned __int8 Stimul=SetStimul(rec, n);
          if(Stimul)//new stimul
            //search for saccade, corresponding to this stimul
          {
            int step=0;// add counter
            //create "virtual" saccade

            Saccade* sac=new Saccade();
            sac->chan=chan;
            sac->rec=rec;
            sac->Points=0;
            sac->EndX=x+MaxTimeAfterStimul;
            sac->EndY=y;
            sac->BeginX=x;
            sac->BeginY=y;
            sac->StimulCode=Stimul;
            sac->TimeFromCal=0;
            sac->TimeFromStimul=0;
            sac->StimulTime=x;
            bool SacFound=false;

            if(SearchForCal.GetCheck())
            {
              //find calibration stimul
              while((n+step<Conan->NDataReal[rec]-1) && Conan->Discr[rec][n+step].Elder!=128)//80 in hex
                step++;

              //detect saccade sign
              if(Conan->Discr[rec][n+step].Elder!=128)
              {
                //calibration stimul not found
                if(CreateVirtual)//create virtual saccade
                {
                  Saccades.push_back(sac);
                  continue;
                }
                else
                {
                  delete(sac);//discard & continue
                  continue;
                }
              }
            }
            LastCalStimulTime=Win->XToRealCoordsFromPoint(n+step);
            sac->CalStimulTime=LastCalStimulTime;
            //calibration stimul ok. search for saccade.
            
            int sign=this->GetStimulSign(Stimul);
            if(sign==0)
            {
              delete(sac);
              continue;
            }

            YPoints=0;
            while(x<=this->LastCalStimulTime+MaxTimeAfterStimul)
            {
              step++;
              x=Win->XToRealCoordsFromPoint(n+step);              
              y=YAppr[n+step];
              y1=YAppr[n+step+1];
              if(x<MinTimeBeforeStimul+this->LastCalStimulTime)
                continue;//too early
              //almost same as free search

              int sign2;//check for speed sign
              if(y1-y>=0)
                sign2=1;
              else
                sign2=-1;
              if(sign!=sign2)
              {
                YPoints=0;
                continue;//wrong sign
              }
              if(abs(y1-y)>=MinYSpeed)//probably, saccade point found
              {
                  YPoints++;
              }
              else
                YPoints=0;
              if(YPoints>=MinSpeedPoints)
              {
                //probably, saccade found :)
                //going to it's last point
                while((abs(y1-y)>=MinYSpeed)&&(sign2==sign))
                {
                  step++;
                  YPoints++;
                  y=YAppr[n+step];
                  y1=YAppr[n+step+1];
                  if(y1-y>=0)
                    sign2=1;
                  else
                    sign2=-1;
                }
                step--;//last point was passed in extra circle
                YPoints--;
                
                Saccade* sacF=new Saccade();
                sacF->chan=chan;
                sacF->rec=rec;
                sacF->Points=YPoints;
                sacF->EndX=Win->XToRealCoordsFromPoint(n+step+1);
                sacF->EndY=YCoords[n+step+1];
                sacF->BeginX=Win->XToRealCoordsFromPoint(n+step+1-YPoints);
                sacF->BeginY=YCoords[n+step+1-YPoints];
                sacF->StimulCode=Stimul;
                sacF->TimeFromStimul=sacF->BeginX-LastStimulTime;
                sacF->TimeFromCal=sacF->BeginX-LastCalStimulTime;


                if((sacF->AmplitudeX()<MinXLength)||(abs(sacF->AmplitudeY())<MinYLength))
                {
                  delete(sacF);
                }
                else//saccade is fine too
                {
                  Saccades.push_back(sacF);
                  SacFound=1;
                  break;
                }
              }
            }
            if(SacFound)
              delete(sac);//we already have a kawaii saccade, no need for dummy
             else
            {
              //if we're here, it means that saccade was not found.
              //Then - insert virtual - if we need to...
              if(CreateVirtual)//create virtual saccade
                Saccades.push_back(sac);
              else
                delete(sac);//discard & continue
            }
          }
      }
    }
  }
  if(ClearOther.GetCheck())
  {
    Win->CurSaccade=0;
    Conan->ResetSaccades();
  }
  int s=Saccades.size();

  for(int i=0;i<s;i++)//found some, woohoo. copy it to conan
  {
    Conan->Saccades.push_back(Saccades.at(i));
  }
  if(s!=0)
  {
    Win->CurSaccade=Conan->Saccades.size()-s;//the first from the new
    FocusToSaccade();
    OutputSaccades();
    Win->changed=1;
    Win->WinPaint();
    
    this->SacChanT.EnableWindow(1);
    this->SacRecT.EnableWindow(1);
    this->SacXBeginT.EnableWindow(1);
    this->SacYBeginT.EnableWindow(1);
    this->SacXEndT.EnableWindow(1);
    this->SacYEndT.EnableWindow(1);
    this->SacAmplitudeT.EnableWindow(1);
    this->SacStimulCodeT.EnableWindow(1);
    this->SacTimeFromCalT.EnableWindow(1);
    this->SacTimeFromStimulT.EnableWindow(1);
  }
  else
  {//no sacades, show it
    OutputSaccades();
    Win->changed=1;
    Win->WinPaint();
  }
}

void CSaccadeSearchDlg::OnBnClickedButton11()
{
  //next saccade
  int z=Conan->Saccades.size();
  if(z==0)
    return;
  if(Win->CurSaccade+2>z)
    Win->CurSaccade=0;
  else
    Win->CurSaccade++;
  OutputSaccades();
  Conan->CurChannel=Conan->Saccades.at(Win->CurSaccade)->chan;
  Conan->CurRec=Conan->Saccades.at(Win->CurSaccade)->rec;
  FocusToSaccade();
}

void CSaccadeSearchDlg::OnBnClickedButton8()
{
  //prev saccade
  int z=Conan->Saccades.size();
  if(z==0)
    return;

  if(Win->CurSaccade==0)
    Win->CurSaccade=z-1;
  else
    Win->CurSaccade--;
  OutputSaccades();
  Conan->CurChannel=Conan->Saccades.at(Win->CurSaccade)->chan;
  Conan->CurRec=Conan->Saccades.at(Win->CurSaccade)->rec;
  FocusToSaccade();
}

//void CSaccadeSearchDlg::OnBnClickedButton10()
//{
//  Win->changed=1;
//  Win->WinPaint();
//}

const wchar_t *GetWC(const char *c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

void CSaccadeSearchDlg::FocusToSaccade()
{
  if(Conan->Saccades.size()==0)
    return;
  float x=Conan->Saccades.at(Win->CurSaccade)->BeginX-Win->RealGraphWidth()/2;
  float y=Conan->Saccades.at(Win->CurSaccade)->BeginY-Win->RealGraphHeight()/2;
  CString tmp;
  Win->hold();
  tmp.Format(_T("%f"), x);
  xoffset.SetWindowText(tmp);
  tmp.Format(_T("%f"), y);
  yoffset.SetWindowText(tmp);
  Win->changed=1;
  Win->unhold();
}

void CSaccadeSearchDlg::OnBnClickedButton12()
{
  //save saccade
  if(Conan->Saccades.size()==0)
  {
    Saccade* s=new Saccade();
    Conan->Saccades.push_back(s);
    Win->CurSaccade=0;
  }
  CString tmp;
  Saccade* s=Conan->Saccades.at(Win->CurSaccade);
  this->Notice.GetWindowText(s->Notice);
  this->SacChanT.GetWindowText(tmp);
  s->chan = _wtoi(tmp) - 1;
  if(s->chan<0)
    s->chan=0;
  this->SacRecT.GetWindowText(tmp);
  s->rec = _wtoi(tmp) - 1;
  if(s->rec<0)
    s->rec=0;
  this->SacXBeginT.GetWindowText(tmp);
  s->BeginX=_wtof(tmp);
  this->SacYBeginT.GetWindowText(tmp);
  s->BeginY=_wtof(tmp);
  this->SacXEndT.GetWindowText(tmp);
  s->EndX=_wtof(tmp);
  this->SacYEndT.GetWindowText(tmp);
  s->EndY=_wtof(tmp);


  tmp.Format(_T("%1.4f"), s->AmplitudeY());
  this->SacAmplitudeT.SetWindowText(tmp);

  this->SacStimulCodeT.GetWindowText(tmp);
  //s->StimulCode=_wtoi(tmp);
  swscanf_s(tmp, _T("%x"), &s->StimulCode);
  this->SacTimeFromCalT.GetWindowText(tmp);
  s->TimeFromCal=_wtof(tmp);
  this->SacTimeFromStimulT.GetWindowText(tmp);
  s->TimeFromStimul=_wtof(tmp);
  OutputSaccades();
  FocusToSaccade();
}

void CSaccadeSearchDlg::OnBnClickedButton13()
{
  this->SacChanT.EnableWindow(1);
  this->SacRecT.EnableWindow(1);
  this->SacXBeginT.EnableWindow(1);
  this->SacYBeginT.EnableWindow(1);
  this->SacXEndT.EnableWindow(1);
  this->SacYEndT.EnableWindow(1);
  this->SacAmplitudeT.EnableWindow(1);
  this->SacStimulCodeT.EnableWindow(1);
  this->SacTimeFromCalT.EnableWindow(1);
  this->SacTimeFromStimulT.EnableWindow(1);

  Saccade* s=new Saccade();
  s->chan=Conan->CurChannel;
  s->rec=Conan->CurRec;
  Conan->Saccades.push_back(s);
  Win->CurSaccade=Conan->Saccades.size()-1;
  OutputSaccades();
  FocusToSaccade();
}

void CSaccadeSearchDlg::OnBnClickedButton7()
{
  if(FirstLoad==true)
  {
    SearchForCal.SetCheck(1);
    PreprocessAllRecords.SetCheck(1);
    CreateVirtualSaccades.SetCheck(1);
	MinYSpeedT.SetWindowText(_T("1"));
	MinSpeedPointsT.SetWindowText(_T("10"));
	MinYLengthT.SetWindowText(_T("100"));
	MinXLengthT.SetWindowText(_T("1"));
	MinTimeOffsetT.SetWindowText(_T("0"));
	ApproxPrec.SetWindowText(_T("1"));
	AprIterationsT.SetWindowText(_T("5"));
	MinExtremPointsT.SetWindowText(_T("10"));
    UseApproximation.SetCheck(1);
    FirstLoad=false;
  }
  //this->DoModal();
  CString path;
  CFileDialog dlg(TRUE/*Open=TRUE Save=False*/, _T("cad")/*Filename Extension*/, _T("")/*Initial Filename*/, OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST/*Flags*/, _T("Conan File Format(*.cad)|*.cad||")/*Filetype Filter*/, this/*parent Window*/);
	int z=dlg.DoModal();
  if (z == IDOK)
		path=dlg.GetPathName();
  else if (Conan!=NULL)//smth already loaded
    return;
  else
  {
    this->OnCancel();//exit program if nothing in data
    return;
  }

  OpenedFile.SetWindowText(path);
  if(Conan!=NULL)
    delete(Conan);
  try
  {
	  Conan = ReadConanFile(path.GetBuffer(), &log);
  }
  catch (std::exception& e)
  {
	  CString err = _T("Failed to read file!");
	  MessageBox(err, _T("Error"), MB_OK);
	  return;
  }
  if(Conan==NULL)
    return;

  chan.ResetContent();
  rec.ResetContent();
  CString chnum,chname,channels;
  const wchar_t* tmp33 = GetWC(Conan->Header->chNames->get());
  channels.Append(tmp33);
  delete(tmp33);
  for(int i=0;i<Conan->Header->nChan;i++)
  {
    chname=channels.Mid(i*4,4);
	chnum.Format(_T("%d "), i + 1);
    chnum.Append(chname);
    chan.AddString(chnum.GetBuffer());
  }
  SetDropDownHeight(&chan,Conan->Header->nChan);
  
  if(Conan->Header->nChan>=26)
  {
    chan.SetCurSel(25);
    Conan->CurChannel=25;
  }
  else
  {
    chan.SetCurSel(Conan->Header->nChan-1);
    Conan->CurChannel=Conan->Header->nChan-1;
  }

  CString RecNum;
  for(int i=0;i<Conan->Header->nRec;i++)
  {
	  RecNum.Format(_T("%d "), i + 1);
    rec.AddString(RecNum.GetBuffer());
  }
  SetDropDownHeight(&rec,20);
  rec.SetCurSel(0);
  Conan->CurRec=0;


  CString tmp;
  tmp.Format(_T("%d"), Conan->Header->freq);
  xfreq.SetWindowText(tmp);


	this->RecreateEeGraph();	
  Win->hold();
  xscale.SetWindowText(_T("0.2"));
  yscale.SetWindowText(_T("0.75"));
  xoffset.SetWindowText(_T("0"));
  yoffset.SetWindowText(_T("0"));

  AutoDefineYOffset.SetCheck(1);
  OnBnClickedRadio2();
  Win->unhold();
  AutoSetXYOffset();


  
  /*
  
  do NOT reset parameters
  MinYSpeedT.SetWindowText("1");
  MinSpeedPointsT.SetWindowText("10");
  MinYLengthT.SetWindowText("100");
  MinXLengthT.SetWindowText("1");
  MinTimeOffsetT.SetWindowText("0");

  
  ApproxPrec.SetWindowText("1");
  AprIterationsT.SetWindowText("5");
  MinExtremPointsT.SetWindowText("10");

  UseApproximation.SetCheck(1);*/
  
  ApproxPrec.GetWindowText(tmp);
  Conan->AproxCoef = _wtoi(tmp);
  AprIterationsT.GetWindowText(tmp);
  Conan->Iterations = _wtoi(tmp);
  MinExtremPointsT.GetWindowText(tmp);
  Conan->MinExtremumPoints = _wtoi(tmp);
  Conan->Approximate=UseApproximation.GetCheck();
  Approximate(Conan->CurChannel,Conan->CurRec);
  
  Conan->ResetSaccades();
  Win->CurSaccade=0;
  this->OutputSaccades();
  this->SacChanT.EnableWindow(0);
  this->SacRecT.EnableWindow(0);
  this->SacXBeginT.EnableWindow(0);
  this->SacYBeginT.EnableWindow(0);
  this->SacXEndT.EnableWindow(0);
  this->SacYEndT.EnableWindow(0);
  this->SacAmplitudeT.EnableWindow(0);
  this->SacStimulCodeT.EnableWindow(0);
  this->SacTimeFromCalT.EnableWindow(0);
  this->SacTimeFromStimulT.EnableWindow(0);

  OnBnClickedCheck5();
}

void CSaccadeSearchDlg::OnBnClickedButton14()
{
  if(Conan->Saccades.size()==0)//nothing to erase
    return;
  delete(Conan->Saccades.at(Win->CurSaccade));
  Conan->Saccades.erase(Conan->Saccades.begin()+Win->CurSaccade);
  if(Win->CurSaccade>0)
    Win->CurSaccade--;
  else if(Win->CurSaccade==0 && Conan->Saccades.size()>0)
    ;//CurSaccade++;
  else if(Conan->Saccades.size()==0)
  {
    this->SacChanT.Clear();
	this->SacRecT.Clear();
    this->SacXBeginT.Clear();
    this->SacYBeginT.Clear();
    this->SacXEndT.Clear();
    this->SacYEndT.Clear();
    this->SacAmplitudeT.Clear();
    this->SacStimulCodeT.Clear();
    this->SacTimeFromCalT.Clear();
    this->SacTimeFromStimulT.Clear();
  }
  if(Conan->Saccades.size()==0)
  {
    this->SacChanT.EnableWindow(0);
    this->SacRecT.EnableWindow(0);
    this->SacXBeginT.EnableWindow(0);
    this->SacYBeginT.EnableWindow(0);
    this->SacXEndT.EnableWindow(0);
    this->SacYEndT.EnableWindow(0);
    this->SacAmplitudeT.EnableWindow(0);
    this->SacStimulCodeT.EnableWindow(0);
    this->SacTimeFromCalT.EnableWindow(0);
    this->SacTimeFromStimulT.EnableWindow(0);
  }
  OutputSaccades();
  FocusToSaccade();
}

void CSaccadeSearchDlg::OnBnClickedButton1()
{
	USES_CONVERSION;
  CString save,tmp;
  int method=SearchMethod.GetCurSel();
  Saccade* s;
  Saccade* s2;
  CString path;
  CFileDialog dlg(FALSE/*Open=TRUE Save=False*/,_T("txt")/*Filename Extension*/,_T("")/*Initial Filename*/,OFN_ENABLESIZING|OFN_EXPLORER/*Flags*/,_T("Saccade file(*.txt)|*.txt||")/*Filetype Filter*/,this/*parent Window*/);
	int z=dlg.DoModal();
  if (z!= IDOK)
    return;
		
  path=dlg.GetPathName();
  bool OhFileExists=FileExists(path);
  FILE * pFile;
  fopen_s(&pFile, W2A(path.GetBuffer()), "a+");
  CString header;

  OpenedFile.GetWindowText(path);
  path=path.Mid(1+path.ReverseFind('\\'));
  if(method==0 || method==1)
  {
    if(!OhFileExists)
    {
      header="File_Name Record Saccade_BeginX Saccade_BeginY Saccade_EndX Saccade_EndY Amplitude Stimul_Code Time_From_Calibration_Stimul Time_From_Last_Stimul Notice";
      fwrite(W2A(header.GetBuffer()),1,header.GetLength(),pFile);
      fwrite("\n",1,1,pFile);
    }
    for(unsigned int i=0;i<Conan->Saccades.size();i++)
    {
      s=Conan->Saccades.at(i);
	  fprintf(pFile, "%s %d %1.1f %1.4f %1.1f %1.4f %1.4f %x %1.1f %1.1f %s", W2A(path.GetBuffer()), s->rec + 1, s->BeginX, s->BeginY, s->EndX, s->EndY, s->AmplitudeY(), s->StimulCode, s->TimeFromCal, s->TimeFromStimul, W2A(s->Notice.GetBuffer()));
     fwrite("\n",1,1,pFile);
    }
  }
  else if(method==2)//double saccade
  {
    if(!OhFileExists)
    {
      header="File_Name Record Time_Between_Stimuls Stimul_1_code Stimul_2_code 1_Saccade_Latent_Time 2_Saccade_Latent_Time 1_Amplitude 2_Amplitude 1_Notice 2_Notice";
	  fwrite(W2A(header.GetBuffer()), 1, header.GetLength(), pFile);
      fwrite("\n",1,1,pFile);
    }
    int SacNum=0;
    for(int RecNum=0;RecNum<Conan->Header->nRec;RecNum++)
    {
      int SacQ=0;
      while(Conan->Saccades.size()>SacNum && Conan->Saccades.at(SacNum)->rec==RecNum)
      {
        SacQ++;
        SacNum++;
      }
      if(SacQ==0)
		  fprintf(pFile, "%s %d -", W2A(path.GetBuffer()), RecNum + 1);
      else if(SacQ>2)//like fatal error
      {
        CString err;
		err.Format(_T("In record # %d, %d saccades found. Please, correct it and try to save again. Aborting..."), RecNum + 1, SacQ);
		fwrite(W2A(err.GetBuffer()), 1, err.GetLength(), pFile);
        fclose(pFile);
		MessageBox(err, _T("Error"), MB_OK);
        return;
      }
      else if(SacQ==1)
      {
        s=Conan->Saccades.at(SacNum-1);
		fprintf(pFile, "%s %d - %x - %1.0f - %1.4f - %s", W2A(path.GetBuffer()), s->rec + 1, s->StimulCode, s->TimeFromCal, s->AmplitudeY(), W2A(s->Notice.GetBuffer()));
      }
      else if(SacQ==2)
      {
        s=Conan->Saccades.at(SacNum-2);
        s2=Conan->Saccades.at(SacNum-1);
        float TimeBetweenStimul=abs(s->BeginX-s->TimeFromStimul-(s2->BeginX-s2->TimeFromStimul));
		fprintf(pFile, "%s %d %1.0f %x %x %1.0f %1.0f %1.4f %1.4f %s %s", W2A(path.GetBuffer()), s->rec + 1, TimeBetweenStimul, s->StimulCode, s2->StimulCode, s->TimeFromCal, s2->TimeFromCal, s->AmplitudeY(), s2->AmplitudeY(), W2A(s->Notice.GetBuffer()), W2A(s2->Notice.GetBuffer()));
      }
      fwrite("\n",1,1,pFile);
    }
  }
  fclose(pFile);
}

void CSaccadeSearchDlg::OnBnClickedButton6()
{
  bool erase=0;
  if(Conan->Saccades.size())
  {
	  int c = MessageBox(_T("Do you want to erase currently found values?"), _T("Confirmation"), MB_YESNO);
    if(c==IDYES)
      erase=1;
  }
  CString path;
  CFileDialog dlg(TRUE/*Open=TRUE Save=False*/, _T("sac")/*Filename Extension*/, _T("")/*Initial Filename*/, OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST/*Flags*/, _T("Saccade file(*.sac)|*.sac||")/*Filetype Filter*/, this/*parent Window*/);
	int z=dlg.DoModal();
  if (z == IDOK)
  {
		path=dlg.GetPathName();
    FILE * pFile;
    _wfopen_s(&pFile, path.GetBuffer(), _T("r"));
    if (pFile==NULL)
    {
		MessageBox(_T("Не могу прочитать файл!"), _T("Ошибка"), MB_OK);
	    return;
    }
    //erase
    if(erase)
    {
      Conan->ResetSaccades();
      Win->CurSaccade=0;
      this->OutputSaccades();
    }

    int r;
    float amp;
    while(true)
    {
      Saccade* s=new Saccade();
      r=fscanf_s(pFile,"%d %d %f %f %f %f %f %x %f %f\n",&s->chan,&s->rec,&s->BeginX,&s->BeginY,&s->EndX,&s->EndY,&amp,&s->StimulCode,&s->TimeFromCal,&s->TimeFromStimul);
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

void CSaccadeSearchDlg::OnEnChangeEdit26()
{
  if(Conan==NULL)
    return;
  CString tmp;
  ApproxPrec.GetWindowText(tmp);
  Conan->AproxCoef=_wtoi(tmp);
  Approximate(Conan->CurChannel,Conan->CurRec);
}

void CSaccadeSearchDlg::Approximate(int chan, int rec)
{  
  
  if(!(Conan->Approximate&&Conan->AproxCoef&&Conan->Iterations&&Conan->MinExtremumPoints))
  {
    //Conan->Approximate=0;
    UseApproximation.SetCheck(0);
    Win->changed=1;
    Win->WinPaint();
    return;
  }
  //Conan->Approximate=1;

  if(Conan->EegApproximated==NULL)//init
  {    
    Conan->EegApproximated=new float** [Conan->Header->nRec];      
    for (int i=0;i<Conan->Header->nRec;i++)
    {        
      Conan->EegApproximated[i]=new float*[Conan->Header->nChan];
      for (int j=0;j<Conan->Header->nChan;j++)
      {
        Conan->EegApproximated[i][j]=new float[Conan->NDataReal[i]];
      }
    }
  }

  //init points
  for(int k=0;k<Conan->NDataReal[rec];k++)
  {
    Conan->EegApproximated[rec][chan][k]=Conan->Eeg[rec][chan][k];
  }
  //now count approximation for current channel&record

  for(int a=0;a<Conan->Iterations;a++){
  for(int k=Conan->MinExtremumPoints+1;k<Conan->NDataReal[rec]-Conan->MinExtremumPoints+1;k++)
  {
    //check if that's global extremum. if so - exclude.
    bool GlobalExtremum=true;
    for(int z=1;z<Conan->MinExtremumPoints+1;z++)
    {
      if(((Conan->EegApproximated[rec][chan][k+z] - Conan->EegApproximated[rec][chan][k])>0) != ((Conan->EegApproximated[rec][chan][k-z] - Conan->Eeg[rec][chan][k])>0))
        //if those signs are the same, it means that point is extremim
      {
        GlobalExtremum=false;
        break;
      }    
    }
    if(GlobalExtremum)
    {
      Conan->EegApproximated[rec][chan][k]=Conan->EegApproximated[rec][chan][k];
      continue;//no need to change value for global extremum points.
    }
    
    //detect if it is a local extremum - same but with other parameters.

    bool LocalExtremum=true;
    for(int z=1;z<Conan->AproxCoef;z++)
    {
      if(((Conan->EegApproximated[rec][chan][k+z] - Conan->EegApproximated[rec][chan][k])>0) != ((Conan->EegApproximated[rec][chan][k-z] - Conan->EegApproximated[rec][chan][k])>0))
        //if those signs are the same, it means that point is extremim
      {
        LocalExtremum=false;
        break;
      }    
    }
    if(LocalExtremum)
      //aproximate to the middle of nearest points
    {
      Conan->EegApproximated[rec][chan][k]=
        (Conan->EegApproximated[rec][chan][k+1]+
        Conan->EegApproximated[rec][chan][k-1])
        /2.0;
      continue;
    }

    //if it is not any extremum - the value also stays as is
    Conan->EegApproximated[rec][chan][k]=Conan->EegApproximated[rec][chan][k];
  }
  }
  Win->changed=1;
  Win->WinPaint();
}
void CSaccadeSearchDlg::OnEnChangeEdit27()
{
  if(Conan==NULL)
    return;
  CString tmp;
  AprIterationsT.GetWindowText(tmp);
  Conan->Iterations=_wtoi(tmp);
  Approximate(Conan->CurChannel,Conan->CurRec);
}

void CSaccadeSearchDlg::OnEnChangeEdit28()
{
  if(Conan==NULL)
    return;
  CString tmp;
  MinExtremPointsT.GetWindowText(tmp);
  Conan->MinExtremumPoints=_wtoi(tmp);
  Approximate(Conan->CurChannel,Conan->CurRec);
}

void CSaccadeSearchDlg::OnBnClickedCheck5()
{
  if(UseApproximation.GetCheck())
  {
    ApproxPrec.EnableWindow(1);
    AprIterationsT.EnableWindow(1);
    MinExtremPointsT.EnableWindow(1);
    Conan->Approximate=1;
  }
  else
  {
    MinExtremPointsT.EnableWindow(0);
    AprIterationsT.EnableWindow(0);
    ApproxPrec.EnableWindow(0);
    Conan->Approximate=0;
  }
  Approximate(Conan->CurChannel,Conan->CurRec);
  Win->changed=1;
  Win->WinPaint();
}

void CSaccadeSearchDlg::OnEnChangeEdit7()
{
  //next saccade
  CString tmp;
  CurSaccadeT.GetWindowText(tmp);
  unsigned int c=_wtoi(tmp);
  if((c>Conan->Saccades.size()) ||(Conan->Saccades.size()==0))
    return;//saccade num, not valid
  Win->CurSaccade=c-1;
  OutputSaccades();
  Conan->CurChannel=Conan->Saccades.at(Win->CurSaccade)->chan;
  Conan->CurRec=Conan->Saccades.at(Win->CurSaccade)->rec;
  FocusToSaccade();
}

void CSaccadeSearchDlg::ScanHexToText(CEdit* from,StimulVec* to)
{
  int s=from->GetWindowTextLength();
  TCHAR* buf = new TCHAR[s + 1];
  from->GetWindowText(buf,s+1);
  int c=0;
  for(int i=0;i<s;i++)
  {
    if(i==0)
		swscanf_s((&buf[0] + i), _T("%x"), &c);
    else if(buf[i]=='\n' && (i+1<s))
		swscanf_s((&buf[0] + i + 1), _T("%x"), &c);
    else
      continue;
    to->push_back(c);
  }
  delete[] buf;
}
void CSaccadeSearchDlg::OnEnChangeEdit29()
{
  StimulPositive.clear();
  ScanHexToText(&PositiveStimulCodesT,&StimulPositive);
}
void CSaccadeSearchDlg::OnEnChangeEdit15()
{
  StimulNegative.clear();
  ScanHexToText(&NegativeStimulCodesT,&StimulNegative);
}

void CSaccadeSearchDlg::OnEnChangeEdit8()
{
  CString tmp;
  rulerx.GetWindowText(tmp);
  float x=_wtof(tmp);
  float y=Win->GetYFromX(Conan->CurChannel,Conan->CurRec,x);
  Win->RulerPointX=x;
  Win->RulerPointY=y;
  tmp.Format(_T("%f"), y);
  rulery.SetWindowText(tmp);
  Win->changed=1;
  Win->WinPaint();
}


void CSaccadeSearchDlg::OnEnChangeEdit18()
{
  //set Y corresponding to X
  if(Conan->Saccades.size()<=Win->CurSaccade)
    return;
  Saccade * sac=Conan->Saccades.at(Win->CurSaccade);
  if(sac->AmplitudeY()==0)
    return;//no stimul time for virtual saccades!
  CString tmp;
  this->SacXBeginT.GetWindowText(tmp);
  float x=_wtof(tmp);
  float y=Win->GetYFromX(sac->chan,sac->rec,x);
  tmp.Format(_T("%1.0f"), y);
  this->SacYBeginT.SetWindowText(tmp);

  //set new stimul & calibration stimul time
  int n=Win->XRealToPoint(x);
  unsigned __int8 stim=0;
  sac->TimeFromCal=0;
  sac->TimeFromStimul=0;
  bool CalFound=0;
  bool StimFound=0;
  //find stimul
  while(n>=0 )
  {
    stim=Conan->Discr[sac->rec][n].Elder;
    if(stim==sac->StimulCode)
    {
      sac->TimeFromStimul=x-Win->XToRealCoordsFromPoint(n);
      sac->StimulTime=Win->XToRealCoordsFromPoint(n);
      StimFound=1;
      break;
    }
    n--;
  }

  if(StimFound)
  {
    //find calibration stimul
    while(n<Conan->NDataReal[sac->rec])
    {
      stim=Conan->Discr[sac->rec][n].Elder;
      if(stim==128)//80 in hex
      {
        sac->TimeFromCal=x-Win->XToRealCoordsFromPoint(n);
        CalFound=1;
        break;
      }
      n++;
    }
  }
  //all found
  tmp.Format(_T("%1.0f"), sac->TimeFromCal);
  SacTimeFromCalT.SetWindowText(tmp);

  tmp.Format(_T("%1.0f"), sac->TimeFromStimul);
  SacTimeFromStimulT.SetWindowText(tmp);
}

void CSaccadeSearchDlg::OnEnChangeEdit20()
{
  //set Y corresponding to X
  if(Conan->Saccades.size()<=Win->CurSaccade)
    return;
  CString tmp;
  this->SacXEndT.GetWindowText(tmp);
  float x=(float)_wtof(tmp);
  Saccade * sac=Conan->Saccades.at(Win->CurSaccade);
  float y=Win->GetYFromX(sac->chan,sac->rec,x);
  tmp.Format(_T("%1.0f"), y);
  this->SacYEndT.SetWindowText(tmp);
  
}

void CSaccadeSearchDlg::OnBnClickedButton10()
{
  if(Conan->Saccades.size()==0)
    return;
  CString tmp;
  rulerx.GetWindowText(tmp);
  this->SacXBeginT.SetWindowText(tmp);
  OnEnChangeEdit18();
}

void CSaccadeSearchDlg::OnBnClickedButton15()
{
  if(Conan->Saccades.size()==0)
    return;
  CString tmp;
  rulerx.GetWindowText(tmp);
  this->SacXEndT.SetWindowText(tmp);
  OnEnChangeEdit20();
}

void CSaccadeSearchDlg::OnBnClickedButton17()
{
  CString tmp;
  CString data;

  tmp.Format(_T("%d"), this->chan.GetCurSel());
  data.Append(tmp);
  data.Append(_T(" "));
  
  tmp.Format(_T("%d"), this->rec.GetCurSel());
  data.Append(tmp);
  data.Append(_T(" "));

  this->xscale.GetWindowText(tmp);
  data.Append(tmp);
  data.Append(_T(" "));

  this->yscale.GetWindowText(tmp);
  data.Append(tmp);
  data.Append(_T(" "));

  this->xoffset.GetWindowText(tmp);
  data.Append(tmp);
  data.Append(_T(" "));

  this->yoffset.GetWindowText(tmp);
  data.Append(tmp);
  data.Append(_T(" "));
  
  tmp.Format(_T("%d"),this->AutoDefineYOffset.GetCheck());
  data.Append(tmp);
  data.Append(_T(" "));

  tmp.Format(_T("%d"), this->GridEnabled.GetCheck());
  data.Append(tmp);
  data.Append(_T(" "));

  tmp.Format(_T("%d"), this->showpoints.GetCheck());
  data.Append(tmp);
  data.Append(_T(" "));

  tmp.Format(_T("%d"), this->mouse_move.GetCheck());
  data.Append(tmp);
  data.Append(_T(" "));

  tmp.Format(_T("%d"), this->mouse_zoom.GetCheck());
  data.Append(tmp);
  data.Append(_T(" "));

  tmp.Format(_T("%d"), this->mouse_ruler.GetCheck());
  data.Append(tmp);
  data.Append(_T(" "));

  this->rulerx.GetWindowText(tmp);
  data.Append(tmp);
  data.Append(_T(" "));

  ///search options
  this->MinSpeedPointsT.GetWindowText(tmp);
  data.Append(tmp);
  data.Append(_T(" "));
  this->MinTimeOffsetT.GetWindowText(tmp);
  data.Append(tmp);
  data.Append(_T(" "));
  this->MinXLengthT.GetWindowText(tmp);
  data.Append(tmp);
  data.Append(_T(" "));
  this->MinYLengthT.GetWindowText(tmp);
  data.Append(tmp);
  data.Append(_T(" "));
  this->MinYSpeedT.GetWindowText(tmp);
  data.Append(tmp);
  data.Append(_T(" "));
  
  tmp.Format(_T("%d"), this->SearchMethod.GetCurSel());
  data.Append(tmp);
  data.Append(_T(" "));

  tmp.Format(_T("%d"), this->CreateVirtualSaccades.GetCheck());
  data.Append(tmp);
  data.Append(_T(" "));
  tmp.Format(_T("%d"), this->ClearOther.GetCheck());
  data.Append(tmp);
  data.Append(_T(" "));
  tmp.Format(_T("%d"), this->PreprocessAllRecords.GetCheck());
  data.Append(tmp);
  data.Append(_T(" "));
  tmp.Format(_T("%d"), this->SearchForCal.GetCheck());
  data.Append(tmp);
  data.Append(_T(" "));


  tmp.Format(_T("%d"), this->UseApproximation.GetCheck());
  data.Append(tmp);
  data.Append(_T(" "));

  this->ApproxPrec.GetWindowText(tmp);
  data.Append(tmp);
  data.Append(_T(" "));
  this->AprIterationsT.GetWindowText(tmp);
  data.Append(tmp);
  data.Append(_T(" "));
  this->MinExtremPointsT.GetWindowText(tmp);
  data.Append(tmp);
  data.Append(_T(" "));

  data.Append(_T("<PositiveStimuls>"));
  this->PositiveStimulCodesT.GetWindowText(tmp);
  data.Append(tmp);
  data.Append(_T("</PositiveStimuls>"));
  data.Append(_T("<NegativeStimuls>"));
  this->NegativeStimulCodesT.GetWindowText(tmp);
  data.Append(tmp);
  data.Append(_T("</NegativeStimuls>"));

  
  CString path;
  CFileDialog dlg(FALSE/*Open=TRUE Save=False*/, _T("cfg")/*Filename Extension*/, _T("")/*Initial Filename*/, OFN_ENABLESIZING | OFN_EXPLORER/*Flags*/, _T("Config file(*.cfg)|*.cfg||")/*Filetype Filter*/, this/*parent Window*/);
	int z=dlg.DoModal();
  if (z == IDOK)
  {
		path=dlg.GetPathName();
    FILE * pFile;
	_wfopen_s(&pFile, path, _T("w"));
	USES_CONVERSION;
	fwrite(W2A(data.GetBuffer()), sizeof(char), data.GetLength(), pFile);
    fclose(pFile);
  }
}

void CSaccadeSearchDlg::OnBnClickedButton16()
{
	USES_CONVERSION;
  CString path;
  CFileDialog dlg(TRUE/*Open=TRUE Save=False*/, _T("cfg")/*Filename Extension*/, _T("")/*Initial Filename*/, OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST/*Flags*/, _T("Config file(*.cfg)|*.cfg||")/*Filetype Filter*/, this/*parent Window*/);
	int z=dlg.DoModal();
  if (z == IDOK)
		path=dlg.GetPathName();
  else
    return;
  
  Win->hold();
  FILE * pFile;
  long lSize;
  char* buffer;
  size_t result;

  _wfopen_s(&pFile, path, _T("r+t"));
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  rewind (pFile);
  buffer = new char[lSize];
  result = fread (buffer,1,lSize,pFile);
  fclose(pFile);
  CString data;
  data.Append(A2W(buffer));
  delete[] buffer;
  
  int p=0;
  int p2=0;
  CString tmp;
//////
  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  chan.SetCurSel(_wtoi(tmp));
  p=p2+1;
  

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  rec.SetCurSel(_wtoi(tmp));
  p=p2+1;
  

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  xscale.SetWindowText(tmp);
  p=p2+1;
  

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  yscale.SetWindowText(tmp);
  p=p2+1;
  

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  xoffset.SetWindowText(tmp);
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  yoffset.SetWindowText(tmp);
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  AutoDefineYOffset.SetCheck(_wtoi(tmp));
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  GridEnabled.SetCheck(_wtoi(tmp));
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  showpoints.SetCheck(_wtoi(tmp));
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  if(_wtoi(tmp))
    OnBnClickedRadio2();
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  if(_wtoi(tmp))
    OnBnClickedRadio1();
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  if(_wtoi(tmp))
    OnBnClickedRadio3();
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  rulerx.SetWindowText(tmp);
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  MinSpeedPointsT.SetWindowText(tmp);
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  MinTimeOffsetT.SetWindowText(tmp);
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  MinXLengthT.SetWindowText(tmp);
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  MinYLengthT.SetWindowText(tmp);
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  MinYSpeedT.SetWindowText(tmp);
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  SearchMethod.SetCurSel(_wtoi(tmp));
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  CreateVirtualSaccades.SetCheck(_wtoi(tmp));
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  ClearOther.SetCheck(_wtoi(tmp));
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  PreprocessAllRecords.SetCheck(_wtoi(tmp));
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  SearchForCal.SetCheck(_wtoi(tmp));
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  UseApproximation.SetCheck(_wtoi(tmp));
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  ApproxPrec.SetWindowText(tmp);
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  AprIterationsT.SetWindowText(tmp);
  p=p2+1;

  p2 = data.Find(_T(" "), p);
  tmp=data.Mid(p,p2-p);
  MinExtremPointsT.SetWindowText(tmp);
  p=p2+1;

  p = data.Find(_T("<PositiveStimuls>"), p2);
  p2 = data.Find(_T("</PositiveStimuls>"), p);
  tmp=data.Mid(p+17,p2-p-17);
  PositiveStimulCodesT.SetWindowText(tmp);

  p = data.Find(_T("<NegativeStimuls>"), p2);
  p2 = data.Find(_T("</NegativeStimuls>"), p);
  tmp=data.Mid(p+17,p2-p-17);
  NegativeStimulCodesT.SetWindowText(tmp);
  
  
  OnCbnSelchangeCombo3();//event search method
  OnCbnSelchangeCombo1();//event channel
  OnCbnSelchangeCombo2();//event record
  OnBnClickedCheck1();//autodef XY
  OnBnClickedCheck2();//grid
  OnBnClickedCheck3();//point vals
  OnBnClickedCheck5();

  OnEnChangeEdit29();//update negative and positive stimuls
  OnEnChangeEdit15();

  OnBnClickedCheck5();//approximation
  OnEnChangeEdit26();
  OnEnChangeEdit27();
  OnEnChangeEdit28();

  Win->changed=1;
  Win->unhold();
}

void CSaccadeSearchDlg::OnBnClickedButton18()
{
  if(Conan->Saccades.size()==0)//nothing to virtualize
    return;
  Saccade* s=Conan->Saccades.at(Win->CurSaccade);
  s->BeginX-=s->TimeFromStimul;//begin right after stimul
  s->EndY=s->BeginY;
  s->TimeFromCal=0;
  s->TimeFromStimul=0;
  s->EndX=s->BeginX+MaxTimeAfterStimul;
  OutputSaccades();
  FocusToSaccade();
}
