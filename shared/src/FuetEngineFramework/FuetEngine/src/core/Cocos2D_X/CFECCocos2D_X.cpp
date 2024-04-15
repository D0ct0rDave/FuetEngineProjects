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
#if (TARGETPLATFORM == USE_COCOS2D_X)
// ----------------------------------------------------------------------------
#include "CCStdC.h"
#include "cocos2d.h"

#include "../CFECore.h"
#include <stdarg.h>
#include "CFECGlobals.h"

#include "platform/CCFileUtils.h"
using namespace cocos2d;

// ----------------------------------------------------------------------------
class CRendererHandler
{
	public:
		cocos2d::CCTexture2D*	m_poCurTex;
		FEReal					m_rCurDepth;
		EFEBlendMode			m_eBlendMode;
		cocos2d::CCSprite		m_oSprite;
};
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
	return( 0 );
}
// ------------------------------------------------------------------------
/// Retrieves time in seconds of the given system tics.
FEReal CFECore::rGetTickTime(FETick _oTicks)
{
	return(0);// ((float)(_oTicks)/1000.0f) );
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/// Logging Functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void CFECore::Log::Print(const char * format, va_list arg)
{
	/*
	#ifndef MASTER_RELEASE_VERSION
	char* szProcString = CFECore::String::szFormatString(format,arg);

	if (IsDebuggerPresent())
	{
		OutputDebugString(szProcString);
	}
	else
	{
		if (LogFuncCallback != NULL)
		{
			LogFuncCallback(szProcString);
		}
		else
		{
			printf(szProcString);
		}
	}
    #endif
	*/
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
	return(NULL);
}
// ----------------------------------------------------------------------------
/// Closes the file associated to the given handler.
void CFECore::File::Close(FEHandler _hFile)
{

}
// ----------------------------------------------------------------------------
/// Reads an amount data from the file storing at the given buffer.
uint CFECore::File::uiRead(FEHandler _hFile,void* _pData, uint _uiSize)
{
	return(0);
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
	return(0);
}
// ----------------------------------------------------------------------------
/// Retrieves the current file read/write position.
uint CFECore::File::uiPos(FEHandler _hFile)
{
	return(0);
}
// ----------------------------------------------------------------------------
/// Retrieves the total file length
uint CFECore::File::uiLen(FEHandler _hFile)
{
	return(0);
}
// ----------------------------------------------------------------------------
/// Reads the contents of a file and returns it as a memory buffer.
FEPointer CFECore::File::pReadFile(const CFEString& _sFilename,uint* _puiSize)
{
	CFECore::Log::Print("Loading %s: ",_sFilename.szString());
	
	unsigned long ulSize;
	unsigned char* pRes = NULL;

	if (CFECore::Local::bIsAutoLocEnabled())
	{
		CFEString sLocalFilename;

		// then try localized version of the file.
		sLocalFilename = CFECore::Local::sGetLocalID() + CFEString("/") + _sFilename;

		pRes = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(sLocalFilename.szString(),"rb", &ulSize);
		if (pRes != NULL)
		{
			*_puiSize = ulSize;

			CFECore::Log::Print("Ok (%s)\n",CFECore::Local::sGetLocalID().szString());
			return(pRes);
		}

		// first try the non-localized or common version of the file.
		sLocalFilename = CFECore::Local::sGetLocalID(LID_COMMON) + CFEString("/") + _sFilename;

		pRes = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(sLocalFilename.szString(),"rb", &ulSize);
		if (pRes != NULL)
		{
			*_puiSize = ulSize;

			CFECore::Log::Print("Ok (%s)\n",CFECore::Local::sGetLocalID(LID_COMMON).szString());
		}
		else
		{
			CFECore::Log::Print("Not found\n");
		}
	}
	else
	{
		pRes = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(_sFilename.szString(),"rb", &ulSize);
		if (pRes != NULL)
		{
			*_puiSize = ulSize;

			CFECore::Log::Print("Ok\n");
		}
		else
		{
			CFECore::Log::Print("Not found\n");
		}
	}

	return(pRes);
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
// Graphics management stuff.
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/// Creates and initializes a rendering system
FEHandler CFECore::Graphics::hInit(FEHandler _hParam)
{
	FECglobals.m_poCurrentRenderer = new CRendererHandler;
	return(FECglobals.m_poCurrentRenderer);
}
// ----------------------------------------------------------------------------
/// Finalizes the given rendering system
void CFECore::Graphics::Finish(FEHandler _hRenderer)
{
}
// ----------------------------------------------------------------------------
/// Loads a material. Doesn't upload it to the graphics system.
FEHandler CFECore::Graphics::hLoadMaterial(const CFEString& _sFilename)
{
	char* szDefaultExtensions[] = {(char*)"png",(char*)"tga",(char*)"jpg",(char*)"bmp",NULL};

	uint uiExt = 0;
	while (szDefaultExtensions[uiExt] != NULL)
	{
		// Filename has no extension, so try appending some standard extensions like TGA, JPG, PNG
		uint uiSize = 0;
		void*pData = NULL;

		char szFilename[1024];
		sprintf_s(szFilename,1024,"%s.%s",_sFilename.szString(),szDefaultExtensions[uiExt]);

		pData = CFECore::File::pReadFile(CFEString(szFilename),&uiSize);
		if (pData != NULL)
		{
			cocos2d::CCImage* poImage = new cocos2d::CCImage;
			if (! poImage->initWithImageData(pData,uiSize,cocos2d::CCImage::kFmtPng))
			{
				delete poImage;
				return(NULL);
			}

			cocos2d::CCTexture2D* poTex = new cocos2d::CCTexture2D;
			if (! poTex->initWithImage(poImage))
			{
				delete poImage;
				delete poTex;
				return(NULL);

			}

			return((FEHandler)poTex);
		}


		// Nop, try next extension
		uiExt ++;
	}

	return(NULL);
}
// ----------------------------------------------------------------------------
/// Deletes the material and free resources.
void CFECore::Graphics::DeleteMaterial(FEHandler _hMaterial)
{
	if (_hMaterial != NULL)
	{
		cocos2d::CCTexture2D* poTex = (cocos2d::CCTexture2D*)_hMaterial;
		delete poTex;
	}
}
// ----------------------------------------------------------------------------
/// Reloads the material associated to the given handler with the contents of the file provided. Handler is still reusable after function call.
void CFECore::Graphics::ReloadMaterial(FEHandler _hMaterial,const CFEString& _sFilename)
{
}
// ----------------------------------------------------------------------------
/// Retrieves a renderer property if exists.
FEBool CFECore::Graphics::bGetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
	return(false);
}
// ----------------------------------------------------------------------------
/// Sets a renderer property if exists and it's possible.
FEBool CFECore::Graphics::bSetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
	return(false);
}
// ----------------------------------------------------------------------------
/// Retrieves a material property if exists.
FEBool CFECore::Graphics::bGetMaterialProperty(FEHandler _hMaterial,const CFEString& _sProperty,FEPointer _pParam)
{
	return(false);
}
// ----------------------------------------------------------------------------
/// Sets a material property if exists and it's possible.
FEBool CFECore::Graphics::bSetMaterialProperty(FEHandler _hMaterial,const CFEString& _sProperty,FEPointer _pParam)
{
	return(false);
}
// ----------------------------------------------------------------------------
/// Sets the screen virtual width.
void CFECore::Graphics::SetScreenVWidth(uint _uiVScrWidth)
{
}
// ----------------------------------------------------------------------------
/// Sets the screen virtual height.
void CFECore::Graphics::SetScreenVHeight(uint _uiVScrHeight)
{
}
// ----------------------------------------------------------------------------
/// Sets the currently active renderer.
void CFECore::Graphics::SetCurrentRenderer(FEHandler _hRenderer)
{
}
// ----------------------------------------------------------------------------
/// Retrieves the currently active renderer.
FEHandler CFECore::Graphics::hGetCurrentRenderer()
{
	return(NULL);
}
// ----------------------------------------------------------------------------
/// Prepares the rendering system to begin the drawing of the scene, and clears the render buffer.
void CFECore::Graphics::BeginScene()
{

}
// ----------------------------------------------------------------------------
/// Blits the result of the current renderer operations to the screen.
void CFECore::Graphics::EndScene()
{
}
// ----------------------------------------------------------------------------
/// Sets the global transformation Matrix for the scene
void CFECore::Graphics::SetTransform(const CFEMatrix& _rMat)
{
}
// ----------------------------------------------------------------------------
/// Sets the current blend mode.
void CFECore::Graphics::SetBlendMode(EFEBlendMode _eBlendMode)
{
	FECglobals.m_poCurrentRenderer->m_eBlendMode = _eBlendMode;
}
// ----------------------------------------------------------------------------
/// Sets the current material.
void CFECore::Graphics::SetMaterial(FEHandler _hMaterial)
{
	FECglobals.m_poCurrentRenderer->m_poCurTex = (cocos2d::CCTexture2D*)_hMaterial;
}
// ----------------------------------------------------------------------------
/// Sets the current depth.
void CFECore::Graphics::SetDepth(FEReal _rDepth)
{
	FECglobals.m_poCurrentRenderer->m_rCurDepth = _rDepth;
}
// ----------------------------------------------------------------------------
/// Draws a sprite. (for sprites)
void CFECore::Graphics::RenderSprite(	const CFEVect2& _oPos,const CFEVect2& _oPivot,
					const CFEVect2& _oScale,FEReal _rAngle,
					const CFEVect2& _oIUV,const CFEVect2& _oFUV,
                    const CFEColor& _oColor,FEReal _rAlpha)
{

	if (FECglobals.m_poCurrentRenderer->m_poCurTex != NULL)
	{
		// FECglobals.m_poCurrentRenderer->m_oSprite.setTexture(FECglobals.m_poCurrentRenderer->m_poCurTex);
		FECglobals.m_poCurrentRenderer->m_oSprite.initWithTexture(FECglobals.m_poCurrentRenderer->m_poCurTex);
		// FECglobals.m_poCurrentRenderer->m_oSprite.setBlendFunc(FECglobals.m_poCurrentRenderer->m_eBlendMode);
		
		ccColor3B color;
		color.r = _oColor.r*255;
		color.g = _oColor.g*255;
		color.b = _oColor.b*255;

		FECglobals.m_poCurrentRenderer->m_oSprite.setRotation(_rAngle);
		FECglobals.m_poCurrentRenderer->m_oSprite.setPosition(CCPoint(_oPos.x,_oPos.y));
		FECglobals.m_poCurrentRenderer->m_oSprite.setVertexZ(FECglobals.m_poCurrentRenderer->m_rCurDepth);
		FECglobals.m_poCurrentRenderer->m_oSprite.setAnchorPoint(CCPoint(_oPivot.x,_oPivot.y));
		FECglobals.m_poCurrentRenderer->m_oSprite.setScaleX(_oScale.x);
		FECglobals.m_poCurrentRenderer->m_oSprite.setScaleY(_oScale.x);
		FECglobals.m_poCurrentRenderer->m_oSprite.setTextureRect(CCRect(_oIUV.x,_oIUV.y,_oFUV.x,_oFUV.y));;
		FECglobals.m_poCurrentRenderer->m_oSprite.setColor(color);
		FECglobals.m_poCurrentRenderer->m_oSprite.setOpacity(255*_rAlpha);

		FECglobals.m_poCurrentRenderer->m_oSprite.draw();
	}
}
// ----------------------------------------------------------------------------
/// Draws a quad. (for text rendernig)
void CFECore::Graphics::RenderQuad(	const CFEVect2& _oIVX,const CFEVect2& _oFVX,
					const CFEVect2& _oIUV,const CFEVect2& _oFUV,
                    const CFEColor& _oColor)
{
	if (FECglobals.m_poCurrentRenderer->m_poCurTex != NULL)
	{
		FECglobals.m_poCurrentRenderer->m_oSprite.initWithTexture(FECglobals.m_poCurrentRenderer->m_poCurTex);
		// FECglobals.m_poCurrentRenderer->m_oSprite.setTexture(FECglobals.m_poCurrentRenderer->m_poCurTex);
		// FECglobals.m_poCurrentRenderer->m_oSprite.setBlendFunc(FECglobals.m_poCurrentRenderer->m_eBlendMode);
		
		ccColor3B color;
		color.r = _oColor.r*255;
		color.g = _oColor.g*255;
		color.b = _oColor.b*255;
		
		FECglobals.m_poCurrentRenderer->m_oSprite.setScaleX(1.0f);
		FECglobals.m_poCurrentRenderer->m_oSprite.setScaleY(1.0f);
		FECglobals.m_poCurrentRenderer->m_oSprite.setRotation(0.0f);
		FECglobals.m_poCurrentRenderer->m_oSprite.setAnchorPoint(CCPoint(0.0f,0.0f));
		

		FECglobals.m_poCurrentRenderer->m_oSprite.setVertexRect(CCRect(_oIVX.x,_oIVX.y,_oFVX.x,_oFVX.y));
		FECglobals.m_poCurrentRenderer->m_oSprite.setVertexZ(FECglobals.m_poCurrentRenderer->m_rCurDepth);
		FECglobals.m_poCurrentRenderer->m_oSprite.setTextureRect(CCRect(_oIUV.x,_oIUV.y,_oFUV.x,_oFUV.y));;
		FECglobals.m_poCurrentRenderer->m_oSprite.setColor(color);
		FECglobals.m_poCurrentRenderer->m_oSprite.setOpacity(255*_oColor.a);

		FECglobals.m_poCurrentRenderer->m_oSprite.draw();
	}
}
// ----------------------------------------------------------------------------
/// Draws a mesh immediate. Arrays are NOT used internally.
void CFECore::Graphics::RenderQuadList(CFEVect2* _poVXs,CFEVect2* _poUVs,CFEColor* _poVCs,uint _uiQuads)
{
}
// ----------------------------------------------------------------------------
/// Draws a mesh. Arrays ARE USED internally.
void CFECore::Graphics::RenderMesh(unsigned short* _pusIdx,CFEVect2* _poVXs,CFEVect2* _poUVs,const CFEColor& _oColor,uint _uiPoints)
{
}
// ----------------------------------------------------------------------------
/// Draws a line.
void CFECore::Graphics::RenderLine(const CFEVect2& _oIni,const CFEVect2& _oEnd,const CFEColor& _oColor)
{
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Input management stuff.
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/// Creates and initializes a input manager
FEHandler CFECore::Input::hInit(FEHandler _hParam)
{
	return(NULL);
}
// ----------------------------------------------------------------------------
/// Finalizes the given input manager
void CFECore::Input::Finish(FEHandler _hInputHandler)
{
}
// ----------------------------------------------------------------------------
/// Retrieves the inputs of a given input manager.
void CFECore::Input::Update(FEHandler _hInputHandler,TFEInputStruct* _poInputs)
{
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Sound related stuff.
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/// Initializes the sound system.
FEHandler  CFECore::Sound::hInit(FEHandler _hParam)
{
	return(NULL);
}
// ----------------------------------------------------------------------------
/// Finalizes the sound system.
void  CFECore::Sound::Finish()
{
}
// ----------------------------------------------------------------------------

/// Performs an update step over the sound system, once per application frame.
void  CFECore::Sound::Update()
{
}
// ----------------------------------------------------------------------------
/// Sets the volume of the given Line to a given level.
void  CFECore::Sound::SetLineLevel(EFESoundMixerLine _eSoundLine,FEReal _rVol)
{
}
// ----------------------------------------------------------------------------
/// Loads a sound indicating if we want to treat it as a BGM or not.
FEHandler  CFECore::Sound::hLoadSound(const CFEString& _sFilename,FEBool _bBGM)
{
	return(NULL);
}
// ----------------------------------------------------------------------------
/// Deletes a given sound.
void  CFECore::Sound::DeleteSound(FEHandler _hSound)
{
}
// ----------------------------------------------------------------------------
/// Retrieves a sound property if exists.
FEBool  CFECore::Sound::bGetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam)
{
	return(false);
}
// ----------------------------------------------------------------------------
/// Sets a sound property if exists and it's possible.
FEBool  CFECore::Sound::bSetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam)
{
	return(false);
}
// ----------------------------------------------------------------------------
/// Plays a given sound in the given sound line.
void  CFECore::Sound::PlaySound(FEHandler _hSound,EFESoundMixerLine _eSoundLine,FEReal _rVol,FEReal _rPan)
{
}
// ----------------------------------------------------------------------------
/// Stops a given sound.
void  CFECore::Sound::StopSound(FEHandler _hSound)
{
}
// ----------------------------------------------------------------------------
/// Stops all the currently playing sounds.
void  CFECore::Sound::StopSounds()
{
}
// ----------------------------------------------------------------------------
/// Pauses all the currently playing sounds.
void  CFECore::Sound::PauseSounds()
{
}
// ----------------------------------------------------------------------------
/// Resumes the playing of the previously paused sounds.
void CFECore::Sound::ResumeSounds()
{
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Localization management stuff.
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/// Enables the autolocalization system for file loading.
void CFECore::Local::EnableAutoLoc()
{
}
// ----------------------------------------------------------------------------

/// Disables the autolocalization system for file loading.
void CFECore::Local::DisableAutoLoc()
{
}
// ----------------------------------------------------------------------------

/// Retrieves whether the autolocalization system is active or not.
FEBool CFECore::Local::bIsAutoLocEnabled()
{
	return(false);
}
// ----------------------------------------------------------------------------
/// Retrieves the localization string ID.
const CFEString& CFECore::Local::sGetLocalID()
{
	return( sGetLocalID(eGetLocalID()) );
}
// ----------------------------------------------------------------------------
/// Retrieves the localization string ID of a given ID enum value.
const CFEString& CFECore::Local::sGetLocalID(EFELocalID _eLID)
{
	return(CFEString::sNULL());
}
// ----------------------------------------------------------------------------
/// Retrieves the localization ID as an enumerated type.
EFELocalID CFECore::Local::eGetLocalID()
{
	return(LID_NONE);
}
// ----------------------------------------------------------------------------
/// Sets the localization ID that will be used by the system.
void CFECore::Local::SetLocalID(EFELocalID _eLocalID)
{
}
// ----------------------------------------------------------------------------
/// Sets whether a specific local ID is supported or not.
void CFECore::Local::SetLocalIDSupport(EFELocalID _eLocalID,FEBool _bSupport)
{
}
// ----------------------------------------------------------------------------
/// Retrieves whether the specific local ID is supported or not.
FEBool CFECore::Local::bGetLocalIDSupport(EFELocalID _eLocalID)
{
	return(false);
}
// ----------------------------------------------------------------------------
/// Retrieves the local region string ID.
const CFEString& CFECore::Local::sGetLocalRegionID()
{
	return( sGetLocalRegionID(eGetLocalRegionID()) );
}
// ----------------------------------------------------------------------------
/// Retrieves the local region string ID  of a given ID enum value.
const CFEString& CFECore::Local::sGetLocalRegionID(EFELocalRegionID _eLocalRegionID)
{
	return(CFEString::sNULL());
}
// ----------------------------------------------------------------------------
/// Retrieves the local region ID as an enumerated type.
EFELocalRegionID CFECore::Local::eGetLocalRegionID()
{
	return(LRID_NONE);
}
// ----------------------------------------------------------------------------
/// Sets the localization ID that will be used by the system.
void CFECore::Local::SetLocalRegionID(EFELocalRegionID _eLocalRegionID)
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
#endif // (TARGETPLATFORM == USE_COCOS2D_X)
// ---------------------------------------------------------------------------
