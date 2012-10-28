// SaccadeSearchDlg.h : header file
//

#pragma once
//#include "afxwin.h"
#include "stdafx.h"
#include "Eegraph.h"
#include "ConanData.h"
#include "afxwin.h"
typedef vector <__int8> StimulVec;

// CSaccadeSearchDlg dialog
class CSaccadeSearchDlg : public CDialog
{
// Construction
public:
	CSaccadeSearchDlg(CWnd* pParent = NULL);	// standard constructor
  ~CSaccadeSearchDlg();
// Dialog Data
	enum { IDD = IDD_SACCADESEARCH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
  CEdit log;

public:
 //CButton LoadBtn;
// afx_msg void OnBnClickedButton1();
 CEdit xscale;
 CEdit yscale;
 CComboBox chan;
 afx_msg void OnCbnSelchangeCombo1();
 afx_msg void OnEnChangeEdit2();
 afx_msg void OnEnChangeEdit3();
 CEdit xfreq;
 CEdit xoffset;
 CEdit yoffset;
 afx_msg void OnEnUpdateEdit6();
 afx_msg void OnEnUpdateEdit5();
 void ChangeParam();
 void RecreateEeGraph();
 void OutputSaccades();
 void AutoSetXYOffset();
 void Move(int x,int y);
 afx_msg void OnSize(UINT nType, int cx, int cy);
 CComboBox rec;
 afx_msg void OnCbnSelchangeCombo2();
 CButton AutoDefineYOffset;
 afx_msg void OnBnClickedCheck1();
 afx_msg void OnBnClickedButton2();
 afx_msg void OnBnClickedButton3();
 afx_msg void OnBnClickedButton4();
 afx_msg void OnBnClickedButton5();
 afx_msg void OnBnClickedCheck2();
 CButton GridEnabled;
 afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
 afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
 afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
 afx_msg void OnBnClickedCheck3();
 CButton showpoints;
 afx_msg void OnBnClickedOk();
 afx_msg void OnBnClickedCancel();
 afx_msg void OnClose();
 afx_msg void OnCbnSelchangeCombo3();
// afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
 CButton mouse_move;
 CButton mouse_zoom;
 CButton mouse_ruler;
 afx_msg void OnBnClickedRadio2();
 afx_msg void OnBnClickedRadio1();
 afx_msg void OnBnClickedRadio3();
 CEdit rulerx;
 CEdit rulery;
 afx_msg void OnBnClickedButton9();
 CEdit MinYSpeedT;
 CEdit MinSpeedPointsT;
 CEdit MinYLengthT;
 CEdit MinXLengthT;
 CEdit MinTimeOffsetT;
 CEdit NegativeStimulCodesT;
 CButton ClearOther;
 CEdit CurSaccadeT;
 afx_msg void OnBnClickedButton11();
 afx_msg void OnBnClickedButton8();
 CEdit SacChanT;
 CEdit SacRecT;
 CEdit SacXBeginT;
 CEdit SacYBeginT;
// afx_msg void OnBnClickedButton10();
 CEdit SacXEndT;
 CEdit SacYEndT;
 CEdit SacAmplitudeT;
 CEdit SacStimulCodeT;
 CEdit SacTimeFromCalT;
 void FocusToSaccade();
 int SetStimul(int rec,int n);
 afx_msg void OnBnClickedButton12();
 afx_msg void OnBnClickedButton13();
 afx_msg void OnBnClickedButton7();
 int init;
 afx_msg void OnBnClickedButton14();
 afx_msg void OnBnClickedButton1();
 afx_msg void OnBnClickedButton6();
// afx_msg void OnMove(int x, int y);
 
  unsigned __int8 LastStimulCode;
  unsigned __int8 Stimul;
  float LastStimulTime;
  float LastCalStimulTime;
  CEdit SacTimeFromStimulT;
//  afx_msg void OnBnClickedButton17();
  afx_msg void OnEnChangeEdit26();
  CEdit ApproxPrec;
  afx_msg void OnEnChangeEdit27();
  CEdit AprIterationsT;
  void Approximate(int chan, int rec);
  afx_msg void OnEnChangeEdit28();
  CEdit MinExtremPointsT;
  CButton UseApproximation;
  afx_msg void OnBnClickedCheck5();
  CButton PreprocessAllRecords;
  CComboBox SearchMethod;
  CEdit PositiveStimulCodesT;
  CButton CreateVirtualSaccades;
  CEdit AllSaccades;
  afx_msg void OnEnChangeEdit7();
  afx_msg void OnEnChangeEdit29();
  int GetStimulSign(__int8 stimul);
  afx_msg void OnEnChangeEdit15();
  void ScanHexToText(CEdit* from,StimulVec* to);
  CButton SearchForCal;
  afx_msg void OnEnChangeEdit8();
  afx_msg void OnEnChangeEdit18();
  afx_msg void OnEnChangeEdit20();
  afx_msg void OnBnClickedButton10();
  afx_msg void OnBnClickedButton15();
  afx_msg void OnBnClickedButton17();
  afx_msg void OnBnClickedButton16();
  CEdit OpenedFile;
  afx_msg void OnBnClickedButton18();
//  afx_msg void OnBnClickedCheck7();
  CEdit Notice;
};
