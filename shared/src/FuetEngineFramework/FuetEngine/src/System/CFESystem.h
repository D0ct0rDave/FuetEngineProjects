// ----------------------------------------------------------------------------
/*! \class CFESystem
 *  \brief System Support Class for the rest of the engine
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESystemH
#define CFESystemH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "Types/CFEString.h"
#include "Types/CFEVect2.h"
#include "Types/CFEMatrix.h"
#include "Types/CFEColor.h"
#include "FEEnums.h"
// ----------------------------------------------------------------------------
/// TODO: define a better place to put the input structures
// ----------------------------------------------------------------------------
typedef struct TFEInputCursor
{
    CFEVect2 m_oCoords;
    FEReal    m_rPressure[IPB_NUMIPBS];
}TFEInputCursor;

typedef struct TFEInputStruct
{
	/// Digital Buttons
	bool m_bButtons[IB_NUM_BUTTONS];

	/// Axis
	FEReal m_rAxis[IA_NUMAXIS];

	/// Cursor
	TFEInputCursor m_oCursor;

	/// Other miscelaneous controls
	/// keyboard ???

}TFEInputStruct;
// ----------------------------------------------------------------------------
namespace CFESystem
{
    /// Assert functionality
    void Check(bool _bCondition,const CFEString& _sMessage);

    /// Retrieves time in seconds since the start of the engine.
    FEReal rGetEngineTime();

    /// Performs an update step over the entire system. Call it once per frame.
    void Update();

	/// Basic Memory Management functions
    namespace Mem
    {
        /// Allocates a given amount of memory and returns a pointer to the allocated buffer.
        FEPointer pAlloc(uint _uiSize);

        /// Frees the buffer associated by the given pointer.
        void Free(FEPointer _pPtr);
    }

    /// Logging Functionality
    namespace Log
    {
    	/// Print a formated message into the log device.
    	void Print(char *fmt,...);
    }

	/// Basic String functions
    namespace String
    {   
        /// Performs a case sensitive comparaison of the given strings, and returns if the first is lower,equal or bigger than the second string.
        int iStrCmp(const CFEString& _sA,const CFEString& _sB);

        /// Performs a case sensitive comparaison of the given strings, on a given number of characters, and returns if the first is lower,equal or bigger than the second string.
        int iStrNCmp(const CFEString& _sA,const CFEString& _sB,uint _uiChars);

        /// Performs a case insensitive comparaison of the given strings, and returns if the first is lower,equal or bigger than the second string.
        int iStrICmp(const CFEString& _sA,const CFEString& _sB);
        
        /// Performs a case insensitive comparaison of the given strings, on a given number of characters, and returns if the first is lower,equal or bigger than the second string.
        int iStrNICmp(const CFEString& _sA,const CFEString& _sB,uint _uiChars);
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

        /// Retrieves a bool variable value from the configuration file.
        bool bGetBool(FEHandler _hConfig,const CFEString& _sVar,bool _bDefaultValue);
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

        /// Retrieves a material property if exists.
        bool bGetMaterialProperty(FEHandler _hMaterial,const CFEString& _sProperty,FEPointer _pParam);

        /// Sets a material property if exists and it's possible.
        bool bSetMaterialProperty(FEHandler _hMaterial,const CFEString& _sProperty,FEPointer _pParam);

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

        /// Retrieves the currently active renderer.
        FEHandler hGetCurrentRenderer();

        /// Prepares the rendering system to begin the drawing of the scene. Sets the given renderer as the current one and clears the render buffer.
        void BeginScene(FEHandler _hRenderer);

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

        /// Draws a list of quads.
        void RenderQuads(CFEVect2* _poVX,CFEVect2* _poUV,CFEColor* _poVC,uint _uiNumQuads);

        /// Draws a mesh.
        void RenderMesh(unsigned short* _pusIdx,CFEVect2* _poVXs,CFEVect2* _poUVs,CFEColor* _poVCs,uint _uiPoints);

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

        /// Loads a sound. Doesn't upload it to the graphics system.
        FEHandler hLoadSound(const CFEString& _sFilename);

        /// Deletes a given sound.
        void DeleteSound(FEHandler _hSound);

        /// Retrieves a sound property if exists.
        bool bGetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam);

        /// Sets a sound property if exists and it's possible.
        bool bSetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam);

        /// Plays a given sound in the given sound line.
        void PlaySound(FEHandler _hSound,EFESoundMixerLine _eSoundLine,FEReal _rVol,FEReal _rPan);

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

        /// Retrieves the square root of the given value.
        FEReal rSqrt(FEReal _rValue);
        
        /// Retrieves the modulus of the division between the numerator and the given denominator.
        FEReal rMod(FEReal _rNum,FEReal _rDen);
    }
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
