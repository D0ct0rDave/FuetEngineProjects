//---------------------------------------------------------------------------
//
// Name:        C2DisplaysApp.h
// Author:      Demented
// Created:     19/04/2010 22:10:46
// Description: 
//
//---------------------------------------------------------------------------

#ifndef __C2DisplaysApp_h__
#define __C2DisplaysApp_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
#endif

class C2DisplaysApp : public wxApp
{
	public:
		bool OnInit();
		int OnExit();
};

#endif
