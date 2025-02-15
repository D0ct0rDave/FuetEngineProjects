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
#include "CKFRMActorEditorGUI.h"
#include "CKFRMActorEditorGUIFuncs.h"

#include <FreeImage.h>
#include <FreeImagePlus.h>
#include <FuetEngine.h>
#include "CLibConfigFileWriter.h"
#include "CAppGlobals.h"
#include "CKFRMActorEditorGUIFileOps.h"

///-----------------------------------------------------------------
wxString sRelativizePath(const wxString& _sFilename,const wxString& _sBasePath)
{	
	wxFileName sFilename(_sFilename);
	sFilename.MakeRelativeTo(_sBasePath);

	wxString sRelativePath = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
	
	// take the slash from the beginning of the filename in case.
	if ((sRelativePath.Find("\\") == 0) || (sRelativePath.Find("/") == 0))
		sRelativePath = sRelativePath.SubString(1,sRelativePath.Len());

	return( sRelativePath);
}
///-----------------------------------------------------------------
CFEString sRelativizePath(const CFEString& _sFilename,const CFEString& _sBasePath)
{	
	wxFileName sFilename( wxString(_sFilename.szString()) );
	sFilename.MakeRelativeTo( wxString(_sBasePath.szString()) );

	wxString sRelativePath = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
	
	// take the slash from the beginning of the filename in case.
	if ((sRelativePath.Find("\\") == 0) || (sRelativePath.Find("/") == 0))
		sRelativePath = sRelativePath.SubString(1,sRelativePath.Len());

	return( sRelativePath);
}
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
//---------------------------------------------------------------------------
void DisplayPanelMouseDown(wxInputPanel& panel,wxMouseEvent& event)
{
    /*
	FEReal rX = event.GetX();
	FEReal rY = event.GetY();

	// Select HUD element.
	// if ( event.ShiftDown() || event.ControlDown())
	{
		globals.m_oIniCursorCoord.x = rX;
		globals.m_oIniCursorCoord.y = rY;

		if (event.ShiftDown())
		{
		    if (globals.m_iSelBone != -1)
			    globals.m_eEditMode = EM_EDITBONES;
		}
    else if (event.AltDown())
		{
            globals.m_eEditMode = EM_CROSSHAIR;
            globals.m_oCrossHair = (globals.m_oIniCursorCoord/globals.m_fZoom) - globals.m_oTrans;
		}
	else if (event.ControlDown())
		{
			globals.m_eEditMode = EM_SCALE;
			globals.m_fOldZoom  = globals.m_fZoom;
		}
	else
		{
			globals.m_eEditMode = EM_TRANSLATE;
			globals.m_oOldTrans = globals.m_oTrans;

			if (globals.m_iSelBone != -1)
			{
                globals.m_iSelEditBone = globals.m_iSelBone;
                SetupBoneControls();
			}
		}
	}
	*/
}
//----------------------------------------------------------------------------
void DisplayPanelMouseUp(wxInputPanel& panel,wxMouseEvent& event)
{
    /*
    CFEVect2 oNewCursorPos(event.m_x,event.m_y);
	CFEVect2 oDeltaPos = (oNewCursorPos - globals.m_oIniCursorCoord);
	oDeltaPos /= globals.m_fZoom;
    CFEVect2 oDisplayPos = (oNewCursorPos/globals.m_fZoom) - globals.m_oTrans;

    switch (globals.m_eEditMode)
    {
        case EM_EDITBONES:
        {
            if (event.ShiftDown())
            {
	            // Insert this as a new (or not) keyframe.
	            // CFEVect2 oPos = globals.m_poSkeleton->m_oBone[ globals.m_iSelBone ].m_oPos.oGetValue(globals.m_fTime);
	            globals.m_poSkeleton->MoveBoneTo(globals.m_iSelBone,globals.m_fTime,oDisplayPos / CFEVect2(640.0f,480.0f) );
            }
        }
        break;
    }

    globals.m_eEditMode = EM_NONE;
    */
}

void DisplayPanelMouseMove(wxInputPanel& panel,wxMouseEvent& event)
{
    /*
    CFEVect2 oNewCursorPos(event.m_x,event.m_y);
	CFEVect2 oDeltaPos = (oNewCursorPos - globals.m_oIniCursorCoord);
	oDeltaPos /= globals.m_fZoom;
    CFEVect2 oDisplayPos = (oNewCursorPos/globals.m_fZoom) - globals.m_oTrans;

	switch (globals.m_eEditMode)
    {
        case EM_CROSSHAIR:
        {
            if (event.AltDown())
                globals.m_oCrossHair = oDisplayPos;
        }
        break;

        case EM_TRANSLATE:
        {
            globals.m_oTrans = globals.m_oOldTrans + oDeltaPos;
            
            CFEString sStr;
            sStr.Format("Trans: %.2f,%.2f",globals.m_oTrans.x,globals.m_oTrans.y);
            globals.GUI->lb_Trans->SetLabel( sStr.szString() );
        }
        break;

        case EM_SCALE:
        {
            if (event.ControlDown())
            {
                globals.m_fZoom = globals.m_fOldZoom - ((float)oDeltaPos.y / 500.0f);
                if (globals.m_fZoom < 0.1)  globals.m_fZoom = 0.1;
                if (globals.m_fZoom > 16.0)  globals.m_fZoom = 16.0;

	            CFEString sStr;
	            sStr.Format("Zoom: %.2f",globals.m_fZoom);
                globals.GUI->lb_Zoom->SetLabel( sStr.szString() );
            }
        }
        break;

        case EM_EDITBONES:
        {
            if (event.ShiftDown())
            {
	            // Insert this as a new (or not) keyframe.
	            // CFEVect2 oPos = globals.m_poSkeleton->m_oBone[ globals.m_iSelBone ].m_oPos.oGetValue(globals.m_fTime);
	            globals.m_poSkeleton->MoveBoneTo(globals.m_iSelBone,globals.m_fTime,oDisplayPos / CFEVect2(640.0f,480.0f) );
            }
        }
        break;
        
        case EM_NONE:
        {
            // see if there is a selected bone there
	        globals.m_iSelBone = -1;

	        for (uint i=0;i<MAX_BONES;i++)
	        {
	            CFEVect2 oPos = globals.m_poSkeleton->m_oBone [i].m_poNodeAction->m_oPosFunc.oGetValue(globals.m_fTime) * CFEVect2(640.0f,480.0f);

	            /// podría haber una función distancia!
	            CFEVect2 oD = oPos - oDisplayPos;
	            if (oD.rLength() < RADIUS_THRESHOLD)
	            {
	                globals.m_iSelBone = i;
	                return;
	            }
            }
		    
		    CFEString sStr;
		    sStr.Format("Coords: (%.1f,%.1f)",oDisplayPos.x,oDisplayPos.y);
		    globals.GUI->lb_Frame->SetLabel(sStr.szString());
        }
        break;
	}
	*/
}
//---------------------------------------------------------------------------
void SetupSkeleton()
{
    /*
    globals.m_poSkeleton     = new CSkeleton;
    globals.m_poSkeleton->RescaleSkeleton(640.0f,480.0f);

    for (uint i=0;i<MAX_BONES;i++)
    {
        if (globals.m_poSkeleton->m_oBone[i].m_eBoneType == BT_SPRITE)
        {
            FEHandler hSprInst = CFESpriteInstMgr::hGetInstance("bone");
            globals.m_poSkeleton->m_oBone[i].m_poSpr->SetSprite(hSprInst);
            // globals.m_poPrevSkeleton->m_oBone[i].m_poSpr->SetSprite(hSprInst);
        }
    }
    */
}
///-----------------------------------------------------------------
void FillSpriteActionList(wxString& _sSprite)
{
    /*
    CFESprite* poSprite = CFESpriteMgr::poLoad( CFEString(_sSprite.char_str()) );    
    if (poSprite != NULL)
    {
        globals.GUI->cbSpriteAction->Clear();
        for (uint i=0;i<poSprite->m_oActions.size();i++)
        {
            char szStr[8];
            sprintf(szStr,"%d",i);            
            globals.GUI->cbSpriteAction->Append( wxString(szStr) );
        }
    }
    */
}
//---------------------------------------------------------------------------
void SetupControls()
{
    /*
    globals.GUI->sld_HScale->SetValue( globals.m_fHScale * 100 );
    globals.GUI->sld_VScale->SetValue( globals.m_fVScale * 100 );
   
    // globals.GUI->sb_KeyFrame->SetScrollbar(0,globals.m_oFrames.size()/10,globals.m_oFrames.size(),globals.m_oFrames.size()/10,true);
    globals.GUI->sld_AnimTime->SetValue(globals.m_fTotalAnimTime*100);

    globals.GUI->cb_ShowLabels->SetValue(globals.m_bShowLabels);

    SetupBoneControls();
    */
}
//---------------------------------------------------------------------------
void SetupBoneControls()
{
    /*
    if ((globals.m_iSelEditBone != -1) && (globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_eBoneType != BT_CONTROL))
    {
        switch ( globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_eBoneType )
        {
            case BT_SPRITE_AND_BONE:
            case BT_SPRITE:
            {    
                globals.GUI->pnIconLayer->Enable(true);
                globals.GUI->sld_XOfs->Enable(true);
                globals.GUI->sld_YOfs->Enable(true);
                globals.GUI->cbSpriteAction->Enable(true);

                globals.GUI->sld_XOfs->SetValue( globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_oOfs.x );
                globals.GUI->sld_YOfs->SetValue( globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_oOfs.y );

                globals.GUI->edIconSprite->SetValue( globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_sExternalFile.szString() );
                FillSpriteActionList( wxString( globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_sExternalFile.szString()) );

                globals.GUI->cbSpriteAction->Select(0);
            }
            break;

            case BT_BONE:
            {
                globals.GUI->pnIconLayer->Enable(true);
                globals.GUI->sld_XOfs->Enable(false);
                globals.GUI->sld_YOfs->Enable(false);
                globals.GUI->cbSpriteAction->Enable(false);

                FEHandler hMaterial = globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_poMesh->hGetMaterial();
                CFEString sMaterial = CFEMaterialMgr::sGetResourceName( (FEHandler*)hMaterial );

                //
                globals.GUI->edIconSprite->SetValue( wxString(sMaterial.szString()) );
            }
            break;
        }
    }
    else
        globals.GUI->pnIconLayer->Enable(false);
        */
}
//---------------------------------------------------------------------------
