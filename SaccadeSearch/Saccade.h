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
  float LatentTime;
  float StimulCode;
  int Points;
  int chan;
  int rec;
public:
  float AmplitudeX();
  float AmplitudeY();
};
