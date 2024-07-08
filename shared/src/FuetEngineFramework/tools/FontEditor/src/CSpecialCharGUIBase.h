///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __CSpecialCharGUIBase__
#define __CSpecialCharGUIBase__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/clrpicker.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define ID_CDLGSPECIALCHAR 1000

///////////////////////////////////////////////////////////////////////////////
/// Class CSpecialCharGUIBase
///////////////////////////////////////////////////////////////////////////////
class CSpecialCharGUIBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText12;
		wxStaticText* m_staticText141;
		
		wxStaticText* m_staticText1411;
		
		
		
		wxStaticText* m_staticText14112;
		
		wxStaticText* m_staticText14113;
		
		
		wxStaticText* m_staticText14111;
		
		
		wxStaticText* m_staticText121;
		wxStaticText* m_staticText16;
		wxButton* m_button1;
		wxButton* m_button2;
		
		// Virtual event handlers, overide them in your derived class
		virtual void CSpecialCharGUIBaseOnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void m_button1OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_button2OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxChoice* m_cbCharID;
		wxSpinCtrl* m_sbTop;
		wxSpinCtrl* m_sbLeft;
		wxSpinCtrl* m_sbRight;
		wxSpinCtrl* m_sbBottom;
		wxChoice* m_cbKCEq;
		wxColourPickerCtrl* m_oCP;
		
		CSpecialCharGUIBase( wxWindow* parent, wxWindowID id = ID_CDLGSPECIALCHAR, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 366,368 ), long style = wxDEFAULT_DIALOG_STYLE );
		~CSpecialCharGUIBase();
	
};

#endif //__CSpecialCharGUIBase__
