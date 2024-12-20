// ----------------------------------------------------------------------------
/*! \class CFESpriteRenderer
 *  \brief Class to render sprite instances.
 *  \author David M�rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M�rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
// #include "core/CFECore.h"

#include "CFESprite.h"
#include "support/graphics/CFERenderer.h"
#include "FEConsts.h"
#include "CFESpriteRenderer.h"
// ----------------------------------------------------------------------------
// _poFrame->m_oUV va en coordenadas entre (0..1,0..1)
// Generate frame corners.
// Translate to the center.
// Resize.
// Translate negatively the sized pivot.
// Rotate.
// Translate positivelly the pivot + the position.
// ----------------------------------------------------------------------------
static void GenerateSpriteGeometry(CFESpriteFrame* _poFrame,const CFEVect2& _oPos,const CFEVect2& _oScale,FEReal _rAngle,CFEVect2* _poVX)
{
    FEReal rCosA = CFEMath::rCos(_rAngle);
    FEReal rSinA = CFEMath::rSin(_rAngle);

    CFEVect2 oPivot = (CFEVect2(_05r,_05r) - _poFrame->m_oPivot);

    _poVX[0]   = CFEVect2(-_05r,-_05r);
    _poVX[2]   = CFEVect2( _05r, _05r);
    _poVX[1].x = _poVX[2].x;
    _poVX[1].y = _poVX[0].y;
    _poVX[3].x = _poVX[0].x;
    _poVX[3].y = _poVX[2].y;

    for (uint i=0;i<4;i++)
    {
        // 1
        _poVX[i] += oPivot;

        // 2
        _poVX[i] *= _oScale;

		_poVX[i].x *= _poFrame->m_oSize.x;
		_poVX[i].y *= _poFrame->m_oSize.y;
		
		// 4
        CFEVect2 oPos = _poVX[i];
        _poVX[i].x = oPos.x*rCosA - oPos.y*rSinA;
        _poVX[i].y = oPos.x*rSinA + oPos.y*rCosA;

        // 5
        _poVX[i] += _oPos;
    }
}
// ----------------------------------------------------------------------------
inline void RenderSpriteFrame(CFERenderer *_poRenderer,CFESpriteFrame* _poFrame,const CFEVect2& _oPos,const CFEVect2& _oScale,FEReal _rAngle,const CFEColor& _oColor,FEReal _rAlpha)
{
	_poRenderer->SetMaterial(_poFrame->m_hMaterial);

	// Normal path / Scale Equally XU / YV ...
	if ((_poFrame->m_bUWorldCoords  == false) && (_poFrame->m_bVWorldCoords  == false))
	{
		CFEVect2 oIUV = _poFrame->m_oUV.m_oIni;
		CFEVect2 oFUV = _poFrame->m_oUV.m_oEnd;

		if (_poFrame->m_bScaleXUEqually)
		{
			oIUV.x *= _oScale.x;
			oFUV.x *= _oScale.x;
		}
		if (_poFrame->m_bScaleYVEqually)
		{
			oIUV.y *= _oScale.y;
			oFUV.y *= _oScale.y;
		}

		_poRenderer->RenderSprite(_oPos,_poFrame->m_oPivot,_oScale * _poFrame->m_oSize,_rAngle,oIUV,oFUV,_oColor,_rAlpha);
	}

	// UVProject on only one axis
	else if ((_rAngle==_0r) || (_poFrame->m_bUWorldCoords != _poFrame->m_bVWorldCoords))
	{
		/*
		CFEVect2 oPivot = (CFEVect2(_05r,_05r) - _poFrame->m_oPivot);

		CFEVect2 oIUV = CFEVect2(-_05r,-_05r);
		oIUV += oPivot;
		oIUV *= _oScale;
		oIUV *= _poFrame->m_oSize;
		oIUV += _oPos;						// these are world coords (without transform)
		oIUV *= _poFrame->m_oUV.m_oEnd;

		CFEVect2 oFUV = CFEVect2(_05r,_05r);
		oFUV += oPivot;
		oFUV *= _oScale;
		oFUV *= _poFrame->m_oSize;
		oFUV += _oPos;						// these are world coords (without transform)
		oFUV *= _poFrame->m_oUV.m_oEnd;
		*/
		
		/*
		V   = (_05r,_05r);
		PIV' = V - PIV
		
		I = ((-V + PIV')*SCL*SZ + POS)*ITD 
		F = (( V + PIV')*SCL*SZ + POS)*ITD
		
		I = ((-V + V - PIV)*SCL*SZ + POS)*ITD =>
		F = (( V + V - PIV)*SCL*SZ + POS)*ITD =>

		I = ((-PIV)*SCL*SZ + POS)*ITD = (-PIV*SCL*SZ + POS)*ITD = oIUV
		F = (( 2V - PIV)*SCL*SZ + POS)*ITD =((ONE - PIV)*SCL*SZ + POS)*ITD => (SCL*SZ -PIV*SCL*SZ + POS)*ITD =  (SCL*SZ*ITD) + (-PIV*SCL*SZ + POS)*ITD = (SCL*SZ*ITD) + oIUV
		*/

		CFEVect2 oMult = _poFrame->m_oSize * _oScale;

		CFEVect2 oIUV(_poFrame->m_oPivot);
		oIUV *= (-_1r);
		oIUV *= oMult;
		oIUV += _oPos;
		oIUV *= _poFrame->m_o1OverDims;

		CFEVect2 oFUV(oIUV);
		oMult *= _poFrame->m_o1OverDims;
		oFUV += oMult;
		
		if (_poFrame->m_bUWorldCoords == false)
		{
			oIUV.x = _poFrame->m_oUV.m_oIni.x;
			oFUV.x = _poFrame->m_oUV.m_oEnd.x;
			
			if (_poFrame->m_bScaleXUEqually)
			{
				oIUV.x *= _oScale.x;
				oFUV.x *= _oScale.x;
			}
		}
		else if (_poFrame->m_bVWorldCoords == false)
		{
			oIUV.y = _poFrame->m_oUV.m_oIni.y;
			oFUV.y = _poFrame->m_oUV.m_oEnd.y;

			if (_poFrame->m_bScaleYVEqually)
			{
				oIUV.y *= _oScale.y;
				oFUV.y *= _oScale.y;
			}
		}

		_poRenderer->RenderSprite(_oPos,_poFrame->m_oPivot,_oScale * _poFrame->m_oSize,_rAngle,oIUV,oFUV,_oColor,_rAlpha);
	}
	else
	{
		CFEVect2 oVX[4];
		CFEVect2 oUV[4];
		CFEColor oVC[4];

		GenerateSpriteGeometry(_poFrame,_oPos,_oScale,_rAngle,oVX);

		for (uint i=0;i<4;i++)
		{
			oUV[i] = oVX[i]*_poFrame->m_o1OverDims;
			oVC[i] = _oColor;
		}

		_poRenderer->RenderQuadList(oVX,oUV,oVC,1);	
	}
}
// ----------------------------------------------------------------------------
void RenderSpriteFrames(CFERenderer *_poRenderer,CFESpriteFrame* _poSrc,CFESpriteFrame* _poDst,FEReal _rFactor,const CFEVect2& _oPos,const CFEVect2& _oScale,FEReal _rAngle,const CFEColor& _oColor)
{
    FEReal rSFactor = CFEMath::rClamp(_0r,_1r,_2r*(_1r-_rFactor));
    FEReal rDFactor = CFEMath::rClamp(_0r,_1r,_2r*(_rFactor));

    RenderSpriteFrame(_poRenderer,_poSrc,_oPos,_oScale,_rAngle,_oColor,rSFactor);
    RenderSpriteFrame(_poRenderer,_poDst,_oPos,_oScale,_rAngle,_oColor,rDFactor);
}
// ----------------------------------------------------------------------------
void SafeRenderFrame(CFERenderer *_poRenderer,CFESpriteAction* _poAction,uint _uiFrame,FEReal _rActionTime,const CFEVect2& _oPos,FEReal _rDepth,const CFEVect2& _oScale,FEReal _rAngle,const CFEColor& _oColor)
{
	uint uiLoops = _rActionTime / _poAction->m_rActionTime;
	FEReal rTime = _rActionTime - (FEReal(uiLoops)*_poAction->m_rActionTime);

	CFESpriteFrame* poSF = &_poAction->m_oSeq[ _uiFrame ];
	FEReal rRelTime  = rTime - poSF->m_rStartTime;
	uint uiNextFrame = _poAction->uiNextFrame( _uiFrame );

	// Sets the rendering depth.
	_poRenderer->SetDepth(_rDepth);

	if (rRelTime <= poSF->m_rDelay)
	{
		// Render only the current frame.
		RenderSpriteFrame(_poRenderer,poSF,_oPos,_oScale,_rAngle,_oColor,_1r);
	}
	else
	{
		// Blend factor between frames.	
		FEReal rFact = (rRelTime - poSF->m_rDelay) / poSF->m_rBlendTime;
		RenderSpriteFrames(_poRenderer,&_poAction->m_oSeq[_uiFrame],&_poAction->m_oSeq[uiNextFrame],rFact,_oPos,_oScale,_rAngle,_oColor);
	}
}
// ----------------------------------------------------------------------------
void CFESpriteRenderer::Render(CFERenderer *_poRenderer,CFESpriteInst* _poInstance,const CFEVect2& _oPos,FEReal _rDepth,const CFEVect2& _oScale,FEReal _rAngle,const CFEColor& _oColor)
{
    // sanity checks
    if (( _poInstance == NULL ) || (_poInstance->m_poSprite == NULL) || (_poRenderer==NULL)) return;

    CFESprite* poSprite = _poInstance->m_poSprite;
	/*
	if (poSprite->m_bFastPath)
	{
		_poRenderer->SetDepth(_rDepth);
		_poRenderer->SetBlendMode(poSprite->m_oActions[0].m_eBlendMode);
		CFESpriteFrame poFrame = poSprite->m_oActions[0].m_oSeq[0];
		
		_poRenderer->SetMaterial(poFrame->m_hMaterial);
		_poRenderer->RenderSprite(_oPos,poFrame->m_oPivot,_oScale * poFrame->m_oSize,_rAngle,poFrame->m_oIni,poFrame->m_oEnd,_oColor,_rAlpha);
		return;
	}
	*/

    CFESpriteAction* poAction = &poSprite->m_oActions[_poInstance->m_uiSpriteAction];

	// if (poAction->m_oSeq.size() == 0) return; // <- the below condition tests this and other problems (in strange cases like mem corruption)
	if (_poInstance->m_uiCurrentActionFrame >= poAction->m_oSeq.size()) return;

    // First set the blendmode
    _poRenderer->SetBlendMode(poAction->m_eBlendMode);

	// Special cases: (_poInstance->m_rActionTime  <= _0r) is a specific case of the one treated on SafeRenderFrame
    if ((poAction->m_eBlendMode == BM_NONE) || (poAction->m_eBlendMode == BM_COPY) /* || (_poInstance->m_rActionTime  <= _0r) */ || (poAction->m_oSeq.size() == 1) || (poAction->m_rActionTime<=_0r))
	{
	    _poRenderer->SetDepth(_rDepth);
        RenderSpriteFrame(_poRenderer,&poAction->m_oSeq[_poInstance->m_uiCurrentActionFrame],_oPos,_oScale,_rAngle,_oColor, _1r);
		return;
	}

    // Render the frame
    uint uiCurFrame  = _poInstance->m_uiCurrentActionFrame;

    switch (poAction->m_ePlayMode)
    {
        // ------------------------------------------------
        case SFSPM_NONE:
        case SFSPM_ONESHOT:
        {
			SafeRenderFrame(_poRenderer,poAction,uiCurFrame,_poInstance->m_rActionTime,_oPos,_rDepth,_oScale,_rAngle,_oColor);
        }
        break;

        // ------------------------------------------------
        case SFSPM_LOOP:
        {
			SafeRenderFrame(_poRenderer,poAction,uiCurFrame,_poInstance->m_rActionTime,_oPos,_rDepth,_oScale,_rAngle,_oColor);
        }
        break;

        // ------------------------------------------------
        case SFSPM_PINGPONGSTOP:
			// #pragma message(__FUNCTION__"/"__FILE__)
        	#pragma message("#################")
			#pragma message("TO BE IMPLEMENTED")
			#pragma message("#################")
        #if 0
        if (rFrame > 2*uiMaxFrames)
            return;
        #endif
        /*
        else
             // continue in the next switch branch.
        */

        case SFSPM_PINGPONG:
        {
			// #pragma message(__FUNCTION__"/"__FILE__)
        	#pragma message("#################")
			#pragma message("TO BE IMPLEMENTED")
			#pragma message("#################")
        
			#if 0
            // initial frame.
            uint uiIFrame = (uint)rFrame % (2*uiMaxFrames); // LOOP
            int iEFrame = 0;

            // Blend ractor between frames.
            FEReal rFactor = rFrame - (uint)rFrame;

            if (uiIFrame >= uiMaxFrames)
            {
                // backward direction
                uiIFrame = (2*uiMaxFrames - uiIFrame)-1;

                iEFrame = uiIFrame-1;
                if (iEFrame < 0)
                    iEFrame = 0;
            }
            else
            {
                // initial frame.
                uint uiIFrame = (uint)rFrame % uiMaxFrames;

                // Last frame.
                iEFrame = uiIFrame + 1;
                if (iEFrame >= uiMaxFrames)
                    iEFrame = uiMaxFrames-1;
            }

            if (
                (uiIFrame != iEFrame)
                &&
                ((poSprite->m_eBlendMode == BM_ALPHA) || (poSprite->m_eBlendMode == BM_ALPHAADD))
                )
                RenderSpriteFrames(&poAction->m_oSeq[uiIFrame],&poAction->m_oSeq[iEFrame],rFactor,_oPos,_oScale,_rAngle,_oColor);
            else
                RenderSpriteFrame(&poAction->m_oSeq[uiIFrame],_oPos,_oScale,_rAngle,_oColor,_1r);
			#endif
        }
        break;
    }
}
// ----------------------------------------------------------------------------
void CFESpriteRenderer::GetSpriteGeometry(CFESpriteInst* _poInstance,const CFEVect2& _oPos,const CFEVect2& _oScale,FEReal _rAngle,CFEVect2* _poVX)
{
    CFESprite* poSprite = _poInstance->m_poSprite;
    CFESpriteAction* poAction = &poSprite->m_oActions[_poInstance->m_uiSpriteAction];

	GenerateSpriteGeometry(&poAction->m_oSeq[_poInstance->m_uiCurrentActionFrame],_oPos,_oScale,_rAngle,_poVX);
}
// ----------------------------------------------------------------------------
