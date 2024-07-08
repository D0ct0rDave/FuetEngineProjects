///-----------------------------------------------------------------
///
/// @file      FSpriteEditorGUI.cpp
/// @author    dmarquez
/// Created:   19/04/2010 8:46:27
/// @section   DESCRIPTION
///            CSpriteEditorGUI class implementation
///
///------------------------------------------------------------------
#include "wx/listbase.h"
#include "wx/grid.h"
#include "wx/bitmap.h"
// ---------------------------------------------------------------------------
class CActionGridRenderer : public wxGridCellStringRenderer
{
public:
    CActionGridRenderer();
    virtual void Draw(  wxGrid& grid,
                wxGridCellAttr& attr,
                wxDC& dc,
                const wxRect& rect,
                int row, int col,
                
                bool isSelected);

    wxBitmap m_oEyeBmp[2];
    wxBitmap m_oLockBmp[2];
    wxBitmap m_oElemBmp[4];
};
// ---------------------------------------------------------------------------
class CFrameSequenceGridRenderer : public wxGridCellStringRenderer
{
public:
    CFrameSequenceGridRenderer();
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