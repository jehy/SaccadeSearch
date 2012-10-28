#pragma once
#include "stdafx.h"
#include "class.h"

MagicArray::MagicArray(int nrows,int ncols,void* from)
{
    bufsize=nrows * ncols * sizeof(unsigned __int16);
    aptr = (unsigned __int16*)malloc(bufsize);
    if (aptr == NULL)
    {
        MessageBox(NULL,"Failure to allocate room for the array","Error",MB_OK);
        exit(0);
    }

    memcpy_s(aptr,bufsize,from,bufsize);
    /* next we allocate room for the pointers to the rows */

    rptr = (unsigned __int16**)malloc(nrows * sizeof(unsigned __int16 *));
    if (rptr == NULL)
    {
        MessageBox(NULL,"Failure to allocate room for pointers","Error",MB_OK);
        exit(0);
    }

    /* and now we 'point' the pointers */

    for (int k = 0; k < nrows; k++)
    {
        rptr[k] = aptr + (k * ncols);
    }
}

MagicArray::~MagicArray()
{
  free(rptr);
  free(aptr);
}