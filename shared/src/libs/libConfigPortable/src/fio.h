// ----------------------------------------------------------------------------
#ifndef FIOH
#define FIOH
// ----------------------------------------------------------------------------
// File input output abstraction
// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
// ----------------------------------------------------------------------------

typedef void* FIOHANDLER;

// ----------------------------------------------------------------------------
// Function callback prototypes
// ----------------------------------------------------------------------------
typedef FIOHANDLER (*TFIOOpenFunction)(const char* _szFilename,const char* _szMode);
typedef void (*TFIOCloseFunction)(FIOHANDLER _pFD);
typedef unsigned int (*TFIOReadFunction)(void* _pData,unsigned int _uiSize,unsigned int _uiElems,FIOHANDLER _pFD);
typedef unsigned int (*TFIOWriteFunction)(void* _pData,unsigned int _uiSize,unsigned int _uiElems,FIOHANDLER _pFD);
typedef unsigned int (*TFIOErrorFunction)(FIOHANDLER _pFD);
typedef void (*TFIOClearErrorFunction)(FIOHANDLER _pFD);

// ----------------------------------------------------------------------------
// Setup callbacks functions
// ----------------------------------------------------------------------------
void FIOSetOpenFunction(TFIOOpenFunction _pFunction);
void FIOSetCloseFunction(TFIOCloseFunction _pFunction);
void FIOSetReadFunction(TFIOReadFunction _pFunction);
void FIOSetWriteFunction(TFIOWriteFunction _pFunction);
void FIOSetErrorFunction(TFIOErrorFunction _pFunction);
void FIOSetClearErrorFunction(TFIOClearErrorFunction _pFunction);

// ----------------------------------------------------------------------------

FIOHANDLER FIOOpen(const char* _szFilename,const char* _szMode);
void FIOClose(FIOHANDLER _pFD);
unsigned int FIORead(void* _pData,unsigned int _uiSize,unsigned int _uiElems,FIOHANDLER _pFD);
unsigned int FIOWrite(void* _pData,unsigned int _uiSize,unsigned int _uiElems,FIOHANDLER _pFD);
unsigned int FIOError(FIOHANDLER _pFD);
void FIOClearError(FIOHANDLER _pFD);
// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif /* __cplusplus */

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------

