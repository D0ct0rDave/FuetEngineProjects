///-----------------------------------------------------------------
///
/// @file      C2DisplaysGUI.cpp
/// @author    Pepe
/// Created:   19/04/2010 22:21:44
/// @section   DESCRIPTION
///            C2DisplaysGUI class implementation
///
///------------------------------------------------------------------

#include "C2DisplaysGui.h"
#include <FuetEngine.h>
#include "2DisplaysApp.h"
//Do not add custom headers between
//Header Include Start and Header Include End
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// C2DisplaysGUI
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(C2DisplaysGUI,wxFrame)
	////Manual Code Start
	EVT_IDLE(C2DisplaysGUI::OnIdle)
	////Manual Code End
	
	EVT_CLOSE(C2DisplaysGUI::OnClose)
END_EVENT_TABLE()
////Event Table End

C2DisplaysGUI::C2DisplaysGUI(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxFrame(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

C2DisplaysGUI::~C2DisplaysGUI()
{
}

void C2DisplaysGUI::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxMenuBar1 = new wxMenuBar();
	wxMenu *ID_MNU_FILE_1007_Mnu_Obj = new wxMenu(0);

	/*
	ID_MNU_FILE_1007_Mnu_Obj->Append(ID_MNU_LOADMAP_1010, wxT("Load Map"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILE_1007_Mnu_Obj->Append(ID_MNU_SAVEMAP_1011, wxT("Save Map"), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILE_1007_Mnu_Obj->Append(ID_MNU_SAVEMAPAS____1012, wxT("Save Map As ..."), wxT(""), wxITEM_NORMAL);
	ID_MNU_FILE_1007_Mnu_Obj->AppendSeparator();
	*/

	ID_MNU_FILE_1007_Mnu_Obj->Append(ID_MNU_QUIT_1014, wxT("Quit"), wxT(""), wxITEM_NORMAL);
	WxMenuBar1->Append(ID_MNU_FILE_1007_Mnu_Obj, wxT("File"));
	SetMenuBar(WxMenuBar1);

	WxOpenFileDialog1 =  new wxFileDialog(this, wxT("Choose a file"), wxT(""), wxT(""), wxT("*.*"), wxOPEN);

	WxBoxSizer1 = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(WxBoxSizer1);
	this->SetAutoLayout(true);

	m_pnUpperScreen = new wxPanel(this, ID_M_PNUPPERSCREEN, wxPoint(5, 5), wxSize(512, 384));
	WxBoxSizer1->Add(m_pnUpperScreen,0,wxALIGN_CENTER | wxALL,5);

	m_pnLowerScreen = new wxPanel(this, ID_M_PNLOWERSCREEN, wxPoint(5, 399), wxSize(512, 384));
	WxBoxSizer1->Add(m_pnLowerScreen,0,wxALIGN_CENTER | wxALL,5);

	SetTitle(wxT("Game"));
	SetIcon(wxNullIcon);
	
	GetSizer()->Layout();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Center();
	
	////GUI Items Creation End
	InitEngine((void*)m_pnUpperScreen->GetHandle(),(void*)m_pnLowerScreen->GetHandle());
}

/*
http://www.gamedev.net/community/forums/topic.asp?topic_id=452969

Reply Quoting This MessageEdit Message gjaegy    Member since: 6/3/2004  From: Mulhouse, France
Posted - 6/23/2007 11:42:14 AM
you can use the idle event:

EVT_IDLE(gjWindowRender::OnIdle)

then:
void WindowRender::OnIdle(wxIdleEvent& event)
{
	UpdateAndRender();
	event.RequestMore();
}

RequestMore() means, once all windows messages have been processed by wxWidgets it will generate an idle event again, and so on.
*/

void C2DisplaysGUI::OnIdle(wxIdleEvent& event)
{
	// insert your code here
	LoopEngine();
	event.RequestMore();
}

void C2DisplaysGUI::OnClose(wxCloseEvent& event)
{
	FinishEngine();
	Destroy();
}
