#include "CSpecialCharGUI.h"
#include <FuetEngine.h>
#include "CFontGenerator.h"
#include "CAppGlobals.h"

CSpecialCharGUI::CSpecialCharGUI( wxWindow* parent,CSpecialChar* _poSC )
:
CSpecialCharGUIBase( parent )
{
	// fill combo boxes
	m_poSC = _poSC;

	m_cbCharID->Clear();
	m_cbKCEq->Clear();

	for (int i=32;i<128;i++)
	{
		wxString sElem = wxString::Format(wxT("%c"),i);

		m_cbCharID->AppendString(sElem);
		m_cbKCEq->AppendString(sElem);

		if (m_poSC->m_cID == i)
			m_cbCharID->SetSelection(i-32);

		if (m_poSC->m_cKCEq == i)
			m_cbKCEq->SetSelection(i-32);
	}

    m_sbLeft->SetValue(m_poSC->m_uiLeft);
    m_sbRight->SetValue(m_poSC->m_uiRight);
    m_sbTop->SetValue(m_poSC->m_uiTop);
    m_sbBottom->SetValue(m_poSC->m_uiBottom);

    m_oCP->SetColour(m_poSC->m_uiColor);
}


void CSpecialCharGUI::CSpecialCharGUIBaseOnClose( wxCloseEvent& event )
{
// TODO: Implement CSpecialCharGUIBaseOnClose
}

		// Virtual event handlers, overide them in your derived class
void CSpecialCharGUI::m_button1OnButtonClick( wxCommandEvent& event )
{
	EndModal(wxID_CANCEL);
}

void CSpecialCharGUI::m_button2OnButtonClick( wxCommandEvent& event )
{
	m_poSC->m_cID       = m_cbCharID->GetSelection()+32;
	m_poSC->m_cKCEq     = m_cbKCEq->GetSelection()+32;

    m_poSC->m_uiLeft	= m_sbLeft->GetValue();
    m_poSC->m_uiRight	= m_sbRight->GetValue();
    m_poSC->m_uiTop		= m_sbTop->GetValue();
    m_poSC->m_uiBottom	= m_sbBottom->GetValue();

    m_poSC->m_uiColor	= WXCOLOR2UICOLOR( m_oCP->GetColour() );

	EndModal(wxID_OK);
}
