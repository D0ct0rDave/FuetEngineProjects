// ----------------------------------------------------------------------------
/*! \class CFECore
 *  \brief System Support Class for the rest of the engine
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFECoreH
#define CFECoreH
// ----------------------------------------------------------------------------
#include <stdarg.h>
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "Types/CFEMomentInTime.h"
#include "Types/CFEString.h"
#include "Types/CFEVect2.h"
#include "Types/CFEMatrix.h"
#include "Types/CFEColor.h"
#include "FEEnums.h"
// ----------------------------------------------------------------------------
#include "FECoreTypes.h"
// ----------------------------------------------------------------------------
#ifdef DEBUG
#define CFECoreCheck(a,b)	CFECore::Check((a),(b))
#else
#define CFECoreCheck(a,b)
#endif
// ----------------------------------------------------------------------------
namespace CFECore
{
	/// Assert functionality
	void Check(FEBool _bCondition,const CFEString& _sMessage);

	/// Retrieves time in ticks since the start of the engine.
	FETick oGetEngineTicks();

    /// Retrieves time in seconds of the given system tics.
    FEReal rGetTickTime(FETick _oTicks);

	/// Retrieves the current moment in time
	CFEMomentInTime oGetMomentInTime();

    /// Performs an update step over the entire system. Call it once per frame.
    void Update();

	/// Basic Memory Management functions
    namespace Mem
    {
        /// Allocates a given amount of memory and returns a pointer to the allocated buffer.
        FEPointer pAlloc(uint _uiSize,uint _uiAlignment = 0);

        /// Frees the buffer associated by the given pointer.
        void Free(FEPointer _pPtr);

        /// Retrieves a memory property if exists.
        FEBool bGetProperty(const CFEString& _sProperty,FEPointer _pParam);

        /// Sets a memory property if exists and it's possible.
        FEBool bSetProperty(const CFEString& _sProperty,FEPointer _pParam);
    }

    /// Logging Functionality
    namespace Log
    {
		/// Print a formated message into the log device.
    	void Print(const char *fmt,...);

		/// Print a formated message into the log device.
		// NOTE va_list should be passed as pointer and not as value for portability reasons.
		void Print(const char * format, va_list* pArg );

		/// Log callback function prototype.
		typedef void(*TCoreLogFunc)(const char * _sLogMessage);

		/// Install a log callback to be called every time CFECore::Log::Print is called.
    	void SetSystemLogFunc(TCoreLogFunc _pLogFunc);
    }

	/// Basic String functions
    namespace String
    {
        /// Retrieves a formated string using a variable number of arguments.
		// NOTE va_list should be passed as pointer and not as value for portability reasons.
		char* szFormatString(const char * format, va_list* pArg);

    	/// Performs a case sensitive comparaison of the given strings, and returns if the first is lower,equal or bigger than the second string.
        int iStrCmp(const char* _szA,const char* _szB);

        /// Performs a case sensitive comparaison of the given strings, on a given number of characters, and returns if the first is lower,equal or bigger than the second string.
        int iStrNCmp(const char* _szA,const char* _szB,uint _uiChars);

        /// Performs a case insensitive comparaison of the given strings, and returns if the first is lower,equal or bigger than the second string.
        int iStrICmp(const char* _szA,const char* _szB);

        /// Performs a case insensitive comparaison of the given strings, on a given number of characters, and returns if the first is lower,equal or bigger than the second string.
        int iStrNICmp(const char* _szA,const char* _szB,uint _uiChars);
    }

    /// File I/O Management
    namespace File
    {
        /// Opens a given file in the given access mode and retrieves its file handler.
        FEHandler hOpen(const CFEString& _sFilename, EFEFileOpenMode _eMode);

        /// Closes the file associated to the given handler.
        void Close(FEHandler _hFile);

        /// Reads an amount data from the file storing at the given buffer.
        uint uiRead(FEHandler _hFile,void* _pData, uint _uiSize);

        /// Writes the amount of data from the given buffer to the file.
        uint uiWrite(FEHandler _hFile,void* _pData, uint _uiSize);

        /// Sets the read/write position of the file.
        int iSeek(FEHandler _hFile,int _iOffset, EFEFileSeekMode _eMode);

        /// Retrieves the current file read/write position.
        uint uiPos(FEHandler _hFile);

        /// Retrieves the total file length
        uint uiLen(FEHandler _hFile);

        /// Reads the contents of a file and returns it as a memory buffer.
        FEPointer pReadFile(const CFEString& _sFilename,uint* _puiSize = NULL);
    }

    /// Config File Functionality
    namespace Config
    {
        /// Opens a config file.
        FEHandler hOpen(const CFEString& _sConfigFile);

        /// Closes the configuration file associated with the given handler.
        void Close(FEHandler _hConfig);

        /// Retrieves a string variable value from the configuration file.
        CFEString sGetString(FEHandler _hConfig,const CFEString& _sVar,const CFEString& _sDefaultValue);

        /// Retrieves a FEReal variable value from the configuration file.
        FEReal rGetReal(FEHandler _hConfig,const CFEString& _sVar,FEReal _rDefaultValue);

        /// Retrieves a integer variable value from the configuration file.
        int iGetInteger(FEHandler _hConfig,const CFEString& _sVar,int _iDefaultValue);

        /// Retrieves a FEBool variable value from the configuration file.
        FEBool bGetBool(FEHandler _hConfig,const CFEString& _sVar,FEBool _bDefaultValue);

        /// Retrieves whether a given variable is defined in the config file or not.
        FEBool bExists(FEHandler _hConfig,const CFEString& _sVar);
    }

	/// Parallel Processing
	namespace Process
    {
		typedef void(*TCoreProcessFunc)(FEPointer _pParam);

		/// Starts a process to be executed in parallel to the main process (thread)
		FEHandler hStartProcess(TCoreProcessFunc _pProcess,FEPointer _pParam);

		/// Tells whether a parallel process has finished its execution or not.
		FEBool bIsProcessFinished(FEHandler _hProcess);

		/// Frees the resources allocated by an already finished process.
		void EndProcess(FEHandler _hProcess);
	}

    /// Graphic related stuff
    namespace Graphics
    {
        /// Creates and initializes a rendering system
        FEHandler hInit(FEHandler _hParam);

        /// Finalizes the given rendering system
        void Finish(FEHandler _hRenderer);

        /// Loads a material. Doesn't upload it to the graphics system.
        FEHandler hLoadMaterial(const CFEString& _sFilename);

        /// Deletes the material and free resources.
        void DeleteMaterial(FEHandler _hMaterial);

        /// Reloads the material associated to the given handler with the contents of the file provided. Handler is still reusable after function call.
        void ReloadMaterial(FEHandler _hMaterial,const CFEString& _sFilename);

        /// Retrieves a renderer property if exists.
        FEBool bGetProperty(const CFEString& _sProperty,FEPointer _pParam);

        /// Sets a renderer property if exists and it's possible.
        FEBool bSetProperty(const CFEString& _sProperty,FEPointer _pParam);

        /// Retrieves a material property if exists.
        FEBool bGetMaterialProperty(FEHandler _hMaterial,const CFEString& _sProperty,FEPointer _pParam);

        /// Sets a material property if exists and it's possible.
        FEBool bSetMaterialProperty(FEHandler _hMaterial,const CFEString& _sProperty,FEPointer _pParam);

        /// Sets the screen virtual width.
        void SetScreenVWidth(uint _uiVScrWidth);

        /// Sets the screen virtual height.
        void SetScreenVHeight(uint _uiVScrHeight);

        /*
        /// Retrieves the current screen virtual width.
        uint uiGetScreenVWidth(FEHandler _hRenderer);

        /// Retrieves the current screen virtual height.
        uint uiGetScreenVHeight(FEHandler _hRenderer);
        */

        /// Sets the currently active renderer.
        void SetCurrentRenderer(FEHandler _hRenderer);

        /// Retrieves the currently active renderer.
        FEHandler hGetCurrentRenderer();

        /// Prepares the rendering system to begin the drawing of the scene, and clears the render buffer.
        void BeginScene();

        /// Blits the result of the current renderer operations to the screen.
        void EndScene();

        /// Sets the global transformation Matrix for the scene
        void SetTransform(const CFEMatrix& _rMat);

        /** ------------------------------------------------------------------------

        These functions must be called inside a BeginScene/EndScene block.

        ------------------------------------------------------------------------ **/

        /// Sets the current blend mode.
        void SetBlendMode(EFEBlendMode _eBlendMode);

        /// Sets the current material.
        void SetMaterial(FEHandler _hMaterial);

        /// Sets the current depth.
        void SetDepth(FEReal _rDepth);

        /// Draws a sprite. (for sprites)
        void RenderSprite(	const CFEVect2& _oPos,const CFEVect2& _oPivot,
							const CFEVect2& _oScale,FEReal _rAngle,
							const CFEVect2& _oIUV,const CFEVect2& _oFUV,
                            const CFEColor& _oColor,FEReal _rAlpha);

        /// Draws a quad. (for text rendernig)
        void RenderQuad(	const CFEVect2& _oIVX,const CFEVect2& _oFVX,
							const CFEVect2& _oIUV,const CFEVect2& _oFUV,
                            const CFEColor& _oColor);

		/// Draws a mesh immediate. Arrays are NOT used internally.
		void RenderQuadList(CFEVect2* _poVXs,CFEVect2* _poUVs,CFEColor* _poVCs,uint _uiQuads);

		/// Draws a mesh. Arrays ARE USED internally.
        void RenderMesh(unsigned short* _pusIdx,CFEVect2* _poVXs,CFEVect2* _poUVs,const CFEColor& _oColor,uint _uiPoints);

        /// Draws a line.
        void RenderLine(const CFEVect2& _oIni,const CFEVect2& _oEnd,const CFEColor& _oColor);
    }

	/// Input management
	namespace Input
	{
        /// Creates and initializes a input manager
        FEHandler hInit(FEHandler _hParam);

        /// Finalizes the given input manager
        void Finish(FEHandler _hInputHandler);

        /// Retrieves the inputs of a given input manager.
        void Update(FEHandler _hInputHandler,TFEInputStruct* _poInputs);
	}

	/// Sound related stuff.
	namespace Sound
	{
        /// Initializes the sound system.
        FEHandler hInit(FEHandler _hParam);

        /// Finalizes the sound system.
        void Finish();

        /// Performs an update step over the sound system, once per application frame.
        void Update();

        /// Sets the volume of the given Line to a given level.
        void SetLineLevel(EFESoundMixerLine _eSoundLine,FEReal _rVol);

        /// Loads a sound indicating if we want to treat it as a BGM or not.
        FEHandler hLoadSound(const CFEString& _sFilename,FEBool _bBGM);

        /// Deletes a given sound.
        void DeleteSound(FEHandler _hSound);

        /// Retrieves a sound system property if it exists.
        FEBool bGetProperty(const CFEString& _sProperty,FEPointer _pParam);

        /// Sets a sound system property if it exists.
        FEBool bSetProperty(const CFEString& _sProperty,FEPointer _pParam);

        /// Retrieves a sound property if exists.
        FEBool bGetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam);

        /// Sets a sound property if exists and it's possible.
        FEBool bSetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam);

        /// Plays a given sound in the given sound line.
        FEHandler hPlaySound(FEHandler _hSound, EFESoundMixerLine _eSoundLine, FEReal _rVol, FEReal _rPan);

        /// Stops a given sound.
        void StopSound(FEHandler _hSound);

        /// Stops all the currently playing sounds.
        void StopSounds();

        /// Pauses all the currently playing sounds.
        void PauseSounds();

        /// Resumes the playing of the previously paused sounds.
        void ResumeSounds();
	}

    /// Math
    namespace Math
    {
        /// Retrieves a sinus of the given angle in radians.
        FEReal rSin(FEReal _rAngle);

        /// Retrieves a cosinus of the given angle in radians.
        FEReal rCos(FEReal _rAngle);

        /// Retrieves a random value between 0 and 1.
        FEReal rRand();

        /// Retrieves a random value between _iMin and _iMax.
        int iRand(int _iMin,int _iMax);

        /// Retrieves the square root of the given value.
        FEReal rSqrt(FEReal _rValue);

        /// Retrieves the modulus of the division between the numerator and the given denominator.
        FEReal rMod(FEReal _rNum,FEReal _rDen);
    }

    /// Localization related stuff.
    namespace Local
    {
		/// Enables the autolocalization system for file loading.
		void EnableAutoLoc();

		/// Disables the autolocalization system for file loading.
		void DisableAutoLoc();

		/// Retrieves whether the autolocalization system is active or not.
		FEBool bIsAutoLocEnabled();

		/// Retrieves the localization string ID.
		const CFEString& sGetLocalID();

		/// Retrieves the localization string ID of a given ID enum value.
		const CFEString& sGetLocalID(EFELocalID _eLID);

		/// Retrieves the localization ID as an enumerated type.
		EFELocalID eGetLocalID();

		/// Sets the localization ID that will be used by the system.
		void SetLocalID(EFELocalID _eLocalID);

		/// Sets whether a specific local ID is supported or not.
		void SetLocalIDSupport(EFELocalID _eLocalID,FEBool _bSupport);

		/// Retrieves whether the specific local ID is supported or not.
		FEBool bGetLocalIDSupport(EFELocalID _eLocalID);

		/// Retrieves the local region string ID.
		const CFEString& sGetLocalRegionID();

		/// Retrieves the local region string ID  of a given ID enum value.
		const CFEString& sGetLocalRegionID(EFELocalRegionID _eLocalRegionID);

		/// Retrieves the local region ID as an enumerated type.
		EFELocalRegionID eGetLocalRegionID();

		/// Sets the localization ID that will be used by the system.
		void SetLocalRegionID(EFELocalRegionID _eLocalRegionID);
	}

    /// Profile management stuff.
    namespace Profile
    {
		/// Registers and initializes the application profile assigned area.
		void RegisterApplication(const CFEString& _sApplicationName,uint _uiApplicationVersion = 0);

		/// Retrieves the application profile version.
		uint uiGetVersion();

		/// Saves data to the application profile assigned area.
		FEBool bSave(FEPointer _pData, uint _uiSize);

		/// Loads the data from the application profile assigned area.
		FEBool bLoad(FEPointer _pData, uint _uiSize);

		/// Cleans'up the profile information from the assigned area.
		void Reset();

		/// Finalizes the profile system.
		void Finish();
	}

	/// TRC management stuff.
    namespace TRC
    {
		typedef void(*TOpenMessageBoxFunc)(const CFEString& _sMessage,const CFEString& _sOptions);
		typedef int(*TMessageBoxStepFunc)();
		typedef FEBool(*TCloseMessageBoxFunc)();

		/// Initializes the TRC Technical Requirements system
		void Init();

		/// Performs a check step in the TRC system.
		void CheckStep();

		/// Registers a the function to open a message box.
		void RegisterOpenMessageBoxFunc(TOpenMessageBoxFunc _pFunc);

		/// Registers a the function to write a message in a message box.
		void RegisterMessageBoxStepFunc(TMessageBoxStepFunc _pFunc);

		/// Registers a the function to close a message box.
		void RegisterCloseMessageBoxFunc(TCloseMessageBoxFunc _pFunc);

		/// Opens a message box.
		void OpenMessageBox(const CFEString& _sMessage,const CFEString& _sOptions);

		/// Performs a message box step. Returns the input selected option or -1 if no option selected.
		int iMessageBoxStep();

		/// Closes the message box.
		FEBool bCloseMessageBox();

		/// Finalizes the TRC system.
		void Finish();
	}
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
