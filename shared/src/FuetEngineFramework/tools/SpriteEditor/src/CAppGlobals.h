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
#ifdef __WXMSW__
	#include <wx/wx.h>
	#include <wx/frame.h>
#endif
#else
	#include <wx/wxprec.h>
#endif

#include <vector>
#include <FreeImage.h>
#include <FreeImagePlus.h>
#include <FuetEngine.h>
//---------------------------------------------------------------------------
const FEReal INITIAL_ZOOM = _1r;
const FEReal MIN_ZOOM     = pow(2.0f,-8.0f);
const FEReal MAX_ZOOM     = pow(2.0f, 4.0f);

const CFEVect2 INITIAL_TRANS  = CFEVect2(0,0);
const CFEVect2 INITIAL_AIMPOS = CFEVect2(640,480);

const FEReal   TRANS_STEP = 16.0;

extern float SCREEN_WIDTH;
extern float SCREEN_HEIGHT;
//---------------------------------------------------------------------------
class CSpriteEditorGUI;
//---------------------------------------------------------------------------
const wxString APP_NAME = wxString("FuetEngine Sprite Editor"); 
const wxString SE_SPRITE_FILE_EXT= wxString(".spr");
//---------------------------------------------------------------------------
typedef enum EEditMode { 

	EM_NONE,
	
	EM_TRANSLATE,
	EM_SCALE,
	EM_EDITPIVOT,
	EM_CROSSHAIR,
	EM_EDITRECT,
	
	EM_NUM_MODES,
};
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
        CFEColor	m_oFrameSeqRect;
        CFEColor	m_oSelFrameSeqRect;
        CFEColor	m_oPivotColor;

        CFEColor	m_oCrossHairColor;
        
        CFEColor	m_oFrameNumColor;
        CFEColor	m_oFrameStartTimeColor;
        CFEColor	m_oFrameEndTimeColor;

		wxColor		m_oAppBack;
		wxColor		m_oGridCellBase;
		wxColor		m_oGridCellText;
		wxColor		m_oGridLabelBase;
		
		wxColor		m_oElemBack;
		wxColor		m_oGenObjBack0;
		wxColor		m_oGenObjBack1;

		wxColor		m_oSelCellBack;
		wxColor		m_oSelCellText;

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
        
        /*
        wxBitmap m_oShowLayer;
        wxBitmap m_oHideLayer;
        wxBitmap m_oLockedLayer;
        wxBitmap m_oUnlockedLayer;
        wxBitmap m_o1StKeyFrame;
        wxBitmap m_oKeyFrame;
        wxBitmap m_oAnimation;
        */
        wxBitmap m_oWrapFinalValue;
        wxBitmap m_oWrapInitialValue;
        wxBitmap m_oWrapLoop;
        wxBitmap m_oWrapPingPong;
};
//---------------------------------------------------------------------------
class CAppGlobals
{
    public:
        CAppGlobals();

        // customization
		CAppColors					m_oColorScheme;
		CAppIcons                   m_oIconScheme;

        // Initialization
        wxString                    m_sExecCmdLine;
        wxString                    m_sInputFilename;
        bool                        m_bRelaunchImage;

        //
        CFESprite*                  m_poSprite;             // The sprite we're working on
        FEHandler                   m_hCurSprInst;          // Current Sprite Instance
        FEHandler                   m_hCurMaterial;         // The current material we're using with the sprite.
        int                         m_iSelectedAction;      // The selected action.
        int                         m_iSelectedFrame;       // The selected frame in the current action.
        int                         m_iFirstVisFrame;       // The first visible frame in the current action sequence.
        

        uint                        m_uiNumFrames;          // The number of keyframes in the animation.
        // uint                        m_uiCurFrame;           // The current frame of the sequence
        std::vector<FEHandler>      m_oFrames;
        CFEString                   m_sFrameFile;           // The current frame file

        CFERenderer*                m_poRenderer;
        CFERenderer*                m_poPrevRenderer;
        CFERenderer*                m_poFrameSeqRenderer;

		///
        CFEFont*                    m_poDefaultFont;
        FEHandler                   m_hDefaultSprite;

        float                       m_fHScale;
        float                       m_fVScale;

        float                       m_fTotalAnimTime;            // The total animation time.
        float                       m_fCurAnimTime;         // The current animation time while playing.
        bool                        m_bLoop;                // Animation loops?
        bool                        m_bPlaying;

        // Runtime values
        CFEVect2                    m_oOldPivot;            // The pivot before editing the current frame.
        CFEVect2                    m_oIniRectPos;          // The starting position when editing rects
        float                       m_fTime;                // The current animation time
        uint                        m_uiCurAction;          // Current sprite action (when creating new ones)

        bool                        m_bShowBackground;      // Should we show the background image?

        CFEVect2                    m_oTrans;               // Translation over the display.
        float                       m_fZoom;                // Zoom over the display.
		CFEVect2                    m_oAimPos;              // Center point of the canvas (in canvas coords)
        
        // Edit time vars
        bool                        m_bDisplayGrid;         // Show Grid on preview display.

        uint                        m_uiGridSize;
        uint                        m_uiGridSelection;
        
        bool                        m_bDisplayCrosshair;
        CFEVect2                    m_oCrossHair;          // Position of the crosshair.
        float                       m_fOldZoom;            // Zoom over the display.
        CFEVect2                    m_oOldTrans;           // Zoom over the display.
        CFEVect2                    m_oIniCursorCoord;
        EEditMode                   m_eEditMode;            // Edition mode

        wxString                    m_sWorkingDir;          // Working dir.
        CSpriteEditorGUI*           GUI;
		bool						m_bFinishingApp;
		bool						m_bSkipEvents;

        // Coordinate conversion routines
        CFEVect2 oScreen2CanvasCoords(const CFEVect2& _oPos);
        CFEVect2 oCanvas2ScreenCoords(const CFEVect2& _oPos);        

        ///
        void OnException();
};
//---------------------------------------------------------------------------
extern CAppGlobals globals;
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------