//---------------------------------------------------------------------------
//
// Name:        Render.h
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class declaration
//
//---------------------------------------------------------------------------
#ifndef RenderH
#define RenderH
//---------------------------------------------------------------------------
#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/frame.h>
#else
	#include <wx/wxprec.h>
#endif

#ifdef __BORLANDC__
	#pragma hdrstop
#endif
#include <FuetEngine.h>
//-----------------------------------------------------------------------------
const FEReal FRAME_WIDTH = 250.0f;
//---------------------------------------------------------------------------
void RenderPreview(CFERenderer* _poRenderer);
void RenderMain(CFERenderer* _poRenderer);
void RenderFrameSeq(CFERenderer* _poRenderer);
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------

