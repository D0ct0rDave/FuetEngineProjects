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
#include <wx/cmdline.h>

class CActorEditorApp : public wxApp
{
	public:
		virtual bool OnInit();
        virtual int OnExit();
		
        virtual void OnInitCmdLine(wxCmdLineParser& parser);
        virtual bool OnCmdLineParsed(wxCmdLineParser& parser);   

		/*
		// if we want MyTraits to be used we must override CreateTraits()
		virtual wxAppTraits *CreateTraits()
		{
			delete wxRendererNative::Set(new CWindowRenderer);
			return new CWRTraits;
		}
		*/
};

#endif
