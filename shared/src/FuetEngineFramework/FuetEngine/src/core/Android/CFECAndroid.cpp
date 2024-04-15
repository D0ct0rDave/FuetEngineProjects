// ----------------------------------------------------------------------------
/*! \class CFECoundMgr
 *  \brief Sound Manager Class Definition
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (TARGETPLATFORM == USE_ANDROID)
// ----------------------------------------------------------------------------
#include "../CFECore.h"
#include <stdarg.h>
#include "CFECGlobals.h"

// for native asset manager
#include <sys/types.h>
#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "jnihelper.h"
// ----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
// ----------------------------------------------------------------------------
void AndroidFuetEngineInit()
{
    CFECore::Log::Print("lolo paco");

    CFEString sLocal = GetLocale();
    CFEString sLang  = (sLocal.uiLen() >= 2)?sLocal.SubString(0,2):"";
    CFEString sReg   = (sLocal.uiLen() >  2)?sLocal.SubString(3,2):"";

    CFECore::Local::SetLocalRegionID(LRID_COMMON);

    if (sLang == "es")
    {
        CFECore::Local::SetLocalID(LID_ES);
    }
    else if (sLang == "en")
    {
        CFECore::Local::SetLocalID(LID_EN);
    }
    else if (sLang == "fr")
    {
        CFECore::Local::SetLocalID(LID_FR);
    }
    else if (sLang == "it")
    {
        CFECore::Local::SetLocalID(LID_IT);
    }
    else if (sLang == "de")
    {
        CFECore::Local::SetLocalID(LID_DE);
    }
    else if (sLang == "jp")
    {
        CFECore::Local::SetLocalID(LID_JP);
    }
    else
    {
        CFECore::Local::SetLocalID(LID_EN);
    }

    if (sLang != "")
        CFECore::Log::Print("Language set to %s",sLang.szString());
    if (sReg != "")
        CFECore::Log::Print("Region %s",sReg.szString());
}
// ----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif
// ----------------------------------------------------------------------------
namespace CFECore
{
	namespace TRC
	{

		static TOpenMessageBoxFunc 	OpenMessageBoxFunc = NULL;
		static TMessageBoxStepFunc	iMessageBoxStepFunc= NULL;
		static TCloseMessageBoxFunc bCloseMessageBoxFunc= NULL;
	}

	namespace Log
	{
		static TCoreLogFunc			LogFuncCallback = NULL;
	}
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Assert functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void CFECore::Check(FEBool _bCondition,const CFEString& _sMessage)
{
    /*
	if (_bCondition != true)
    {
        printf(_sMessage.szString());
        MessageBox(0,_sMessage.szString(), "Error", MB_OK);
        exit(0);
    }
	*/
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Timing functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
FETick CFECore::oGetEngineTicks()
{
	return( GetSystemMilis() );
}
// ------------------------------------------------------------------------
/// Retrieves time in seconds of the given system tics.
FEReal CFECore::rGetTickTime(FETick _oTicks)
{
	return((FEReal)_oTicks/1000.0f);
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/// Logging Functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void CFECore::Log::Print(const char * format, va_list arg)
{
	#ifndef MASTER_RELEASE_VERSION
	char* szProcString = CFECore::String::szFormatString(format,arg);

	if (LogFuncCallback != NULL)
	{
		LogFuncCallback(szProcString);
	}
	else
	{
        __android_log_print(ANDROID_LOG_VERBOSE,"FuetEngine","%s",szProcString);
	}
    #endif
}
//-----------------------------------------------------------------------------
void CFECore::Log::Print(const char *fmt,...)
{
	#ifndef MASTER_RELEASE_VERSION
		va_list argptr;
		va_start(argptr,fmt);
		Print(fmt,argptr);
		va_end  (argptr);
	#endif
}
// ----------------------------------------------------------------------------
void CFECore::Log::SetSystemLogFunc(TCoreLogFunc _pLogFunc)
{
	LogFuncCallback = _pLogFunc;
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// String functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int CFECore::String::iStrICmp(const char* _szA,const char* _szB)
{
    return(0); // _stricmp(_szA,_szB) );
}
// ----------------------------------------------------------------------------
int CFECore::String::iStrNICmp(const char* _szA,const char* _szB,uint _uiChars)
{
    return(0); // _strnicmp(_szA,_szB,_uiChars) );
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// File management stuff.
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/// Opens a given file in the given access mode and retrieves its file handler.
FEHandler CFECore::File::hOpen(const CFEString& _sFilename, EFEFileOpenMode _eMode)
{
    AAssetManager* mgr = GetAssetManager();

    char* szOpenMode = NULL;
    switch (_eMode)
    {
        case FOM_READ:
        break;

        case FOM_WRITE:
        return(false);
        break;

        default:
        return(false);
    }

	CFECore::Log::Print("Loading %s: ",_sFilename.szString());
	if (CFECore::Local::bIsAutoLocEnabled())
	{
        AAsset* asset = NULL;
		CFEString sLocalFilename;

		// then try localized version of the file.
		sLocalFilename = CFECore::Local::sGetLocalID() + CFEString("/") + _sFilename;

        CFECore::Log::Print("hOpen 4");
		asset = AAssetManager_open(mgr, sLocalFilename.szString(), AASSET_MODE_UNKNOWN);
		CFECore::Log::Print("hOpen 5");

		if (asset != NULL)
		{
			CFECore::Log::Print("Ok (%s)\n",CFECore::Local::sGetLocalID().szString());
			return(asset);
		}

		// first try the non-localized or common version of the file.
		sLocalFilename = CFECore::Local::sGetLocalID(LID_COMMON) + CFEString("/") + _sFilename;
        asset = AAssetManager_open(mgr, sLocalFilename.szString(), AASSET_MODE_UNKNOWN);

		if (asset != NULL)
			CFECore::Log::Print("Ok (%s)\n",CFECore::Local::sGetLocalID(LID_COMMON).szString());
		else
			CFECore::Log::Print("Not found\n");

		return(asset);
	}
	else
	{
        AAsset* asset = AAssetManager_open(mgr, _sFilename.szString(), AASSET_MODE_UNKNOWN);

		if (asset != NULL)
			CFECore::Log::Print("Ok\n");
		else
			CFECore::Log::Print("Not found\n");

		return(asset);
	}

    CFECore::Log::Print("hOpen 120012");
	return(NULL);
}
// ----------------------------------------------------------------------------
/// Closes the file associated to the given handler.
void CFECore::File::Close(FEHandler _hFile)
{
    AAsset_close((AAsset*)_hFile);
}
// ----------------------------------------------------------------------------
/// Reads an amount data from the file storing at the given buffer.
uint CFECore::File::uiRead(FEHandler _hFile,void* _pData, uint _uiSize)
{
	return( AAsset_read ((AAsset*)_hFile,_pData,_uiSize) );
}
// ----------------------------------------------------------------------------
/// Writes the amount of data from the given buffer to the file.
uint CFECore::File::uiWrite(FEHandler _hFile,void* _pData, uint _uiSize)
{
	return(0);
}
// ----------------------------------------------------------------------------
/// Sets the read/write position of the file.
int CFECore::File::iSeek(FEHandler _hFile,int _iOffset, EFEFileSeekMode _eMode)
{
    switch (_eMode)
    {
        case FSM_SET:    return(AAsset_seek((AAsset*)_hFile,_iOffset,SEEK_SET));
        case FSM_CUR:    return(AAsset_seek((AAsset*)_hFile,_iOffset,SEEK_CUR));
        case FSM_END:    return(AAsset_seek((AAsset*)_hFile,_iOffset,SEEK_END));
    }
	return(0);
}
// ----------------------------------------------------------------------------
/// Retrieves the current file read/write position.
uint CFECore::File::uiPos(FEHandler _hFile)
{
	return( AAsset_getLength((AAsset*)_hFile) - AAsset_getRemainingLength((AAsset*)_hFile) );
}
// ----------------------------------------------------------------------------
/// Retrieves the total file length
uint CFECore::File::uiLen(FEHandler _hFile)
{
	return( AAsset_getLength((AAsset*)_hFile) );
}
// ----------------------------------------------------------------------------
/// Reads the contents of a file and returns it as a memory buffer.
FEPointer CFECore::File::pReadFile(const CFEString& _sFilename,uint* _puiSize)
{
	FEHandler hFile = hOpen(_sFilename, FOM_READ);
	if (hFile != NULL)
	{
		uint uiSize = uiLen(hFile);
		FEPointer pBuffer = CFECore::Mem::pAlloc(uiSize);
		uiRead(hFile,pBuffer,uiSize);
		Close(hFile);

		if (_puiSize != NULL) *_puiSize = uiSize;
		return(pBuffer);
	}

	return(NULL);
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Process functionality stuff.
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/// Starts a process to be executed in parallel to the main process (thread)
FEHandler CFECore::Process::hStartProcess(TCoreProcessFunc _pProcess,FEPointer _pParam)
{
	_pProcess(_pParam);
	return((FEHandler)1);
}
// ----------------------------------------------------------------------------
/// Tells whether a parallel process has finished its execution or not.
FEBool CFECore::Process::bIsProcessFinished(FEHandler _hProcess)
{
	return(true);
}
// ----------------------------------------------------------------------------
/// Frees the resources allocated by an already finished process.
void CFECore::Process::EndProcess(FEHandler _hProcess)
{

}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Profile management stuff.
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/// Registers and initializes the application profile assigned area.
void CFECore::Profile::RegisterApplication(const CFEString& _sApplicationName)
{
}
// ----------------------------------------------------------------------------
/// Saves data to the application profile assigned area.
FEBool CFECore::Profile::bSave(FEPointer _pData, uint _uiSize)
{
	return(false);
}
// ----------------------------------------------------------------------------
/// Loads the data from the application profile assigned area.
FEBool CFECore::Profile::bLoad(FEPointer _pData, uint _uiSize)
{
	return(false);
}
// ----------------------------------------------------------------------------
/// Cleans'up the profile information from the assigned area.
void CFECore::Profile::Reset()
{
}
// ----------------------------------------------------------------------------
/// Finalizes the profile system.
void CFECore::Profile::Finish()
{
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// TRC management stuff.
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/// Initializes the TRC Technical Requirements system
void CFECore::TRC::Init()
{
}
// ----------------------------------------------------------------------------
/// Performs a check step in the TRC system.
void CFECore::TRC::CheckStep()
{
}
// ----------------------------------------------------------------------------
/// Registers a the function to open a message box.
void CFECore::TRC::RegisterOpenMessageBoxFunc(TOpenMessageBoxFunc _pFunc)
{
	OpenMessageBoxFunc = _pFunc;
}
// ----------------------------------------------------------------------------
/// Registers a the function to write a message in a message box.
void CFECore::TRC::RegisterMessageBoxStepFunc(TMessageBoxStepFunc _pFunc)
{
	iMessageBoxStepFunc = _pFunc;
}
// ----------------------------------------------------------------------------
/// Registers a the function to close a message box.
void CFECore::TRC::RegisterCloseMessageBoxFunc(TCloseMessageBoxFunc _pFunc)
{
	bCloseMessageBoxFunc = _pFunc;
}
// ----------------------------------------------------------------------------
/// Opens a message box.
void CFECore::TRC::OpenMessageBox(const CFEString& _sMessage,const CFEString& _sOptions)
{
	if (OpenMessageBoxFunc != NULL)
		OpenMessageBoxFunc(_sMessage,_sOptions);
}
// ----------------------------------------------------------------------------
int CFECore::TRC::iMessageBoxStep()
{
	if (iMessageBoxStepFunc != NULL)
		return( iMessageBoxStepFunc() );
	else
		return(-1);
}
// ----------------------------------------------------------------------------
/// Closes the message box.
FEBool CFECore::TRC::bCloseMessageBox()
{
	if (bCloseMessageBoxFunc != NULL)
		return(bCloseMessageBoxFunc());
	else
		return(true);
}
// ----------------------------------------------------------------------------
/// Finalizes the TRC system.
void CFECore::TRC::Finish()
{
}
// ----------------------------------------------------------------------------
#endif // (TARGETPLATFORM == USE_ANDROID)
// ---------------------------------------------------------------------------
