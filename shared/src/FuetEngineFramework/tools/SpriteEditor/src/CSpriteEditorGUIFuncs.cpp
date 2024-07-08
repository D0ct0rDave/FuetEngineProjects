//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class implementation
//
//---------------------------------------------------------------------------
#include <windows.h>
#include <vector>

#include <wx/fontenum.h>
#include <wx/rawbmp.h>
#include <wx/filename.h>
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/xml/xml.h>
#include <wx/xml/xml.h>
#include "components/wxInputPanel.h"
#include "CSpriteEditorGUI.h"
#include "CSpriteEditorGUIFuncs.h"

#include <FreeImage.h>
#include <FreeImagePlus.h>
#include <FuetEngine.h>
#include "CLibConfigFileWriter.h"
#include "CAppGlobals.h"
#include "Render.h"
#include "CSpriteEditorGUIFileOps.h"
#include "edutils.h"
//---------------------------------------------------------------------------
const float RADIUS_THRESHOLD = 10;
//---------------------------------------------------------------------------
// Saves a given bitmap with a specified dimensions as a TGA file.
//---------------------------------------------------------------------------
void SaveTGAFile(wxBitmap* _poBmp,unsigned int _uiWidth,unsigned int _uiHeight,const wxString& _sFilename)
{
    wxImage oImg = _poBmp->ConvertToImage();
   
    // The bitmap to be saved oh disk    
    FIBITMAP* oOutBmp = FreeImage_Allocate(_uiWidth,_uiHeight,32);

    for (int j=0;j<_uiHeight;j++)
    {
        for (int i=0;i<_uiWidth;i++)
        {
            RGBQUAD oColor;
            oColor.rgbRed   = oImg.GetRed(i,j);
            oColor.rgbGreen = oImg.GetGreen(i,j);
            oColor.rgbBlue  = oImg.GetBlue(i,j);
            oColor.rgbReserved = 255; // oImg.GetAlpha(i,j);

            FreeImage_SetPixelColor(oOutBmp,i,(_uiHeight-j-1),&oColor);
        }
    }
    
    FreeImage_Save(FIF_TARGA, oOutBmp, _sFilename.ToAscii(),0);
}
//----------------------------------------------------------------------------
void UpdateInfoLabels(const CFEVect2& _oCursorPos)
{
    CFEString sStr;
    sStr.Format("Trans: %.2f,%.2f",globals.m_oTrans.x,globals.m_oTrans.y);
    globals.GUI->lb_Trans->SetLabel( sStr.szString() );

	sStr.Format("Zoom: %.2f",globals.m_fZoom);
    globals.GUI->lb_Zoom->SetLabel( sStr.szString() );

	sStr.Format("Coords: (%.1f,%.1f)",_oCursorPos.x,_oCursorPos.y);
    globals.GUI->lb_Frame->SetLabel(sStr.szString());	
}
//---------------------------------------------------------------------------
void DoZoom(float _fNewZoom)
{
	// mantener el punto señalado en la misma posición de la pantalla tras hacer el zoom
//    CFEVect2 oAimPos       = globals.oScreen2CanvasCoords(globals.m_oAimPos);
	CFEVect2 oAimPos = (globals.m_oAimPos/globals.m_fZoom) - globals.m_oTrans;

    globals.m_fZoom = CFEMath::rClamp(MIN_ZOOM,MAX_ZOOM,_fNewZoom);

	// CFEVect2 oNewAimPos = globals.oScreen2CanvasCoords(globals.m_oAimPos);
	CFEVect2 oNewAimPos = (globals.m_oAimPos/globals.m_fZoom) - globals.m_oTrans;
	
	CFEVect2 oDisplacement = oNewAimPos-oAimPos;

	globals.m_oTrans -= oDisplacement;
}
//---------------------------------------------------------------------------
void SetPivot(const CFEVect2& _oScrPos)
{
    if (globals.m_iSelectedAction < 0) return;

    CFESpriteAction* poSprAct = &globals.m_poSprite->m_oActions[globals.m_iSelectedAction];
    if (poSprAct->m_oSeq.size() <= 0) return;

    CFESpriteFrame* poFrame = &poSprAct->m_oSeq[ globals.m_iSelectedFrame ];
    if (poFrame->m_hMaterial == NULL) return;

	// if there is a snap factor
	CFEVect2 oScrPos = _oScrPos;
	if ((globals.m_uiGridSize >= 1) && (globals.m_eEditMode != EM_TRANSLATE))
	{
		oScrPos.x = iRound(oScrPos.x / globals.m_uiGridSize) * globals.m_uiGridSize;
		oScrPos.y = iRound(oScrPos.y / globals.m_uiGridSize) * globals.m_uiGridSize;
	}


    int w,h;
    CFEMaterialMgr::I()->bGetMaterialProperty(poFrame->m_hMaterial,"DiffuseMap.Width",(FEPointer)&w);
    CFEMaterialMgr::I()->bGetMaterialProperty(poFrame->m_hMaterial,"DiffuseMap.Height",(FEPointer)&h);

    // Render frame
    CFEVect2 oPos = oScrPos / CFEVect2(w,h);

    CFEVect2 oExtents = poFrame->m_oUV.m_oEnd - poFrame->m_oUV.m_oIni;
    poFrame->m_oPivot = (oPos - poFrame->m_oUV.m_oIni) / oExtents;
}
//---------------------------------------------------------------------------
CFEVect2 oGetCurrentPivotPos()
{
    if (globals.m_iSelectedAction < 0) return(CFEVect2::ZERO());

    CFESpriteAction* poSprAct = &globals.m_poSprite->m_oActions[globals.m_iSelectedAction];
    if (poSprAct->m_oSeq.size() <= 0)  return(CFEVect2::ZERO());

    CFESpriteFrame* poFrame = &poSprAct->m_oSeq[ globals.m_iSelectedFrame ];
    if (poFrame->m_hMaterial == NULL)  return(CFEVect2::ZERO());

    int w,h;
    CFEMaterialMgr::I()->bGetMaterialProperty(poFrame->m_hMaterial,"DiffuseMap.Width",(FEPointer)&w);
    CFEMaterialMgr::I()->bGetMaterialProperty(poFrame->m_hMaterial,"DiffuseMap.Height",(FEPointer)&h);

    // 
    CFEVect2 oTexDims = CFEVect2(w,h);
    CFEVect2 oExtents     = poFrame->m_oUV.m_oEnd - poFrame->m_oUV.m_oIni;

    CFEVect2 oPivot = ((poFrame->m_oPivot*oExtents) + poFrame->m_oUV.m_oIni) * oTexDims;
    return( oPivot );
}
//---------------------------------------------------------------------------
void SetRectCoord(bool _bIni,const CFEVect2& _oScrPos)
{
    if (globals.m_iSelectedAction < 0) return;

    CFESpriteAction* poSprAct = &globals.m_poSprite->m_oActions[globals.m_iSelectedAction];
    if (poSprAct->m_oSeq.size() <= 0) return;

    CFESpriteFrame* poFrame = &poSprAct->m_oSeq[ globals.m_iSelectedFrame ];
    if (poFrame->m_hMaterial == NULL) return;

    int w,h;
    CFEMaterialMgr::I()->bGetMaterialProperty(poFrame->m_hMaterial,"DiffuseMap.Width",(FEPointer)&w);
    CFEMaterialMgr::I()->bGetMaterialProperty(poFrame->m_hMaterial,"DiffuseMap.Height",(FEPointer)&h);

	// if there is a snap factor
	CFEVect2 oScrPos = _oScrPos;
	if ((globals.m_uiGridSize >= 1) && (globals.m_eEditMode != EM_TRANSLATE))
	{
		oScrPos.x = iRound(oScrPos.x / globals.m_uiGridSize) * (int)globals.m_uiGridSize;
		oScrPos.y = iRound(oScrPos.y / globals.m_uiGridSize) * (int)globals.m_uiGridSize;
	}

    // 
    CFEVect2 oTexDims = CFEVect2(w,h);
    CFEVect2 oPos = oScrPos / oTexDims;    

    if (_bIni)
        globals.m_oIniRectPos /= oTexDims;


    if (oPos.x > globals.m_oIniRectPos.x) 
    {
        poFrame->m_oUV.m_oIni.x = globals.m_oIniRectPos.x;
        poFrame->m_oUV.m_oEnd.x = oPos.x;
    }
    else
    {
        poFrame->m_oUV.m_oIni.x = oPos.x;
        poFrame->m_oUV.m_oEnd.x = globals.m_oIniRectPos.x;
    }

    if (oPos.y > globals.m_oIniRectPos.y) 
    {
        poFrame->m_oUV.m_oIni.y = globals.m_oIniRectPos.y;
        poFrame->m_oUV.m_oEnd.y = oPos.y;
    }
    else
    {
        poFrame->m_oUV.m_oIni.y = oPos.y;
        poFrame->m_oUV.m_oEnd.y = globals.m_oIniRectPos.y;
    }

    /*
    if (_bIni)
    {
        poFrame->m_oUV.m_oIni = oPos;
        poFrame->m_oUV.m_oEnd = oPos;
    }
    else
    {
        poFrame->m_oUV.m_oIni.x = CFEMath::rMin(oPos.x,poFrame->m_oUV.m_oIni.x);
        poFrame->m_oUV.m_oIni.y = CFEMath::rMin(oPos.y,poFrame->m_oUV.m_oIni.y);
        poFrame->m_oUV.m_oEnd.x = CFEMath::rMax(oPos.x,poFrame->m_oUV.m_oEnd.x);
        poFrame->m_oUV.m_oEnd.y = CFEMath::rMax(oPos.y,poFrame->m_oUV.m_oEnd.y);
    }
    */

    CFEVect2 oExtents     = poFrame->m_oUV.m_oEnd - poFrame->m_oUV.m_oIni;
    poFrame->m_oSize = oExtents * oTexDims;

    SetPivot(globals.m_oOldPivot);
}
//---------------------------------------------------------------------------
void DisplayPanelMouseDown(wxInputPanel& panel,wxMouseEvent& event)
{
	FEReal rX = event.GetX();
	FEReal rY = event.GetY();

	globals.m_oIniCursorCoord.x = rX;
	globals.m_oIniCursorCoord.y = rY;
    	
    CFEVect2 oDisplayPos = globals.oScreen2CanvasCoords(globals.m_oIniCursorCoord);

	if (event.ShiftDown())
	{
		// Save the position the pivot is before the rectangle begins being re-adjusted.
		globals.m_oOldPivot = oGetCurrentPivotPos();
        globals.m_oIniRectPos = oDisplayPos;
			
		// if there is some snap factor ...
		if (globals.m_uiGridSize >= 1)
		{
			globals.m_oIniRectPos.x = iRound(globals.m_oIniRectPos.x / globals.m_uiGridSize) * globals.m_uiGridSize;
			globals.m_oIniRectPos.y = iRound(globals.m_oIniRectPos.y / globals.m_uiGridSize) * globals.m_uiGridSize;
		}

		// Sprite Frame edition
    	globals.m_oCrossHair = oDisplayPos;
		SetRectCoord(true,oDisplayPos);
	    
		globals.m_eEditMode = EM_EDITRECT;
	}
else if (event.ControlDown())
	{
		globals.m_eEditMode = EM_SCALE;
		globals.m_fOldZoom  = globals.m_fZoom;
	}
else if (event.AltDown())
	{
		// Sprite Pivot
		globals.m_oCrossHair = oDisplayPos;
		SetPivot(oDisplayPos);

		globals.m_eEditMode = EM_EDITPIVOT;
	}
else
	{
		globals.m_eEditMode = EM_CROSSHAIR;
	}
}
//----------------------------------------------------------------------------
void DisplayPanelMouseUp(wxInputPanel& panel,wxMouseEvent& event)
{
    CFEVect2 oNewCursorPos(event.GetX(),event.GetY());
	CFEVect2 oDisplayPos = globals.oScreen2CanvasCoords(oNewCursorPos);
	CFEVect2 oDeltaPos = (oNewCursorPos - globals.m_oIniCursorCoord) / globals.m_fZoom;

    switch (globals.m_eEditMode)
    {
        case EM_EDITRECT:
        {
            if (event.ShiftDown())
            {
                // Update info
                SetRectCoord(false,oDisplayPos);
                SelectFrame(globals.m_iSelectedFrame);
            }
        }
        break;

        case EM_EDITPIVOT:
        {
            if (event.AltDown())
            {
                SetPivot(oDisplayPos);
                SelectFrame(globals.m_iSelectedFrame);
            }
        }
        break;
    }

    globals.m_eEditMode = EM_NONE;
}
//----------------------------------------------------------------------------
void DisplayPanelMouseMiddleDown(wxInputPanel& panel,wxMouseEvent& event)
{
	FEReal rX = event.GetX();
	FEReal rY = event.GetY();

	globals.m_oIniCursorCoord.x = rX;
	globals.m_oIniCursorCoord.y = rY;

	CFEVect2 oDisplayPos = globals.oScreen2CanvasCoords(globals.m_oIniCursorCoord);

	globals.m_eEditMode = EM_TRANSLATE;
	globals.m_oOldTrans = globals.m_oTrans;
}
//----------------------------------------------------------------------------
void DisplayPanelMouseMiddleUp(wxInputPanel& panel,wxMouseEvent& event)
{
	CFEVect2 oNewCursorPos(event.GetX(),event.GetY());
	CFEVect2 oDisplayPos = globals.oScreen2CanvasCoords(oNewCursorPos);
	CFEVect2 oDeltaPos = (oNewCursorPos - globals.m_oIniCursorCoord) / globals.m_fZoom;

	globals.m_eEditMode = EM_NONE;
}
//----------------------------------------------------------------------------
void DisplayPanelMouseMove(wxInputPanel& panel,wxMouseEvent& event)
{
    CFEVect2 oNewCursorPos(event.GetX(),event.GetY());
	CFEVect2 oDisplayPos = globals.oScreen2CanvasCoords(oNewCursorPos);
	CFEVect2 oDeltaPos = (oNewCursorPos - globals.m_oIniCursorCoord) / globals.m_fZoom;

	if (globals.m_eEditMode != EM_SCALE)
		globals.m_oAimPos = oNewCursorPos;

	/*
	if ((globals.m_uiGridSize > 1) && (globals.m_eEditMode != EM_TRANSLATE))
	{
		oDisplayPos.x = (uint)(oDisplayPos.x / globals.m_uiGridSize) * globals.m_uiGridSize;
		oDisplayPos.y = (uint)(oDisplayPos.y / globals.m_uiGridSize) * globals.m_uiGridSize;
	}
	*/

	switch (globals.m_eEditMode)
    {
        case EM_CROSSHAIR:
		{
			globals.m_oCrossHair = oDisplayPos;
		}
		break;
		
		case EM_EDITPIVOT:
		{
			globals.m_oCrossHair = oDisplayPos;

			if (event.AltDown())
				SetPivot(oDisplayPos);
			else
				globals.m_eEditMode = EM_NONE;			
		}
		break;

		case EM_TRANSLATE:
        {
            globals.m_oTrans = globals.m_oOldTrans - oDeltaPos;
        }
        break;

        case EM_SCALE:
        {
            if (event.ControlDown())
            {
				DoZoom( globals.m_fOldZoom - ((float)oDeltaPos.y / 500.0f) );
            }
        }
        break;

        case EM_EDITRECT:
        {
            if (event.ShiftDown())
            {                
                SetRectCoord(false,oDisplayPos);
            }
        }
        break;
        
        case EM_NONE:
        {
		    CFEString sStr;
		    sStr.Format("Coords: (%.1f,%.1f)",oDisplayPos.x,oDisplayPos.y);
		    globals.GUI->lb_Frame->SetLabel(sStr.szString());
        }
        break;
	}
	
	UpdateInfoLabels(oDisplayPos);
	event.Skip();
}
//----------------------------------------------------------------------------
void DisplayPanelOnMouseWheel(wxMouseEvent& event)
{
	CFEVect2 oCursorPos = CFEVect2(event.GetX(),event.GetY());
    CFEVect2 oNewCursorPos(event.GetX(),event.GetY());
	CFEVect2 oDeltaPos = (oNewCursorPos - globals.m_oIniCursorCoord);
	oDeltaPos /= globals.m_fZoom;
    CFEVect2 oDisplayPos = (oNewCursorPos/globals.m_fZoom) - globals.m_oTrans;

	int iRot = event.GetWheelRotation();
	FEReal fFact = (float)event.GetWheelRotation() / (float)event.GetWheelDelta();

	if (fFact > 0.0)
		DoZoom( globals.m_fZoom * (1.1f * fFact ) );
	else
		DoZoom( globals.m_fZoom / (1.1f * (-fFact) ) );

	UpdateInfoLabels(oDisplayPos);
	event.Skip();
}	
///-----------------------------------------------------------------
void FrameSeqPanelMouseDown(wxInputPanel& panel,wxMouseEvent& event)
{
    if (globals.m_iSelectedAction<0) return;
    CFESpriteAction* poSprAct = &globals.m_poSprite->m_oActions[globals.m_iSelectedAction];
    if (poSprAct->m_oSeq.size() <= 0)return;

	/*
	uint FS_VIS_FRAMES = globals.m_poFrameSeqRenderer->uiGetScreenVWidth() / FRAME_WIDTH;
    FEReal rFrameWidth = globals.m_poFrameSeqRenderer->uiGetScreenVWidth() / (FEReal)FS_VIS_FRAMES;
	*/

    CFEVect2 oCursorPos(event.m_x*2,event.m_y*2);
    int iFrame = globals.m_iFirstVisFrame + (oCursorPos.x / FRAME_WIDTH);

    if (iFrame < poSprAct->m_oSeq.size())
    {
        SelectFrame(iFrame);
    }
}
//---------------------------------------------------------------------------
void SetupControls()
{
    CFEString sStr;
    sStr.Format("Zoom: %.2f",globals.m_fZoom);
    globals.GUI->lb_Zoom->SetLabel( sStr.szString() );

    sStr.Format("Trans: %.2f,%.2f",globals.m_oTrans.x,globals.m_oTrans.y);
    globals.GUI->lb_Trans->SetLabel( sStr.szString() );

    // Timeline control
    if (globals.m_poSprite != NULL)
    {
        globals.GUI->m_gdSpriteActions->Enable();

        if (globals.GUI->m_gdSpriteActions->GetRows()>0)
			globals.GUI->m_gdSpriteActions->DeleteRows(0,globals.GUI->m_gdSpriteActions->GetRows());

	    globals.GUI->m_gdSpriteActions->AppendRows(globals.m_poSprite->m_oActions.size(),false);

        SelectAction(globals.m_iSelectedAction);
    }
}
//---------------------------------------------------------------------------
void SetupFrameControls()
{
	globals.m_bSkipEvents = true;

		if (globals.m_iSelectedFrame != -1)
		{
			uint FS_VIS_FRAMES = globals.m_poFrameSeqRenderer->uiGetScreenVWidth() / FRAME_WIDTH;

			globals.GUI->m_pnFrameProps->Enable(true);
			globals.GUI->sb_KeyFrame->Enable(true);
			globals.GUI->sb_KeyFrame->SetScrollbar(globals.m_iFirstVisFrame,FS_VIS_FRAMES,globals.m_poSprite->poGetAction(globals.m_iSelectedAction)->m_oSeq.size(),FS_VIS_FRAMES,true);
			globals.GUI->m_btDeleteFrame->Enable(true);
			globals.GUI->m_lbSnapFactor->Enable(true);
			globals.GUI->m_cbSnapFactor->Enable(true);
			globals.GUI->m_btAutoAdjustBottomCenter->Enable(true);
			globals.GUI->m_btAutoAdjustCenter->Enable(true);
			globals.GUI->m_btAdjustPivotCenterCenter->Enable(true);
			globals.GUI->m_btAdjustPivotBottomCenter->Enable(true);
			globals.GUI->m_btAdjustPivotVerticalBottom->Enable(true);
			globals.GUI->m_btAdjustPivotVerticalCenter->Enable(true);
			globals.GUI->m_cbSnapFactor->Enable(true);

			/// Setup info
			CFESpriteFrame* poFrame = &globals.m_poSprite->poGetAction(globals.m_iSelectedAction)->m_oSeq[globals.m_iSelectedFrame];
			CFEString sMatName = CFEMaterialMgr::I()->sGetResourceName( (FEHandler*)poFrame->m_hMaterial );

			if (poFrame->m_hMaterial != NULL)
				globals.GUI->m_edFrameMaterial->SetLabel( sMatName.szString() );
			else
				globals.GUI->m_edFrameMaterial->SetLabel( "" );


			// retrieve wrap modes
			int iSWM,iTWM;
			if (! CFEMaterialMgr::I()->bGetMaterialProperty(poFrame->m_hMaterial,"DiffuseMap.SWrapMode",(FEPointer)&iSWM)) iSWM = 0;
			if (! CFEMaterialMgr::I()->bGetMaterialProperty(poFrame->m_hMaterial,"DiffuseMap.TWrapMode",(FEPointer)&iTWM)) iTWM = 0;
			globals.GUI->m_cbSWrapMode->SetSelection(iSWM);
			globals.GUI->m_cbTWrapMode->SetSelection(iTWM);

			// retrieve filter
			int iFilter;
			if (! CFEMaterialMgr::I()->bGetMaterialProperty(poFrame->m_hMaterial,"DiffuseMap.Filter",(FEPointer)&iFilter)) iFilter = 1;
			globals.GUI->m_cbFilter->SetSelection(iFilter);

			// setup frame geometric properties
			int w,h;
			if (! CFEMaterialMgr::I()->bGetMaterialProperty(poFrame->m_hMaterial,"DiffuseMap.Width",(FEPointer)&w)) w = 0;
			if (! CFEMaterialMgr::I()->bGetMaterialProperty(poFrame->m_hMaterial,"DiffuseMap.Height",(FEPointer)&h)) h = 0;

			CFEVect2 oI = poFrame->m_oUV.m_oIni * CFEVect2(w,h);
			CFEVect2 oE = poFrame->m_oUV.m_oEnd * CFEVect2(w,h);
			CFEVect2 oPivot;
			oPivot.x = CFEMath::rLerp(oI.x,oE.x,poFrame->m_oPivot.x);
			oPivot.y = CFEMath::rLerp(oI.y,oE.y,poFrame->m_oPivot.y);

			globals.GUI->m_edPivotImageXCoord->SetValue(iRound(oPivot.x));
			globals.GUI->m_edPivotImageYCoord->SetValue(iRound(oPivot.y));

			CFEString sString;

			// ------------------------------------------
			//
			sString.Format("(%.2f,%.2f)-(%.2f,%.2f)",
						oI.x,
						oI.y,
						oE.x,
						oE.y);
			globals.GUI->m_lbImgRect->SetLabel(wxString(sString.szString()));

			//
			sString.Format("(%.2f,%.2f)",
							oE.x - oI.x,
							oE.y - oI.y);
			globals.GUI->m_lbImageRectSize->SetLabel( wxString(sString.szString()) );

			// 
			globals.GUI->m_cbUWorldCoords->SetValue(poFrame->m_bUWorldCoords);
			globals.GUI->m_cbVWorldCoords->SetValue(poFrame->m_bVWorldCoords);
			globals.GUI->m_cbScaleXUEqually->SetValue(poFrame->m_bScaleXUEqually);
			globals.GUI->m_cbScaleYVEqually->SetValue(poFrame->m_bScaleYVEqually);
		
			sString.Format("(%.2f,%.2f)-(%.2f,%.2f)",
							poFrame->m_oUV.m_oIni.x,
							poFrame->m_oUV.m_oIni.y,
							poFrame->m_oUV.m_oEnd.x,
							poFrame->m_oUV.m_oEnd.y);
		
			globals.GUI->m_lbUVRect->SetLabel( wxString(sString.szString()) );

			//
			sString.Format("(%.2f,%.2f)",poFrame->m_oSize.x,poFrame->m_oSize.y);
			globals.GUI->m_lbFrameRectSize->SetLabel(wxString(sString.szString()));
        
			// ------------------------------------------
			//
      
			//
			sString.Format("(%.2f,%.2f)",poFrame->m_oPivot.x,poFrame->m_oPivot.y);
			globals.GUI->m_lbPivotFrameCoords->SetLabel(wxString(sString.szString()));

			// Frame Time Sliders
			globals.GUI->m_sldFrameTime->SetValue( poFrame->m_rFrameTime * 1000.0f );
			globals.GUI->m_sldDelayBlendFact->SetValue( (poFrame->m_rBlendTime/poFrame->m_rFrameTime) * 100.0f );

			// Show frame time vs total anim time
			CFEString sStr;
			sStr.Format("%.3f / %.3f secs",poFrame->m_rFrameTime,globals.m_poSprite->poGetAction(globals.m_iSelectedAction)->m_rActionTime);
			globals.GUI->m_lbFrameTime->SetLabel( wxString(sStr.szString()) );
		}
		else
		{
			globals.GUI->m_pnFrameProps->Enable(false);
			globals.GUI->sb_KeyFrame->Enable(false);
			globals.GUI->m_btDeleteFrame->Enable(false);
			globals.GUI->m_lbSnapFactor->Enable(false);
			globals.GUI->m_cbSnapFactor->Enable(false);
			globals.GUI->m_btAutoAdjustBottomCenter->Enable(false);
			globals.GUI->m_btAutoAdjustCenter->Enable(false);
			globals.GUI->m_btAdjustPivotCenterCenter->Enable(false);
			globals.GUI->m_btAdjustPivotBottomCenter->Enable(false);
			globals.GUI->m_btAdjustPivotVerticalBottom->Enable(false);
			globals.GUI->m_btAdjustPivotVerticalCenter->Enable(false);
		
			globals.GUI->m_edPivotImageXCoord->SetValue(0);
			globals.GUI->m_edPivotImageYCoord->SetValue(0);

			globals.GUI->m_edFrameMaterial->SetLabel("");
			globals.GUI->m_lbImgRect->SetLabel("---");
			globals.GUI->m_lbImageRectSize->SetLabel("---");
			globals.GUI->m_lbUVRect->SetLabel("---");
			globals.GUI->m_lbFrameRectSize->SetLabel("---");
			globals.GUI->m_sldFrameTime->SetValue(0);        
			globals.GUI->m_sldDelayBlendFact->SetValue(0);
		}

	globals.m_bSkipEvents = false;
}
//---------------------------------------------------------------------------
void SetupActionControls()
{    
    if (globals.m_iSelectedAction == -1)
    {
		// globals.GUI->m_pnFrameControls->Enable(false); 
        globals.GUI->m_btDeleteAction->Enable(false);
        globals.GUI->m_btActionUp->Enable(false);
        globals.GUI->m_btActionDown->Enable(false);
        globals.GUI->bt_Play->Enable(false);
        globals.GUI->m_gdSpriteActions->Enable(false);
        globals.GUI->m_btReloadMaterials->Enable(false);
        globals.GUI->m_btNewFrame->Enable(false);        
    }    
    else
    {
        // globals.GUI->m_pnFrameControls->Enable(true); 
        globals.GUI->m_btDeleteAction->Enable(true);
        globals.GUI->m_btActionUp->Enable(true);
        globals.GUI->m_btActionDown->Enable(true);        
        globals.GUI->bt_Play->Enable(true);
        globals.GUI->m_gdSpriteActions->Enable(true);
        globals.GUI->m_btReloadMaterials->Enable(true);
        globals.GUI->m_btNewFrame->Enable(true);

        SetupFrameControls();
    }

    globals.GUI->m_gdSpriteActions->Refresh();
}
//---------------------------------------------------------------------------
void SelectAction(uint _uiActionIdx)
{
    globals.m_iSelectedAction = _uiActionIdx;

    if (globals.m_iSelectedAction != -1)
    {
        if (globals.m_hCurSprInst != NULL)
            CFESpriteInstMgr::I()->SetAction(globals.m_hCurSprInst,globals.m_iSelectedAction);
        
        globals.m_iFirstVisFrame = 0;

        if (globals.m_poSprite->poGetAction(globals.m_iSelectedAction)->m_oSeq.size()==0)
            globals.m_iSelectedFrame = -1;
        else
            globals.m_iSelectedFrame = 0;
    }
    else
    {
        globals.m_iSelectedFrame = -1;
    }    
    
    SelectFrame(globals.m_iSelectedFrame);
    SetupActionControls();        
}
//---------------------------------------------------------------------------
void SelectFrame(uint _uiSelectedFrame)
{
    globals.m_iSelectedFrame = _uiSelectedFrame;   
    SetupFrameControls();
}
//---------------------------------------------------------------------------
void RecalcFrameTimes(CFESpriteAction* _poAction)
{
    if (_poAction == NULL) return;
    FEReal rStartTime = 0.0f;
    for (uint i=0;i<_poAction->m_oSeq.size();i++)
    {
        _poAction->m_oSeq[i].m_rStartTime = rStartTime;
        rStartTime += _poAction->m_oSeq[i].m_rFrameTime;
    }

    _poAction->m_rActionTime = rStartTime;
}
//---------------------------------------------------------------------------
void SetFrameValues(CFESpriteFrame* _poFrame,FEReal _rFrameTime,FEReal _rDelayBlendFact)
{
    if (_poFrame == NULL) return;

    _poFrame->m_rFrameTime = _rFrameTime;
    _poFrame->m_rDelay     = _rFrameTime * (1.0f - _rDelayBlendFact);
    _poFrame->m_rBlendTime = _rFrameTime * _rDelayBlendFact;
}
///-----------------------------------------------------------------
void SetFrameGeometry(CFESpriteFrame* _poFrame,const CFEVect2& _oIUV,const CFEVect2& _oFUV,const CFEVect2& _oPivot)
{
	_poFrame->m_oPivot = _oPivot;
	_poFrame->m_oUV.m_oIni = _oIUV;
	_poFrame->m_oUV.m_oEnd = _oFUV;

	if (_poFrame->m_hMaterial == NULL) return;

	int w,h;
    CFEMaterialMgr::I()->bGetMaterialProperty(_poFrame->m_hMaterial,"DiffuseMap.Width",(FEPointer)&w);
    CFEMaterialMgr::I()->bGetMaterialProperty(_poFrame->m_hMaterial,"DiffuseMap.Height",(FEPointer)&h);

	_poFrame->m_oSize.x = (_oFUV.x - _oIUV.x)*w;
	_poFrame->m_oSize.y = (_oFUV.y - _oIUV.y)*h;
}
///-----------------------------------------------------------------
int iGetGridSize()
{
    if (globals.m_uiGridSelection < 0) return(0);

    int iGridSize[15] = {0,1,2,4,5,8,10,16,20,25,32,50,64,100,128};
    return ( iGridSize[globals.m_uiGridSelection] );
}
//---------------------------------------------------------------------------
void AdjustFramePivot(CFESpriteAction* _poSprAct,int _iFrame,const CFEVect2& _oPivot,FEBool _bAdjustX,FEBool _bAdjustY)
{
	CFEVect2 oPivot = _poSprAct->m_oSeq[_iFrame].m_oPivot;
	if (_bAdjustX) oPivot.x = _oPivot.x;
	if (_bAdjustY) oPivot.y = _oPivot.y;

	SetFrameGeometry(&_poSprAct->m_oSeq[_iFrame],
					_poSprAct->m_oSeq[_iFrame].m_oUV.m_oIni,
					_poSprAct->m_oSeq[_iFrame].m_oUV.m_oEnd,
					oPivot);
}
//---------------------------------------------------------------------------
void AdjustPivot(const CFEVect2& _oPivot,FEBool _bAdjustX,FEBool _bAdjustY,FEBool _bAffectAllFrames)
{
	if ( globals.m_iSelectedAction < 0 ) return;
	CFESpriteAction* poSprAct = &globals.m_poSprite->m_oActions[globals.m_iSelectedAction];
	if ( poSprAct->m_oSeq.size() <= 0 ) return;

	if (_bAffectAllFrames == true)
	{
		for ( uint i = 0; i < poSprAct->m_oSeq.size(); i++ )
			AdjustFramePivot(poSprAct,i,_oPivot,_bAdjustX,_bAdjustY);
	}
	else
	{
		AdjustFramePivot(poSprAct,globals.m_iSelectedFrame,_oPivot,_bAdjustX,_bAdjustY);
	}

	SelectFrame(globals.m_iSelectedFrame);
}
//---------------------------------------------------------------------------
