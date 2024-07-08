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
//-----------------------------------------------------------------------------
CFERect oBuidBoundingRect(CFEVect2* _poVXs)
{
	CFERect oRect;
	oRect.m_oIni.x =  _INFr;
	oRect.m_oIni.y =  _INFr;
	oRect.m_oEnd.x = -_INFr;
	oRect.m_oEnd.y = -_INFr;

	for (uint i=0;i<4;i++)
	{
		oRect.m_oIni.x = CFEMath::rMin(oRect.m_oIni.x,_poVXs[i].x);
		oRect.m_oIni.y = CFEMath::rMin(oRect.m_oIni.y,_poVXs[i].y);
		oRect.m_oEnd.x = CFEMath::rMax(oRect.m_oEnd.x,_poVXs[i].x);
		oRect.m_oEnd.y = CFEMath::rMax(oRect.m_oEnd.y,_poVXs[i].y);
	}

	return(oRect);
}
//---------------------------------------------------------------------------
float fGetSpriteFrameTime(uint _uiFrameNum)
{
	CFESpriteAction* poSprAct = &globals.m_poSprite->m_oActions[globals.m_iSelectedAction];

	/*
	float fTime = 0;
	for (uint i=0;i<=_uiFrameNum;i++)
	{
		fTime += ;
	}
	*/

	return(poSprAct->m_oSeq[_uiFrameNum].m_rStartTime);
}
//---------------------------------------------------------------------------
void RenderFrameNumber(CFERenderer* _poRenderer,int _iPos,uint _uiFrameNum,const CFEColor& _oColor)
{
    uint uiX = (uint)((FEReal)(_iPos+1)*FRAME_WIDTH)-17;
    uint uiY = _poRenderer->uiGetScreenVHeight()-19;
	// uiX &= 0xfffffffc;
	uiX|=0x01;

	_poRenderer->SetFont(globals.m_poDefaultFont);
	_poRenderer->SetTextPointSize(_2r);
    _poRenderer->RenderText(CFEString((int)_uiFrameNum),(uiX    ),(uiY),CFEColor::BLACK(),THAM_RIGHT,TVAM_BOTTOM);
    _poRenderer->RenderText(CFEString((int)_uiFrameNum),(uiX - 1),(uiY),_oColor,THAM_RIGHT,TVAM_BOTTOM);
}
//---------------------------------------------------------------------------
void RenderTime(CFERenderer* _poRenderer,int _iPos,float _fTime,const CFEColor& _oColor,bool _bLeft)
{
    uint uiX;
    if (_bLeft)
		uiX = ((FEReal)(_iPos  )*FRAME_WIDTH) + 11;
	else
		uiX = ((FEReal)(_iPos+1)*FRAME_WIDTH) - 75;

    uint uiY = 10;
    uiX|=0x01;

    _poRenderer->SetFont(globals.m_poDefaultFont);
    CFEString sTime;
    sTime.Format("%.3f",_fTime);
	_poRenderer->SetTextPointSize(_2r);
    _poRenderer->RenderText(sTime,(uiX    ),(uiY),CFEColor::BLACK(),THAM_LEFT,TVAM_TOP);
    _poRenderer->RenderText(sTime,(uiX - 1),(uiY),_oColor,THAM_LEFT,TVAM_TOP);
}
//---------------------------------------------------------------------------
void RenderSpriteFrame(CFERenderer* _poRenderer,int _iPos,const CFEColor& _oColor)
{
    FEReal rIX = (FEReal)_iPos*FRAME_WIDTH;
    FEReal rIY = 0;
    FEReal rFX = ((FEReal)(_iPos+1)*FRAME_WIDTH)-1;
    FEReal rFY = _poRenderer->uiGetScreenVHeight();

	_poRenderer->SetMaterial(NULL);
    _poRenderer->RenderRect(rIX,rIY,rFX,rFY,_oColor);
}
//---------------------------------------------------------------------------
void RenderFrameSeq(CFERenderer* _poRenderer)
{
    // insert your code here
	_poRenderer->BeginScene();

        _poRenderer->ResetCamera();
        _poRenderer->SetDepth(1);
        _poRenderer->SetBlendMode(BM_COPY);
        _poRenderer->SetMaterial(NULL);

        // Render background color
        _poRenderer->RenderQuad(0,
                               0,
                               _poRenderer->uiGetScreenVWidth(),
                               _poRenderer->uiGetScreenVHeight(),
                               globals.m_oColorScheme.m_oFrameBackground);

        // Display bones (including pivot)
        _poRenderer->ResetCamera();
        _poRenderer->SetBlendMode(BM_ALPHA);

        if (globals.m_iSelectedAction>-1)
        {
			uint FS_VIS_FRAMES = _poRenderer->uiGetScreenVWidth() / FRAME_WIDTH;

			CFESpriteAction* poSprAct = &globals.m_poSprite->m_oActions[globals.m_iSelectedAction];
            uint uiMaxFrames = poSprAct->m_oSeq.size();
            if (uiMaxFrames > FS_VIS_FRAMES) uiMaxFrames = FS_VIS_FRAMES;

            for (uint i=0;i<uiMaxFrames;i++)
            {
				uint uiFrameNum = globals.m_iFirstVisFrame+i;
				if (uiFrameNum >= poSprAct->m_oSeq.size()) continue;

                CFESpriteFrame* poFrame = &poSprAct->m_oSeq[uiFrameNum];

                FEReal rX = ((FEReal)i + 0.5f)*FRAME_WIDTH;
                FEReal rY = (FEReal)_poRenderer->uiGetScreenVHeight()*0.5f;

                // Render sprite frame
                if (poFrame->m_hMaterial)
                {
					FEReal rScale  = _1r;
					FEReal rWRatio = (FEReal)FRAME_WIDTH  / poFrame->m_oSize.x;
					FEReal rHRatio = (FEReal)_poRenderer->uiGetScreenVHeight() / poFrame->m_oSize.y;
			
					rScale = CFEMath::rMin(CFEMath::rMin(rScale,rWRatio),rHRatio);
			
			
			
                    _poRenderer->SetMaterial( poFrame->m_hMaterial );
                    if ((poFrame->m_bUWorldCoords == false) && (poFrame->m_bVWorldCoords == false))
                    {
						_poRenderer->RenderSprite(CFEVect2(rX,rY),CFEVect2(_05r,_05r),poFrame->m_oSize*rScale,0,
												poFrame->m_oUV.m_oIni,
												poFrame->m_oUV.m_oEnd,
												CFEColor::WHITE(),
												1);
					}
					else
					{
						int w,h;
						CFEMaterialMgr::I()->bGetMaterialProperty(poFrame->m_hMaterial,"DiffuseMap.Width",(FEPointer)&w);
						CFEMaterialMgr::I()->bGetMaterialProperty(poFrame->m_hMaterial,"DiffuseMap.Height",(FEPointer)&h);

						CFEVect2 oInvTexDims(_1r/(FEReal)w,_1r/(FEReal)h);
						CFEVect2 oPivot = (CFEVect2(_05r,_05r) - poFrame->m_oPivot);

						CFEVect2 oIUV = CFEVect2(-_05r,-_05r);
						oIUV += oPivot;
						oIUV *= rScale;
						oIUV *= poFrame->m_oSize;
						// oIUV += _oPos;						// these are world coords (without transform)
						oIUV *= oInvTexDims;

						CFEVect2 oFUV = CFEVect2(_05r,_05r);
						oFUV += oPivot;
						oFUV *= rScale;
						oFUV *= poFrame->m_oSize;
						// oFUV += _oPos;						// these are world coords (without transform)
						oFUV *= oInvTexDims;
						
						if (poFrame->m_bUWorldCoords == false) 
						{
							oIUV.x = poFrame->m_oUV.m_oIni.x;
							oFUV.x = poFrame->m_oUV.m_oEnd.x;
						}

						if (poFrame->m_bVWorldCoords == false)
						{
							oIUV.y = poFrame->m_oUV.m_oIni.y;
							oFUV.y = poFrame->m_oUV.m_oEnd.y;
						}

						_poRenderer->RenderSprite(CFEVect2(rX,rY),CFEVect2(_05r,_05r),poFrame->m_oSize*rScale,0,
												oIUV,
												oFUV,
												CFEColor::WHITE(),
												1);
					}
                }

                // Render frame                
                RenderSpriteFrame(_poRenderer,i,globals.m_oColorScheme.m_oFrameSeqRect);

                // Show frame number
                RenderFrameNumber(_poRenderer,i,uiFrameNum,globals.m_oColorScheme.m_oFrameNumColor);
                
                // Show the time of the frame
                float fFrameTime = fGetSpriteFrameTime(uiFrameNum);
                RenderTime(_poRenderer,i,fFrameTime                      ,globals.m_oColorScheme.m_oFrameStartTimeColor,true);
                RenderTime(_poRenderer,i,fFrameTime+poFrame->m_rFrameTime,globals.m_oColorScheme.m_oFrameEndTimeColor,false);
            }

            if ((globals.m_iSelectedFrame != -1) && (globals.m_iSelectedFrame>=globals.m_iFirstVisFrame))
            {
                RenderSpriteFrame(_poRenderer,globals.m_iSelectedFrame - globals.m_iFirstVisFrame,globals.m_oColorScheme.m_oSelFrameSeqRect);
            }
        }

        // CFESpriteInstMgr::I()->Render(_poRenderer);
        _poRenderer->ResetCamera();

        // Render rectangle
        // _poRenderer->RenderRect(0,0,FS_SCR_WIDTH,FS_SCR_HEIGHT,globals.m_oColorScheme.m_oFrameRect);

    _poRenderer->EndScene();
}
//---------------------------------------------------------------------------
FEReal rGetZoomRatio(CFERenderer* _poRenderer,FEHandler _hSprInst,CFESpriteAction* _poSprAct)
{
	FEReal rMinWRatio = _INFr;
	FEReal rMinHRatio = _INFr;

	FEReal rCurrentSpriteTime = CFESpriteInstMgr::I()->rGetCurrentActionTime(_hSprInst);
	FEReal rFrameTime = _0r;
	for (uint i=0;i<_poSprAct->m_oSeq.size();i++)
	{
		// Set the sprite in the appropriate time moment.
		CFEVect2 oVX[4];
		CFESpriteInstMgr::I()->SetCurrentActionTime(_hSprInst,rFrameTime);
		CFESpriteInstMgr::I()->SetPos(_hSprInst,CFEVect2::ZERO());
		CFESpriteInstMgr::I()->GetGeometry(_hSprInst,oVX);

		CFERect oR = oBuidBoundingRect(oVX);

		FEReal rWRatio = (FEReal)_poRenderer->uiGetScreenVWidth()  / oR.rWidth();
		FEReal rHRatio = (FEReal)_poRenderer->uiGetScreenVHeight() / oR.rHeight();

		if (rWRatio < rMinWRatio) rMinWRatio = rWRatio;
		if (rHRatio < rMinHRatio) rMinHRatio = rHRatio;

		rFrameTime += (_poSprAct->m_oSeq[i].m_rBlendTime + _poSprAct->m_oSeq[i].m_rDelay);
	}

	FEReal rZoom   = _1r;
	rZoom = CFEMath::rMin(CFEMath::rMin(rZoom,rMinWRatio),rMinHRatio);
	
	// Restore sprite instance time
	CFESpriteInstMgr::I()->SetCurrentActionTime(_hSprInst,rCurrentSpriteTime);

	return(rZoom);
}
//---------------------------------------------------------------------------
void RenderPreview(CFERenderer* _poRenderer)
{   
	// insert your code here
	_poRenderer->BeginScene();

        _poRenderer->ResetCamera();
        _poRenderer->SetDepth(1);
        _poRenderer->SetBlendMode(BM_COPY);
        _poRenderer->SetMaterial(NULL);

        // Render background color
        _poRenderer->RenderQuad(0,
                               0,
                               _poRenderer->uiGetScreenVWidth(),
                               _poRenderer->uiGetScreenVHeight(),
                               globals.m_oColorScheme.m_oBackground);

        // _poRenderer->TranslateCamera(-_poRenderer->uiGetScreenVWidth()*0.5f,-_poRenderer->uiGetScreenVHeight()*0.5f);
        if ((globals.m_poSprite != NULL) && (globals.m_iSelectedAction>-1) && (globals.m_poSprite->poGetAction(globals.m_iSelectedAction)->m_oSeq.size()>0))
        {
			// Fix values internal to the engine.
			CFEVect2 oOldFUV;
			CFESpriteFrame* poFrame = NULL;

			CFESpriteAction* poSprAct = &globals.m_poSprite->m_oActions[globals.m_iSelectedAction];
			if (poSprAct->m_oSeq.size() > 0)
			{
				poFrame = &poSprAct->m_oSeq[ globals.m_iSelectedFrame ];

				// They only new to be fixed if world coords
				if ((poFrame != NULL) && (poFrame->m_hMaterial!=NULL) && ((poFrame->m_bUWorldCoords) || (poFrame->m_bVWorldCoords)) )
				{
					int w,h;
					oOldFUV = poFrame->m_oUV.m_oEnd;

					if (poFrame->m_bUWorldCoords)
					{
						if (CFEMaterialMgr::I()->bGetMaterialProperty(poFrame->m_hMaterial,"DiffuseMap.Width", (FEPointer)&w) == 0) w = 1;
						poFrame->m_oUV.m_oEnd.x = _1r/(FEReal)w;
					}

					if (poFrame->m_bVWorldCoords)
					{
						if (CFEMaterialMgr::I()->bGetMaterialProperty(poFrame->m_hMaterial,"DiffuseMap.Height",(FEPointer)&h) == 0) h = 1;
						poFrame->m_oUV.m_oEnd.y = _1r/(FEReal)h;
					}
				}
				else
				{
					poFrame = NULL;
				}
			}

            // Compute and set the camera in the proper zoom and position            
			FEReal rZoom   = rGetZoomRatio(_poRenderer,globals.m_hCurSprInst,poSprAct);
			_poRenderer->ZoomCamera(rZoom);

			// Now render the sprite
			CFEVect2 oPos = CFEVect2(_poRenderer->uiGetScreenVWidth()*0.5f/rZoom,_poRenderer->uiGetScreenVHeight()*0.5f/rZoom);
			CFESpriteInstMgr::I()->SetPos(globals.m_hCurSprInst,oPos);
            CFESpriteInstMgr::I()->Update(_poRenderer->rGetDeltaT());
            CFESpriteInstMgr::I()->Render(_poRenderer);

            // show the pivot in the view
            CFESprite* poSpr = CFESpriteInstMgr::I()->poGetSprite(globals.m_hCurSprInst);
            uint uiAction = CFESpriteInstMgr::I()->uiGetAction(globals.m_hCurSprInst);
            uint uiFrame = CFESpriteInstMgr::I()->uiGetCurrentFrame(globals.m_hCurSprInst);

			// Restore values.
			if (poFrame != NULL)
				poFrame->m_oUV.m_oEnd = oOldFUV;

			// Show pivot.
			RenderPivot(_poRenderer,oPos,globals.m_oColorScheme.m_oPivotColor);
        }

    _poRenderer->EndScene();
}
///-----------------------------------------------------------------
void SpriteEditorRenderGrid(CFERenderer* _poRenderer)
{
	/// security check
	globals.m_fZoom = CFEMath::rClamp(MIN_ZOOM,MAX_ZOOM,globals.m_fZoom);

	// Prevent grid clustering	
	FEReal rGridSize = globals.m_uiGridSize;
	while ( (rGridSize*globals.m_fZoom) <= _4r)
		rGridSize *= _2r;
	// 	rGridSize = globals.m_uiGridSize;

	// Get rendering window...
	CFERect oRect;
	oRect.m_oIni = globals.oScreen2CanvasCoords(CFEVect2::ZERO());
	oRect.m_oEnd = globals.oScreen2CanvasCoords(CFEVect2(SCREEN_WIDTH,SCREEN_HEIGHT));

    ::RenderGrid(_poRenderer,oRect,rGridSize,globals.m_oColorScheme.m_oGrid,globals.m_oColorScheme.m_oGridHL);
}
//---------------------------------------------------------------------------
void RenderMain(CFERenderer* _poRenderer)
{
    // insert your code here
	_poRenderer->BeginScene();
		
		_poRenderer->ResetCamera();
        _poRenderer->SetDepth(0);
        _poRenderer->SetBlendMode(BM_ALPHA);
        _poRenderer->SetMaterial(NULL);

        // Render background color
        _poRenderer->RenderQuad(0,
                               0,
                               _poRenderer->uiGetScreenVWidth(),
                               _poRenderer->uiGetScreenVHeight(),
                               globals.m_oColorScheme.m_oBackground);

        float fTime = globals.m_fTime;
        _poRenderer->ResetCamera();
        _poRenderer->ZoomCamera(globals.m_fZoom);
        _poRenderer->TranslateCamera(globals.m_oTrans.x,globals.m_oTrans.y);

        // Draw frame material
        if (globals.m_iSelectedAction>-1)
        {
            CFESpriteAction* poSprAct = &globals.m_poSprite->m_oActions[globals.m_iSelectedAction];
            if (poSprAct->m_oSeq.size() > 0)
            {
                CFESpriteFrame* poFrame = &poSprAct->m_oSeq[ globals.m_iSelectedFrame ];
                
                if (poFrame->m_hMaterial)
                {
                    int w,h;
                    CFEMaterialMgr::I()->bGetMaterialProperty(poFrame->m_hMaterial,"DiffuseMap.Width",(FEPointer)&w);
                    CFEMaterialMgr::I()->bGetMaterialProperty(poFrame->m_hMaterial,"DiffuseMap.Height",(FEPointer)&h);
                    
                    // Render sprite frame
                    _poRenderer->SetMaterial( poFrame->m_hMaterial );
                    _poRenderer->RenderQuad(0,0,w,h,
                                            CFEColor::WHITE());

                    // Render frame                    
                    CFEVect2 oI = poFrame->m_oUV.m_oIni * CFEVect2(w,h);
                    CFEVect2 oE = poFrame->m_oUV.m_oEnd * CFEVect2(w,h);
                    _poRenderer->SetMaterial(NULL);
                    _poRenderer->RenderRect(oI.x,oI.y,oE.x,oE.y,globals.m_oColorScheme.m_oSelFrameSeqRect);

                    // Render Pivot
                    CFEVect2 oPivot;
                    oPivot.x = CFEMath::rLerp(oI.x,oE.x,poFrame->m_oPivot.x);
                    oPivot.y = CFEMath::rLerp(oI.y,oE.y,poFrame->m_oPivot.y);
                    
                    RenderPivot(_poRenderer,oPivot,globals.m_oColorScheme.m_oPivotColor);
                }
            }
        }

        if (globals.m_bDisplayCrosshair)
        {
            /*
            _poRenderer->RenderLine(globals.m_oCrossHair.x  ,globals.m_oCrossHair.y-7,globals.m_oCrossHair.x  ,globals.m_oCrossHair.y+7,globals.m_oColorScheme.m_oCrossHairColor);
            _poRenderer->RenderLine(globals.m_oCrossHair.x-7,globals.m_oCrossHair.y  ,globals.m_oCrossHair.x+7,globals.m_oCrossHair.y  ,globals.m_oColorScheme.m_oCrossHairColor);
            */
            const FEReal INF = 1e6;
			_poRenderer->SetMaterial(NULL);
            _poRenderer->RenderLine(globals.m_oCrossHair.x,-INF,globals.m_oCrossHair.x,INF,globals.m_oColorScheme.m_oCrossHairColor);
            _poRenderer->RenderLine(-INF,globals.m_oCrossHair.y,INF,globals.m_oCrossHair.y,globals.m_oColorScheme.m_oCrossHairColor);
        }

        // Show grid
        if (globals.m_uiGridSize >= 1)
			SpriteEditorRenderGrid(_poRenderer);

        // CFESpriteInstMgr::I()->Render(_poRenderer);

    _poRenderer->EndScene();
}
//---------------------------------------------------------------------------
