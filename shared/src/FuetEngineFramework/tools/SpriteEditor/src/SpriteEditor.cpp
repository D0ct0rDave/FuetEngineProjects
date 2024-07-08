//---------------------------------------------------------------------------
//
// Name:        GTSViewerApp.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: 
//
//---------------------------------------------------------------------------
#include "CAppGlobals.h"
#include "SpriteEditor.h"
#include "CSpriteEditorGUI.h"
#include <FreeImage.h>

static const wxCmdLineEntryDesc g_cmdLineDesc [] =
{
     { wxCMD_LINE_SWITCH,  wxT("h"), wxT("help"), wxT("displays help on the command line parameters"), wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
     { wxCMD_LINE_OPTION,  wxT("i"), wxT("input"), wxT("The input filename to start with"), wxCMD_LINE_VAL_STRING, 0 },
     // { wxCMD_LINE_SWITCH, wxT("t"), wxT("test"), wxT("test switch"),    wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_MANDATORY  },
     // { wxCMD_LINE_SWITCH, wxT("s"), wxT("silent"), wxT("disables the GUI") },
      { wxCMD_LINE_NONE }
};
 
IMPLEMENT_APP(CSpriteEditorApp)

bool CSpriteEditorApp::OnInit()
{
	// should wxApp::OnFatalException() be called?
	wxHandleFatalExceptions(true);

    FreeImage_Initialise();
	wxInitAllImageHandlers();   //Initialize graphic format handlers
    globals.m_sExecCmdLine   = wxString( argv[0] );	
    globals.m_sInputFilename = "";

    if (!wxApp::OnInit())
        return false;

    CSpriteEditorGUI* frame = new CSpriteEditorGUI(NULL);
    SetTopWindow(frame);
    frame->Show();
    return true;
}
 
int CSpriteEditorApp::OnExit()
{
    FreeImage_DeInitialise(); // Cleanup !
    return 0;
}

void CSpriteEditorApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    parser.SetDesc(g_cmdLineDesc);
    // must refuse '/' as parameter starter or cannot use "/path" style paths
    parser.SetSwitchChars (wxT("-"));
}
 
bool CSpriteEditorApp::OnCmdLineParsed(wxCmdLineParser& parser)
{    
    // to get at your unnamed parameters use
    parser.Found( wxT("i"), &globals.m_sInputFilename);
    return true;
}

// and now for something different: this function is called in case of a
// crash (e.g. dereferencing null pointer, division by 0, ...)
void CSpriteEditorApp::OnFatalException()
{
	globals.OnException();
}
