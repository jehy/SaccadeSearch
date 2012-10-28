#pragma once

class Saccade
{
public:
  Saccade(void);
  ~Saccade(void);
  float BeginX;
  float BeginY;
  float EndX;
  float EndY;
  //bool Sign;
  float TimeFromStimul;
  float TimeFromCal;
  unsigned __int8 StimulCode;
  int Points;
  int chan;
  int rec;
public:
  float AmplitudeX();
  float AmplitudeY();
};
