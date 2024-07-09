//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.h
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class declaration
//
//---------------------------------------------------------------------------
#ifndef CAppGlobalsH
#define CAppGlobalsH
//---------------------------------------------------------------------------
#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/frame.h>
#else
	#include <wx/wxprec.h>
#endif

#include <vector>
#include <FreeImage.h>
#include <FreeImagePlus.h>
#include <FuetEngine.h>
#include "CSkeleton.h"
#include "CUndoStack.h"
//---------------------------------------------------------------------------
class CActorEditorGUI;
class CUndoState;
//---------------------------------------------------------------------------
// #define DISABLE_UNDO_REDO
const unsigned int MAX_UNDO_STATES = 128;
//---------------------------------------------------------------------------
const FEReal COLOR_CYCLE_PERIOD = 3.0f;

const FEReal INITIAL_ZOOM		= 1.0f;
const FEReal MIN_ZOOM			= 0.01;
const FEReal MAX_ZOOM			= 32.0;

extern float SCREEN_WIDTH;
extern float SCREEN_HEIGHT;

const CFEVect2 INITIAL_TRANS  = CFEVect2(0,0);
extern CFEVect2 INITIAL_AIMPOS;

const FEReal TRANS_STEP = 16.0;

const FEReal MAX_ANIM_TIME = 60.0f;
const FEReal TICKS_PER_SEC = 10.0f;
const FEReal SECS_PER_TICK = 1.0f / TICKS_PER_SEC;

//---------------------------------------------------------------------------
const wxString AE_SKELETON_DEFINITION_EXT = ".def";
const wxString AE_ANIMATION_FILE_EXT = ".aea";
const wxString AE_POSE_FILE_EXT = ".aep";
const wxString AE_FRAMELIST_FILE_EXT = ".avi";
const wxString FE_SKELETON_FILE_EXT = ".ske";
const wxString FE_SKELACTION_FILE_EXT = ".ska";
//---------------------------------------------------------------------------
typedef enum EEditMode { 

	EM_NONE,
	
	EM_TRANSLATE,
	EM_SCALE,
	EM_CROSSHAIR,
	EM_EDITBONES,
	
	EM_NUM_MODES,
};
//---------------------------------------------------------------------------
const wxString APP_NAME = "FuetEngine Actor Editor";
const wxString SE_SPRITE_FILE_EXT= ".aea";
//---------------------------------------------------------------------------
class CAppColors
{
	public:
		CAppColors();
	
        // Colors
        CFEColor	m_oGroupBV;
        CFEColor	m_oLayerBV;
        CFEColor	m_oLayerPivot;
        CFEColor	m_oGrid;
        CFEColor	m_oGridHL;
        
        CFEColor	m_oBackground;
        CFEColor	m_oFrameBackground;
        CFEColor	m_oPrevFrameBackground;
        CFEColor	m_oFrameRect;
        CFEColor	m_oBone;
        CFEColor	m_oBoneKF;
        CFEColor	m_oSelBone;
        CFEColor	m_oSelEditBone;
        CFEColor	m_oCrossHairColor;
        CFEColor	m_oIniColor;

        CFEColor	m_oBoneInPreview;

		wxColor		m_oAppBack;
		wxColor		m_oGridCellBase;
		wxColor		m_oGridCellText;
		wxColor		m_oGridLabelBase;
		
		wxColor		m_oElemBack;
		wxColor		m_oGenObjBack0;
		wxColor		m_oGenObjBack1;

		wxColor		m_oSelCellBack;
		wxColor		m_oSelCellText;
		wxColor		m_oSelBlockFrame;
		wxColor		m_oSelBlockBack0;
		wxColor		m_oSelBlockBack1;
		wxColor		m_oSelBlockText;

		wxColor		m_oVisCellBack;
		wxColor		m_oLockCellBack;
		wxColor		m_oTLGridLine;
		wxColor		m_oTLLineBack;
		wxColor		m_oTLDisabled;
		wxColor		m_oTLMarkerLine;
		wxColor		m_oTLMarkerBack;
		wxColor		m_oTLThroughLine;
		wxColor		m_oTLTick;
		wxColor		m_oTLTickNumber;
		wxColor		m_oTLLabels;		
		wxColor		m_oTLSelRow;
		wxColor		m_oActionDefValBack;
};
//---------------------------------------------------------------------------
class CAppIcons
{
	public:
		CAppIcons();
	
        // Icons
        wxIcon   m_oAppLogo;
        
        wxBitmap m_oShowLayer;
        wxBitmap m_oHideLayer;

        wxBitmap m_o1StKeyFrame;
        wxBitmap m_oKeyFrame;
        
        wxBitmap m_oWrapFinalValue;
        wxBitmap m_oWrapInitialValue;
        wxBitmap m_oWrapLoop;
        wxBitmap m_oWrapPingPong;

        wxBitmap m_oGroupNode;
        wxBitmap m_oSpriteNode;
        wxBitmap m_oBoneNode;
        wxBitmap m_oMeshNode;
};
//---------------------------------------------------------------------------
class CSkelPose
{
	public:
		CFEArray<CFEVect2> m_oPos;
		CFEArray<FEReal> m_rAngle;
};
//---------------------------------------------------------------------------
class CAppGlobals
{
    public:
        CAppGlobals();
		~CAppGlobals();

        // customization
		CAppColors					m_oColorScheme;
		CAppIcons                   m_oIconScheme;

		// color funcs
		CFEKFBFunc<CFEColor>		m_oBoneColor;
        CFEKFBFunc<CFEColor>		m_oBoneKFColor;
        CFEKFBFunc<CFEColor>		m_oSelBoneColor;
        CFEKFBFunc<CFEColor>		m_oSelEditBoneColor;
        

        // Initialization
        wxString                    m_sAppDir;
        wxString                    m_sExecCmdLine;
        wxString                    m_sInputFilename;
        wxString                    m_sSkeletonDefinition;
        bool                        m_bRelaunchImage;

        //
        CSkeleton*                  m_poSkeleton;
        // CSkeleton*                  m_poPrevSkeleton;
        
        uint                        m_uiNumFrames;          // The number of keyframes in the animation.
        uint                        m_uiCurFrame;           // The current frame of the sequence
        std::vector<FEHandler>      m_oFrames;
        CFEString                   m_sFrameFile;           // The current frame file

        CFERenderer*                m_poRenderer;
        CFERenderer*                m_poPrevRenderer;

		///
        CFEFont*                    m_poDefaultFont;
        FEHandler                   m_hDefaultSprite;

               // std::vector<fipImage*>  m_oFrames;
        float                       m_fTotalAnimTime;       // The total animation time.
        float                       m_fCurAnimTime;         // The current animation time while playing.
        float                       m_fGlobalTime;			// Time since app start
        bool                        m_bPlaying;        
		uint						m_uiFrameBeforePlay;	// Selected frame before start playing the animation
		bool						m_bSettingBoneControls;	// mutex to deal with generation of events from another event

        // Runtime values
        float                       m_fTime;                // The current animation time
        int                         m_iSelBone;             // The currently selected bone
        int                         m_iSelEditBone;         // Selected bone for editing
        CFEVect2                    m_oOldBonePos;          // Old bone position before start editing.
        float						m_fOldAngle;			// Old angle between the editing bone and it's parent bone.
		CFEVect2                    m_oOldParentPos;		// Old position of the parent.

		CFEVect2                    m_oAimPos;              // Center point of the canvas (in canvas coords)
        CFEVect2                    m_oTrans;               // Translation over the display.
        float                       m_fZoom;                // Zoom over the display.

        // Edit time vars      
        CFEVect2                    m_oCrossHair;          // Position of the crosshair.
        float                       m_fOldZoom;            // Zoom over the display.
        CFEVect2                    m_oOldTrans;           // Zoom over the display.
        CFEVect2                    m_oIniCursorCoord;        
        EEditMode                   m_eEditMode;            // Edition mode

        wxString                    m_sWorkingDir;          // Working dir.
        CActorEditorGUI*			GUI;
   
        CSkelPose*					m_poCopyPose;			// Clipboarded bone positions.
		
		CUndoStack<CUndoState>		m_oUndoStack;
		
		// block selection
		uint						m_uiBlockStartFrame;
		uint						m_uiBlockEndFrame;
		uint						m_uiBlockSelState;
		uint						m_uiBlockCurCol;
				
    public:
		
		CUndoState* poGetUndoState();
		void PasteUndoState(CUndoState* _poUS);

		// To be called once after edition of some feature is going to start.
		void EditBegin();
		
		// To be called once after edition of some feature has finished.
		void EditEnd();

		void EditUndo();
		void EditRedo();

        // public global methods
        int				iGetKeyFrame(CBone* _poBone,uint _uiFrameTick,unsigned int _uiFlags = 0x03);
        EFEKFLerpFunc	eGetLerpFunc(CBone* _poBone,uint _uiFrameTick);

		/// Retrieves the current pose into a CSkelPose structure
        CSkelPose*		poGetCurrentPose();
        
        /// Pastes the given pose
        void			PastePose(CSkelPose* _poPose,bool _bForcedPaste = false);

        /// Removes all the key frames avobe the given tick.
        void			TrimAnimation(uint _uiFrameTick);

        /// Inserts a frame in the given animation tick
        void			InsertFrameAt(uint _uiFrameTick);

        /// Deletes the frame in the given animation tick
        void			ClearFrameAt(uint _uiFrameTick);

        /// Deletes the frame in the given animation tick
        void			DeleteFrameAt(uint _uiFrameTick);

        /// Copies the keyframes contained in the source frame tick into the destination frametick
        void			CopyKeyFramesFromTo(uint _uiSrcFrameTick,uint _uiDstFrameTick);

		/// Fixes sprites to be synched with skeleton animation.
        void			SyncSprites(bool _bInvalidateBackups = false);
        
        void SetKeyFrameFuncInterpolation(uint _uiBone,EFEKFLerpFunc _eLF);
        void SetKeyFrameInterpolation(uint _uiBone,uint _uiKeyFrame,EFEKFLerpFunc _eLF);
        
        // Coordinate conversion routines
        CFEVect2 oScreen2CanvasCoords(const CFEVect2& _oPos);
        CFEVect2 oCanvas2ScreenCoords(const CFEVect2& _oPos);

        //----------------------------------------------------------------------------
		// returns a key frame index from the given time or -1 if it doesn't exist.
		//---------------------------------------------------------------------------
		template <class T> int iGetKeyFrame(CFEKFBFunc<T>& _oFunc,uint _uiFrameTick);
};

//----------------------------------------------------------------------------
// returns a key frame index from the given time or -1 if it doesn't exist.
//---------------------------------------------------------------------------
template <class T>
inline int CAppGlobals::iGetKeyFrame(CFEKFBFunc<T>& _oFunc,uint _uiFrameTick)
{
	for (uint i=0;i<_oFunc.uiGetNumKeyFrames();i++)
	{
		CFEKeyFrame* poKF = _oFunc.poGetKeyFrame(i);

		if (poKF != NULL)
		{
			FEReal rKF = (poKF->rGetKeyTime() * (FEReal)TICKS_PER_SEC);

			if (CFEMath::bBetween(rKF-EPSILON,rKF+EPSILON,_uiFrameTick))
				return(i);
		}
	}

	return(-1);
}
       
//---------------------------------------------------------------------------
extern CAppGlobals globals;
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------