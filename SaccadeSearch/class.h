#pragma once
#include "stdafx.h"

class MagicArray
{
public:
  unsigned __int16 **rptr;
  size_t bufsize;
  unsigned __int16 *aptr;
  MagicArray(int nrows,int ncols,void* from);
  ~MagicArray();
};