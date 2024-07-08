//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class implementation
//
//---------------------------------------------------------------------------
#include "CAppGlobals.h"
/*
#include "images/group_layer_icon.xpm"
#include "images/icon_layer_icon.xpm"
#include "images/rect_layer_icon.xpm"
#include "images/label_layer_icon.xpm"
#include "images/icon_icon.xpm"
#include "images/fkeyframe_icon.xpm"
#include "images/keyframe_icon.xpm"
#include "images/link_icon.xpm"
#include "images/lock_grayed_icon.xpm"
#include "images/lock_ok_icon.xpm"
#include "images/lock_open_icon.xpm"
#include "images/lock_icon.xpm"
#include "images/logo_icon.xpm"
#include "images/new_layer_icon.xpm"
#include "images/ok_icon.xpm"
#include "images/play_icon.xpm"
#include "images/rename_icon.xpm"
#include "images/delete_icon.xpm"
#include "images/reset_icon.xpm"
#include "images/anim_icon.xpm"
#include "images/save_icon.xpm"
#include "images/eye_grayed_icon.xpm"
#include "images/eye_icon.xpm"
#include "images/rect_edition.xpm"
*/

#include "images/wrap_initialvalue_icon.xpm"
#include "images/wrap_finalvalue_icon.xpm"
#include "images/wrap_loop_icon.xpm"
#include "images/wrap_pingpong_icon.xpm"
#include "images/editor_icon_32x32.xpm"

#include "CSpriteEditorGUIFileOps.h"
#include "CSpriteEditorGUI.h"
//---------------------------------------------------------------------------
CAppGlobals globals;
float SCREEN_WIDTH;
float SCREEN_HEIGHT;
//---------------------------------------------------------------------------
CAppColors::CAppColors()
{
	m_oGroupBV = CFEColor(0.25,1,0,0.75);
	m_oLayerBV = CFEColor(1,1,0,0.75);
    m_oLayerPivot = CFEColor(1,0,0,0.75);
    m_oGrid = CFEColor(1,1,1,0.25);
    m_oGridHL = CFEColor(1,1,1,0.5);
    m_oBackground = CFEColor(0.35,0.35,0.35,1);
    m_oFrameBackground = CFEColor(0.25f,0.25f,0.5f,1.0f);
    m_oFrameRect = CFEColor(1,0,0,1); 
    m_oFrameSeqRect = CFEColor(1,1,0,1);     
    m_oSelFrameSeqRect = CFEColor(1,0,0,1);
    m_oPivotColor = CFEColor(0,1,0,1);
    m_oFrameNumColor = CFEColor(1.0f,1.0f,0.0f,1.0f);
    m_oFrameStartTimeColor = CFEColor(0.5f,1.0f,0.0f,1.0f);
    m_oFrameEndTimeColor   = CFEColor(1.0f,0.5f,0.0f,1.0f);
    
    m_oPrevFrameBackground = CFEColor(0.35f,0.35f,0.35f,1.0f);

    m_oCrossHairColor = CFEColor(0.0f,0.75f,1.0f,0.35f);

	m_oAppBack		= wxColor(0xc0,0xc0,0xc0);

	m_oGridLabelBase= wxColor(0xd6,0xd6,0xd6);
	m_oGridCellBase = wxColor(0xff,0xff,0xff);
	m_oGridCellText = wxColor(0x00,0x00,0x00);	

	m_oElemBack		= wxColor(255,250,229);
	m_oGenObjBack0	= wxColor(0xff,0xff,0xff);
	m_oGenObjBack1	= wxColor(0xf0,0xf0,0xf0);

	m_oSelCellBack	= wxColor(0x2e,0x54,0xf5);
	m_oSelCellText  = wxColor(0xff,0xff,0xff);
    
	m_oVisCellBack	= wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
	m_oLockCellBack = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
	m_oTLGridLine   = wxColor(0xc0,0xc0,0xc0);
	m_oTLDisabled	= wxColor(0xd6,0xd6,0xd6);
	m_oTLLineBack	= wxColor(0xd6,0xd6,0xd6);
	m_oTLMarkerLine	= wxColor(0xff,0x00,0x00);
	m_oTLMarkerBack	= wxColor(0xca,0x84,0x87);
	m_oTLThroughLine= wxColor(0x80,0x80,0xff);
	m_oTLTick		= wxColor(0x83,0x83,0x83);
	m_oTLTickNumber = wxColor(0x40,0x40,0x40);
	m_oTLLabels     = wxColor(0x40,0x40,0x40);
	m_oActionDefValBack = wxColor(0xff,0x99,0x55);
	m_oTLSelRow     = wxColor(0x00,0x00,0x55);
}
//---------------------------------------------------------------------------
CAppIcons::CAppIcons()
{
	m_oAppLogo = wxIcon(editor_icon_32x32_xpm);
/*
    m_oAppLogo = wxIcon(logo_icon);

    m_oShowLayer = wxBitmap(eye_icon);
    m_oHideLayer = wxBitmap(eye_grayed_icon);

    m_oLockedLayer   = wxBitmap(lock_icon);
    m_oUnlockedLayer = wxBitmap(lock_grayed_icon);

    m_oElemLayer   = wxBitmap(group_layer_icon);
    m_oLabelLayer  = wxBitmap(label_layer_icon);
    m_oIconLayer   = wxBitmap(icon_layer_icon_xpm);
    m_oRectLayer   = wxBitmap(rect_layer_icon);

    m_o1StKeyFrame = wxBitmap(fkeyframe_icon);
    m_oKeyFrame    = wxBitmap(keyframe_icon);
    
    m_oAnimation    = wxBitmap(anim_icon);
*/    
	m_oWrapInitialValue = wxBitmap(wrap_initialvalue_icon);
	m_oWrapFinalValue = wxBitmap(wrap_finalvalue_icon);
	m_oWrapLoop = wxBitmap(wrap_loop_icon);
	m_oWrapPingPong = wxBitmap(wrap_pingpong_icon);

};
//---------------------------------------------------------------------------
CAppGlobals::CAppGlobals()
{
    // m_uiCurFrame = 0;
    m_fTime = 0.0f; 
    m_uiNumFrames = 0;
    
    m_sFrameFile = "";
    
    m_fTotalAnimTime = 1.0f;
    m_fCurAnimTime = 0.0;
    
    m_bLoop = true;
    m_bDisplayGrid = false;
    m_oTrans.x = 0.0;
    m_oTrans.y = 0.0;
    m_fZoom = 1.0;

    m_fHScale = 1.0;
    m_fVScale = 1.0;

    m_oIniCursorCoord.x = 0.0;
    m_oIniCursorCoord.y = 0.0;
    
    m_oCrossHair.x = 0.0;
    m_oCrossHair.y = 0.0;

    m_bDisplayCrosshair = true;
    
    m_uiCurAction       = 0;    
    m_bRelaunchImage = false;

    GUI = NULL;
    m_iSelectedFrame    = -1;
    m_iFirstVisFrame    = 0;
    m_iSelectedAction   = -1;    
    m_hCurMaterial      = NULL;
    m_poSprite          = NULL;
    m_hCurSprInst       = NULL;
	m_bFinishingApp		= false;
	m_bSkipEvents		= false;
}
//---------------------------------------------------------------------------
/// Code to be called when an unhandled exception happens.
void CAppGlobals::OnException()
{
	// get time and date
	const uint BUFCHARS = 256;
	char szDate[BUFCHARS];	memset(szDate,0,BUFCHARS);
	char szTime[BUFCHARS];	memset(szTime,0,BUFCHARS);
	GetDateFormat(LOCALE_SYSTEM_DEFAULT,DATE_SHORTDATE,NULL,NULL,szDate,BUFCHARS);
	GetTimeFormat(LOCALE_SYSTEM_DEFAULT,TIME_FORCE24HOURFORMAT|TIME_NOTIMEMARKER,NULL,NULL,szTime,BUFCHARS);

	char *pszDate = szDate;
	char *pszTime = szTime;
	while (*pszDate)
	{
		if ((*pszDate == '\\') || (*pszDate == '/')) *pszDate = '-';
		*pszDate++;
	}
	while (*pszTime)
	{
		if (*pszTime== ':') *pszTime = '.';
		*pszTime++;
	}

	// Generate a meaningful filename.
	wxString sFilename = globals.m_sInputFilename + wxString("_Backup_") + wxString(szDate) + wxString('_') + wxString(szTime);

	// try saving the editor data in a safe place!
	bSaveSprite(m_poSprite,CFEString(sFilename.char_str()) );

	// Notify the user where has been the data saved!
	wxMessageBox("An error has occurred and the application will be closed.\n"
				 "The following backup file has been saved:\n"
				 +
				 sFilename
				 );
}
//---------------------------------------------------------------------------
// Coordinate conversion routines
CFEVect2 CAppGlobals::oScreen2CanvasCoords(const CFEVect2& _oPos)
{
	// punto en el "canvas"
	CFEVect2 oCanvasPos;
	oCanvasPos.x = (_oPos.x / globals.m_fZoom) + globals.m_oTrans.x;
	oCanvasPos.y = (_oPos.y / globals.m_fZoom) + globals.m_oTrans.y;
	return(oCanvasPos);
}
//---------------------------------------------------------------------------
CFEVect2 CAppGlobals::oCanvas2ScreenCoords(const CFEVect2& _oPos)
{
	// Punto en pantalla
	CFEVect2 oScreenPos;
	oScreenPos.x = (_oPos.x - globals.m_oTrans.x)*globals.m_fZoom;
	oScreenPos.y = (_oPos.y - globals.m_oTrans.y)*globals.m_fZoom;
	return(oScreenPos);
}
//---------------------------------------------------------------------------