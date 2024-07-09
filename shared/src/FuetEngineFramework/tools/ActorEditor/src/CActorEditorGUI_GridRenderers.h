///-----------------------------------------------------------------
///
/// @file      FActorEditorGUI.cpp
/// @author    dmarquez
/// Created:   19/04/2010 8:46:27
/// @section   DESCRIPTION
///            CActorEditorGUI class implementation
///
///------------------------------------------------------------------
#include "wx/listbase.h"
#include "wx/grid.h"
#include "wx/bitmap.h"
// ---------------------------------------------------------------------------
class wxMyGrid : public wxGrid
{
	public:
		
		    wxMyGrid() ;

        wxMyGrid( wxWindow *parent,
            wxWindowID id,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxWANTS_CHARS,
            const wxString& name = wxGridNameStr ) : wxGrid(parent,id,pos,size,style,name)
            {
            }           

		virtual void DrawColLabel( wxDC& dc, int col );
};
// ---------------------------------------------------------------------------
class CActionTimelineGridRenderer : public wxGridCellStringRenderer
{
public:
    CActionTimelineGridRenderer();
    virtual void Draw(  wxGrid& grid,
                wxGridCellAttr& attr,
                wxDC& dc,
                const wxRect& rect,
                int row, int col,
                bool isSelected);

    wxBitmap m_o1KeyFrameBmp;
    wxBitmap m_oKeyFrameBmp;
};
// ---------------------------------------------------------------------------