//---------------------------------------------------------------------------
//
// Name:        GTSViewerApp.h
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: 
//
//---------------------------------------------------------------------------

#ifndef __CGTSVIEWERGUIApp_h__
#define __CGTSVIEWERGUIApp_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
#endif

class CActorEditorApp : public wxApp
{
	public:
		bool OnInit();
		int OnExit();
};

#endif
