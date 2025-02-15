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

#include <cairo.h>
#include <cairo-win32.h>
#include <vector>
#include <FreeImage.h>
#include <FreeImagePlus.h>
#include <FuetEngine.h>
#include "FuetEngineExt.h"
//---------------------------------------------------------------------------

const wxString AE_ANIMATION_FILE_EXT = ".aea";
const wxString AE_POSE_FILE_EXT = ".aep";
const wxString AE_FRAMELIST_FILE_EXT = ".lst";

//---------------------------------------------------------------------------
typedef enum EEditMode { 

	EM_NONE,
	
	EM_TRANSLATE,
	EM_SCALE,
	EM_EDITBONES,
	
	EM_NUM_MODES,
};
//---------------------------------------------------------------------------
class CBone
{
    public:
        CFEKFBFunc<CFEVect2>    m_oPos;
        CFESprite*              m_poSprite;
};
//---------------------------------------------------------------------------
enum 
{
    BONE_HEAD,
    BONE_NECK,
    
    BONE_LSHOULDER,
    BONE_RSHOULDER,
    BONE_LELBOW,
    BONE_RELBOW,
    BONE_LHAND,
    BONE_RHAND,
    
    BONE_HIP,
    BONE_LHIP,
    BONE_RHIP,
    BONE_LKNEE,
    BONE_RKNEE,
    BONE_LFOOT,
    BONE_RFOOT,
    BONE_LTFOOT,
    BONE_RTFOOT,
    
    MAX_BONES
};
//---------------------------------------------------------------------------
extern const char* BONE_NAMES[MAX_BONES];
//---------------------------------------------------------------------------
class CPuppet
{
    public:
        CBone   m_oBone[MAX_BONES];
};
//---------------------------------------------------------------------------
class CAppGlobals
{
    public:
        CAppGlobals()
        {
            m_uiCurFrame = 0;
            m_fTime = 0.0f; 
            
            m_sFrameFile = "";
            m_fAnimTime = 1.0f;
            m_bLoop = true;
            m_iSelBone = -1;
            m_bMovingBone = false;
            m_bShowLabels = true;
            m_oTrans.x = 0.0;
            m_oTrans.y = 0.0;
            m_fZoom = 1.0;
            
            m_oIniCursorCoord.x = 0.0;
            m_oIniCursorCoord.y = 0.0;
        }

        //
        CPuppet                 m_oPuppet;
        std::vector<cairo_surface_t *>  m_oFrames;
        uint                    m_uiCurFrame;           // The current frame of the sequence
        CFEString               m_sFrameFile;           // The current frame file
        // std::vector<fipImage*>  m_oFrames;

        float                   m_fAnimTime;            // The total animation time.
        float                   m_fCurAnimTime;         // The current animation time while playing.
        bool                    m_bLoop;                // Animation loops?
        bool                    m_bPlaying;

        // Runtime values       
        float                   m_fTime;                // The current animation time
        int                     m_iSelBone;             // The currently selected bone
        bool                    m_bMovingBone;          // The currently selected bone
        bool                    m_bShowLabels;          // Show bone labels
        
        CFEVect2                m_oTrans;               // Translation over the display.
        float                   m_fZoom;               // Zoom over the display.
        
        float                   m_fOldZoom;            // Zoom over the display.
        CFEVect2                m_oOldTrans;           // Zoom over the display.

        CFEVect2                m_oIniCursorCoord;        
        EEditMode               m_eEditMode;            // Edition mode
};
//---------------------------------------------------------------------------
extern CAppGlobals globals;
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------