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
  float GetYFromX(int chan, int rec,float x);
  int XRealToPoint(float x);

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
  void DrawStimuls();
  void DrawApproximation();
  afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
  void AppendFormatFloat(CString* string,float f,int maxchars);

	CDC *dc;					//”казатель на контекст отображени€ окна

	//ќбъ€вление инструментов рисовани€ в окне
	CPen Pen,*OldPen;			//ѕеро
	CBrush Brush,*OldBrush;		// исть
	LOGFONT LogFont;			//Cтруктура параметров шрифта
	CFont Font,*OldFont;		//Ўрифт

	//объ€вление идентификаторов структуры цвета
	COLORREF ColorPen,ColorBrush;//,ColorText;
  
	CDC	DC;							//объ€вл€ем объект контекста отображени€
	CBitmap BmpDC;					//объ€вл€ем объект
	CBitmap *OldBmpDC;				//объ€вл€ем указатель на объект Cbitmap окна
  COLORREF* LinePens;

	RECT ClientRect;			//объ€вление индентификатора пр€моугольника клиентской части окна
	int x,y;					// оординаты вывода в окно
  int created,changed,grid,held,points,MouseAction;
  CPoint MovePoint;
  float RulerPointX;
  float RulerPointY;
 int CurSaccade;

protected:
	DECLARE_MESSAGE_MAP()
  afx_msg void OnLButtonDown( UINT nFlags, CPoint pt );
  afx_msg void OnLButtonUp( UINT nFlags, CPoint pt );
  afx_msg void OnRButtonDown( UINT nFlags, CPoint pt );

};
