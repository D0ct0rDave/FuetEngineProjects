//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class implementation
//
//---------------------------------------------------------------------------

#include "CActorEditorGUI.h"
#include <vector>
#include <wx/fontenum.h>
#include <wx/rawbmp.h>
#include <wx/filename.h>
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/xml/xml.h>
#include <wx/xml/xml.h>

#include <FreeImage.h>
#include <FreeImagePlus.h>

#include "components/wxInputPanel.h"
#include "components/dcsvg.h"
#include "CFontGenerator.h"
#include <cairo.h>
#include <cairo-win32.h>
#include <FuetEngine.h>

#include "FuetEngineExt.h"
#include "CLibConfigFileWriter.h"
//---------------------------------------------------------------------------
// CGTSViewerGUI
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(CActorEditorGUI,wxFrame)
	////Manual Code Start
	EVT_IDLE(CActorEditorGUI::OnIdle)
	// EVT_PAINT(CActorEditorGUI::OnPaint)
	EVT_LEFT_DOWN(CActorEditorGUI::OnMouseDown)
	EVT_MOTION(CActorEditorGUI::OnMouseMove)
	EVT_LEFT_UP(CActorEditorGUI::OnMouseUp)
	////Manual Code End
	
	EVT_CLOSE(CActorEditorGUI::OnClose)
	EVT_SLIDER(ID_SLD_ANIMTIME,CActorEditorGUI::WxSlider1Slider)
	EVT_CHECKBOX(ID_CB_LOOP,CActorEditorGUI::cb_LoopClick)
	EVT_BUTTON(ID_BT_PLAY,CActorEditorGUI::WxButton1Click)
	
	EVT_COMMAND_SCROLL(ID_SB_KEYFRAME,CActorEditorGUI::sb_KeyFrameScroll)
	EVT_MENU(ID_MNU_LOADACTORANIM_1021, CActorEditorGUI::MnuloadActorAnim1021Click)
	EVT_MENU(ID_MNU_SAVEACTORANIM_1022, CActorEditorGUI::MnuSaveActorAnim1022Click)
	EVT_MENU(ID_MNU_QUIT_1004, CActorEditorGUI::Mnuquit1004Click0)
	EVT_MENU(ID_MNU_SAVEASSTANDARDPOSE_1032, CActorEditorGUI::Mnusaveasstandardpose1032Click)
	EVT_MENU(ID_MNU_LOADSTANDARDPOSE_1045, CActorEditorGUI::Mnuloadstandardpose1045Click)
	EVT_MENU(ID_MNU_LOADFRAMESEQUENCELIST_1047, CActorEditorGUI::Mnuloadframesequencelist1047Click)
	EVT_MENU(ID_MNU_SNAPKEYFRAMESTOSEQUENCE_1065, CActorEditorGUI::Mnusnapkeyframestosequence1065Click)
	EVT_TIMER(ID_WXTIMER1,CActorEditorGUI::WxTimer1Timer)
	EVT_CHECKBOX(ID_CB_SHOWLABELS,CActorEditorGUI::cb_ShowLabelsClick)
END_EVENT_TABLE()
////Event Table End

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

const char* BONE_NAMES[MAX_BONES] = {
    "head",
    "neck",
    "lshoulder",
    "rshoulder",
    "lelbow",
    "relbow",
    "lhand",
    "rhand",
    "hip",
    "lhip",
    "rhip",
    "lknee",
    "rknee",
    "lfoot",
    "rfoot",
    "ltfoot",
    "rtfoot"
};

class CPuppet
{
    public:
        CBone   m_oBone[MAX_BONES];
};

class CGlobals
{
    public:
        CGlobals()
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
CGlobals globals;
class CActorEditorGUI* goEditorGUI;
//---------------------------------------------------------------------------

const wxString AE_ANIMATION_FILE_EXT = ".aea";
const wxString AE_POSE_FILE_EXT = ".aep";
const wxString AE_FRAMELIST_FILE_EXT = ".lst";

//---------------------------------------------------------------------------
// Fucntion prototypes
void RenderMainDisplay(cairo_t* cr, unsigned int width, unsigned int height);
void RenderPreviewDisplay(cairo_t* cr, unsigned int width, unsigned int height);

//Do not add custom headers between
//Header Include Start and Header Include End
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//---------------------------------------------------------------------------
void MainDisplayPaintEvent(wxWindow* _poWnd)
{
    wxPaintDC dc(_poWnd);
    wxRect rect = _poWnd->GetClientRect();
    
    if(rect.width == 0 || rect.height == 0)
    {
        return;
    }

    // Get the handle of the wxWindow
    HWND hwnd = (HWND)_poWnd->GetHandle();

    // Now get the native windows DC for this window
    HDC hdc = ::GetDC(hwnd);

    // Create a double buffer for blitting to
    // the screen to prevent screen flicker. Pass
    // the double buffer to cairo and blit it
    // in the paint routine.
    HDC dcbuffer = CreateCompatibleDC(hdc);
    HBITMAP hbuffer = CreateCompatibleBitmap(hdc, rect.width, rect.height);
    SelectObject(dcbuffer, hbuffer); 

    // Create a cairo surface we can draw on directly
    cairo_surface_t* cairo_surface = cairo_win32_surface_create(dcbuffer);
    cairo_t* cairo_image = cairo_create(cairo_surface);

    // Render the output into the cairo image
    RenderMainDisplay(cairo_image, rect.width, rect.height);

    // Now blit the object to the screen
    BitBlt(hdc, 0, 0, rect.width, rect.height, dcbuffer, 0, 0, SRCCOPY);

    // Tear down the cairo object now that we don't need
    // it anymore.
    cairo_destroy(cairo_image);
    cairo_surface_destroy(cairo_surface);

    DeleteDC(dcbuffer);
    DeleteObject(hbuffer);

    // Because we called ::GetDC make sure we release the handle
    // back to the system or we'll have a memory leak.
    ::ReleaseDC(hwnd,hdc);
}
//---------------------------------------------------------------------------
void PreviewDisplayPaintEvent(wxWindow* _poWnd)
{
    wxPaintDC dc(_poWnd);
    wxRect rect = _poWnd->GetClientRect();
    
    if(rect.width == 0 || rect.height == 0)
    {
        return;
    }

    // Get the handle of the wxWindow
    HWND hwnd = (HWND)_poWnd->GetHandle();

    // Now get the native windows DC for this window
    HDC hdc = ::GetDC(hwnd);

    // Create a double buffer for blitting to
    // the screen to prevent screen flicker. Pass
    // the double buffer to cairo and blit it
    // in the paint routine.
    HDC dcbuffer = CreateCompatibleDC(hdc);
    HBITMAP hbuffer = CreateCompatibleBitmap(hdc, rect.width, rect.height);
    SelectObject(dcbuffer, hbuffer); 

    // Create a cairo surface we can draw on directly
    cairo_surface_t* cairo_surface = cairo_win32_surface_create(dcbuffer);
    cairo_t* cairo_image = cairo_create(cairo_surface);

    // Render the output into the cairo image
    RenderPreviewDisplay(cairo_image, rect.width, rect.height);

    // Now blit the object to the screen
    BitBlt(hdc, 0, 0, rect.width, rect.height, dcbuffer, 0, 0, SRCCOPY);

    // Tear down the cairo object now that we don't need
    // it anymore.
    cairo_destroy(cairo_image);
    cairo_surface_destroy(cairo_surface);

    DeleteDC(dcbuffer);
    DeleteObject(hbuffer);

    // Because we called ::GetDC make sure we release the handle
    // back to the system or we'll have a memory leak.
    ::ReleaseDC(hwnd,hdc);
}
//---------------------------------------------------------------------------
bool bLoadFrameSequence(const wxString& _sFilename)
{
    FILE* fd = fopen(_sFilename.char_str(),"rb");
    if (fd != NULL)
    {
        // Delete possible existant frames.
        globals.m_oFrames.erase(globals.m_oFrames.begin(),globals.m_oFrames.end());

        char szFilename[1024];
        while (! feof(fd))
        {
            if (fscanf(fd,"%s",szFilename) == 1)
            {
                /*
                fipImage* poImg = new fipImage;
	            if ( poImg->load(szFilename))
	            {	
		            poImg->convertTo32Bits();
		            // globals.m_oFrames.push_back(poImg);
		            cairo_surface_t *img = cairo_image_surface_create_for_data(poImg->accessPixels(),CAIRO_FORMAT_ARGB32,poImg->getWidth(),poImg->getHeight(),poImg->getScanWidth());
		            globals.m_oFrames.push_back(img);
	            }
                */

	            cairo_surface_t* poImg = cairo_image_surface_create_from_png(szFilename);
	            globals.m_oFrames.push_back(poImg);
	        }
        }

        globals.m_sFrameFile = CFEString( _sFilename.char_str() );
        return(true);
    }
    else
    {
        globals.m_sFrameFile = "";
        return(false);
    }
}
//---------------------------------------------------------------------------
void SavePoseFile(const wxString& _sFilename,float _fAnimTime)
{
    CLibConfigFileWriter oFW(std::string( _sFilename.char_str()) );
    oFW.OpenSection("Pose");
    for (uint i=0;i<MAX_BONES;i++)
    {
        oFW.OpenSection(BONE_NAMES[i]);
            oFW.OpenSection("Pos");

                CFEVect2 oPos = globals.m_oPuppet.m_oBone[i].m_oPos.oGetValue(_fAnimTime);
                oFW.AddVar("x",oPos.x);
                oFW.AddVar("y",oPos.y);

            oFW.CloseSection();
        oFW.CloseSection();
    }

    oFW.CloseSection();
    oFW.Write();
}
//---------------------------------------------------------------------------
void LoadPoseFile(const wxString& _sFilename,float _fAnimTime)
{
    CFEConfigFile oFile( CFEString(_sFilename.char_str()) );
    if (oFile.bInitialized())
    {
        for (uint i=0;i<MAX_BONES;i++)
        {
            CFEString sVar = CFEString("Pose.") + CFEString(BONE_NAMES[i]);

            CFEVect2 oPos = globals.m_oPuppet.m_oBone[i].m_oPos.oGetValue(_fAnimTime);
            
            oPos.x = oFile.rGetReal(sVar + CFEString(".Pos.x"),oPos.x);
            oPos.y = oFile.rGetReal(sVar + CFEString(".Pos.y"),oPos.y);

            globals.m_oPuppet.m_oBone[i].m_oPos.Reset();
            globals.m_oPuppet.m_oBone[i].m_oPos.InsertKeyFrame(oPos,_fAnimTime,KFLF_LERP);
        }       
    }
}
// ----------------------------------------------------------------------------
std::string sGetLerpFunc(EFEKFLerpFunc _eLerpFunc)
{
    switch(_eLerpFunc)
    {
        case KFLF_CONSTANT:
	    return("constant");
	    break;
    	
	    case KFLF_LERP:
	    return("linear");
	    break;

	    case KFLF_SIN:
	    return("sin");
	    break;

	    case KFLF_EXP:
	    return("exp");
	    break;

	    case KFLF_RAND:
	    return("random");
	    break;

	    case KFLF_SINSIN:
	    return("sinsin");
	    break;

	    case KFLF_EXPLOG:
	    return("explog");
	    break;
	}
	
	return("linear");
}
//-----------------------------------------------------------------------------
static EFEKFBFuncWrapMode eGetWrapMode(const CFEString& _sWrapMode)
{
    if (_sWrapMode |= "Loop")
        return(KFBFWM_LOOP);

else if (_sWrapMode |= "PingPong")
        return(KFBFWM_PINGPONG);

else if (_sWrapMode |= "InitialValue")
        return(KFBFWM_INITIALVALUE);

else if (_sWrapMode |= "FinalValue")
        return(KFBFWM_FINALVALUE);

	return(KFBFWM_FINALVALUE);
}
//-----------------------------------------------------------------------------
static EFEKFLerpFunc eGetLerpFunc(const CFEString& _sLerpFunc)
{
    if (_sLerpFunc |= "constant")
        return(KFLF_CONSTANT);

else if (_sLerpFunc |= "sin")
        return(KFLF_SIN);

else if (_sLerpFunc |= "exp")
        return(KFLF_EXP);

else if (_sLerpFunc |= "random")
        return(KFLF_RAND);

else if (_sLerpFunc |= "linear")
        return(KFLF_LERP);

else if (_sLerpFunc |= "sinsin")
        return(KFLF_SINSIN);

else if (_sLerpFunc |= "explog")
        return(KFLF_EXPLOG);
        
	return(KFLF_LERP);
}
//-----------------------------------------------------------------------------
void WriteKeyFrames(CFEKFBFunc<CFEVect2>& _oCoords,CConfigFileWriter* _poFWriter,const std::string& _sWrapMode)
{
    _poFWriter->OpenSection("PosFunc");

		_poFWriter->AddVar("WrapMode",_sWrapMode);
        _poFWriter->AddVar("NumKeyFrames",_oCoords.uiGetNumKeyFrames());

        for (uint uiKeyFrame=0;uiKeyFrame<_oCoords.uiGetNumKeyFrames();uiKeyFrame++)
        {
            CFEKeyFrame* poKF = _oCoords.poGetKeyFrame(uiKeyFrame);

            std::string sLF = sGetLerpFunc(poKF->eGetLerpFunc());
            CFEVect2 oPos = _oCoords.oGetValue( poKF->rGetKeyTime() );

            _poFWriter->OpenSection("KeyFrame",uiKeyFrame);

                _poFWriter->AddVar("x", oPos.x);
                _poFWriter->AddVar("y", oPos.y);
                _poFWriter->AddVar("Time",poKF->rGetKeyTime());
                _poFWriter->AddVar("LerpFunc", sLF );

            _poFWriter->CloseSection();
        };

    _poFWriter->CloseSection();
}
//---------------------------------------------------------------------------
void SaveAnimation(const wxString& _sFilename)
{
    CLibConfigFileWriter oFW(std::string( _sFilename.char_str()) );

    oFW.OpenSection("AnimEditor");

        oFW.AddVar("FrameSequenceFile", CFEStringUtils::sNormalizePath( globals.m_sFrameFile ).szString() );
        oFW.AddVar("AnimTime", globals.m_fAnimTime );
        oFW.AddVar("Loop", globals.m_bLoop  );

        for (uint i=0;i<MAX_BONES;i++)
        {
            oFW.OpenSection(BONE_NAMES[i]);

                WriteKeyFrames(globals.m_oPuppet.m_oBone[i].m_oPos,&oFW,globals.m_bLoop?"Loop":"FinalValue");

            oFW.CloseSection();
        }

    oFW.CloseSection();
    oFW.Write();    
}
//---------------------------------------------------------------------------
bool bLoadAnimation(const wxString& _sFilename)
{
    CFEConfigFile oFile( CFEString(_sFilename.char_str()) );
    if (oFile.bInitialized())
    {
        globals.m_sFrameFile = oFile.sGetString("AnimEditor.FrameSequenceFile","");
        if (! bLoadFrameSequence( globals.m_sFrameFile.szString() ))
        {       
             
        }

        globals.m_uiCurFrame = 0;
        globals.m_fAnimTime = oFile.rGetReal("AnimEditor.AnimTime",0.01f);
        globals.m_bLoop     = oFile.bGetBool("AnimEditor.Loop",true);

        for (uint i=0;i<MAX_BONES;i++)        
        {
            // delete the current contents.
            globals.m_oPuppet.m_oBone[i].m_oPos.Reset();

            // add key frames
            CFEString sVar      = CFEString("AnimEditor.") + CFEString( BONE_NAMES[i] ) + CFEString(".PosFunc");
            EFEKFBFuncWrapMode eWrapMode = eGetWrapMode( oFile.sGetString(sVar+".WrapMode","Loop") );
            uint uiNumKeyFrames = oFile.iGetInteger(sVar+".NumKeyFrames",0);

            globals.m_oPuppet.m_oBone[i].m_oPos.SetWrapMode( eWrapMode );

            for (uint k=0;k<uiNumKeyFrames;k++)
            {
                CFEString sKVar = sVar + CFEString(".KeyFrame") + CFEString(k);
                CFEVect2 oPos;

                oPos.x = oFile.rGetReal(sKVar + CFEString(".x"), 0.0);
                oPos.y = oFile.rGetReal(sKVar + CFEString(".y"), 0.0);
                FEReal rTime = oFile.rGetReal(sKVar + CFEString(".Time"), 0.0);
                EFEKFLerpFunc eLerpFunc = eGetLerpFunc( oFile.sGetString(sKVar+".LerpFunc","linear") );

                globals.m_oPuppet.m_oBone[i].m_oPos.InsertKeyFrame(oPos,rTime,eLerpFunc);
            }
        }

        return(true);
    }
    else
        return(false);
}
//---------------------------------------------------------------------------
const float RADIUS_THRESHOLD = 10;

void CActorEditorGUI::OnIdle(wxIdleEvent& event)
{
    MainDisplayPaintEvent(pn_Display);
    PreviewDisplayPaintEvent(pn_PreviewDisplay);
    
    event.RequestMore();
}

void CActorEditorGUI::OnMouseDown(wxMouseEvent& event)
{
}

void CActorEditorGUI::OnMouseMove(wxMouseEvent& event)
{
}

void CActorEditorGUI::OnMouseUp(wxMouseEvent& event)
{
}
//---------------------------------------------------------------------------
/*
void CActorEditorGUI::OnPaint(wxPaintEvent &event)
{
    return;
    MainDisplayPaintEvent(pn_Display);
    PreviewDisplayPaintEvent(pn_PreviewDisplay);
}
*/
//---------------------------------------------------------------------------
// Saves a given bitmap with a specified dimensions as a TGA file.
//---------------------------------------------------------------------------
void SaveTGAFile(wxBitmap* _poBmp,unsigned int _uiWidth,unsigned int _uiHeight,const wxString& _sFilename)
{
    wxImage oImg = _poBmp->ConvertToImage();
   
    // The bitmap to be saved oh disk    
    FIBITMAP* oOutBmp = FreeImage_Allocate(_uiWidth,_uiHeight,32);

    for (int j=0;j<_uiHeight;j++)
    {
        for (int i=0;i<_uiWidth;i++)
        {
            RGBQUAD oColor;
            oColor.rgbRed   = oImg.GetRed(i,j);
            oColor.rgbGreen = oImg.GetGreen(i,j);
            oColor.rgbBlue  = oImg.GetBlue(i,j);
            oColor.rgbReserved = 255; // oImg.GetAlpha(i,j);

            FreeImage_SetPixelColor(oOutBmp,i,(_uiHeight-j-1),&oColor);
        }
    }
    
    FreeImage_Save(FIF_TARGA, oOutBmp, _sFilename.ToAscii(),0);


    // wxImagePixelData oBmpData(oImg);
    
    /*
    wxNativePixelData oBmpData(*_poBmp);
    if (! oBmpData)
        return;
   
    // The bitmap to be saved oh disk    
    FIBITMAP* oOutBmp = FreeImage_Allocate(_uiWidth,_uiHeight,32);
    
    wxNativePixelData::Iterator p(oBmpData);
    for (int j=0;j<_uiHeight;j++)
    {
        for (int i=0;i<_uiWidth;i++)
        {
            p.Offset(oBmpData,j,i);
            RGBQUAD oColor;
            
            oColor.rgbRed   = p.Red();
            oColor.rgbGreen = p.Green();
            oColor.rgbBlue  = p.Blue();
            oColor.rgbReserved = p.Red();
            
            FreeImage_SetPixelColor(oOutBmp,i,j,&oColor);
        }
    }
    
    FreeImage_Save(FIF_TARGA, oOutBmp, _sFilename.ToAscii(),0);
    */
}
//----------------------------------------------------------------------------
void DisplayPanelMouseDown(wxInputPanel& panel,wxMouseEvent& event)
{
	FEReal rX = event.GetX();
	FEReal rY = event.GetY();

	// Select HUD element.
	// if ( event.ShiftDown() || event.ControlDown())
	{
		globals.m_oIniCursorCoord.x = rX;
		globals.m_oIniCursorCoord.y = rY;

		if (event.ShiftDown())
		{
		    if (globals.m_iSelBone != -1)
			    globals.m_eEditMode = EM_EDITBONES;
		}
	else if (event.ControlDown())
		{
			globals.m_eEditMode = EM_SCALE;
			globals.m_fOldZoom  = globals.m_fZoom;
		}
	else
		{
			globals.m_eEditMode = EM_TRANSLATE;
			globals.m_oOldTrans = globals.m_oTrans;
		}
	}
}
//----------------------------------------------------------------------------
void DisplayPanelMouseUp(wxInputPanel& panel,wxMouseEvent& event)
{
    globals.m_eEditMode = EM_NONE;
}

void DisplayPanelMouseMove(wxInputPanel& panel,wxMouseEvent& event)
{
    CFEVect2 oNewCursorPos(event.m_x,event.m_y);
	CFEVect2 oDeltaPos = (oNewCursorPos - globals.m_oIniCursorCoord);
	oDeltaPos /= globals.m_fZoom;
    CFEVect2 oDisplayPos = (oNewCursorPos/globals.m_fZoom) - globals.m_oTrans;

	switch (globals.m_eEditMode)
    {
        case EM_TRANSLATE:
        {
            globals.m_oTrans = globals.m_oOldTrans + oDeltaPos;
            
            CFEString sStr;
            sStr.Format("Trans: %.2f,%.2f",globals.m_oTrans.x,globals.m_oTrans.y);
            goEditorGUI->lb_Trans->SetLabel( sStr.szString() );
        }
        break;

        case EM_SCALE:
        {
            if (event.ControlDown())
            {
                globals.m_fZoom = globals.m_fOldZoom - ((float)oDeltaPos.y / 500.0f);
                if (globals.m_fZoom < 0.1)  globals.m_fZoom = 0.1;
                if (globals.m_fZoom > 16.0)  globals.m_fZoom = 16.0;

	            CFEString sStr;
	            sStr.Format("Zoom: %.2f",globals.m_fZoom);
                goEditorGUI->lb_Zoom->SetLabel( sStr.szString() );
            }
        }
        break;

        case EM_EDITBONES:
        {
            if (event.ShiftDown())
            {
	            // Insert this as a new (or not) keyframe.
	            // CFEVect2 oPos = globals.m_oPuppet.m_oBone[ globals.m_iSelBone ].m_oPos.oGetValue(globals.m_fTime);
                globals.m_oPuppet.m_oBone[ globals.m_iSelBone ].m_oPos.InsertKeyFrame(oDisplayPos,globals.m_fTime,KFLF_LERP);
            }
        }
        break;
        
        case EM_NONE:
        {
            // see if there is a selected bone there
	        globals.m_iSelBone = -1;

	        for (uint i=0;i<MAX_BONES;i++)
	        {
	            CFEVect2 oPos = globals.m_oPuppet.m_oBone[i].m_oPos.oGetValue(globals.m_fTime);

	            /// podría haber una función distancia!
	            CFEVect2 oD = oPos - oDisplayPos;
	            if (oD.rLength() < RADIUS_THRESHOLD)
	            {
	                globals.m_iSelBone = i;
	                return;
	            }
            }
		    
		    CFEString sStr;
		    sStr.Format("Coords: (%d,%d)",event.m_x,event.m_y);
		    goEditorGUI->lb_Frame->SetLabel(sStr.szString());
        }
        break;
	}
}

//---------------------------------------------------------------------------
CActorEditorGUI::CActorEditorGUI(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxFrame(parent, id, title, position, size, style)
{
	CreateGUIControls();
	goEditorGUI = this;	
}
//---------------------------------------------------------------------------
CActorEditorGUI::~CActorEditorGUI()
{
}
//---------------------------------------------------------------------------
void CActorEditorGUI::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
    ////GUI Items Creation Start

	CActorEditorGUIBAR = new wxStatusBar(this, ID_CFONTEDITORGUI, wxDOUBLE_BORDER | wxSTATIC_BORDER);
	CActorEditorGUIBAR->SetFieldsCount(1);
	CActorEditorGUIBAR->SetStatusText(wxT("Filename:"),0);
	int CActorEditorGUIBAR_Widths[1];
	CActorEditorGUIBAR_Widths[0] = -1;
	CActorEditorGUIBAR->SetStatusWidths(1,CActorEditorGUIBAR_Widths);

	dlgSelectFont =  new wxFontDialog(this);

	WxBoxSizer1 = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(WxBoxSizer1);
	this->SetAutoLayout(true);

	WxBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer3, 0, wxALIGN_CENTER | wxALL, 5);

	WxBoxSizer5 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer3->Add(WxBoxSizer5, 0, wxALIGN_CENTER | wxALL, 5);

	WxBoxSizer4 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer3->Add(WxBoxSizer4, 0, wxALIGN_CENTER | wxALL, 5);

	pn_PreviewDisplay = new wxPanel(this, ID_PN_PREVIEWDISPLAY, wxPoint(5, 5), wxSize(256, 192));
	WxBoxSizer4->Add(pn_PreviewDisplay,0,wxALIGN_CENTER | wxALL,5);

	WxBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer2, 0, wxALIGN_CENTER | wxALL, 5);

	lb_Frame = new wxStaticText(this, ID_LB_FRAME, wxT("Coords:-----------"), wxPoint(5, 5), wxDefaultSize, wxALIGN_CENTRE, wxT("lb_Frame"));
	lb_Frame->SetFont(wxFont(18, wxSWISS, wxNORMAL, wxNORMAL, false));
	WxBoxSizer2->Add(lb_Frame,0,wxALIGN_CENTER | wxALL,5);

	lb_Trans = new wxStaticText(this, ID_LB_TRANS, wxT("Offset:-------------------------"), wxPoint(199, 13), wxDefaultSize, 0, wxT("lb_Trans"));
	WxBoxSizer2->Add(lb_Trans,0,wxALIGN_CENTER | wxALL,5);

	lb_Zoom = new wxStaticText(this, ID_LB_ZOOM, wxT("Zoom:--------------"), wxPoint(348, 13), wxDefaultSize, 0, wxT("lb_Zoom"));
	WxBoxSizer2->Add(lb_Zoom,0,wxALIGN_CENTER | wxALL,5);

	cb_ShowLabels = new wxCheckBox(this, ID_CB_SHOWLABELS, wxT("Show labels"), wxPoint(448, 13), wxSize(97, 17), 0, wxDefaultValidator, wxT("cb_ShowLabels"));
	cb_ShowLabels->SetValue(true);
	WxBoxSizer2->Add(cb_ShowLabels,0,wxALIGN_CENTER | wxALL,5);

	WxTimer1 = new wxTimer();
	WxTimer1->SetOwner(this, ID_WXTIMER1);
	WxTimer1->Start(100);

	dlg_OpenFont =  new wxFileDialog(this, wxT("Choose a file"), wxT(""), wxT(""), wxT("*.font"), wxOPEN | wxCHANGE_DIR);

	dlg_SaveFont =  new wxFileDialog(this, wxT("Choose a file"), wxT(""), wxT(""), wxT("*.font"), wxSAVE | wxCHANGE_DIR);

	WxMenuBar1 = new wxMenuBar();
	wxMenu *ID_MNU_FILEMENU_1001_Mnu_Obj = new wxMenu(0);
	ID_MNU_FILEMENU_1001_Mnu_Obj->Append(ID_MNU_LOADACTORANIM_1021, wxT("&Load Actor Anim\tCtrl+O"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILEMENU_1001_Mnu_Obj->Append(ID_MNU_SAVEACTORANIM_1022, wxT("&Save Actor Anim\tCtrl+S"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILEMENU_1001_Mnu_Obj->AppendSeparator();
	ID_MNU_FILEMENU_1001_Mnu_Obj->Append(ID_MNU_QUIT_1004, wxT("&Quit\tCtrl+Q"), wxT(""), wxITEM_NORMAL);
	WxMenuBar1->Append(ID_MNU_FILEMENU_1001_Mnu_Obj, wxT("&File"));
	
	wxMenu *ID_MNU_MISC_1031_Mnu_Obj = new wxMenu(0);
	ID_MNU_MISC_1031_Mnu_Obj->Append(ID_MNU_SAVEASSTANDARDPOSE_1032, wxT("Save Here as Standard Pose\tCtrl+P"), wxT(""), wxITEM_NORMAL);
	ID_MNU_MISC_1031_Mnu_Obj->Append(ID_MNU_LOADSTANDARDPOSE_1045, wxT("Load Here Standard Pose\tCtrl+Shift+P"), wxT(""), wxITEM_NORMAL);
	ID_MNU_MISC_1031_Mnu_Obj->AppendSeparator();
	ID_MNU_MISC_1031_Mnu_Obj->Append(ID_MNU_LOADFRAMESEQUENCELIST_1047, wxT("Load frame sequence list\tCtrl+L"), wxT(""), wxITEM_NORMAL);
	ID_MNU_MISC_1031_Mnu_Obj->Append(ID_MNU_SNAPKEYFRAMESTOSEQUENCE_1065, wxT("Snap Keyframes to Sequence"), wxT(""), wxITEM_NORMAL);
	WxMenuBar1->Append(ID_MNU_MISC_1031_Mnu_Obj, wxT("&Misc"));
	SetMenuBar(WxMenuBar1);

	pn_Display = new wxInputPanel(this, ID_PN_DISPLAY, wxPoint(5, 5), wxSize(640, 480));
	WxBoxSizer5->Add(pn_Display,0,wxALIGN_CENTER | wxALL,5);

	sb_KeyFrame = new wxScrollBar(this, ID_SB_KEYFRAME, wxPoint(5, 495), wxSize(640, 17), wxSB_HORIZONTAL, wxDefaultValidator, wxT("sb_KeyFrame"));
	sb_KeyFrame->Enable(false);
	WxBoxSizer5->Add(sb_KeyFrame,0,wxALIGN_CENTER | wxALL,5);

	WxBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer4->Add(WxBoxSizer6, 0, wxALIGN_CENTER | wxALL, 5);

	bt_Play = new wxButton(this, ID_BT_PLAY, wxT("Play"), wxPoint(5, 5), wxSize(75, 25), 0, wxDefaultValidator, wxT("bt_Play"));
	WxBoxSizer6->Add(bt_Play,0,wxALIGN_CENTER | wxALL,5);

	cb_Loop = new wxCheckBox(this, ID_CB_LOOP, wxT("Loop"), wxPoint(90, 9), wxSize(40, 17), 0, wxDefaultValidator, wxT("cb_Loop"));
	WxBoxSizer6->Add(cb_Loop,0,wxALIGN_CENTER | wxALL,5);

	WxBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer4->Add(WxBoxSizer7, 0, wxALIGN_CENTER | wxALL, 5);

	sld_AnimTime = new wxSlider(this, ID_SLD_ANIMTIME, 1, 1, 100, wxPoint(5, 5), wxSize(150, 45), wxSL_HORIZONTAL | wxSL_LABELS | wxSL_BOTH | wxSL_SELRANGE , wxDefaultValidator, wxT("sld_AnimTime"));
	sld_AnimTime->SetRange(1,100);
	sld_AnimTime->SetValue(1);
	WxBoxSizer7->Add(sld_AnimTime,0,wxALIGN_CENTER | wxALL,5);

	SetStatusBar(CActorEditorGUIBAR);
	SetTitle(wxT("ActorEditor"));
	SetIcon(wxNullIcon);
	
	GetSizer()->Layout();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Center();
	
	////GUI Items Creation End

    // Hook mouse move function...
	pn_Display->HookMouseMove(DisplayPanelMouseMove);
	pn_Display->HookMouseDown(DisplayPanelMouseDown);
    pn_Display->HookMouseUp(DisplayPanelMouseUp);
    // pn_Display->HookKeyDown(DisplayPanelKeyDown);

	for (uint i=0;i<MAX_BONES;i++)
	{
	    globals.m_oPuppet.m_oBone[BONE_HEAD].m_oPos.InsertKeyFrame(CFEVect2(0.0,0.0),0.0,KFLF_LERP);
    }
}
//---------------------------------------------------------------------------
void CActorEditorGUI::OnClose(wxCloseEvent& event)
{
	Destroy();
}
//---------------------------------------------------------------------------
void CActorEditorGUI::MnuSaveActorAnim1022Click(wxCommandEvent& event)
{
	// insert your code here
	wxFileDialog dlgSaveFile(this, wxT("Save animation"), wxT(""), wxT(""), wxT("*")+AE_ANIMATION_FILE_EXT, wxSAVE | wxCHANGE_DIR);

    if (dlgSaveFile.ShowModal() == wxID_OK)
	{
		wxFileName sFilename(dlgSaveFile.GetDirectory(),dlgSaveFile.GetFilename());
        wxString sFullFilename  = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
		// globals.m_sFilename     = sFullFilename;
		// globals.m_sCurDir		= sFilename.GetPath(true);
		// globals.m_sCurFilename	= sFilename.GetName();

		SaveAnimation(sFullFilename+AE_ANIMATION_FILE_EXT);
	}

    /*
    // Create bitmap to write font letters
    wxBitmap oBitmap(640,480);
    wxMemoryDC oDC(oBitmap);

	// Setup the display context
	oDC.SetBackground(wxBrush(wxT("black"), wxSOLID));
	oDC.Clear();	
	oDC.SetTextForeground(*wxWHITE);
    oDC.DrawArc(100,100,50,200,200,100);

    // wxSVGFileDC oSVGDC("c:/out.svg",640,480,90);
    // oDC.Blit(wxPoint(0,0),wxSize(640,480),&oSVGDC,wxPoint(0,0));

    SaveTGAFile(&oBitmap,640,480,"c:/test.tga");
    */
}
//---------------------------------------------------------------------------
/*
 * MnuloadActorAnim1021Click
 */
void CActorEditorGUI::MnuloadActorAnim1021Click(wxCommandEvent& event)
{
	// insert your code here
	wxFileDialog dlgLoadFile(this, wxT("Save as standard pose"), wxT(""), wxT(""), wxT("*")+AE_ANIMATION_FILE_EXT, wxOPEN | wxCHANGE_DIR);

    if (dlgLoadFile.ShowModal() == wxID_OK)
	{
		wxFileName sFilename(dlgLoadFile.GetDirectory(),dlgLoadFile.GetFilename());
        wxString sFullFilename  = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
		// globals.m_sFilename     = sFullFilename;
		// globals.m_sCurDir		= sFilename.GetPath(true);
		// globals.m_sCurFilename	= sFilename.GetName();

		if (bLoadAnimation(sFullFilename+AE_ANIMATION_FILE_EXT))
		{
            sb_KeyFrame->SetScrollbar(0,globals.m_oFrames.size()/10,globals.m_oFrames.size(),globals.m_oFrames.size()/10,true);
            sld_AnimTime->SetValue(globals.m_fAnimTime*10);
            cb_Loop->SetValue(globals.m_bLoop);
		}
	}
}
//---------------------------------------------------------------------------
/*
 * Mnusaveasstandardpose1032Click
 */
void CActorEditorGUI::Mnusaveasstandardpose1032Click(wxCommandEvent& event)
{
	// insert your code here
	wxFileDialog dlgSaveFile(this, wxT("Save as standard pose"), wxT(""), wxT(""), wxT("*")+AE_POSE_FILE_EXT, wxSAVE | wxCHANGE_DIR);

    if (dlgSaveFile.ShowModal() == wxID_OK)
	{
		wxFileName sFilename(dlgSaveFile.GetDirectory(),dlgSaveFile.GetFilename());
        wxString sFullFilename  = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
		// globals.m_sFilename     = sFullFilename;
		// globals.m_sCurDir		= sFilename.GetPath(true);
		// globals.m_sCurFilename	= sFilename.GetName();

		SavePoseFile(sFullFilename+AE_POSE_FILE_EXT,globals.m_fTime);
	}
}
//---------------------------------------------------------------------------
/*
 * Mnuloadstandardpose1045Click
 */
void CActorEditorGUI::Mnuloadstandardpose1045Click(wxCommandEvent& event)
{
	// insert your code here
	wxFileDialog dlgLoadFile(this, wxT("Save as standard pose"), wxT(""), wxT(""), wxT("*")+AE_POSE_FILE_EXT, wxOPEN | wxCHANGE_DIR);

    if (dlgLoadFile.ShowModal() == wxID_OK)
	{
		wxFileName sFilename(dlgLoadFile.GetDirectory(),dlgLoadFile.GetFilename());
        wxString sFullFilename  = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
		// globals.m_sFilename     = sFullFilename;
		// globals.m_sCurDir		= sFilename.GetPath(true);
		// globals.m_sCurFilename	= sFilename.GetName();

		LoadPoseFile(sFullFilename+AE_POSE_FILE_EXT,globals.m_fTime);
	}
}
//---------------------------------------------------------------------------
/*
 * Mnuquit1004Click0
 */
void CActorEditorGUI::Mnuquit1004Click0(wxCommandEvent& event)
{
	// insert your code here
	if (wxMessageBox("Are you sure you want to exit?\n"
					"All data not saved will be lost",
					"Confirm",
					wxYES_NO,this) != wxYES) return;

	Close();
}
//---------------------------------------------------------------------------
// Render the output into the cairo image
void RenderMainDisplay(cairo_t* cr, unsigned int width, unsigned int height)
{
    float fTime = globals.m_fTime;

    double x=25.6,  y=128.0;
    double x1=102.4, y1=230.4,
           x2=153.6, y2=25.6,
           x3=230.4, y3=128.0;

    // Show current frame (if any ...)
    cairo_identity_matrix(cr);
    cairo_scale(cr,globals.m_fZoom,globals.m_fZoom);
    cairo_translate(cr, globals.m_oTrans.x, globals.m_oTrans.y);

    if (globals.m_oFrames.size())
    {
        cairo_surface_t *image = globals.m_oFrames[ globals.m_uiCurFrame ];
        
        int w = cairo_image_surface_get_width (image);
        int h = cairo_image_surface_get_height (image);
        uint uiXOfs = (width - w)/2;
        uint uiYOfs = (height - h)/2;

        cairo_translate(cr, uiXOfs, uiYOfs);
        cairo_set_operator(cr,CAIRO_OPERATOR_SOURCE);
        cairo_set_source_surface (cr, image, 0, 0);
        cairo_paint (cr);

        cairo_translate(cr, -uiXOfs, -uiYOfs);
    }
    else
    {
        cairo_set_source_rgba(cr,0.5, 0.5, 0.5, 1.0);
        cairo_paint(cr);
    }

    // draw bones
    for (uint i=0;i<MAX_BONES;i++)
    {
        cairo_identity_matrix(cr);
        cairo_scale(cr,globals.m_fZoom,globals.m_fZoom);
        cairo_translate(cr, globals.m_oTrans.x, globals.m_oTrans.y);

        CFEVect2 oPos = globals.m_oPuppet.m_oBone[i].m_oPos.oGetValue(globals.m_fTime);
        
        if (i == globals.m_iSelBone)
        {
            // This is the current selected key frame
            cairo_set_source_rgba (cr, 1, 1, 0, 0.85);            
            cairo_move_to(cr, oPos.x, oPos.y);
            cairo_arc (cr, oPos.x, oPos.y, 12.0, 0, 2*M_PI);

            cairo_set_line_width (cr, 1.0);
            cairo_stroke (cr);
        }
        else
        {
            if (globals.m_oPuppet.m_oBone[i].m_oPos.poGetKeyFrameAtTime(globals.m_fTime))
            {
                // This is a key frame.
                cairo_set_source_rgba (cr, 1, 1, 1, 0.5);
                
                cairo_move_to(cr, oPos.x, oPos.y);
                cairo_arc (cr, oPos.x, oPos.y, 10.0, 0, 2*M_PI);
                cairo_fill (cr);

                cairo_set_line_width (cr, 2.0);
                cairo_stroke (cr);
            }
            else
            {
                cairo_set_source_rgba (cr, 1, 0, 0, 0.5);
                cairo_move_to(cr, oPos.x, oPos.y);
                cairo_arc (cr, oPos.x, oPos.y, 10.0, 0, 2*M_PI);
                cairo_fill (cr);

                cairo_set_line_width (cr, 2.0);
                cairo_stroke (cr);
            }
        }

        // Show bone labels
        if (globals.m_bShowLabels)
        {
            cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                                           CAIRO_FONT_WEIGHT_BOLD);

            cairo_set_source_rgba (cr, 1, 1, 1, 0.75);
            cairo_set_font_size (cr,10.0);
            cairo_move_to (cr, oPos.x+16, oPos.y+2);
            cairo_show_text (cr, BONE_NAMES[i]);
        }
    }
    // draw extremities
    CFEVect2 oP0,oP1,oP2,oP3;
    //--
    oP0 = globals.m_oPuppet.m_oBone[BONE_LSHOULDER].m_oPos.oGetValue(fTime);
    oP1 = globals.m_oPuppet.m_oBone[BONE_LELBOW].m_oPos.oGetValue(fTime);
    oP2 = globals.m_oPuppet.m_oBone[BONE_LHAND].m_oPos.oGetValue(fTime);

    cairo_set_source_rgba(cr,0, 0, 0, 1.0);
    cairo_set_line_width (cr, 4.0);
    cairo_new_path(cr);
    cairo_move_to (cr, oP0.x, oP0.y);
    cairo_curve_to (cr, oP0.x, oP0.y, oP1.x, oP1.y, oP2.x, oP2.y);
    cairo_stroke (cr);
    //--
    oP0 = globals.m_oPuppet.m_oBone[BONE_RSHOULDER].m_oPos.oGetValue(fTime);
    oP1 = globals.m_oPuppet.m_oBone[BONE_RELBOW].m_oPos.oGetValue(fTime);
    oP2 = globals.m_oPuppet.m_oBone[BONE_RHAND].m_oPos.oGetValue(fTime);
    

    cairo_set_source_rgba(cr,0, 0, 0, 1.0);
    cairo_set_line_width (cr, 4.0);
    cairo_new_path(cr);
    cairo_move_to (cr, oP0.x, oP0.y);
    cairo_curve_to (cr, oP0.x, oP0.y, oP1.x, oP1.y, oP2.x, oP2.y);
    cairo_stroke (cr);    
    
    
    //--
    oP0 = globals.m_oPuppet.m_oBone[BONE_LHIP].m_oPos.oGetValue(fTime);
    oP1 = globals.m_oPuppet.m_oBone[BONE_LKNEE].m_oPos.oGetValue(fTime);
    oP2 = globals.m_oPuppet.m_oBone[BONE_LFOOT].m_oPos.oGetValue(fTime);

    cairo_set_source_rgba(cr,0, 0, 0, 1.0);
    cairo_set_line_width (cr, 4.0);
    cairo_new_path(cr);
    cairo_move_to (cr, oP0.x, oP0.y);
    cairo_curve_to (cr, oP0.x, oP0.y, oP1.x, oP1.y, oP2.x, oP2.y);
    cairo_stroke (cr);
    //--
    oP0 = globals.m_oPuppet.m_oBone[BONE_RHIP].m_oPos.oGetValue(fTime);
    oP1 = globals.m_oPuppet.m_oBone[BONE_RKNEE].m_oPos.oGetValue(fTime);
    oP2 = globals.m_oPuppet.m_oBone[BONE_RFOOT].m_oPos.oGetValue(fTime);

    cairo_set_source_rgba(cr,0, 0, 0, 1.0);
    cairo_set_line_width (cr, 4.0);
    cairo_new_path(cr);
    cairo_move_to (cr, oP0.x, oP0.y);
    cairo_curve_to (cr, oP0.x, oP0.y, oP1.x, oP1.y, oP2.x, oP2.y);
    cairo_stroke (cr);
}
//---------------------------------------------------------------------------
// Render the output into the cairo image
void RenderPreviewDisplay(cairo_t* cr, unsigned int width, unsigned int height)
{
    float fTime;
    if (globals.m_bPlaying)
        fTime = globals.m_fCurAnimTime / globals.m_fAnimTime;
    else
        fTime = globals.m_fTime;

    // Show current frame (if any ...)
    cairo_identity_matrix(cr);
    cairo_scale(cr,(float)width / 640.0f,(float)height / 480.0f);

    cairo_set_source_rgba(cr,0.25,0.25,0.25, 1.0);
    cairo_paint(cr);

    // draw bones
    for (uint i=0;i<MAX_BONES;i++)
    {
        CFEVect2 oPos = globals.m_oPuppet.m_oBone[i].m_oPos.oGetValue(fTime);

        // This is the current selected key frame
        cairo_set_source_rgba (cr, 1, 0, 0, 1);            
        cairo_move_to(cr, oPos.x, oPos.y);
        cairo_arc (cr, oPos.x, oPos.y, 6.0, 0, 2*M_PI);

        cairo_set_line_width (cr, 1.0);
        cairo_stroke (cr);
    }
    
    for (uint i=0;i<2;i++)
    {
        if (i==0)
        {
            cairo_set_line_width (cr, 25.0);
            cairo_set_source_rgba(cr,0, 0, 0, 1.0);
        }
        else
        {
            cairo_set_line_width (cr, 15.0);
            cairo_set_source_rgba(cr,1,1, 1, 1.0);
        }
       
        // draw extremities
        CFEVect2 oP0,oP1,oP2,oP3;
        //--
        oP0 = globals.m_oPuppet.m_oBone[BONE_LSHOULDER].m_oPos.oGetValue(fTime);
        oP1 = globals.m_oPuppet.m_oBone[BONE_LELBOW].m_oPos.oGetValue(fTime);
        oP2 = globals.m_oPuppet.m_oBone[BONE_LHAND].m_oPos.oGetValue(fTime);

        cairo_new_path(cr);
        cairo_move_to (cr, oP0.x, oP0.y);
        cairo_curve_to (cr, oP0.x, oP0.y, oP1.x, oP1.y, oP2.x, oP2.y);
        cairo_stroke(cr);

        //--
        oP0 = globals.m_oPuppet.m_oBone[BONE_RSHOULDER].m_oPos.oGetValue(fTime);
        oP1 = globals.m_oPuppet.m_oBone[BONE_RELBOW].m_oPos.oGetValue(fTime);
        oP2 = globals.m_oPuppet.m_oBone[BONE_RHAND].m_oPos.oGetValue(fTime);      

        cairo_new_path(cr);
        cairo_move_to (cr, oP0.x, oP0.y);
        cairo_curve_to (cr, oP0.x, oP0.y, oP1.x, oP1.y, oP2.x, oP2.y);
        cairo_stroke(cr);

        //--
        oP0 = globals.m_oPuppet.m_oBone[BONE_LHIP].m_oPos.oGetValue(fTime);
        oP1 = globals.m_oPuppet.m_oBone[BONE_LKNEE].m_oPos.oGetValue(fTime);
        oP2 = globals.m_oPuppet.m_oBone[BONE_LFOOT].m_oPos.oGetValue(fTime);

        cairo_new_path(cr);
        cairo_move_to (cr, oP0.x, oP0.y);
        cairo_curve_to (cr, oP0.x, oP0.y, oP1.x, oP1.y, oP2.x, oP2.y);
        cairo_stroke(cr);

        //--
        oP0 = globals.m_oPuppet.m_oBone[BONE_RHIP].m_oPos.oGetValue(fTime);
        oP1 = globals.m_oPuppet.m_oBone[BONE_RKNEE].m_oPos.oGetValue(fTime);
        oP2 = globals.m_oPuppet.m_oBone[BONE_RFOOT].m_oPos.oGetValue(fTime);

        cairo_new_path(cr);
        cairo_move_to (cr, oP0.x, oP0.y);
        cairo_curve_to (cr, oP0.x, oP0.y, oP1.x, oP1.y, oP2.x, oP2.y);
        cairo_stroke(cr);
    }
}
//---------------------------------------------------------------------------
/*
 * WxTimer1Timer
 */
void CActorEditorGUI::WxTimer1Timer(wxTimerEvent& event)
{
	// insert your code here
    if (globals.m_bPlaying)
    {
        globals.m_fCurAnimTime += 0.1f;
        if (globals.m_fCurAnimTime > globals.m_fAnimTime)
        {
            if (! globals.m_bLoop) 
            {
                bt_Play->SetLabel("Play");
                globals.m_bPlaying = false;
                WxTimer1->Stop();
            }
            else
            {
                globals.m_fCurAnimTime -= globals.m_fAnimTime;
            }
        }
        else
        {
            
        } 
    }
//	Refresh();
}
/*
 * sb_KeyFrameScroll
 */
void CActorEditorGUI::sb_KeyFrameScroll(wxScrollEvent& event)
{
	// insert your code here
	globals.m_uiCurFrame = sb_KeyFrame->GetThumbPosition();
	globals.m_fTime = ((float)globals.m_uiCurFrame / (float)globals.m_oFrames.size());
}
//---------------------------------------------------------------------------
/*
 * WxButton1Click
 */
void CActorEditorGUI::WxButton1Click(wxCommandEvent& event)
{
	// insert your code here
	if (globals.m_bPlaying == false)
	{
	    globals.m_bPlaying = true;
	    bt_Play->SetLabel("Stop");
	    globals.m_fCurAnimTime = 0.0f;
	    WxTimer1->Start();
	}
	else
	{
	    globals.m_bPlaying = false;
	    bt_Play->SetLabel("Play");
	    globals.m_fCurAnimTime = 0.0f;
	    WxTimer1->Stop();
	}
}
//---------------------------------------------------------------------------
/*
 * cb_LoopClick
 */
void CActorEditorGUI::cb_LoopClick(wxCommandEvent& event)
{
	// insert your code here
	globals.m_bLoop = cb_Loop->GetValue();
}
//---------------------------------------------------------------------------

/*
 * WxSlider1Slider
 */
void CActorEditorGUI::WxSlider1Slider(wxCommandEvent& event)
{
	// insert your code here
	globals.m_fAnimTime = (float)sld_AnimTime->GetValue() / 10.0f;
}

/*
 * cb_ShowLabelsClick
 */
void CActorEditorGUI::cb_ShowLabelsClick(wxCommandEvent& event)
{
	// insert your code here
	globals.m_bShowLabels = cb_ShowLabels->GetValue();
}
//---------------------------------------------------------------------------
/*
 * Mnuloadframesequencelist1047Click
 */
void CActorEditorGUI::Mnuloadframesequencelist1047Click(wxCommandEvent& event)
{
	// insert your code here
	// insert your code here
	wxFileDialog dlgLoadFile(this, wxT("Load frame sequence list"), wxT(""), wxT(""), wxT("*")+AE_FRAMELIST_FILE_EXT, wxOPEN | wxCHANGE_DIR);

    if (dlgLoadFile.ShowModal() == wxID_OK)
	{
		wxFileName sFilename(dlgLoadFile.GetDirectory(),dlgLoadFile.GetFilename());
        wxString sFullFilename  = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
		// globals.m_sFilename     = sFullFilename;
		// globals.m_sCurDir		= sFilename.GetPath(true);
		// globals.m_sCurFilename	= sFilename.GetName();
		wxString sCurDir		= sFilename.GetPath(true);
		
		if (chdir(sCurDir.char_str()) == 0)
		{
    		if (bLoadFrameSequence( sFullFilename + AE_FRAMELIST_FILE_EXT ))
    		{
		        sb_KeyFrame->SetScrollbar(0,globals.m_oFrames.size()/10,globals.m_oFrames.size(),globals.m_oFrames.size()/10,true);
		        globals.m_uiCurFrame = 0;
            }
        }
	}
}
//---------------------------------------------------------------------------
/*
 * Mnusnapkeyframestosequence1065Click
 */
void CActorEditorGUI::Mnusnapkeyframestosequence1065Click(wxCommandEvent& event)
{
    if (globals.m_oFrames.size() == 0) return;

	// insert your code here
    for (uint i=0;i<MAX_BONES;i++)        
    {
        // add key frames
        uint k;
        CFEKFBFunc<CFEVect2> oNewPos;
        for (k=0;k<globals.m_oPuppet.m_oBone[i].m_oPos.uiGetNumKeyFrames();k++)
        {
            CFEKeyFrame* poKF = globals.m_oPuppet.m_oBone[i].m_oPos.poGetKeyFrame(k);
            float fOldTime = poKF->rGetKeyTime();
            CFEVect2 oPos = globals.m_oPuppet.m_oBone[i].m_oPos.oGetValue(fOldTime);
            EFEKFLerpFunc eLerpFunc = poKF->eGetLerpFunc();
            
            int iFrame = fOldTime * globals.m_oFrames.size();
            float fTime = (float)iFrame / (float)globals.m_oFrames.size();
            
            oNewPos.InsertKeyFrame(oPos,fTime,eLerpFunc);
	    }
        
        // Rebuild array.	    
	    globals.m_oPuppet.m_oBone[i].m_oPos.Reset();
        for (k=0;k<oNewPos.uiGetNumKeyFrames();k++)
        {
            CFEKeyFrame* poKF = oNewPos.poGetKeyFrame(k);
            float fTime = poKF->rGetKeyTime();
            CFEVect2 oPos = oNewPos.oGetValue(fTime);
            EFEKFLerpFunc eLerpFunc = poKF->eGetLerpFunc();
            
            globals.m_oPuppet.m_oBone[i].m_oPos.InsertKeyFrame(oPos,fTime,eLerpFunc);
	    }
    }
}
//---------------------------------------------------------------------------