#include "StdAfx.h"
#include "Eegraph.h"
#include "Conandata.h"
#include <math.h>;
extern ConanData* Conan;
IMPLEMENT_DYNAMIC(Eegraph, CWnd)

BOOL Eegraph::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
  HCURSOR CursorRuler = ::LoadCursor(NULL,IDC_CROSS);
  HCURSOR CursorMove = ::LoadCursor(NULL,IDC_HAND);
  //works only with static link to mfc
  //HCURSOR CursorZoom = AfxGetApp()->LoadCursor(AFX_IDC_MAGNIFY);
  HCURSOR CursorZoom = ::LoadCursor(NULL,IDC_ARROW);


      //if ( m_ChangeCursor )
  if(this->MouseAction==0)
  {
      ::SetCursor(CursorMove);
      return TRUE;
  }
  if(this->MouseAction==1)
  {
    ::SetCursor(CursorZoom);
      return TRUE;
  }
  if(this->MouseAction==2)
  {
      ::SetCursor(CursorRuler);
      return TRUE;
  }

      return Eegraph::OnSetCursor(pWnd, nHitTest, message);
}


Eegraph::Eegraph()
{
	ColorPen=RGB(0,0,0);
	ColorBrush=RGB(255,255,255);
	//ColorText=RGB(50,131,35);
	{//Задание параметров структуры шрифта по умолчанию
		memset(&LogFont,0,sizeof(LogFont));
		LogFont.lfHeight=15; 
		LogFont.lfWidth=0; 
		LogFont.lfEscapement=0; 
		LogFont.lfOrientation=0; 
		LogFont.lfWeight=FW_BOLD; 
		LogFont.lfItalic=0; 
		LogFont.lfUnderline=0; 
		LogFont.lfStrikeOut=0; 
		LogFont.lfCharSet=45; 
		LogFont.lfOutPrecision=3; 
		LogFont.lfClipPrecision=2; 
		LogFont.lfQuality=1; 
		LogFont.lfPitchAndFamily=49; 
//		strcpy(LogFont.lfFaceName,"Courier New");
	}
	x=0;
	y=0;
  created=0;
  changed=0;
  grid=0;
  held=0;
  points=0;
  MouseAction=0;
  RulerPointX=0;
  RulerPointY=0;
  MovePoint.x=0;
  MovePoint.y=0;
}

Eegraph::~Eegraph()
{
  if(created)
    this->_DeleteDC();
  DestroyWindow();
}

void Eegraph::hold()
{
  held=1;
}

void Eegraph::unhold()
{
  held=0;
  if(this->changed)
    this->WinPaint();
}

void Eegraph::_CreateDC()
{

	dc=GetDC();						//Получить указатель на контекст вывода
	int nDC=0;
	nDC=DC.CreateCompatibleDC(dc);	//создаем контекст отображения
	if(nDC == 0){
		return;
	}

	GetClientRect(&ClientRect);													//Определение размеров прямоугольника контекста
	BmpDC.CreateCompatibleBitmap(dc, ClientRect.right+1, ClientRect.bottom+1);	//создаем объект CBitmap
	OldBmpDC=DC.SelectObject(&BmpDC);											//подсоединяем его к контексту отображения

	//Создание и подключение к контексту DC инструментов рисования
	DC.SetMapMode(MM_TEXT);			//Задать режим координат
	DC.SetBkMode(1);				//Режим вывода текста
	//DC.SetTextColor(ColorText);		//Цвет текста

	//Инициализация инструментов рисования
	Pen.CreatePen(0,1,ColorPen);			//Создать перо
	Brush.CreateSolidBrush(ColorBrush);		//Создать кисть
	Font.CreateFontIndirect(&LogFont);		//Создать шрифт

	//Загрузка инструментов рисования в контекст окна
	OldPen=DC.SelectObject(&Pen);
	OldBrush=DC.SelectObject(&Brush);
	OldFont=DC.SelectObject(&Font);


}

void Eegraph::OnLButtonDown(UINT nFlags, CPoint point)
{
  if(MouseAction==1)//zoom
    this->GetParent()->PostMessageA(WM_LBUTTONDOWN,666,MAKELPARAM(point.x,point.y));
  else if(MouseAction==0)//move
  {
    MovePoint=point;  
  }
  else if(MouseAction==2)//ruler
  {
    //make it into real coords
    float xp=XToRealCoords(point.x);

    int rec=Conan->CurRec;
    int chan=Conan->CurChannel;
    float x=0;
    for(int n=0;n<Conan->NDataReal[rec];n++)
    {
      
      if(n+1>=Conan->NDataReal[rec])//it was last point
      {
        this->RulerPointX=x;
        this->RulerPointY=Conan->Eeg[rec][chan][n];
        break;
      }

      float x2=x+(float)Conan->Header->freq/1000;//next point value

      if(x2>=xp)
      {
        //not last, check the nearest
        float a1=abs(xp-x);
        float a2=abs(xp-x2);
        if(a1>a2)
        {
          this->RulerPointX=x2;
          this->RulerPointY=Conan->Eeg[rec][chan][n+1];
        }
        else
        {
          this->RulerPointX=x;
          this->RulerPointY=Conan->Eeg[rec][chan][n];
        }

        break;
      }
      x+=(float)Conan->Header->freq/1000;
    }
    this->changed=1;
    this->WinPaint();
    
    this->GetParent()->PostMessageA(WM_RBUTTONDOWN,668,NULL);
    //float y=(Win->ClientRect.bottom-point.y)/Conan->ZoomY-Conan->YOffset;
    //float xm
    //while()
  }
}

void Eegraph::OnRButtonDown(UINT nFlags, CPoint point)
{
  if(MouseAction==1)//zoom out
    this->GetParent()->PostMessageA(WM_RBUTTONDOWN,666,MAKELPARAM(point.x,point.y));
}

void Eegraph::OnLButtonUp(UINT nFlags, CPoint point)
{
  if(MouseAction==0)//move
  {
    if(MovePoint.x||MovePoint.y)
    {
      MovePoint.x-=point.x;
      MovePoint.y-=point.y;
      //if(abs(MovePoint.x)>10 || abs(MovePoint.y)>10)
        this->GetParent()->PostMessageA(WM_RBUTTONDOWN,667,MAKELPARAM(MovePoint.x,MovePoint.y));
      MovePoint.x=0;
      MovePoint.y=0;
    }
  }
}

void Eegraph::ClearDC()
{
	DC.Rectangle(0,0,ClientRect.right+1,ClientRect.bottom+1);	//закрашиваем текущей кистью
}

void Eegraph::DrawSaccades()
{
  if(0)
    return;
  COLORREF SaccadeColor=RGB(10,10,190);
  Pen.DeleteObject();
	Pen.CreatePen(0,2,SaccadeColor);			//Создать перо
	OldPen=DC.SelectObject(&Pen);

  for(int i=0;i<Conan->Saccades.size();i++)
  {
    if(Conan->Saccades.at(i)->rec!=Conan->CurRec || Conan->Saccades.at(i)->chan!=Conan->CurChannel)
      continue;
    DC.MoveTo(XToScreenCoords(Conan->Saccades.at(i)->BeginX),YToScreenCoords(Conan->Saccades.at(i)->BeginY));
    DC.LineTo(XToScreenCoords(Conan->Saccades.at(i)->EndX),YToScreenCoords(Conan->Saccades.at(i)->EndY));
  }
}

float Eegraph::XToScreenCoords(float XRealCoords)
{
  return (XRealCoords+Conan->XOffset)*Conan->ZoomX;
}

float Eegraph::XToRealCoordsFromPoint(float point)
{
  return point*(float)Conan->Header->freq/1000;
}
float Eegraph::YToScreenCoords(float YRealCoords)
{//turn coords upside-down
    return ClientRect.bottom-(YRealCoords+Conan->YOffset)*Conan->ZoomY;
}

float Eegraph::XToRealCoords(float XScreenCoords)
{
  return XScreenCoords/Conan->ZoomX-Conan->XOffset;
}
float Eegraph::YToRealCoords(float YScreenCoords)
{
  return (ClientRect.bottom-YScreenCoords)/Conan->ZoomY-Conan->YOffset;
}
float Eegraph::RealGraphWidth()
{
  return ClientRect.right/Conan->ZoomX;
}
float Eegraph::ScreenGraphWidth()
{
  return ClientRect.right;

}
float Eegraph::RealGraphHeight()
{
  return ClientRect.bottom/Conan->ZoomY;
}
float Eegraph::ScreenGraphHeight()
{
  return ClientRect.bottom;
}


void Eegraph::DrawCoord()
{
  //int grid=1;
  float coord;
  CString out;

  COLORREF GridColor=RGB(10,10,10);
  Pen.DeleteObject();
	Pen.CreatePen(0,1,GridColor);			//Создать перо
	OldPen=DC.SelectObject(&Pen);


  COLORREF ColorText=RGB(200,40,120);
  DC.SetTextColor(ColorText);

	for(float x=0;x<ScreenGraphWidth();x+=50)//every 50 px
  {
    coord=XToRealCoords(x);
    out="";
    AppendFormatFloat(&out,coord,3);
    DC.TextOutA(x+2,ScreenGraphHeight()-12,out.GetBuffer(),out.GetLength());
    if(grid)
    {
      DC.MoveTo(x,0);
      DC.LineTo(x,ScreenGraphHeight());
    }
  }

  
  ColorText=RGB(100,40,200);
  DC.SetTextColor(ColorText);

	for(float y=ScreenGraphHeight()-50;y>=0;y-=50)
  {
    coord=YToRealCoords(y);
    out="";
    AppendFormatFloat(&out,coord,5);
    DC.TextOutA(0+2,y-12,out.GetBuffer(),out.GetLength());
    
    if(grid)
    {
      DC.MoveTo(0,y);
      DC.LineTo(ScreenGraphWidth(),y);
    }
  }

    
}

void Eegraph::DrawRuler()
{
  if((RulerPointX==0)&&(RulerPointY==0))
    return;
  COLORREF RulerColor=RGB(220,40,40);
  Pen.DeleteObject();
	Pen.CreatePen(0,2,RulerColor);			//Создать перо
	OldPen=DC.SelectObject(&Pen);

  float xz=XToScreenCoords(this->RulerPointX);
  DC.MoveTo(floor(xz),0);
  DC.LineTo(floor(xz),ScreenGraphHeight());
}

void Eegraph::DrawDC()
{
  this->ClearDC();
  this->DrawCoord();
  this->DrawRuler();
  float x=0;
  float y=0;
  float xz=0;
  float yz=0;
  CString p;

  COLORREF LinePen=RGB(100,200,10);

  CPen PointPen;
  PointPen.DeleteObject();
  PointPen.CreatePen(0,2,RGB(0,0,0));			//Создать перо

  Pen.DeleteObject();
	Pen.CreatePen(0,2,LinePen);			//Создать перо
	OldPen=DC.SelectObject(&Pen);

  int chan=Conan->CurChannel;
  {
    bool nextbreak=false;
    //for(int rec=0;rec<Conan->Header->nRec;rec++)
    int rec=Conan->CurRec;
    {
      //move to first point

      y=Conan->Eeg[rec][chan][0];
      x=0;

      yz=YToScreenCoords(y);
      xz=XToScreenCoords(x);

      DC.MoveTo(floor(xz),floor(yz));
      /*if(points)
      {
        DC.SetDCPenColor(PointPen);
        DC.Rectangle(xz-1,yz-1,xz+1,yz+1);
        //DC.SetDCPenColor(LinePen);
        p.Format("%4.3f; %4.3f;",x,y);
        DC.TextOutA(floor(xz),floor(yz),p.GetBuffer(),p.GetLength());
      }*/

      for(int n=0;n<Conan->NDataReal[rec];n++)//begin from second point
      {
        y=Conan->Eeg[rec][chan][n];
        yz=YToScreenCoords(y);
        x=XToRealCoordsFromPoint(n);
        xz=XToScreenCoords(x);
        if(n>0)
          DC.LineTo(floor(xz),floor(yz));
        
        if(points)
        {
          //DC.SetDCPenColor(PointPen);
          DC.SelectObject(&PointPen);
          DC.Rectangle(xz-1,yz-1,xz+1,yz+1);
          DC.SelectObject(&Pen);
          //DC.SetDCPenColor(LinePen);
          p.Format("%4.3f; %4.3f;",x,y);
          DC.TextOutA(floor(xz),floor(yz),p.GetBuffer(),p.GetLength());
        }
        if(nextbreak)
          break;
        if(floor(xz)>=ClientRect.right)
          nextbreak=true;
      }
    }
  }

  this->DrawSaccades();
}

void Eegraph::AppendFormatFloat(CString* string,float f,int maxchars)
{
  //also need to calculate if there woud be "." and float remainder
  if(f<0)
    maxchars--;//one place for sign
  int i=0;
  float t=f;
  while(t>=1)
  {
    t/=10;
    i++;
  }
  if(i==0)//one symbol always used, even if it is "0".
    i=1;
  if(i>maxchars)
    maxchars=i;
  int pre=maxchars-i;
  CString format;
  format.Format("%d.%d",i,pre);
  format.Insert(0,"%");
  format.Append("f");
  CString tmp;
  tmp.Format(format,f);
  string->Append(tmp);
}
void Eegraph::_DeleteDC()
{
	DC.SelectObject(OldBmpDC);//восстанавливаем объект Cbitmap в контексте окна

	//восстановление старых инструментов рисования
	DC.SelectObject(OldPen);
	DC.SelectObject(OldBrush);
	DC.SelectObject(OldFont);

	//удаление новых инструментов рисования
	Pen.DeleteObject();
	Brush.DeleteObject();
	Font.DeleteObject();
	DC.DeleteDC();//удаляем контекст
	BmpDC.DeleteObject();//удаляем объект CBitmap

}

void Eegraph::PaintDC()
{
dc->BitBlt(0,0, ClientRect.right ,ClientRect.bottom,&DC,0,0,SRCCOPY);//копируем содержимое созданного контекста в контекст окна
}
void Eegraph::WinPaint()
{
  if(held)
    return;
  if(!created)
  {
    this->_CreateDC();
    created=1;
  }
  if(created&&!changed)
  {
    this->PaintDC();
    return;
  }
  if(changed)
  {
    this->DrawDC();
    this->PaintDC();
    changed=0;
  }
}



BEGIN_MESSAGE_MAP(Eegraph, CWnd)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_RBUTTONDOWN()
ON_WM_SETCURSOR()
END_MESSAGE_MAP()

