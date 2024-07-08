/*
	    wxColor		m_oAppBack;
	    wxColor		m_oGridCellBase;
	    wxColor		m_oElemBack;
	    wxColor		m_oGenObjBack0;
	    wxColor		m_oGenObjBack1;
	    wxColor		m_oSelCellBack;
	    wxColor		m_oVisCellBack;
	    wxColor		m_oLockCellBack;
	    wxColor		m_oTLLineBack;
	    wxColor		m_oTLMarkerLine;
	    wxColor		m_oTLMarkerBack;
	    wxColor		m_oTLThroughLine;
	    wxColor		m_oTLTick;
	    wxColor		m_oTLNumber;
*/
///-----------------------------------------------------------------
///
/// @file      FSpriteEditorGUI.cpp
/// @author    dmarquez
/// Created:   19/04/2010 8:46:27
/// @section   DESCRIPTION
///            CSpriteEditorGUI class implementation
///
///------------------------------------------------------------------

#include "CSpriteEditorGUI_GridRenderers.h"
#include <FuetEngine.h>

#include <wx/wx.h>
#include <wx/frame.h>
#include "wx/listbase.h"
#include "wx/FileName.h"
#include <wx/listbox.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/checkbox.h>
#include <wx/clrpicker.h>
#include <wx/slider.h>
#include <wx/bmpbuttn.h>
#include <wx/grid.h>
#include <wx/statbmp.h>
#include <wx/combobox.h>
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/sizer.h>

#include "CAppGlobals.h"
#include "CSpriteEditorGUIFileOps.h"
////Header Include Start
////Header Include End

// ---------------------------------------------------------------------------

const uint DEF_CELL_HEIGHT = 28;
const uint DEF_CELL_WIDTH  = 28;
const uint DEF_ICON_WIDTH  = 20;
const uint DEF_ICON_HEIGHT = 20;
const uint CELL_XMARGIN    = (DEF_CELL_WIDTH - DEF_ICON_WIDTH) / 2;
const uint CELL_YMARGIN    = (DEF_CELL_HEIGHT - DEF_ICON_HEIGHT) / 2;

// ---------------------------------------------------------------------------
CFrameSequenceGridRenderer::CFrameSequenceGridRenderer()
{
}
///-----------------------------------------------------------------
void CFrameSequenceGridRenderer::Draw(wxGrid& grid,
                      wxGridCellAttr& attr,
                      wxDC& dc,
                      const wxRect& rect,
                      int row, int col,
                      bool isSelected)
{
    wxBrush oBrush;
    wxPen oPen;
    wxRect oCell = rect;

    if (! grid.IsEnabled())
    {
	    oPen.SetColour(globals.m_oColorScheme.m_oTLDisabled);
	    oBrush.SetColour(globals.m_oColorScheme.m_oTLDisabled);
	    dc.SetBrush(oBrush);
	    dc.SetPen(oPen);
	    dc.DrawRectangle(rect);    
	    return;
    }
}

// ---------------------------------------------------------------------------
CActionGridRenderer::CActionGridRenderer()
{
}
///-----------------------------------------------------------------
void CActionGridRenderer::Draw(wxGrid& grid,
                      wxGridCellAttr& attr,
                      wxDC& dc,
                      const wxRect& rect,
                      int row, int col,
                      bool isSelected)
{
    wxBrush oBrush;
    wxPen oPen;

    if ((globals.m_poSprite == NULL) || (row >= globals.m_poSprite->m_oActions.size()))
	{
        oPen.SetColour( globals.m_oColorScheme.m_oGridCellBase );
        oBrush.SetColour( globals.m_oColorScheme.m_oGridCellBase );
        dc.SetBrush(oBrush);
        dc.SetPen(oPen);
        dc.DrawRectangle(rect);
        return;
    }

    CFESpriteAction* poAction =  globals.m_poSprite->poGetAction(row);

    //
    switch (col)
    {
        // Wrap loop mode
        case 1:
        {
			if (globals.m_iSelectedAction == row)
			{
				oPen.SetColour( globals.m_oColorScheme.m_oSelCellBack );
				oBrush.SetColour( globals.m_oColorScheme.m_oSelCellBack );
			}
			else
			{
				oPen.SetColour( globals.m_oColorScheme.m_oVisCellBack );
				oBrush.SetColour( globals.m_oColorScheme.m_oVisCellBack );
			}

            dc.SetBrush(oBrush);
            dc.SetPen(oPen);
            dc.DrawRectangle(rect);

			switch (poAction->m_ePlayMode)
			{
			    case SFSPM_ONESHOT:
			    dc.DrawBitmap(globals.m_oIconScheme.m_oWrapFinalValue,CELL_XMARGIN + rect.x,CELL_YMARGIN + rect.y,true);
			    break;

                /*
                case SFSPM_PINGPONGSTOP:
                dc.DrawBitmap(globals.m_oIconScheme.m_oHideLayer,CELL_XMARGIN + rect.x,CELL_YMARGIN + rect.y,true);
                break;
                */

                case SFSPM_LOOP:
                dc.DrawBitmap(globals.m_oIconScheme.m_oWrapLoop,CELL_XMARGIN + rect.x,CELL_YMARGIN + rect.y,true);
                break;                
                
                case SFSPM_PINGPONG:
                dc.DrawBitmap(globals.m_oIconScheme.m_oWrapPingPong,CELL_XMARGIN + rect.x,CELL_YMARGIN + rect.y,true);
				break;
			}
        }
        break;

        
        default:
        {
            wxGridCellStringRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);

			wxColor oColor;
			int iOfs = 0;
			wxBitmap oBmp;
			
			// Manage selected.
			dc.SetTextForeground(globals.m_oColorScheme.m_oGridCellText);
			if (globals.m_iSelectedAction == row)
            {
			    oColor = globals.m_oColorScheme.m_oSelCellBack;
				dc.SetTextForeground(globals.m_oColorScheme.m_oSelCellText);
			}
			else			
			{
				if (row & 0x01)
					oColor = globals.m_oColorScheme.m_oGenObjBack1;
				else
					oColor = globals.m_oColorScheme.m_oGenObjBack0;			

				dc.SetTextForeground(globals.m_oColorScheme.m_oGridCellText);
			}

			oPen.SetColour( oColor );
			oBrush.SetColour( oColor );

            dc.SetBrush(oBrush);
            dc.SetPen(oPen);
            dc.DrawRectangle(rect);

            CFEString sStr;
            switch (col)
            {
				case 0:
				iOfs = -20;
                sStr = CFEString(row);
                break;

				case 2:
				{
					sStr = sGetBlendMode(poAction->m_eBlendMode);
				}
                break;

				case 3:				
                sStr.Format("%.2f",(float)poAction->m_rRandStartTime);
                break;

                case 4:                
                sStr = poAction->sGetName().szString();
                break;
            }

            dc.DrawText( sStr.szString(),rect.GetLeft()+ 2,rect.GetBottom()-16 );
        }
        break;
    }
}
///-----------------------------------------------------------------
