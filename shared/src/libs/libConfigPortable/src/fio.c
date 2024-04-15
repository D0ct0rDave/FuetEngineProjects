// ----------------------------------------------------------------------------
#include "fio.h"
#include <stdio.h>

// ----------------------------------------------------------------------------
static struct
{
	TFIOOpenFunction 		Open;
	TFIOCloseFunction 		Close;
	TFIOReadFunction 		Read;
	TFIOWriteFunction 		Write;
	TFIOErrorFunction 		Error;
	TFIOClearErrorFunction 	ClearError;
} FIO = 
{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

// ----------------------------------------------------------------------------
// Setup callbacks functions
// ----------------------------------------------------------------------------
void FIOSetOpenFunction(TFIOOpenFunction _pFunction)
{
	FIO.Open = _pFunction;	
}

void FIOSetCloseFunction(TFIOCloseFunction _pFunction)
{
	FIO.Close = _pFunction;	
}

void FIOSetReadFunction(TFIOReadFunction _pFunction)
{
	FIO.Read = _pFunction;
}

void FIOSetWriteFunction(TFIOWriteFunction _pFunction)
{
	FIO.Write = _pFunction;	
}

void FIOSetErrorFunction(TFIOErrorFunction _pFunction)
{
	FIO.Error = _pFunction;	
}

void FIOSetClearErrorFunction(TFIOClearErrorFunction _pFunction)
{
	FIO.ClearError = _pFunction;
}
// ----------------------------------------------------------------------------
FIOHANDLER FIOOpen(const char* _szFilename,const char* _szMode)
{
	if (FIO.Open)
		return(FIO.Open(_szFilename,_szMode));	
	else
		return(NULL);
}
// ----------------------------------------------------------------------------
void FIOClose(FIOHANDLER _pFD)
{
	if (FIO.Close)
		FIO.Close(_pFD);
}
// ----------------------------------------------------------------------------
unsigned int FIORead(void* _pData,unsigned int _uiSize,unsigned int _uiElems,FIOHANDLER _pFD)
{
	if (FIO.Read)
		return(FIO.Read(_pData,_uiSize,_uiElems,(FILE*)_pFD));
	else
		return(0);
}
// ----------------------------------------------------------------------------
unsigned int FIOWrite(void* _pData,unsigned int _uiSize,unsigned int _uiElems,FIOHANDLER _pFD)
{
	if (FIO.Write)
		return(FIO.Write(_pData,_uiSize,_uiElems,(FILE*)_pFD));
	else
		return(0);
}
// ----------------------------------------------------------------------------
unsigned int FIOError(FIOHANDLER _pFD)
{
	if (FIO.Error)
		return(FIO.Error(_pFD));
	else
		return(0);
}
// ----------------------------------------------------------------------------
void FIOClearError(FIOHANDLER _pFD)
{
	if (FIO.ClearError)
		FIO.ClearError(_pFD);
}
// ----------------------------------------------------------------------------
