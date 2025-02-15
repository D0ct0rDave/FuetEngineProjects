//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class implementation
//
//---------------------------------------------------------------------------

#include "Render.h"
#include <vector>
#include <FreeImage.h>
#include <FreeImagePlus.h>
#include <cairo.h>
#include <cairo-win32.h>

#include "CAppGlobals.h"

//---------------------------------------------------------------------------
// Fucntion prototypes
void RenderMainDisplay(cairo_t* cr, unsigned int width, unsigned int height);
void RenderPreviewDisplay(cairo_t* cr, unsigned int width, unsigned int height);

//---------------------------------------------------------------------------
void MainDisplayPaintEvent(wxWindow* _poWnd)
{
    wxPaintDC dc(_poWnd);
    wxRect rect = _poWnd->GetClientRect();
    
    if(rect.width == 0 || rect.height == 0)
    {
        return;
    }

    // Get the handle of the wxWindow
    HWND hwnd = (HWND)_poWnd->GetHandle();

    // Now get the native windows DC for this window
    HDC hdc = ::GetDC(hwnd);

    // Create a double buffer for blitting to
    // the screen to prevent screen flicker. Pass
    // the double buffer to cairo and blit it
    // in the paint routine.
    HDC dcbuffer = CreateCompatibleDC(hdc);
    HBITMAP hbuffer = CreateCompatibleBitmap(hdc, rect.width, rect.height);
    SelectObject(dcbuffer, hbuffer); 

    // Create a cairo surface we can draw on directly
    cairo_surface_t* cairo_surface = cairo_win32_surface_create(dcbuffer);
    cairo_t* cairo_image = cairo_create(cairo_surface);

    // Render the output into the cairo image
    RenderMainDisplay(cairo_image, rect.width, rect.height);

    // Now blit the object to the screen
    BitBlt(hdc, 0, 0, rect.width, rect.height, dcbuffer, 0, 0, SRCCOPY);

    // Tear down the cairo object now that we don't need
    // it anymore.
    cairo_destroy(cairo_image);
    cairo_surface_destroy(cairo_surface);

    DeleteDC(dcbuffer);
    DeleteObject(hbuffer);

    // Because we called ::GetDC make sure we release the handle
    // back to the system or we'll have a memory leak.
    ::ReleaseDC(hwnd,hdc);
}
//---------------------------------------------------------------------------
void PreviewDisplayPaintEvent(wxWindow* _poWnd)
{
    wxPaintDC dc(_poWnd);
    wxRect rect = _poWnd->GetClientRect();
    
    if(rect.width == 0 || rect.height == 0)
    {
        return;
    }

    // Get the handle of the wxWindow
    HWND hwnd = (HWND)_poWnd->GetHandle();

    // Now get the native windows DC for this window
    HDC hdc = ::GetDC(hwnd);

    // Create a double buffer for blitting to
    // the screen to prevent screen flicker. Pass
    // the double buffer to cairo and blit it
    // in the paint routine.
    HDC dcbuffer = CreateCompatibleDC(hdc);
    HBITMAP hbuffer = CreateCompatibleBitmap(hdc, rect.width, rect.height);
    SelectObject(dcbuffer, hbuffer); 

    // Create a cairo surface we can draw on directly
    cairo_surface_t* cairo_surface = cairo_win32_surface_create(dcbuffer);
    cairo_t* cairo_image = cairo_create(cairo_surface);

    // Render the output into the cairo image
    RenderPreviewDisplay(cairo_image, rect.width, rect.height);

    // Now blit the object to the screen
    BitBlt(hdc, 0, 0, rect.width, rect.height, dcbuffer, 0, 0, SRCCOPY);

    // Tear down the cairo object now that we don't need
    // it anymore.
    cairo_destroy(cairo_image);
    cairo_surface_destroy(cairo_surface);

    DeleteDC(dcbuffer);
    DeleteObject(hbuffer);

    // Because we called ::GetDC make sure we release the handle
    // back to the system or we'll have a memory leak.
    ::ReleaseDC(hwnd,hdc);
}
//---------------------------------------------------------------------------
void RenderExtremity(cairo_t* cr,uint _uiBone0,uint _uiBone1,uint _uiBone2,float _fTime,bool _bDoubleRender)
{
    // draw extremities
    CFEVect2 oP[5];
    //--
    oP[0] = globals.m_oPuppet.m_oBone[_uiBone0].m_oPos.oGetValue(_fTime);
    oP[2] = globals.m_oPuppet.m_oBone[_uiBone1].m_oPos.oGetValue(_fTime);
    oP[4] = globals.m_oPuppet.m_oBone[_uiBone2].m_oPos.oGetValue(_fTime);
    oP[1] = (oP[0]+ oP[2])*0.5f;
    oP[3] = (oP[2]+ oP[4])*0.5f;

    if (_bDoubleRender)
    {
        cairo_set_line_width (cr, 25.0);
        cairo_set_source_rgba(cr,0, 0, 0, 1.0);
    }
    else
    {
        cairo_set_line_width (cr, 5.0);        
        cairo_set_source_rgba(cr,0, 1, 0, 0.5);
    }

    cairo_new_path(cr);
    cairo_move_to (cr, oP[0].x, oP[0].y);

    for (uint i=1;i<3;i++)
        cairo_curve_to(cr, oP[i].x, oP[i].y, oP[i+1].x, oP[i+1].y, oP[i+2].x, oP[i+2].y);

    cairo_stroke (cr);


    if (_bDoubleRender)
    {
        cairo_set_line_width (cr, 15.0);
        cairo_set_source_rgba(cr,1, 1, 1, 1.0);
        cairo_new_path(cr);
        cairo_move_to (cr, oP[0].x, oP[0].y);

        for (uint i=1;i<3;i++)
            cairo_curve_to(cr, oP[i].x, oP[i].y, oP[i+1].x, oP[i+1].y, oP[i+2].x, oP[i+2].y);

        cairo_stroke(cr);
    }
}
//---------------------------------------------------------------------------
// Render the output into the cairo image
void RenderMainDisplay(cairo_t* cr, unsigned int width, unsigned int height)
{
    float fTime = globals.m_fTime;

    double x=25.6,  y=128.0;
    double x1=102.4, y1=230.4,
           x2=153.6, y2=25.6,
           x3=230.4, y3=128.0;

    // Show current frame (if any ...)
    cairo_identity_matrix(cr);
    cairo_scale(cr,globals.m_fZoom,globals.m_fZoom);
    cairo_translate(cr, globals.m_oTrans.x, globals.m_oTrans.y);

    if (globals.m_oFrames.size())
    {
        cairo_surface_t *image = globals.m_oFrames[ globals.m_uiCurFrame ];
        
        int w = cairo_image_surface_get_width (image);
        int h = cairo_image_surface_get_height (image);
        uint uiXOfs = (width - w)/2;
        uint uiYOfs = (height - h)/2;

        cairo_translate(cr, uiXOfs, uiYOfs);
        // cairo_set_operator(cr,CAIRO_OPERATOR_SOURCE);
        cairo_set_source_surface (cr, image, 0, 0);
        cairo_paint (cr);

        cairo_translate(cr, -uiXOfs, -uiYOfs);
    }
    else
    {
        cairo_set_source_rgba(cr,0.5, 0.5, 0.5, 1.0);
        cairo_paint(cr);
    }

    // draw bones
    for (uint i=0;i<MAX_BONES;i++)
    {
        cairo_identity_matrix(cr);
        cairo_scale(cr,globals.m_fZoom,globals.m_fZoom);
        cairo_translate(cr, globals.m_oTrans.x, globals.m_oTrans.y);

        CFEVect2 oPos = globals.m_oPuppet.m_oBone[i].m_oPos.oGetValue(globals.m_fTime);
        
        if (i == globals.m_iSelBone)
        {
            // This is the current selected key frame
            cairo_set_source_rgba (cr, 1, 1, 0, 0.85);            
            cairo_move_to(cr, oPos.x, oPos.y);
            cairo_arc (cr, oPos.x, oPos.y, 12.0, 0, 2*M_PI);

            cairo_set_line_width (cr, 1.0);
            cairo_stroke (cr);
        }
        else
        {
            if (globals.m_oPuppet.m_oBone[i].m_oPos.poGetKeyFrameAtTime(globals.m_fTime))
            {
                // This is a key frame.
                cairo_set_source_rgba (cr, 1, 1, 1, 0.5);
                
                cairo_move_to(cr, oPos.x, oPos.y);
                cairo_arc (cr, oPos.x, oPos.y, 10.0, 0, 2*M_PI);
                cairo_fill (cr);

                cairo_set_line_width (cr, 2.0);
                cairo_stroke (cr);
            }
            else
            {
                cairo_set_source_rgba (cr, 1, 0, 0, 0.5);
                cairo_move_to(cr, oPos.x, oPos.y);
                cairo_arc (cr, oPos.x, oPos.y, 10.0, 0, 2*M_PI);
                cairo_fill (cr);

                cairo_set_line_width (cr, 2.0);
                cairo_stroke (cr);
            }
        }

        // Show bone labels
        if (globals.m_bShowLabels)
        {
            cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                                           CAIRO_FONT_WEIGHT_BOLD);

            cairo_set_source_rgba (cr, 1, 1, 1, 0.75);
            cairo_set_font_size (cr,10.0);
            cairo_move_to (cr, oPos.x+16, oPos.y+2);
            cairo_show_text (cr, BONE_NAMES[i]);
        }
    }
    
    // draw extremities
    RenderExtremity(cr, BONE_LHIP,BONE_LKNEE,BONE_LFOOT,fTime,false);
    RenderExtremity(cr, BONE_LSHOULDER,BONE_LELBOW,BONE_LHAND,fTime,false);
    
    RenderExtremity(cr, BONE_RHIP,BONE_RKNEE,BONE_RFOOT,fTime,false);
    RenderExtremity(cr, BONE_RSHOULDER,BONE_RELBOW,BONE_RHAND,fTime,false);
}
//---------------------------------------------------------------------------
// Render the output into the cairo image
void RenderPreviewDisplay(cairo_t* cr, unsigned int width, unsigned int height)
{
    float fTime;
    if (globals.m_bPlaying)
        fTime = globals.m_fCurAnimTime / globals.m_fAnimTime;
    else
        fTime = globals.m_fTime;

    // Show current frame (if any ...)
    cairo_identity_matrix(cr);
    cairo_scale(cr,(float)width / 640.0f,(float)height / 480.0f);

    cairo_set_source_rgba(cr,0.25,0.25,0.25, 1.0);
    cairo_paint(cr);

    // draw bones
    for (uint i=0;i<MAX_BONES;i++)
    {
        CFEVect2 oPos = globals.m_oPuppet.m_oBone[i].m_oPos.oGetValue(fTime);

        // This is the current selected key frame
        cairo_set_source_rgba (cr, 1, 0, 0, 1);            
        cairo_move_to(cr, oPos.x, oPos.y);
        cairo_arc (cr, oPos.x, oPos.y, 6.0, 0, 2*M_PI);

        cairo_set_line_width (cr, 1.0);
        cairo_stroke (cr);
    }
    
    
    // draw extremities
    RenderExtremity(cr, BONE_LHIP,BONE_LKNEE,BONE_LFOOT,fTime,true);
    RenderExtremity(cr, BONE_LSHOULDER,BONE_LELBOW,BONE_LHAND,fTime,true);
    
    RenderExtremity(cr, BONE_RHIP,BONE_RKNEE,BONE_RFOOT,fTime,true);
    RenderExtremity(cr, BONE_RSHOULDER,BONE_RELBOW,BONE_RHAND,fTime,true);
}
//---------------------------------------------------------------------------