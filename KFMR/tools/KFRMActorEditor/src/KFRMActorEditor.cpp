//---------------------------------------------------------------------------
//
// Name:        GTSViewerApp.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: 
//
//---------------------------------------------------------------------------
#include "KFRMActorEditor.h"
#include "CKFRMActorEditorGUI.h"
#include <FreeImage.h>

IMPLEMENT_APP(CKFRMActorEditorApp)

bool CKFRMActorEditorApp::OnInit()
{
    FreeImage_Initialise();
	wxInitAllImageHandlers();   //Initialize graphic format handlers
	
    CKFRMActorEditorGUI* frame = new CKFRMActorEditorGUI(NULL);
    SetTopWindow(frame);
    frame->Show();
    return true;
}
 
int CKFRMActorEditorApp::OnExit()
{
    FreeImage_DeInitialise(); // Cleanup !
    return 0;
}
