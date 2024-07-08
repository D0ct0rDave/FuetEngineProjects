///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 26 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __CFONTEDITORGUIBASE_H__
#define __CFONTEDITORGUIBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/grid.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/clrpicker.h>
#include <wx/choice.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/slider.h>
#include <wx/checkbox.h>
#include <wx/aui/auibook.h>
#include <wx/menu.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define wxID_M_BORDERPIX 1000
#define wxID_M_FORCEDTRACKING 1001
#define wxID_M_POINTSCALE 1002
#define wxID_M_YOFFSET 1003
#define wxID_OPEN_FONT 1004
#define wxID_SAVE_FONT 1005
#define wxID_SAVE_FONT2 1006
#define wxID_SAVE_FONT_DEF 1007
#define wxID_SAVE_FONT_TEX 1008
#define wxID_QUIT 1009

///////////////////////////////////////////////////////////////////////////////
/// Class CFontEditorGUIBase
///////////////////////////////////////////////////////////////////////////////
class CFontEditorGUIBase : public wxFrame 
{
	private:
	
	protected:
		wxStatusBar* m_statusBar1;
		wxStaticText* m_staticText3;
		wxAuiNotebook* m_auinotebook1;
		wxPanel* m_panel1;
		wxStaticText* m_staticText1;
		wxStaticText* m_staticText13111;
		wxStaticText* m_staticText13;
		wxStaticText* m_staticText5;
		wxBitmapButton* m_bpButton1;
		wxStaticText* m_staticText12;
		wxStaticText* m_staticText6;
		wxBitmapButton* m_bpButton2;
		wxStaticText* m_staticText131;
		wxStaticText* m_staticText1311;
		wxStaticText* m_staticText61;
		wxButton* m_btLessStroke;
		wxStaticText* m_staticText121;
		wxStaticText* m_staticText51;
		wxButton* m_btMoreStroke;
		wxPanel* m_panel3;
		wxBitmapButton* m_btNewSC;
		wxBitmapButton* m_btEdiSC;
		wxBitmapButton* m_btDeleteSC;
		wxPanel* m_panel2;
		wxStaticText* m_staticText2;
		wxStaticText* m_staticText7;
		wxStaticText* m_staticText71;
		wxStaticText* m_staticText711;
		wxStaticText* m_staticText7111;
		wxMenuBar* m_menubar1;
		wxMenu* menu;
		
		// Virtual event handlers, overide them in your derived class
		virtual void m_gdFontListOnGridSelectCell( wxGridEvent& event ) { event.Skip(); }
		virtual void m_gdFontListOnKeyDown( wxKeyEvent& event ) { event.Skip(); }
		virtual void m_chFontStyleOnChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_bpButton1OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_lbFontSizeOnLeftDClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void m_bpButton2OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btLessStrokeOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_lbStrokeSizeOnLeftDClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void m_btMoreStrokeOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_gdSpecialCharsOnGridCmdCellLeftDClick( wxGridEvent& event ) { event.Skip(); }
		virtual void m_gdSpecialCharsOnGridSelectCell( wxGridEvent& event ) { event.Skip(); }
		virtual void m_btNewSCOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btEdiSCOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btDeleteSCOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void ch_ConstraintsOnChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_sldBorderPixelsOnCommandScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void m_sldForcedTrackingOnCommandScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void m_sldPointScaleOnCommandScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void m_sldYOffsetOnCommandScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void menuitem1OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuitem2OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuitem22OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuitem21OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuitem211OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuitemOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxGrid* m_gdFontList;
		wxTextCtrl* mem_Chars;
		wxColourPickerCtrl* m_oTypefaceColor;
		wxChoice* m_chFontStyle;
		wxStaticText* m_lbFontSize;
		wxChoice* m_chAntialiasType;
		wxColourPickerCtrl* m_oStrokeColor;
		wxStaticText* m_lbStrokeSize;
		wxGrid* m_gdSpecialChars;
		wxChoice* ch_Constraints;
		wxSlider* m_sldBorderPixels;
		wxSlider* m_sldForcedTracking;
		wxSlider* m_sldPointScale;
		wxSlider* m_sldYOffset;
		wxCheckBox* m_cbUTF8;
		
		CFontEditorGUIBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 847,984 ), long style = wxDEFAULT_FRAME_STYLE|wxMINIMIZE_BOX|wxSYSTEM_MENU|wxTAB_TRAVERSAL );
		
		~CFontEditorGUIBase();
	
};

#endif //__CFONTEDITORGUIBASE_H__
