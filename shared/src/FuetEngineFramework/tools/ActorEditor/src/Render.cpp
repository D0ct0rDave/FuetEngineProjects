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
#include "CAppGlobals.h"
#include "ed_rend_utils.h"
#include "CActorEditorGUI.h"
#include "wx/things/spinctld.h"
//-----------------------------------------------------------------------------
CFERect oBuidBoundingRect()
{
	CFERect oRect;
	oRect.m_oIni.x = _INFr;
	oRect.m_oIni.y = _INFr;
	oRect.m_oEnd.x = -_INFr;
	oRect.m_oEnd.y = -_INFr;
	if (globals.m_poSkeleton != NULL)
	{
		for (uint i = 0; i<globals.m_poSkeleton->m_oBone.size(); i++)
		{
			CFEVect2 oPos  = globals.m_poSkeleton->oGetAbsBonePos(i, _0r, 0);

			oRect.m_oIni.x = CFEMath::rMin(oRect.m_oIni.x, oPos.x);
			oRect.m_oIni.y = CFEMath::rMin(oRect.m_oIni.y, oPos.y);
			oRect.m_oEnd.x = CFEMath::rMax(oRect.m_oEnd.x, oPos.x);
			oRect.m_oEnd.y = CFEMath::rMax(oRect.m_oEnd.y, oPos.y);
		}
	}

	return(oRect);
}
//---------------------------------------------------------------------------
void Update()
{
}
//---------------------------------------------------------------------------
CFEColor oGetBoneColor(uint _uiBoneID,bool _bKeyFrame)
{
    CFEColor oColor;

    if (_uiBoneID == globals.m_iSelEditBone)
    {
        oColor = globals.m_oSelEditBoneColor.oGetValue(globals.m_fGlobalTime);
    }

else if(_uiBoneID == globals.m_iSelBone)
    {
        oColor = globals.m_oSelBoneColor.oGetValue(globals.m_fGlobalTime);
    }
else
    {
        if (_bKeyFrame)
            oColor = globals.m_oBoneKFColor.oGetValue(globals.m_fGlobalTime);
        else
            oColor = globals.m_oBoneColor.oGetValue(globals.m_fGlobalTime);
    }
    return(oColor);
}
//---------------------------------------------------------------------------
void RenderTime(CFERenderer* _poRenderer,float _fTime,const CFEColor& _oColor)
{
	float fTimeFactor = _1r;
    if (globals.m_uiNumFrames>1)
		fTimeFactor = (globals.m_fTotalAnimTime/((globals.m_uiNumFrames-1)*SECS_PER_TICK));
	
	_poRenderer->SetDepth(_0r);

	_poRenderer->SetFont(globals.m_poDefaultFont);
    CFEString sTime;
    sTime.Format("%.3f",_fTime*fTimeFactor);
    _poRenderer->SetTextPointSize(1.0f);
    _poRenderer->RenderText(sTime,10,10,_oColor,THAM_LEFT,TVAM_TOP);
}
//---------------------------------------------------------------------------
void RenderBone(CFERenderer* _poRenderer,const CFEVect2 _oPos, uint _uiBoneID,bool _bKeyFrame,bool _bShowLabel)
{
	FEReal HSIZE = 5.0 / globals.m_fZoom;

    CFEColor oColor = oGetBoneColor(_uiBoneID,_bKeyFrame);
    _poRenderer->RenderRect(_oPos.x - HSIZE,_oPos.y - HSIZE,_oPos.x + HSIZE,_oPos.y + HSIZE, oColor );

    if (_bShowLabel)
    {
        _poRenderer->SetFont(globals.m_poDefaultFont);
		_poRenderer->SetTextPointSize(1.0f/globals.m_fZoom);
        _poRenderer->RenderText(globals.m_poSkeleton->m_oBone[_uiBoneID].m_poBone->sGetName(),_oPos.x+HSIZE,_oPos.y,oColor,THAM_LEFT,TVAM_CENTER);
    }
}
//---------------------------------------------------------------------------
void RenderLink(CFERenderer* _poRenderer,const CFEVect2& _oBonePos,const CFEVect2& _oChildBonePos,uint _uiBoneID,bool _bKeyFrame)
{
	FEReal HSIZE = 10.0 / globals.m_fZoom;

	CFEColor oColor = oGetBoneColor(_uiBoneID,_bKeyFrame);
    CFEVect2 oDir = _oChildBonePos - _oBonePos;
    oDir.Normalize();

    CFEVect2 oV1 = oDir;
    CFEVect2 oV2 = oDir;
    oV1.Rotate( _PIr + _PI8r);
    oV2.Rotate( _PIr - _PI8r);
    oV1 *= HSIZE;
    oV2 *= HSIZE;

	_poRenderer->RenderLine(_oChildBonePos.x,_oChildBonePos.y,_oBonePos.x,_oBonePos.y,oColor);
    _poRenderer->RenderLine(_oChildBonePos.x,_oChildBonePos.y,_oChildBonePos.x+oV1.x,_oChildBonePos.y+oV1.y,oColor);
    _poRenderer->RenderLine(_oChildBonePos.x,_oChildBonePos.y,_oChildBonePos.x+oV2.x,_oChildBonePos.y+oV2.y,oColor);
}
//---------------------------------------------------------------------------
void RenderSkeletonBones(CFERenderer* _poRenderer,float _fTime,bool _bShowLabels)
{
    // Display bones (including pivot)
    _poRenderer->SetDepth(-_1r);
    _poRenderer->SetBlendMode(BM_ALPHA);

    for (uint i=0;i<globals.m_poSkeleton->m_oBone.size();i++)
    {
	    // if ((globals.m_poSkeleton->m_oBone[i].m_poSpr != NULL) && (globals.m_poSkeleton->m_oBone[i].m_poSpr->bIsVisible()))
	    if (globals.m_poSkeleton->m_oBone[i].bVisible())
		{
			bool bKF = globals.m_poSkeleton->m_oBone[i].m_poPJointAction->m_rAngleFunc.poGetKeyFrameAtTime(_fTime) != NULL;
			// CFEVect2 oPos = globals.m_poSkeleton->m_oBone[i].m_poNodeAction->m_oPosFunc.oGetValue(_fTime);

			CFEVect2 oPos = globals.m_poSkeleton->oGetAbsBonePos(i,_fTime);

			// animation coordinates are normalized so we need to transform back again to screen coordinates.
			RenderBone(_poRenderer,oPos,i,bKF,_bShowLabels);

			if (globals.m_poSkeleton->m_oBone[i].m_iChild.size() > 0)
			{
				for (uint j=0;j<globals.m_poSkeleton->m_oBone[i].m_iChild.size();j++)
				{
					// CFEVect2 oCPos = globals.m_poSkeleton->m_oBone[ globals.m_poSkeleton->m_oBone[i].m_iChild[j] ].m_poNodeAction->m_oPosFunc.oGetValue(_fTime);
					CFEVect2 oCPos = globals.m_poSkeleton->oGetAbsBonePos(globals.m_poSkeleton->m_oBone[i].m_iChild[j],_fTime);					
					RenderLink(_poRenderer,oPos,oCPos,i,bKF);
				}
			}
		}
    }
}
//---------------------------------------------------------------------------
void RenderPreview(CFERenderer* _poRenderer)
{
	float fTime = 0.0f;
	globals.m_fGlobalTime = CFEMath::rMod( globals.m_fGlobalTime + globals.m_poPrevRenderer->rGetDeltaT(), _2r*COLOR_CYCLE_PERIOD);
	
    if (globals.m_bPlaying)
    {
        globals.m_fCurAnimTime += globals.m_poPrevRenderer->rGetDeltaT();

        if (globals.m_fCurAnimTime > globals.m_fTotalAnimTime)
        {
            if (! globals.GUI->cb_Loop->GetValue() == true)
            {
                globals.m_bPlaying = false;
            }else
            {
                globals.m_fCurAnimTime -= globals.m_fTotalAnimTime;
            }
        }
        else
        {
            
        }

        fTime = globals.m_fCurAnimTime * (((float)(globals.m_uiNumFrames-1)*SECS_PER_TICK) / globals.m_fTotalAnimTime);
    }
    else
    {
        fTime = globals.m_fTime;
    }


    // A dirty trick to reset the action time to 0, then to the desired time value.
    // CFESkelAnimInstMgr::I()->SetSpeedMult(globals.m_poSkeleton->m_hInst,1.0);
    CFESkelAnimInstMgr::I()->SetTime(globals.m_poSkeleton->m_hInst,fTime);

    // insert your code here
	_poRenderer->BeginScene();
        _poRenderer->ResetCamera();

        //
        _poRenderer->SetBlendMode(BM_ALPHA);
        _poRenderer->SetMaterial(NULL);

        // Render background color
        _poRenderer->SetDepth(_1r);
        _poRenderer->RenderQuad(0,0,_poRenderer->uiGetScreenVWidth(),_poRenderer->uiGetScreenVHeight(),globals.m_oColorScheme.m_oPrevFrameBackground);

        // Setup camera
		CFERect oR = oBuidBoundingRect();

		FEReal rZoom = _1r;
		FEReal rWRatio = (FEReal)_poRenderer->uiGetScreenVWidth() / oR.rWidth();
		FEReal rHRatio = (FEReal)_poRenderer->uiGetScreenVHeight() / oR.rHeight();

		rZoom = 0.7f*CFEMath::rMin(CFEMath::rMin(rZoom, rWRatio), rHRatio);
		CFEVect2 oOfs;
		oOfs.x = -((_05r*_poRenderer->uiGetScreenVWidth()) / rZoom);
		oOfs.y = -((_05r*_poRenderer->uiGetScreenVHeight()) / rZoom) - oR.rHeight()*_05r;

		_poRenderer->ZoomCamera(rZoom);
        _poRenderer->TranslateCamera(oOfs.x,oOfs.y);

		// Render grid?
        if (globals.GUI->cb_ShowGrid->GetValue())
        {
			RenderGrid(_poRenderer,50,globals.m_oColorScheme.m_oGrid,globals.m_oColorScheme.m_oGridHL);
			// void RenderGrid(CFERenderer* _poRenderer,const CFERect& _oRect,int _iGridSize,const CFEColor& _oGridColor,const CFEColor& _oGridHLColor)
		}

		_poRenderer->SetDepth(0.1);

        CFESkelAnimInstMgr::I()->SetDepth(globals.m_poSkeleton->m_hInst,_05r);

        CFEVect2 oPivotPos = CFEVect2::ZERO();

        if (globals.GUI->m_cbAnimatePivot->GetValue() == false)
        {
			oPivotPos = globals.m_poSkeleton->oGetAbsBonePos(globals.m_poSkeleton->m_uiPivotIdx,fTime);
        }
        else
        {
			oPivotPos = globals.m_poSkeleton->oGetAbsBonePos(globals.m_poSkeleton->m_uiPivotIdx,fTime,0);
        }
        
        if (globals.GUI->cb_ShapeMode->GetValue())
			CFESkelAnimInstMgr::I()->SetColor(globals.m_poSkeleton->m_hInst,CFEColor::BLACK());
		else
			CFESkelAnimInstMgr::I()->SetColor(globals.m_poSkeleton->m_hInst,CFEColor::WHITE() );

        CFESkelAnimInstMgr::I()->SetPos(globals.m_poSkeleton->m_hInst,CFEVect2(-1.0f*oPivotPos.x,-1.0f*oPivotPos.y));
        CFESkelAnimInstMgr::I()->Render(_poRenderer);

		if (globals.GUI->cb_ShapeMode->GetValue())
			CFESkelAnimInstMgr::I()->SetColor(globals.m_poSkeleton->m_hInst,CFEColor::WHITE());

		// Render bones over actor?
        if (globals.GUI->cb_ShowPrevBones->GetValue())
        {
			float fOldZoom = globals.m_fZoom;
			globals.m_fZoom = rZoom;

			_poRenderer->TranslateCamera(oOfs.x + oPivotPos.x,oOfs.y + oPivotPos.y);
			RenderSkeletonBones(_poRenderer,fTime,true);

			globals.m_fZoom = fOldZoom;
		}

    _poRenderer->EndScene();
}
//---------------------------------------------------------------------------
void RenderMain(CFERenderer* _poRenderer)
{
    Update();

    // insert your code here
	_poRenderer->BeginScene();

        _poRenderer->SetDepth(_1r);
        _poRenderer->SetBlendMode(BM_COPY);
        _poRenderer->SetMaterial(NULL);

        // Render background color
        _poRenderer->RenderQuad(0,
                               0,
                               SCREEN_WIDTH,
                               SCREEN_HEIGHT,
                               globals.m_oColorScheme.m_oFrameBackground);

        _poRenderer->ResetCamera();
        _poRenderer->ZoomCamera(globals.m_fZoom);
        _poRenderer->TranslateCamera(globals.m_oTrans.x,globals.m_oTrans.y);

        if ((globals.m_oFrames.size()>0) && globals.GUI->cb_ShowBackground->GetValue())
        {
			FEHandler hImg = NULL;
			CFEColor oColor = CFEColor::WHITE();

			if (globals.m_uiCurFrame<globals.m_oFrames.size())
				hImg = globals.m_oFrames[ globals.m_uiCurFrame ];
			else
			{
				// get last frame ...
				hImg = globals.m_oFrames[ globals.m_oFrames.size()-1 ];
				oColor.a = 0.5f;
			}

			int w,h;
			CFEMaterialMgr::I()->bGetMaterialProperty(hImg,"DiffuseMap.Width",(FEPointer)&w);
			CFEMaterialMgr::I()->bGetMaterialProperty(hImg,"DiffuseMap.Height",(FEPointer)&h);
			int iXOfs = 0;//globals.GUI->m_edBackXOffset->GetValue();
			int iYOfs = 0;//globals.GUI->m_edBackYOffset->GetValue();

			_poRenderer->PushTransform();
			_poRenderer->TranslateTransform(globals.GUI->m_edBackXOffset->GetValue(),globals.GUI->m_edBackYOffset->GetValue());
			_poRenderer->ScaleTransform(globals.GUI->m_edBackXScale->GetValue(),globals.GUI->m_edBackYScale->GetValue());

				_poRenderer->SetMaterial(hImg);
				_poRenderer->SetBlendMode(BM_ALPHA);
				_poRenderer->RenderQuad(iXOfs,iYOfs,iXOfs+w,iYOfs+h,oColor);

			_poRenderer->PopTransform();
		}

        // Show sprites ???
        if (globals.GUI->cb_ShowSprites->GetValue())
        {
			CFESkelAnimInstMgr::I()->SetDepth(globals.m_poSkeleton->m_hInst,_075r);
	        CFESkelAnimInstMgr::I()->SetPos(globals.m_poSkeleton->m_hInst,CFEVect2::ZERO());

			FEReal rAlpha = (FEReal)globals.GUI->m_sldSpritesAlpha->GetValue()/ 255.0f;
			
			CFESkelAnimInstMgr::I()->SetColor(globals.m_poSkeleton->m_hInst,CFEColor(_1r,_1r,_1r,rAlpha) );
		    CFESkelAnimInstMgr::I()->Render(_poRenderer);
        }

		// Render bones
        if (globals.GUI->cb_ShowBones->GetValue())
			RenderSkeletonBones(_poRenderer,globals.m_fTime,globals.GUI->cb_ShowLabels->GetValue());

        // Render cross hair
        _poRenderer->SetDepth(_1r);
        _poRenderer->ZoomCamera(globals.m_fZoom);
        _poRenderer->TranslateCamera(globals.m_oTrans.x,globals.m_oTrans.y);

        const FEReal INF = 1e6;
        _poRenderer->RenderLine(globals.m_oCrossHair.x,-INF,globals.m_oCrossHair.x,INF,globals.m_oColorScheme.m_oCrossHairColor);
        _poRenderer->RenderLine(-INF,globals.m_oCrossHair.y,INF,globals.m_oCrossHair.y,globals.m_oColorScheme.m_oCrossHairColor);

        // CFESpriteInstMgr::I()->Render(_poRenderer);
        _poRenderer->ResetCamera();

        // Render rectangle
        /*
_poRenderer->SetDepth(_1r);
        _poRenderer->RenderRect(0,0,SCREEN_VWIDTH,SCREEN_VHEIGHT,globals.m_oColorScheme.m_oFrameRect);
*/

		// Render the current frame time.
		if (globals.m_bPlaying)
		{
            /// Retrieves the current time of the animation being played by the given instance.
			FEReal rTime = globals.m_fCurAnimTime * (((float)(globals.m_uiNumFrames-1)*SECS_PER_TICK) / globals.m_fTotalAnimTime);
			RenderTime(_poRenderer,rTime,CFEColor(1,1,0,1));
		}
		else
			RenderTime(_poRenderer,globals.m_fTime,CFEColor(1,1,0,1));

    _poRenderer->EndScene();
}
//---------------------------------------------------------------------------
