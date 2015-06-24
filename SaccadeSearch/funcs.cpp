#include "stdafx.h"
#include "Conandata.h"
#include <sys/stat.h> 
using namespace std;

void AddLog(CEdit*log,CString z)
{
  CString logstr;
  log->GetWindowText(logstr);
  logstr.Append(z);
  logstr.Append(_T("\r\n"));
  log->SetWindowText(logstr);
}

ConanData* ReadConanFile(TCHAR* fname, CEdit* log)
{
 
  //CString z,logstr;

  FILE * pFile;
  long lSize;
  byte * buffer;
  size_t result;
  _wfopen_s(&pFile, fname, _T("r+b"));
  if (pFile==NULL)
  {
	  MessageBox(NULL, _T("�� ���� ��������� ����!"), _T("������"), MB_OK);
	  return NULL;
  }
  //copy file to buffer for quicker operations
  // obtain file size:
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  rewind (pFile);
  buffer = new byte[lSize];
  result = fread (buffer,1,lSize,pFile);
  fclose(pFile);
  //z.Format("File size: %d",lSize);
  //AddLog(log,z);
  
  //can't copy memory directly into structure because it may not be compacted with some compilers's settings.
  ConanHeader* data=new ConanHeader();

  unsigned int pos=0; 
  memcpy_s(data->cona,sizeof(data->cona),&buffer[0],sizeof(data->cona));
  pos+=sizeof(data->cona);
  memcpy_s(data->head,sizeof(data->head),&buffer[pos],sizeof(data->head));
  pos+=sizeof(data->head);
  memcpy_s(data->chNames,sizeof(data->chNames),&buffer[pos],sizeof(data->chNames));
  pos+=sizeof(data->chNames);
  memcpy_s(&data->TimeData,sizeof(data->TimeData),&buffer[pos],sizeof(data->TimeData));
  pos+=sizeof(data->TimeData);
  memcpy_s(&data->Arec,sizeof(data->Arec),&buffer[pos],sizeof(data->Arec));
  pos+=sizeof(data->Arec);
  pos+=2;
  memcpy_s(&data->nChan,sizeof(data->nChan),&buffer[pos],sizeof(data->nChan));
  pos+=sizeof(data->nChan);
  memcpy_s(&data->freq,sizeof(data->freq),&buffer[pos],sizeof(data->freq));
  pos+=sizeof(data->freq);
  memcpy_s(&data->discrExist,sizeof(data->discrExist),&buffer[pos],sizeof(data->discrExist));
  pos+=sizeof(data->discrExist);
  memcpy_s(&data->nRec,sizeof(data->nRec),&buffer[pos],sizeof(data->nRec));
  pos+=sizeof(data->nRec);
  memcpy_s(data->nilCalibr,sizeof(data->nilCalibr),&buffer[pos],sizeof(data->nilCalibr));
  pos+=sizeof(data->nilCalibr);
  memcpy_s(data->maxCalibr,sizeof(data->maxCalibr),&buffer[pos],sizeof(data->maxCalibr));
  pos+=sizeof(data->maxCalibr);


  memcpy_s(data->coord,sizeof(data->coord),&buffer[pos],sizeof(data->coord));
  pos+=sizeof(data->coord);

  pos+=11;
  memcpy_s(data->sens,sizeof(data->sens),&buffer[pos],sizeof(data->sens));
  pos+=sizeof(data->sens);

  pos=data->Arec;
  
  /*z.Format("Arec: %d \r\n",data->Arec);
  logstr.Append(z);
  z.Format("nChan: %d\r\n",data->nChan);
  logstr.Append(z);
  z.Format("freq: %d\r\n",data->freq);
  logstr.Append(z);
  z.Format("nRec: %d\r\n",data->nRec);


  z.Format("coord [0,0]: %d\r\n",data->coord[0][0]);
  logstr.Append(z);
  z.Format("coord [0,1]: %d\r\n",data->coord[0][1]);
  logstr.Append(z);
  z.Format("coord [1,0]: %d\r\n",data->coord[1][0]);
  logstr.Append(z);
  z.Format("coord [31,2]: %d\r\n",data->coord[31][2]);
  logstr.Append(z);
  z.Format("sens 0: %d\r\n",data->sens[0]);
  logstr.Append(z);
  z.Format("sens last(31): %d\r\n",data->sens[31]);
  logstr.Append(z);
  AddLog(log,logstr);*/


  float *** Eeg=new float** [data->nRec];
  __int32 * TimeCreating=new __int32[data->nRec];
  unsigned __int16* NDataReal2=new unsigned __int16[data->nRec];
  unsigned __int16* NDataReal=new unsigned __int16[data->nRec];
  unsigned __int16* NDataRaw=new unsigned __int16[data->nRec];
	StimulSignal** Discr=new StimulSignal*[data->nRec];

  for (int i=0;i<data->nRec;i++)
  {
    memcpy_s(&NDataRaw[i],sizeof(__int16),&buffer[pos],sizeof(__int16));
    pos+=sizeof(__int16);
    memcpy_s(&NDataReal[i],sizeof(__int16),&buffer[pos],sizeof(__int16));
    pos+=sizeof(__int16);

    //logstr.Format("recNDataRaw: %d\r\nrecNDataReal: %d",NDataRaw[i],NDataReal[i]);
    //AddLog(log,logstr);
    
    Eeg[i]=new float*[data->nChan];
	  for (int j=0;j<data->nChan;j++)
	  {
      Eeg[i][j]=new float[NDataReal[i]];
      for(int k=0;k<NDataReal[i];k++)
      {
        __int16 xxx=*(__int16*)&buffer[pos];
        Eeg[i][j][k]=(*(__int16*)&buffer[pos]-(float)data->nilCalibr[j])/(float)data->maxCalibr[j]*(float)data->sens[j];
        pos+=sizeof(__int16);
      }
	  }
	  
	  // discrete channels for the current record: 
	  if (data->discrExist==1)
	  {
		  Discr[i]=new StimulSignal[NDataReal[i]];
      for(int k=0;k<NDataReal[i];k++)
      {
        memcpy_s(&Discr[i][k].Elder,sizeof(__int8),&buffer[pos],sizeof(__int8));
        pos+=1;
        memcpy_s(&Discr[i][k].Littler,sizeof(__int8),&buffer[pos],sizeof(__int8));
        pos+=1;
      }
	  }
	  else
	  {
		  Discr[i] = new StimulSignal[data->nChan];
		  for(unsigned int k=0;k<data->nChan;k++)
      {
			  Discr[i][k].Littler=0;
			  Discr[i][k].Elder=0;
      }
	  }
///
//test
    /*for(int v=0;v<NDataReal[i];v++)
    {
      if(v>200)
        break;
      if(i>0)
        break;
      z.Format("Discr rec %d num %d: %x %x \r\n",i,v,Discr[i][v].Elder,Discr[i][v].Littler);
      AddLog(log,z);
    }*/

//
     /* % skip blank space ( which may result from deleting some data when 
		 % editing the EEG in CONAN ) : */
    pos+=2*(NDataRaw[i]-NDataReal[i])*data->nChan;//number of 16bit integers to skip

    memcpy_s(&TimeCreating[i], sizeof(__int32), &buffer[pos],sizeof(__int32));
    pos+=sizeof(TimeCreating[i]);

    memcpy_s(&NDataReal2[i],sizeof(__int16),&buffer[pos],sizeof(__int16));
    pos+=sizeof(NDataReal2[i]);
    
    //z.Format("recNDataReal2: %d ",NDataReal2[i]);
    //AddLog(log,z);

    
    //z.Format("Bytes processed: %d \r\n",pos);
    //AddLog(log,z);

  }
  delete[] buffer;

  ConanData* all=new ConanData();
  all->Header=data;
  all->Eeg=Eeg;
  all->TimeCreating=TimeCreating;
  all->NDataReal2=NDataReal2;
  all->NDataReal=NDataReal;
  all->NDataRaw=NDataRaw;
  all->Discr=Discr;

  return all;
}


bool FileExists(CString strFilename) {
	struct _stat stFileInfo;
	bool blnReturn;
	int intStat;

  // Attempt to get the file attributes
	intStat = _wstat(strFilename, &stFileInfo);
  if (intStat == 0) {
    // We were able to get the file attributes
    // so the file obviously exists.
    blnReturn = true;
  } else {
    // We were not able to get the file attributes.
    // This may mean that we don't have permission to
    // access the folder which contains this file. If you
    // need to do that level of checking, lookup the
    // return values of _wstat which will give you
    // more details on why _wstat failed.
    blnReturn = false;
  }
  
  return(blnReturn);
}