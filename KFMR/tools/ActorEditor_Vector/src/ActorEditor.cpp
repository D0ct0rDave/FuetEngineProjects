//---------------------------------------------------------------------------
//
// Name:        GTSViewerApp.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: 
//
//---------------------------------------------------------------------------
#include "ActorEditor.h"
#include "CActorEditorGUI.h"
#include <FreeImage.h>

IMPLEMENT_APP(CActorEditorApp)

bool CActorEditorApp::OnInit()
{
    FreeImage_Initialise();
	wxInitAllImageHandlers();   //Initialize graphic format handlers
	
    CActorEditorGUI* frame = new CActorEditorGUI(NULL);
    SetTopWindow(frame);
    frame->Show();
    return true;
}
 
int CActorEditorApp::OnExit()
{
    FreeImage_DeInitialise(); // Cleanup !
    return 0;
}
