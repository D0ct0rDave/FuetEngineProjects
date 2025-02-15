//---------------------------------------------------------------------------
//
// Name:        wxInputPanel.h
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: 
//
//---------------------------------------------------------------------------

#ifndef __wxInputPanel_h__
#define __wxInputPanel_h__
//----------------------------------------------------------------------------
#include <wx/panel.h>
//----------------------------------------------------------------------------

class wxInputPanel;
typedef void (*wxMouseCallback)(wxInputPanel& panel,wxMouseEvent& event);
typedef void (*wxKeyCallback)(wxInputPanel& panel,wxKeyEvent& event);

//----------------------------------------------------------------------------
class wxInputPanel : public wxPanel
{
       
    public:
        wxInputPanel(wxWindow* parent, int id,wxPoint pnt,wxSize size) : wxPanel(parent,id,pnt,size)
        {
			m_pMouseDownFunc = NULL;
			m_pMouseMoveFunc = NULL;
			m_pMouseUpFunc   = NULL;
			m_pKeyDownFunc   = NULL;
			m_pKeyUpFunc     = NULL;

			m_bMouseDown     = false;
        }

        void mouseDown(wxMouseEvent& event);
        void mouseMove(wxMouseEvent& event);
        void mouseUp(wxMouseEvent& event);
        void keyDown(wxKeyEvent& event);
        void keyUp(wxKeyEvent& event);
		
		// Until I don't know a better way to do it ....        
        void HookMouseDown(wxMouseCallback _pMouseDownFunc)
        {
			m_pMouseDownFunc = _pMouseDownFunc;
        }
		
        void HookMouseMove(wxMouseCallback _pMouseMoveFunc)
        {
			m_pMouseMoveFunc = _pMouseMoveFunc;
        }
		
        void HookMouseUp(wxMouseCallback _pMouseUpFunc)
        {
			m_pMouseUpFunc = _pMouseUpFunc;
        }
                
        void HookKeyDown(wxKeyCallback _pKeyDownFunc)
        {
			m_pKeyDownFunc = _pKeyDownFunc;
        }

        void HookKeyUp(wxKeyCallback _pKeyUpFunc)
        {
			m_pKeyUpFunc = _pKeyUpFunc;
        }
        
        //
        bool bIsMousePressed()
        {
			return(m_bMouseDown);
        }

        // some useful events
		/*
         void mouseMoved(wxMouseEvent& event);
         
         void mouseWheelMoved(wxMouseEvent& event);
         void mouseReleased(wxMouseEvent& event);
         void rightClick(wxMouseEvent& event);
         void mouseLeftWindow(wxMouseEvent& event);
         void keyPressed(wxKeyEvent& event);
         void keyReleased(wxKeyEvent& event);
         */

	protected:

		bool m_bMouseDown;

		wxMouseCallback     m_pMouseDownFunc;
		wxMouseCallback     m_pMouseMoveFunc;
		wxMouseCallback     m_pMouseUpFunc;
		wxKeyCallback       m_pKeyDownFunc;
		wxKeyCallback       m_pKeyUpFunc;

        DECLARE_EVENT_TABLE()
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
