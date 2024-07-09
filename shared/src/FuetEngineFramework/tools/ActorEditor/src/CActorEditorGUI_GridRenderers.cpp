///-----------------------------------------------------------------
///
/// @file      FActorEditorGUI.cpp
/// @author    dmarquez
/// Created:   19/04/2010 8:46:27
/// @section   DESCRIPTION
///            CActorEditorGUI class implementation
///
///------------------------------------------------------------------
#include "CActorEditorGUI_GridRenderers.h"
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
void wxMyGrid::DrawColLabel( wxDC& dc, int col )
{
    int l = GetColLeft( col );
    int r = GetColRight( col );

    wxBrush oBrush;
    wxPen oPen;
    wxRect oCell(l,0,r-l,16);
    wxRect rect = oCell;

    // label columns
    if (col==0)
    {
        oPen.SetColour(globals.m_oColorScheme.m_oTLGridLine);
        oBrush.SetColour(globals.m_oColorScheme.m_oTLLineBack);
        dc.SetBrush(oBrush);
        dc.SetPen(oPen);
        dc.DrawRectangle(oCell);
    }

    // Column != 0
    else
    {
		col--;

        // Draw seconds on tick marks
        bool bSelected = ((col>=globals.m_uiBlockStartFrame) && (col<=globals.m_uiBlockEndFrame)) && (globals.m_uiBlockSelState == 2);
		if (!bSelected)
		{
			oPen.SetColour( globals.m_oColorScheme.m_oTLTick );
			oBrush.SetColour( globals.m_oColorScheme.m_oTLTick );
		}
		else
		{
			oPen.SetColour( globals.m_oColorScheme.m_oSelBlockFrame);
			oBrush.SetColour( globals.m_oColorScheme.m_oSelBlockFrame);
		}

		dc.SetBrush(oBrush);
        dc.SetPen(oPen);
        
        // Draw tick marks
        if (((col) % 5) == 0)
        {
			dc.DrawLine(oCell.GetLeft() + oCell.width/2,oCell.GetBottom(),oCell.GetLeft() + oCell.width/2,oCell.GetBottom()-8);

			if (((col) % 10) == 0)
			{
				// draw time
				int iTime = col;
				if (! bSelected)
					dc.SetTextForeground(globals.m_oColorScheme.m_oTLTickNumber);
				else
					dc.SetTextForeground(globals.m_oColorScheme.m_oSelBlockFrame);

				wxFont oFont = dc.GetFont();
				oFont.SetWeight(wxFONTWEIGHT_LIGHT);
				dc.SetFont(oFont);

				CFEString oS(iTime);
				dc.DrawText( wxString( oS.szString()), oCell.GetLeft() + oCell.width/2+1,oCell.GetBottom()-16 );
            }
        }
        else
        {	
            // Draw tick marks
            dc.DrawLine(oCell.GetLeft() + oCell.width/2,oCell.GetBottom(),oCell.GetLeft() + oCell.width/2,oCell.GetBottom()-3);
        }

        // Draw marker
        if (col == globals.m_uiCurFrame )
        {
            uint h = oCell.height;

            oPen.SetColour(globals.m_oColorScheme.m_oTLMarkerLine);
            oBrush.SetColour(globals.m_oColorScheme.m_oTLMarkerBack);
            oCell.Deflate(3,1);

            dc.SetBrush(oBrush);
            dc.SetPen(oPen);
            dc.DrawRectangle(oCell);
        }

		// handle selection marks
		oPen.SetColour( globals.m_oColorScheme.m_oSelBlockFrame);
		dc.SetPen(oPen);

        if ((globals.m_uiBlockSelState >= 1) && (col == globals.m_uiBlockStartFrame))
		{		
			// draw start mark
			dc.DrawLine(rect.x,rect.y,rect.x,rect.y+rect.height);
		}
		
		if ((globals.m_uiBlockSelState == 2) && (col == globals.m_uiBlockEndFrame))
		{
			// draw end frame
			dc.DrawLine(rect.x+rect.width-1,rect.y,rect.x+rect.width-1,rect.y+rect.height);
		}        
	}
}
// ---------------------------------------------------------------------------
CActionTimelineGridRenderer::CActionTimelineGridRenderer()
{
    m_o1KeyFrameBmp= globals.m_oIconScheme.m_o1StKeyFrame;
    m_oKeyFrameBmp = globals.m_oIconScheme.m_oKeyFrame;
}
///-----------------------------------------------------------------
void CActionTimelineGridRenderer::Draw(wxGrid& grid,
                      wxGridCellAttr& attr,
                      wxDC& dc,
                      const wxRect& rect,
                      int row, int col,
                      bool isSelected)
{
    wxBrush oBrush;
    wxPen oPen;
    wxRect oCell = rect;

    if ( (!grid.IsEnabled()) || (row >= globals.m_poSkeleton->m_oBone.size()))
    {
	    oPen.SetColour(globals.m_oColorScheme.m_oTLDisabled);
	    oBrush.SetColour(globals.m_oColorScheme.m_oTLDisabled);
	    dc.SetBrush(oBrush);
	    dc.SetPen(oPen);
	    dc.DrawRectangle(rect);
	    return;
    }

    // label columns
    if (col==0)
    {
        oPen.SetColour(globals.m_oColorScheme.m_oTLGridLine);
        oBrush.SetColour(globals.m_oColorScheme.m_oTLLineBack);
        dc.SetBrush(oBrush);
        dc.SetPen(oPen);
        dc.DrawRectangle(oCell);

	    // draw wrap icon
	    EFEKFBFuncWrapMode eWrapMode = KFBFWM_NONE;
        switch (row)
        {
		    case 0:
	        break;
	        default:
	        eWrapMode = globals.m_poSkeleton->m_oBone[row-1].m_poPJointAction->m_rAngleFunc.eGetWrapMode();
	        break;
        }

        dc.SetTextForeground(globals.m_oColorScheme.m_oTLLabels);
        wxFont oFont = dc.GetFont();
        oFont.SetWeight(wxFONTWEIGHT_LIGHT);
        dc.SetFont(oFont);

        // draw bone name.
        dc.DrawText( wxString( globals.m_poSkeleton->m_oBone[row].m_poBone->sGetName().szString()), oCell.GetLeft()+ 2,oCell.GetBottom()-14 );

        wxBitmap oBmp;

        // draw wrap mode.
	    /*
	    switch (eWrapMode)
	    {
		    case KFBFWM_LOOP:
		    oBmp = globals.m_oIconScheme.m_oWrapLoop;
		    break;

		    case KFBFWM_PINGPONG:
		    oBmp = globals.m_oIconScheme.m_oWrapPingPong;
		    break;

		    case KFBFWM_FINALVALUE:
		    oBmp = globals.m_oIconScheme.m_oWrapFinalValue;
		    break;

		    case KFBFWM_INITIALVALUE:
		    oBmp = globals.m_oIconScheme.m_oWrapInitialValue;
		    break;

		    default:
		    oBmp = globals.m_oIconScheme.m_oWrapFinalValue;
	    }
	    dc.DrawBitmap(oBmp,oCell.GetRight() - 50, oCell.GetBottom()-12, true);
	    */

        // draw vibility icon.
        if (globals.m_poSkeleton->m_oBone[row].m_poPJointAction->bIsVisible())
			oBmp = globals.m_oIconScheme.m_oShowLayer;
        else
            oBmp = globals.m_oIconScheme.m_oHideLayer;

		dc.DrawBitmap(oBmp,oCell.GetRight() - 25, oCell.GetBottom()-16, true);
        return;
    }

    // Column != 0
    else
    {
		col--;

        if (row != globals.m_iSelEditBone)
        {
			// handle cells inside the block selection
			if (
				((col>=globals.m_uiBlockStartFrame) && (col<=globals.m_uiBlockEndFrame))
				&&
				(globals.m_uiBlockSelState == 2)
				)
			{
				if ((col & 0x01) == 0)
				{
					oPen.SetColour( globals.m_oColorScheme.m_oSelBlockBack0 );
					oBrush.SetColour( globals.m_oColorScheme.m_oSelBlockBack0 );
				}
				else
				{
					oPen.SetColour(globals.m_oColorScheme.m_oSelBlockBack1);
					oBrush.SetColour(globals.m_oColorScheme.m_oSelBlockBack1);
				}
			}
			else
			
			// cells not in selection
			{
				if ((col & 0x01) == 0)
				{
					oPen.SetColour( globals.m_oColorScheme.m_oGenObjBack0 );
					oBrush.SetColour( globals.m_oColorScheme.m_oGenObjBack0 );
				}
				else
				{
					oPen.SetColour(globals.m_oColorScheme.m_oGenObjBack1);
					oBrush.SetColour(globals.m_oColorScheme.m_oGenObjBack1);
				}
			}
        }
		else
        {
            oPen.SetColour(globals.m_oColorScheme.m_oTLSelRow );
            oBrush.SetColour(globals.m_oColorScheme.m_oTLSelRow );
        }
        
		dc.SetBrush(oBrush);
        dc.SetPen(oPen);
        dc.DrawRectangle(rect);
        
        // handle selection marks
		oPen.SetColour( globals.m_oColorScheme.m_oSelBlockFrame);
		dc.SetPen(oPen);

        if ((globals.m_uiBlockSelState >= 1) && (col == globals.m_uiBlockStartFrame))
		{		
			// draw start mark
			dc.DrawLine(rect.x,rect.y,rect.x,rect.y+rect.height);
		}
		
		if ((globals.m_uiBlockSelState == 2) && (col == globals.m_uiBlockEndFrame))
		{
			// draw end frame
			dc.DrawLine(rect.x+rect.width-1,rect.y,rect.x+rect.width-1,rect.y+rect.height);
		}
	}

    // ------------------------------------------------------------
    // Draw keyframe icons
    // ------------------------------------------------------------
    // PRE: row > 0 && col > 0
    // retrieve first if the cell has a key frame
    // ------------------------------------------------------------
    // FEReal rTime = (float)(col-1) / (float)(globals.m_uiNumFrames-1);
	int iKey = -1;

	if (globals.m_poSkeleton->m_oBone[row].m_poPJointAction != NULL)
		iKey = globals.iGetKeyFrame(globals.m_poSkeleton->m_oBone[row].m_poPJointAction->m_rAngleFunc,col);

	if (iKey == -1)
		iKey = globals.iGetKeyFrame(globals.m_poSkeleton->m_oBone[row].m_poBJointAction->m_oPosFunc,col);

    // Draw line
    oPen.SetColour( globals.m_oColorScheme.m_oTLThroughLine );
    dc.SetPen(oPen);

    dc.DrawLine(rect.x,rect.y+rect.height/2,rect.x+rect.width,rect.y+rect.height/2);

    // Draw keyframe marks
    if (iKey != -1)
    {
        uint uiXMargin = (rect.width-m_oKeyFrameBmp.GetWidth()) / 2;
        uint uiYMargin = (rect.height-m_oKeyFrameBmp.GetHeight()) / 2;

        if (col == 0)
            dc.DrawBitmap(m_o1KeyFrameBmp,rect.x + uiXMargin, rect.y + uiYMargin,true);
        else
            dc.DrawBitmap(m_oKeyFrameBmp, rect.x + uiXMargin, rect.y + uiYMargin,true);
    }
    else
    {

    }

    // ------------------------------------------------------------
    // Draw marker
    // ------------------------------------------------------------
    if ( (col == globals.m_uiCurFrame) && (grid.IsEnabled()) )
    {
        uint w = oCell.width/2;

        oPen.SetColour(globals.m_oColorScheme.m_oTLMarkerLine);
        oBrush.SetColour(globals.m_oColorScheme.m_oTLMarkerBack);

        dc.SetBrush(oBrush);
        dc.SetPen(oPen);
        dc.DrawLine(oCell.GetLeft()+w,oCell.GetTop()-1,oCell.GetLeft()+w,oCell.GetBottom()+1);
    }
}
///-----------------------------------------------------------------

