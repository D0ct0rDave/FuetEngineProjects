//---------------------------------------------------------------------------
//
// Name:        wxInputPanel.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: 
//
//---------------------------------------------------------------------------

#include "wxInputPanel.h"


 
BEGIN_EVENT_TABLE(wxInputPanel, wxPanel)
    
    // some useful events
    EVT_LEFT_DOWN(wxInputPanel::mouseDown)
	EVT_MOTION(wxInputPanel::mouseMove)	
	EVT_LEFT_UP(wxInputPanel::mouseUp)

    EVT_KEY_DOWN(wxInputPanel::keyDown)
    EVT_KEY_UP(wxInputPanel::keyUp)

    /*
    EVT_RIGHT_DOWN(wxImagePanel::rightClick)
    EVT_LEAVE_WINDOW(wxImagePanel::mouseLeftWindow) MouseInputPanel
    EVT_MOUSEWHEEL(wxImagePanel::mouseWheelMoved)
    EVT_PAINT(wxImagePanel::paintEvent)
    */

END_EVENT_TABLE()
//---------------------------------------------------------------------------
void wxInputPanel::mouseDown(wxMouseEvent& event)
{
	m_bMouseDown = true;

	if (m_pMouseDownFunc)
		m_pMouseDownFunc(*this,event);
}
//---------------------------------------------------------------------------
void wxInputPanel::mouseMove(wxMouseEvent& event)
{
	if (m_pMouseMoveFunc)
		m_pMouseMoveFunc(*this,event);
}
//---------------------------------------------------------------------------
void wxInputPanel::mouseUp(wxMouseEvent& event)
{
	m_bMouseDown = false;
	
	if (m_pMouseUpFunc)
		m_pMouseUpFunc(*this,event);
}
//---------------------------------------------------------------------------
void wxInputPanel::keyDown(wxKeyEvent& event)
{
    if (m_pKeyDownFunc)
		m_pKeyDownFunc(*this,event);
}
//---------------------------------------------------------------------------
void wxInputPanel::keyUp(wxKeyEvent& event)
{
    if (m_pKeyUpFunc)
		m_pKeyUpFunc(*this,event);
}
//---------------------------------------------------------------------------
