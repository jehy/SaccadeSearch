// SaccadeSearchDlg.h : header file
//

#pragma once
//#include "afxwin.h"
#include "stdafx.h"
#include "Eegraph.h"
#include "ConanData.h"
#include "afxwin.h"


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
 CEdit StimulCodesT;
 CButton ClearOther;
 int CurSaccade;
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
 CEdit SacLatentTimeT;
 void FocusToSaccade();
 afx_msg void OnBnClickedButton12();
 afx_msg void OnBnClickedButton13();
 afx_msg void OnBnClickedButton7();
 int init;
 afx_msg void OnBnClickedButton14();
 afx_msg void OnBnClickedButton1();
 afx_msg void OnBnClickedButton6();
};