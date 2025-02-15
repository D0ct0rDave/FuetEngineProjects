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

#include "wx/things/spinctld.h"	

#include "components/wxInputPanel.h"
#include "CActorEditorGUI.h"
#include "CActorEditorGUIFuncs.h"

#include <FreeImage.h>
#include <FreeImagePlus.h>
#include <FuetEngine.h>

#include "CLibConfigFileWriter.h"
#include "CAppGlobals.h"
#include "Render.h"
#include "CActorEditorGUIFileOps.h"
#include "edutils.h"

#include "CActorEditorGUI_TreeBuilder.h"
#include "CActorEditorGUI_GridRenderers.h"
//---------------------------------------------------------------------------
const float RADIUS_THRESHOLD = 12;
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
    
	if ((globals.m_iSelEditBone >-1) && (globals.m_iSelEditBone < globals.m_poSkeleton->m_oBone.size()))
	{
		CFEVect2 oOffset = globals.m_poSkeleton->m_oBone[globals.m_iSelEditBone].m_poBJointAction->m_oPosFunc.oGetValue(globals.m_fTime);
		sStr.Format("Offset: (%.2f,%.2f)",oOffset.x,oOffset.y);
	}
	else
	{
		sStr = "Offset: (-,-)";
	}

	globals.GUI->m_lbBoneOffset->SetLabel( wxString(sStr.szString()) );    
}
//---------------------------------------------------------------------------
void DoZoom(float _fNewZoom)
{
	// mantener el punto señalado en la misma posición de la pantalla tras hacer el zoom
    CFEVect2 oAimPos       = globals.oScreen2CanvasCoords(globals.m_oAimPos);

    globals.m_fZoom = CFEMath::rClamp(MIN_ZOOM,MAX_ZOOM,_fNewZoom);

	CFEVect2 oNewAimPos    = globals.oScreen2CanvasCoords(globals.m_oAimPos);
	CFEVect2 oDisplacement = oNewAimPos-oAimPos;

	globals.m_oTrans -= oDisplacement;
}
//----------------------------------------------------------------------------
void CheckNewBoneSelection(const CFEVect2& _oInputCoord)
{
	if (globals.m_poSkeleton->m_oBone.size() == 0)
	{
		return;
	}

	int j = (globals.m_iSelEditBone>-1)?((globals.m_iSelEditBone+1) % globals.m_poSkeleton->m_oBone.size()):-1;

    for (uint i=0;i<globals.m_poSkeleton->m_oBone.size();i++)
    {
		if (globals.m_poSkeleton->m_oBone[j].bVisible())
		{
			CFEVect2 oPos = globals.m_poSkeleton->oGetAbsBonePos(j,globals.m_fTime);

			/// podría haber una función distancia!
			CFEVect2 oD = oPos - _oInputCoord;
			if (oD.rLength() < (RADIUS_THRESHOLD/globals.m_fZoom))
			{
				globals.m_iSelBone = j;
				break;
			}
		}

		// next bone to check...
        j = (j+1) % globals.m_poSkeleton->m_oBone.size();
    }
}
//---------------------------------------------------------------------------
void DisplayPanelOnBoneEdit(wxMouseEvent& event,const CFEVect2& _oDisplayPos)
{
	if (globals.GUI->m_cbEditMode->GetSelection() == 1)
	{
		if (event.ShiftDown())
		{
			if (event.ControlDown())
			{
				// bone movement animation
				globals.m_poSkeleton->MoveBoneTo(globals.m_iSelEditBone,globals.m_fTime,_oDisplayPos,globals.m_oOldBonePos,globals.m_oOldParentPos,globals.m_fOldAngle,2);

				globals.GUI->m_edBoneXCoord->SetValue( _oDisplayPos.x );
				globals.GUI->m_edBoneYCoord->SetValue( _oDisplayPos.y );
			}
			else
			{
				// bone movement animation
				globals.m_poSkeleton->MoveBoneTo(globals.m_iSelEditBone,globals.m_fTime,_oDisplayPos,globals.m_oOldBonePos,globals.m_oOldParentPos,globals.m_fOldAngle,1);

				globals.GUI->m_edBoneXCoord->SetValue( _oDisplayPos.x );
				globals.GUI->m_edBoneYCoord->SetValue( _oDisplayPos.y );
			}
		}
	}
	else
	{
		if (event.ShiftDown() || event.ControlDown())
		{
			globals.m_poSkeleton->MoveBoneTo(globals.m_iSelEditBone,globals.m_fTime,_oDisplayPos,globals.m_oOldBonePos,globals.m_oOldParentPos,globals.m_fOldAngle);

			globals.GUI->m_edBoneXCoord->SetValue( _oDisplayPos.x );
			globals.GUI->m_edBoneYCoord->SetValue( _oDisplayPos.y );
		}
	}

	// Update instanced nodes
    CFESkelAnimInstMgr::I()->Update(globals.m_poSkeleton->m_hInst,_0r);	// WARNING: Always update with DeltaT of 0!!!
}
//---------------------------------------------------------------------------
void DisplayPanelOnMouseDown(wxMouseEvent& event)
{
	if (globals.m_eEditMode != EM_NONE) return;

	CFEVect2 oCursorPos = CFEVect2(event.GetX(),event.GetY());
	CFEVect2 oNewCursorPos(event.GetX()*2,event.GetY()*2);
    CFEVect2 oDisplayPos = (oNewCursorPos/globals.m_fZoom) + globals.m_oTrans;
    	
	// Select HUD element.
	// if ( event.ShiftDown() || event.ControlDown())
	{
		globals.m_oIniCursorCoord = oCursorPos;

		if (event.ShiftDown())
		{
			// Is currently the mouse over the currently selected bone?
			if ((globals.m_iSelBone != -1) && (globals.m_iSelEditBone == globals.m_iSelBone))
			{
				// yes, then try to select another possible bone (possibly the user wants to select a bone overlapping the currently selected)
				CheckNewBoneSelection(oDisplayPos);
				
				if ((globals.m_iSelBone != -1) && globals.m_iSelEditBone != globals.m_iSelBone)
					SetupBoneControls();
			}

			// are we over a bone?
		    if (globals.m_iSelBone != -1)
		    {
				globals.EditBegin();

			    globals.m_iSelEditBone = globals.m_iSelBone;
			    globals.m_oOldBonePos  = globals.m_poSkeleton->oGetAbsBonePos(globals.m_iSelEditBone,globals.m_fTime);
			    globals.m_fOldAngle	   = globals.m_poSkeleton->m_oBone[globals.m_iSelEditBone].m_poPJointAction->m_rAngleFunc.oGetValue(globals.m_fTime);

				if (globals.m_poSkeleton->m_oBone[globals.m_iSelEditBone].m_iParent != -1)
					globals.m_oOldParentPos = globals.m_poSkeleton->oGetAbsBonePos(globals.m_poSkeleton->m_oBone[globals.m_iSelEditBone].m_iParent,globals.m_fTime);
				else
					globals.m_oOldParentPos = globals.m_oOldBonePos;

			    SetupBoneControls();

			    globals.m_eEditMode = EM_EDITBONES;
			}
		}
    else if (event.AltDown())
		{
            globals.m_eEditMode = EM_CROSSHAIR;
            globals.m_oCrossHair = globals.oCanvas2ScreenCoords(oCursorPos);
		}
	else if (event.ControlDown())
		{
			globals.m_eEditMode = EM_SCALE;
			globals.m_fOldZoom  = globals.m_fZoom;
		}
		else
		{
			// are we over a bone?
			if (globals.m_iSelBone != -1)
			{
				// Is currently the mouse over the currently selected bone?
				if (globals.m_iSelEditBone == globals.m_iSelBone)
				{
					// yes, then try to select another possible bone (possibly the user wants to select a bone overlapping the currently selected)
					CheckNewBoneSelection(oDisplayPos);
				}

				globals.m_iSelEditBone = globals.m_iSelBone;
				SetupBoneControls();
			}
		}
	}

	event.Skip();
}
//----------------------------------------------------------------------------
void DisplayPanelOnMouseUp(wxMouseEvent& event)
{
	if ((globals.m_eEditMode != EM_EDITBONES) && (globals.m_eEditMode != EM_CROSSHAIR) && (globals.m_eEditMode != EM_SCALE))
		return;

	CFEVect2 oNewCursorPos(event.GetX(),event.GetY());
	CFEVect2 oDeltaPos = (oNewCursorPos - globals.m_oIniCursorCoord);
	oDeltaPos /= globals.m_fZoom;		
    CFEVect2 oDisplayPos = (oNewCursorPos/globals.m_fZoom) + globals.m_oTrans;

    switch (globals.m_eEditMode)
    {
        case EM_EDITBONES:
        {
			DisplayPanelOnBoneEdit(event,oDisplayPos);
			globals.EditEnd();
		}
        break;
    }

    globals.m_eEditMode = EM_NONE;
    event.Skip();
}
//----------------------------------------------------------------------------
void DisplayPanelOnMiddleMouseDown(wxMouseEvent& event)
{
	if (globals.m_eEditMode != EM_NONE) return;
	
	CFEVect2 oCursorPos = CFEVect2(event.GetX(),event.GetY());
	CFEVect2 oNewCursorPos(event.GetX()*2,event.GetY()*2);
    CFEVect2 oDisplayPos = (oNewCursorPos/globals.m_fZoom) + globals.m_oTrans;
    	
	globals.m_oIniCursorCoord = oCursorPos;
	
	globals.m_eEditMode = EM_TRANSLATE;
	globals.m_oOldTrans = globals.m_oTrans;

}

void DisplayPanelOnMiddleMouseUp(wxMouseEvent& event)
{
	if (globals.m_eEditMode != EM_TRANSLATE) return;
	
    CFEVect2 oNewCursorPos(event.GetX()*2,event.GetY()*2);
	CFEVect2 oDeltaPos = (oNewCursorPos - globals.m_oIniCursorCoord);
	oDeltaPos /= globals.m_fZoom;		
    CFEVect2 oDisplayPos = (oNewCursorPos/globals.m_fZoom) + globals.m_oTrans;

    globals.m_eEditMode = EM_NONE;
    event.Skip();
}
//----------------------------------------------------------------------------
void DisplayPanelOnMouseMove(wxMouseEvent& event)
{
	CFEVect2 oCursorPos		= CFEVect2(event.GetX(),event.GetY());
	CFEVect2 oDisplayPos	= globals.oScreen2CanvasCoords(oCursorPos);
	CFEVect2 oDeltaPos		= (oCursorPos - globals.m_oIniCursorCoord) / globals.m_fZoom;
	
	if (globals.m_eEditMode != EM_SCALE)
		globals.m_oAimPos = oCursorPos;

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
            globals.m_oTrans  = globals.m_oOldTrans - oDeltaPos;
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

        case EM_EDITBONES:
        {
			DisplayPanelOnBoneEdit(event,oDisplayPos);
        }
        break;

        case EM_NONE:
        {
            // see if there is a selected bone there
	        globals.m_iSelBone = -1;
	        CheckNewBoneSelection(oDisplayPos);
        }
        break;
	}

	UpdateInfoLabels(oDisplayPos);
	event.Skip();
}
//----------------------------------------------------------------------------
void DisplayPanelOnMouseWheel(wxMouseEvent& event)
{
	CFEVect2 oCursorPos		= CFEVect2(event.GetX(),event.GetY());
	CFEVect2 oDisplayPos	= globals.oScreen2CanvasCoords(oCursorPos);

	int iRot = event.GetWheelRotation();
	FEReal fFact = (float)event.GetWheelRotation() / (float)event.GetWheelDelta();

	if (fFact > 0.0)
		DoZoom( globals.m_fZoom * (1.1f * fFact ) );
	else
		DoZoom( globals.m_fZoom / (1.1f * (-fFact) ) );

	UpdateInfoLabels(oDisplayPos);
	event.Skip();
}	
//----------------------------------------------------------------------------	
void DisplayPanelOnKeyDown(wxKeyEvent& event)
{
	// Get focused control.
    if ((event.GetModifiers() == wxMOD_CONTROL) || (event.GetModifiers() == wxMOD_SHIFT))
    // Control pressed
	{
	    int iKeyCode = event.GetKeyCode();

		CFEVect2 oDisplacement(0,0);
		switch ( iKeyCode )
		{
			case WXK_UP:
			oDisplacement.y -= 1;
			break;
			
			case WXK_DOWN:
			oDisplacement.y += 1;
			break;        
	        
			case WXK_LEFT:
			oDisplacement.x -= 1;
			break;
			
			case WXK_RIGHT:
			oDisplacement.x += 1;
			break;

			default:
			{
				event.Skip();
				return;
			}
		}

		if ((oDisplacement.x != _0r) || (oDisplacement.y != _0r))
		{
			globals.EditBegin();

			globals.m_oOldBonePos  = globals.m_poSkeleton->oGetAbsBonePos(globals.m_iSelEditBone,globals.m_fTime);
			globals.m_fOldAngle	   = globals.m_poSkeleton->m_oBone[globals.m_iSelEditBone].m_poPJointAction->m_rAngleFunc.oGetValue(globals.m_fTime);

			CFEVect2 oDisplayPos = globals.m_poSkeleton->oGetAbsBonePos(globals.m_iSelEditBone,globals.m_fTime);

			oDisplayPos.x += oDisplacement.x;
			oDisplayPos.y += oDisplacement.y;

			wxMouseEvent oME;
			oME.m_controlDown = (event.GetModifiers() == wxMOD_CONTROL);
			oME.m_shiftDown   = (event.GetModifiers() == wxMOD_SHIFT);

			DisplayPanelOnBoneEdit(oME,oDisplayPos);

			globals.GUI->m_edBoneXCoord->SetValue( oDisplayPos.x );
			globals.GUI->m_edBoneYCoord->SetValue( oDisplayPos.y );
		}
	}
	else
	{
	    int iKeyCode = event.GetKeyCode();

		CFEVect2 oDisplacement(0,0);
		switch ( iKeyCode )
		{
			// Play
			case WXK_SPACE:
			{
				PlayStopAnimation();
				return;
			}
			break;

			case WXK_UP:
			globals.m_oAimPos = INITIAL_AIMPOS;
			globals.m_oTrans.y -= TRANS_STEP*(1.0f/globals.m_fZoom);
			break;

			case WXK_DOWN:
			globals.m_oAimPos = INITIAL_AIMPOS;
			globals.m_oTrans.y += TRANS_STEP*(1.0f/globals.m_fZoom);
			break;

			case WXK_LEFT:
			globals.m_oAimPos = INITIAL_AIMPOS;
			globals.m_oTrans.x -= TRANS_STEP*(1.0f/globals.m_fZoom);
			break;

			case WXK_RIGHT:
			globals.m_oAimPos = INITIAL_AIMPOS;
			globals.m_oTrans.x += TRANS_STEP*(1.0f/globals.m_fZoom);
			break;

			case WXK_PAGEUP:
			{
				DoZoom(globals.m_fZoom * 1.5);
			}
			break;

			case WXK_PAGEDOWN:
			{
				DoZoom(globals.m_fZoom / 1.5);
			}
			break;

			case WXK_HOME:
			globals.m_fZoom   = INITIAL_ZOOM;
			globals.m_oTrans  = INITIAL_TRANS;
			globals.m_oAimPos = INITIAL_AIMPOS;
			break;

			case WXK_END:
			break;

			default:
			{
				event.Skip();
				return;
			}
		}
		
		// punto en el "canvas"
		// CFEVect2 oScreenPos(event.GetX(),event.GetY());
		
	}
	
	event.Skip();
}
//----------------------------------------------------------------------------	
void DisplayPanelOnKeyUp(wxKeyEvent& event)
{
    if ((event.GetModifiers() == wxMOD_CONTROL) || (event.GetModifiers() == wxMOD_SHIFT))
	{
		globals.EditEnd();
	}

	event.Skip();
}
//----------------------------------------------------------------------------	
class CSkeletonSetupSprites : public CFESkelAnimNodeVisitor
{
	public:

		static void Setup(CFESkelAnimNode* _poNode)
		{
			CSkeletonSetupSprites oS;
			_poNode->Accept(&oS);
		}

		/// 
        virtual void Visit(CFESkelAnimGroup* _poObj)
        {
			for (uint i=0;i<_poObj->uiNumChildren();i++)
			{
				if (_poObj->poGetChild(i) != NULL)
					_poObj->poGetChild(i)->Accept(this);
			}
        }

        /// 
        virtual void Visit(CFESkelAnimSprite* _poObj)
        {
			_poObj->SetSprite( CFESpriteInstMgr::I()->hGetInstance("bone") );
        }

        /// 
        virtual void Visit(CFESkelAnimBone* _poObj)
        {
			if (_poObj->poGetChild(0) != NULL)
				_poObj->poGetChild(0)->Accept(this);
        }
};
//---------------------------------------------------------------------------
void SetupSkeleton(const wxString& _sFilename)
{
    globals.m_poSkeleton = new CSkeleton( CFEString(_sFilename.char_str()) );
    globals.m_poSkeleton->RescaleSkeleton(SCREEN_WIDTH,SCREEN_HEIGHT);
}
///-----------------------------------------------------------------
void FillActionList(CFESprite* _poSprite,wxChoice* _poChoice)
{
	_poChoice->Clear();

    if (_poSprite != NULL)
    {
		_poChoice->Enable();

        for (uint i=0;i<_poSprite->m_oActions.size();i++)
        {
			CFEString sStr = CFEString((int)i) + CFEString(" - ") + _poSprite->m_oActions[i].sGetName();
            _poChoice->Append( wxString(sStr.szString()) );
        }
    }
    else
    {
		_poChoice->Disable();
	}
	
}
///-----------------------------------------------------------------
void FillActionList(wxString _sSprite,wxChoice* _poChoice)
{
    FillActionList ( CFESpriteMgr::I()->poLoad( CFEString(_sSprite.char_str()) ), _poChoice );
}
///-----------------------------------------------------------------
void FillSpriteActionList(wxString _sSprite)
{
	FillActionList(_sSprite,globals.GUI->m_cbSpriteActionList);
}
///-----------------------------------------------------------------
void FillSpriteActionList(CFESprite* _poSprite)
{
	FillActionList(_poSprite,globals.GUI->m_cbSpriteActionList);
}
///-----------------------------------------------------------------
void FillMeshActionList(wxString _sSprite)
{
	// FillActionList(_sSprite,globals.GUI->m_cbMeshActionList);
}
//---------------------------------------------------------------------------
void SetupControls()
{
    uint uiThumbSize = globals.m_uiNumFrames/10;
    uint uiRange = globals.m_uiNumFrames + (uiThumbSize-1);
    globals.GUI->sb_KeyFrame->SetScrollbar(0,uiThumbSize,uiRange,10,true);

    FEReal rFact = powf((globals.m_fTotalAnimTime / MAX_ANIM_TIME),0.25f);
	globals.GUI->sld_AnimTime->SetValue(rFact*(FEReal)globals.GUI->sld_AnimTime->GetMax());
	globals.GUI->m_edAnimTime->SetValue(globals.m_fTotalAnimTime * 1000 );

	//
	CFEString sStr;
    sStr.Format("Zoom: %.2f",globals.m_fZoom);
    globals.GUI->lb_Zoom->SetLabel( sStr.szString() );

    sStr.Format("Trans: %.2f,%.2f",globals.m_oTrans.x,globals.m_oTrans.y);
    globals.GUI->lb_Trans->SetLabel( sStr.szString() );

    // Timeline control
    globals.GUI->m_gdTimeline->Enable();

    uint uiCurRows = globals.m_poSkeleton->m_oBone.size();
	globals.GUI->m_gdTimeline->DeleteRows(0,globals.GUI->m_gdTimeline->GetRows());
	globals.GUI->m_gdTimeline->AppendRows(uiCurRows,true);

    uint uiCurCols = globals.GUI->m_gdTimeline->GetCols();
    if (uiCurCols>1)
    {
	    globals.GUI->m_gdTimeline->DeleteCols(1,uiCurCols-1);
	}

    uiCurCols = globals.m_uiNumFrames;
	globals.GUI->m_gdTimeline->AppendCols(uiCurCols,true);

	for (uint i=0;i<uiCurCols;i++)
	    globals.GUI->m_gdTimeline->SetColSize(i+1, 20 );

	globals.GUI->m_gdTimeline->SetScrollPos(1,1,true);


    SetupBoneControls();
}
//---------------------------------------------------------------------------
// Assignment of a sprite to one of the nodes inside the current bone hierarchy.
//---------------------------------------------------------------------------
template <class T>
void AssignSpriteToNodeWithSprite(T* _poNode,FEHandler _hSpriteInst)
{
	_poNode->SetSprite(_hSpriteInst);

    CFESkelAnimNode* poRoot = CFESkelAnimInstMgr::I()->poGetInstancedNode(globals.m_poSkeleton->m_hInst);
	CFESkelAnimNode* poInstSprNode = CFESkelAnimNodeLocator::poLocateNode(poRoot,_poNode->sGetName());

    if (poInstSprNode != NULL)
	{
		((T*)poInstSprNode)->SetSprite(_hSpriteInst);
	}

	if (_hSpriteInst!= NULL)
	{
		CFESpriteInstMgr::I()->Enable(_hSpriteInst);
		((T*)_poNode)->SetAction(0);

		if (poInstSprNode != NULL)
			((T*)poInstSprNode)->SetAction(0);
	}
}

void AssignSpriteToBoneNode(CBoneSkelNode* _poNode,FEHandler _hSpriteInst)
{
	switch (_poNode->eGetType())
	{
		case SANT_BONE:
		case SANT_GROUP:
		break;

		case SANT_SPRITE:
		{
			AssignSpriteToNodeWithSprite( (CFESkelAnimSprite*)_poNode->m_poObj,_hSpriteInst);
			SetupBoneNodeControls(_poNode);
		}
		break;

		case SANT_MESH:
		{
			AssignSpriteToNodeWithSprite( (CFESkelAnimMesh*)_poNode->m_poObj,_hSpriteInst);
			SetupBoneNodeControls(_poNode);
		}
		break;
	}
}
//---------------------------------------------------------------------------
// Assignment of a sprite action number to one of the nodes inside the current bone hierarchy.
//---------------------------------------------------------------------------
template <class T>
void AssignSpriteActionToNodeWithSprite(T* _poNode,int _iAction)
{
	_poNode->SetAction(_iAction);

    CFESkelAnimNode* poRoot = CFESkelAnimInstMgr::I()->poGetInstancedNode(globals.m_poSkeleton->m_hInst);
	CFESkelAnimNode* poInstSprNode = CFESkelAnimNodeLocator::poLocateNode(poRoot,_poNode->sGetName());

	if (poInstSprNode != NULL)
		((T*)poInstSprNode)->SetAction(_iAction);
}

void AssignSpriteActionToBoneNode(CBoneSkelNode* _poNode,int _iAction)
{
	switch (_poNode->eGetType())
	{
		case SANT_BONE:
		case SANT_GROUP:
		break;

		case SANT_SPRITE:
		{
			AssignSpriteActionToNodeWithSprite( (CFESkelAnimSprite*)_poNode->m_poObj,_iAction);
			SetupBoneNodeControls(_poNode);
		}
		break;

		case SANT_MESH:
		{
			AssignSpriteActionToNodeWithSprite( (CFESkelAnimMesh*)_poNode->m_poObj,_iAction);
			SetupBoneNodeControls(_poNode);
		}
		break;
	}
}
//---------------------------------------------------------------------------
// Selection of a bone in the main screen or the timeline
//---------------------------------------------------------------------------
int iGetSelectedBoneInTimeline()
{
	wxArrayInt oSelRows = globals.GUI->m_gdTimeline->GetSelectedRows();

	if (oSelRows.size()>0)
	{
		if (oSelRows[0] >= globals.m_poSkeleton->m_oBone.size())
			return(globals.m_poSkeleton->m_oBone.size()-1);
		else
			return(oSelRows[0]);
	}
	else
	
		return(-1);
}

void SelectBoneInTimeline(int _iBone)
{
	globals.GUI->m_gdTimeline->SelectRow(globals.m_iSelEditBone);
	globals.GUI->m_gdTimeline->Refresh();
}

void RefreshBoneControls()
{
	// position in the spin control will be different depending on whether we are editing the skeleton or we are editing the animation pose.
	CFEVect2 oPos = globals.m_poSkeleton->oGetAbsBonePos(globals.m_iSelEditBone,globals.m_fTime,globals.GUI->m_cbEditMode->GetSelection());

	globals.GUI->m_edBoneXCoord->SetValue( oPos.x );
	globals.GUI->m_edBoneYCoord->SetValue( oPos.y );

	CFEVect2 oOffset = oPos - globals.m_poSkeleton->oGetAbsBonePos(globals.m_iSelEditBone,globals.m_fTime,0);
	CFEString sStr;	sStr.Format("Offset: (%.2f,%.2f)",oOffset.x,oOffset.y);
	globals.GUI->m_lbBoneOffset->SetLabel( wxString(sStr.szString()) );
}

void SetupBoneControls()
{
	if ((globals.m_iSelEditBone != -1) && (globals.m_poSkeleton->m_oBone.size() > 0))
	{
		CBone* poBone = &globals.m_poSkeleton->m_oBone[globals.m_iSelEditBone];
		
		globals.GUI->pnIconLayer->Enable(true);
		if (iGetSelectedBoneInTimeline() != globals.m_iSelEditBone)
			SelectBoneInTimeline(globals.m_iSelEditBone);
		
	
		// Rebuild node tree.
		CActorEditorGUI_TreeBuilder::Build(globals.GUI->m_poBoneNodes,poBone->m_poLeafChild);
		globals.GUI->m_poBoneNodes->ExpandAll(globals.GUI->m_poBoneNodes->GetRootItem());

		// Select the first child in the hierarchy
		wxTreeItemId root = globals.GUI->m_poBoneNodes->GetRootItem();
		if (root.IsOk())
		{
			wxTreeItemIdValue oCookie;
			wxTreeItemId oID = globals.GUI->m_poBoneNodes->GetFirstChild(root,oCookie);

			globals.m_bSettingBoneControls = true;
			globals.GUI->m_poBoneNodes->SelectItem( oID );
			globals.m_bSettingBoneControls = false;

			if ( oID.IsOk() )
			{
				CBoneSkelNode* poBoneNode = (CBoneSkelNode* )globals.GUI->m_poBoneNodes->GetItemData(oID);
				if (poBoneNode != NULL)
					SetupBoneNodeControls(poBoneNode);
			}
		}

		globals.GUI->sbSizerBoneProps->GetStaticBox()->SetLabel(wxString( poBone->sGetName().UpperCase().szString()) + wxString(" Bone Props:") );

		if (poBone->bEditable())
			globals.GUI->m_pnNodeProps->Enable();
		else
			globals.GUI->m_pnNodeProps->Disable();
			
		RefreshBoneControls();
	}
	else
    {
        globals.GUI->sbSizerBoneProps->GetStaticBox()->SetLabel("Bone Props:");
        globals.GUI->pnIconLayer->Enable(false);
    }
}
//---------------------------------------------------------------------------
// Setup of a bone node in the tree view of the bone hierarchy.
//---------------------------------------------------------------------------
template <class T>
inline void SetupNodeWithSprite(T* _poNode,const CFEString& _sExternalFile)
{
	FEHandler hInst = _poNode->hGetSprite();
	uint uiAction   = _poNode->uiGetAction();

	globals.GUI->m_pnSpriteProps->Enable(true);

	if (_sExternalFile != "")
	{
		globals.GUI->m_edIconSprite->SetValue( _sExternalFile.szString() );
		FillSpriteActionList( CFESpriteInstMgr::I()->poGetSprite(hInst) );
		
		globals.GUI->m_cbSpriteActionList->Select(uiAction);
	}
	else
	{
		globals.GUI->m_edIconSprite->SetValue( "" );
		FillSpriteActionList( NULL );
	}
}

void SetupBoneNodeControls(CBoneSkelNode* _poNode)
{
	// Common properties
	globals.GUI->m_edNodePosX->SetValue( _poNode->m_poObj->oGetPos().x );
	globals.GUI->m_edNodePosY->SetValue( _poNode->m_poObj->oGetPos().y );
	globals.GUI->m_sldNodeAngle->SetValue( 180*((float)_poNode->m_poObj->rGetAngle() /_PIr) );
	globals.GUI->m_edNodeDepth->SetValue( _poNode->m_poObj->rGetDepth() * 100.0f);

	#ifdef USE_SKELANIM_SCALE_COLOR
	globals.GUI->m_edNodeScaleX->SetValue( _poNode->m_poObj->oGetScale().x );
	globals.GUI->m_edNodeScaleY->SetValue( _poNode->m_poObj->oGetScale().y );
	globals.GUI->m_cpSelectColor->SetColour( FEColor2WXColor(_poNode->m_poObj->oGetColor()) );	
	globals.GUI->m_sldNodeAlpha->SetValue( _poNode->m_poObj->oGetColor().a * 255);
	#endif

	

	switch (_poNode->eGetType())
	{
		case SANT_BONE:
		globals.GUI->m_pnSpriteProps->Enable(false);
		break;

		case SANT_GROUP:
		globals.GUI->m_pnSpriteProps->Enable(false);
		break;

		case SANT_SPRITE:
		{
			SetupNodeWithSprite(((CFESkelAnimSprite*)_poNode->m_poObj),_poNode->sGetExternalFile());
		}
		break;

		case SANT_MESH:
		{
			SetupNodeWithSprite(((CFESkelAnimMesh*)_poNode->m_poObj),_poNode->sGetExternalFile());
		}
		break;
	}
}
//---------------------------------------------------------------------------
void UpdateNodeInstance(CFESkelAnimNode* _poSrcNode)
{
	CFESkelAnimNode* poRoot = CFESkelAnimInstMgr::I()->poGetInstancedNode(globals.m_poSkeleton->m_hInst);

	if (poRoot != NULL)
	{
		CFESkelAnimNode* poInstNode = CFESkelAnimNodeLocator::poLocateNode(poRoot,_poSrcNode->sGetName());

		if (poInstNode != NULL)
		{
			poInstNode->SetPos( _poSrcNode->oGetPos() );
			poInstNode->SetAngle( _poSrcNode->rGetAngle() );
			poInstNode->SetDepth( _poSrcNode->rGetDepth() );
			
			#ifdef USE_SKELANIM_SCALE_COLOR
			poInstNode->SetScale( _poSrcNode->oGetScale() );
			poInstNode->SetColor( _poSrcNode->oGetColor() );
			#endif

			poInstNode->Show( _poSrcNode->bIsVisible() );
		}
	}
}
//---------------------------------------------------------------------------
void PlayStopAnimation()
{
    if (globals.m_bPlaying == false)
	{
		globals.m_bPlaying = true;
		globals.GUI->bt_Play->SetLabel("Stop");
		globals.m_fCurAnimTime = 0.0f;
		globals.m_uiFrameBeforePlay = globals.m_uiCurFrame;

	}
	else
	{
		globals.m_bPlaying = false;
		globals.GUI->bt_Play->SetLabel("Play");
		globals.m_fCurAnimTime = 0.0f;
		globals.m_uiCurFrame = globals.m_uiFrameBeforePlay;
		globals.GUI->sb_KeyFrame->SetThumbPosition(globals.m_uiCurFrame);
		globals.GUI->m_gdTimeline->Refresh(false);
	}				
}
//---------------------------------------------------------------------------
