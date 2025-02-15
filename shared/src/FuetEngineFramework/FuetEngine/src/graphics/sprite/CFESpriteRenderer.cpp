// ----------------------------------------------------------------------------
/*! \class CFESpriteRenderer
 *  \brief Class to render sprite instances.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
// #include "System/CFESystem.h"

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
    _poRenderer->RenderSprite(_oPos,_poFrame->m_oPivot,_oScale * _poFrame->m_oSize,_rAngle,_poFrame->m_oUV.m_oIni,_poFrame->m_oUV.m_oEnd,_oColor,_rAlpha);
}
// ----------------------------------------------------------------------------
void RenderSpriteFrames(CFERenderer *_poRenderer,CFESpriteFrame* _poSrc,CFESpriteFrame* _poDst,FEReal _rFactor,const CFEVect2& _oPos,const CFEVect2& _oScale,FEReal _rAngle,const CFEColor& _oColor)
{    
    FEReal rSFactor = CFEMath::rClamp(0,1,5*(_1r-_rFactor));
    FEReal rDFactor = CFEMath::rClamp(0,1,2.5f*_rFactor-_1r);

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
    if ( _poInstance == NULL ) return;    

    CFESprite* poSprite = _poInstance->m_poSprite;
    CFESpriteAction* poAction = &poSprite->m_oActions[_poInstance->m_uiSpriteAction];

    // First set the blendmode
    _poRenderer->SetBlendMode( poSprite->m_eBlendMode );

	// Special cases: (_poInstance->m_rActionTime  <= _0r) is a specific case of the one treated on SafeRenderFrame
    if ((poSprite->m_eBlendMode == BM_NONE) || (poSprite->m_eBlendMode == BM_COPY) /* || (_poInstance->m_rActionTime  <= _0r) */ || (poAction->m_oSeq.size() == 1) || (poAction->m_rActionTime<=_0r))
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
			#pragma message(__FUNCTION__"/"__FILE__)
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
			#pragma message(__FUNCTION__"/"__FILE__)
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
