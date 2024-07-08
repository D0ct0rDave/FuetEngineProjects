//---------------------------------------------------------------------------
//
// Name:        GTSViewerApp.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: 
//
//---------------------------------------------------------------------------
#include "FontEditor.h"
#include "CFontEditorGUI.h"
#include <FreeImage.h>
#include "CAppGlobals.h"

static const wxCmdLineEntryDesc g_cmdLineDesc [] =
{
     { wxCMD_LINE_PARAM,  wxT(""), wxT(""), wxT("The input filename to start with"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
     { wxCMD_LINE_SWITCH,  wxT("h"), wxT("help"), wxT("displays help on the command line parameters"), wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },     
     // { wxCMD_LINE_SWITCH, wxT("t"), wxT("test"), wxT("test switch"),    wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_MANDATORY  },
     // { wxCMD_LINE_SWITCH, wxT("s"), wxT("silent"), wxT("disables the GUI") },
      { wxCMD_LINE_NONE }
};

IMPLEMENT_APP(CFontEditorApp)

bool CFontEditorApp::OnInit()
{
    FreeImage_Initialise();
	wxInitAllImageHandlers();   //Initialize graphic format handlers
    globals.m_sExecCmdLine   = wxString( argv[0] );	
    globals.m_sInputFilename = _T("");
    
    if (!wxApp::OnInit())
        return false;
	
    CFontEditorGUI* frame = new CFontEditorGUI(NULL);
    SetTopWindow(frame);
    frame->Show();
    return true;
}
 
int CFontEditorApp::OnExit()
{
    FreeImage_DeInitialise(); // Cleanup !
    return 0;
}

void CFontEditorApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    parser.SetDesc(g_cmdLineDesc);
    // must refuse '/' as parameter starter or cannot use "/path" style paths
    parser.SetSwitchChars (wxT("-"));
}

bool CFontEditorApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    // to get at your unnamed parameters use
    if (parser.GetParamCount()>0)
		globals.m_sInputFilename = parser.GetParam();
 
    return true;
}