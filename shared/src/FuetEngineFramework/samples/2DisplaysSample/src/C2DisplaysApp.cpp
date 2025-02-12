//---------------------------------------------------------------------------
//
// Name:        C2DisplaysApp.cpp
// Author:      Demented
// Created:     19/04/2010 22:10:46
// Description: 
//
//---------------------------------------------------------------------------
/*
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
*/

#include "C2DisplaysApp.h"
#include "C2DisplaysGUI.h"

IMPLEMENT_APP(C2DisplaysApp)
static C2DisplaysGUI* frame = NULL;

bool C2DisplaysApp::OnInit()
{
    C2DisplaysGUI* frame = new C2DisplaysGUI(NULL);
    SetTopWindow(frame);
    frame->Show();
    return true;
}

int C2DisplaysApp::OnExit()
{
    delete(frame);

    // http://msdn.microsoft.com/en-us/library/e5ewb1h3%28VS.80%29.aspx
    // _CrtDumpMemoryLeaks
    // _CrtSetReportMode

    _CrtSetDbgFlag(0); // to disable ultra looooong memory leak dump.

	return 0;
}
