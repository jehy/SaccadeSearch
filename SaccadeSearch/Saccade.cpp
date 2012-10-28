#include "StdAfx.h"
#include "Saccade.h"

Saccade::Saccade(void)
{
  BeginX=0;
  BeginY=0;
  EndX=0;
  EndY=0;
  //Sign=0;
  TimeFromCal=0;
  TimeFromStimul=0;
  StimulCode=0;
  Points=0;
  chan=0;
  rec=0;
}

Saccade::~Saccade(void)
{
}

float Saccade::AmplitudeX()
{
  return this->EndX-this->BeginX;
}

float Saccade::AmplitudeY()
{
  return this->EndY-this->BeginY;
}