//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class implementation
//
//---------------------------------------------------------------------------
#include "CAppGlobals.h"
#include "CFontEditorGUI.h"
//---------------------------------------------------------------------------
CAppGlobals globals;
//---------------------------------------------------------------------------
CAppColors::CAppColors()
{
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
CAppGlobals::CAppGlobals() : GUI(NULL)
{
	m_uiCurFont = 0;
	m_uiStrokeSize = 0;
}
//---------------------------------------------------------------------------