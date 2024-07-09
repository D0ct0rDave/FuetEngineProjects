//---------------------------------------------------------------------------
//
// Name:        GTSViewerApp.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: 
//
//---------------------------------------------------------------------------
#include "CAppGlobals.h"
#include "ActorEditor.h"
#include "CActorEditorGUI.h"
#include <wx/filename.h>
#include <FreeImage.h>

/*
namespace std
{
void __cdecl _Xlength_error(char const *);
void __cdecl _Xlength_error(char const *)
{
}
}
*/

static const wxCmdLineEntryDesc g_cmdLineDesc [] =
{
     { wxCMD_LINE_SWITCH,  wxT("h"), wxT("help"), wxT("displays help on the command line parameters"), wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
     { wxCMD_LINE_OPTION,  wxT("i"), wxT("input"), wxT("The input filename to start with"), wxCMD_LINE_VAL_STRING, 0 },
     { wxCMD_LINE_OPTION,  wxT("sd"), wxT("skeleton_definition"), wxT("The filename with the skeleton definition to start with"), wxCMD_LINE_VAL_STRING, 0 },
     // { wxCMD_LINE_SWITCH, wxT("t"), wxT("test"), wxT("test switch"),    wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_MANDATORY  },
     // { wxCMD_LINE_SWITCH, wxT("s"), wxT("silent"), wxT("disables the GUI") },
      { wxCMD_LINE_NONE }
};
 
IMPLEMENT_APP(CActorEditorApp)

bool CActorEditorApp::OnInit()
{
    FreeImage_Initialise();
	wxInitAllImageHandlers();   //Initialize graphic format handlers
    globals.m_sExecCmdLine   = wxString( argv[0] );	
    globals.m_sAppDir = wxFileName(globals.m_sExecCmdLine).GetPath();
    globals.m_sInputFilename = "";
	globals.m_sSkeletonDefinition = globals.m_sAppDir + "\\skeldef.def";

    if (!wxApp::OnInit())
        return false;

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

void CActorEditorApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    parser.SetDesc(g_cmdLineDesc);
    // must refuse '/' as parameter starter or cannot use "/path" style paths
    parser.SetSwitchChars (wxT("-"));
}
 
bool CActorEditorApp::OnCmdLineParsed(wxCmdLineParser& parser)
{    
    // to get at your unnamed parameters use
    parser.Found( wxT("i"), &globals.m_sInputFilename);
    parser.Found( wxT("sd"), &globals.m_sSkeletonDefinition);
    return true;
}