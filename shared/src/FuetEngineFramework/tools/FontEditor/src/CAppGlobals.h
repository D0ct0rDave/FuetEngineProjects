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
	#include <wx/string.h>
#else
	#include <wx/wxprec.h>
#endif

#include <vector>
#include "CFontGenerator.h"
//---------------------------------------------------------------------------

	#define WXCOLOR2UICOLOR(c) (c.Red() | (c.Green()<<8) | (c.Blue()<<16))
	#define UICOLOR2WXCOLOR(c) (wxColour((unsigned long)c))

//---------------------------------------------------------------------------
class CFontEditorGUI;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
const wxString APP_NAME = _T("FuetEngine Font Editor"); 
const wxString FE_FONT_FILE_EXT = _T(".font");
//---------------------------------------------------------------------------
class CAppColors
{
	public:
		CAppColors();
	
        // Colors
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
class CAppGlobals
{
    public:
        CAppGlobals();
        
        CAppColors					m_oColorScheme;
        
		
		std::vector<wxString>		m_sFontList;
		unsigned int				m_uiCurFont;
		
		std::vector<CSpecialChar>	m_oSpecialChars;
		unsigned int				m_uiCurSC;
		
		unsigned int				m_uiFontSize;
		unsigned int				m_uiStrokeSize;
		
        wxString					m_sWorkingDir;
        wxString					m_sInputFilename;
        wxString					m_sExecCmdLine;
        
        CFontEditorGUI*				GUI;
};
//---------------------------------------------------------------------------
extern CAppGlobals globals;
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------