#pragma once
#include "stdafx.h"

class Eegraph : public CWnd
{
	DECLARE_DYNAMIC(Eegraph)
public:
  Eegraph(void);
  ~Eegraph(void);
  //void OnLButtonDown(UINT nFlags, CPoint point);

  float XToScreenCoords(float XRealCoords);
  float YToScreenCoords(float YRealCoords);
  float XToRealCoords(float XScreenCoords);
  float YToRealCoords(float YScreenCoords);
  float XToRealCoordsFromPoint(float point);

  float RealGraphWidth();
  float ScreenGraphWidth();
  float RealGraphHeight();
  float ScreenGraphHeight();

	void WinPaint();
  void _CreateDC();
  void ClearDC();
  void DrawDC();
  void _DeleteDC();
  void PaintDC();
  void DrawCoord();
  void DrawSaccades();
  void hold();
  void unhold();
  void DrawRuler();
  afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
  void AppendFormatFloat(CString* string,float f,int maxchars);

	CDC *dc;					//��������� �� �������� ����������� ����

	//���������� ������������ ��������� � ����
	CPen Pen,*OldPen;			//����
	CBrush Brush,*OldBrush;		//�����
	LOGFONT LogFont;			//C�������� ���������� ������
	CFont Font,*OldFont;		//�����

	//���������� ��������������� ��������� �����
	COLORREF ColorPen,ColorBrush;//,ColorText;
  
	CDC	DC;							//��������� ������ ��������� �����������
	CBitmap BmpDC;					//��������� ������
	CBitmap *OldBmpDC;				//��������� ��������� �� ������ Cbitmap ����
  COLORREF* LinePens;

	RECT ClientRect;			//���������� ��������������� �������������� ���������� ����� ����
	int x,y;					//���������� ������ � ����
  int created,changed,grid,held,points,MouseAction;
  CPoint MovePoint;
  float RulerPointX;
  float RulerPointY;

protected:
	DECLARE_MESSAGE_MAP()
  afx_msg void OnLButtonDown( UINT nFlags, CPoint pt );
  afx_msg void OnLButtonUp( UINT nFlags, CPoint pt );
  afx_msg void OnRButtonDown( UINT nFlags, CPoint pt );

};
