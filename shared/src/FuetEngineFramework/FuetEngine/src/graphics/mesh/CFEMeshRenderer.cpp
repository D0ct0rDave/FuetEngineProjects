// ----------------------------------------------------------------------------
/*! \class CFEMeshRenderer
 *  \brief Class to Render Mesh instances.
 *   DOESN'T SUPPORT STILL. Mesh instance Color&Alpha, Sprite Frames with UV animation, Sprite blending,
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
// #include "core/CFECore.h"

#include "CFEMesh.h"
#include "support/graphics/CFERenderer.h"
#include "FEConsts.h"
#include "CFEMeshRenderer.h"
#include "Graphics/Sprite/CFESprite.h"
#include "Graphics/Sprite/CFESpriteInstMgr.h"
// ----------------------------------------------------------------------------
// _poFrame->m_oUV va en coordenadas entre (0..1,0..1)
// Generate frame corners.
// Translate to the center.
// Resize.
// Translate negatively the sized pivot.
// Rotate.
// Translate positivelly the pivot + the position.
// ----------------------------------------------------------------------------
inline void RenderMeshFrame(CFERenderer *_poRenderer,CFEVect2*_poVX,CFEVect2*_poUV,uint _uiNumVX,unsigned short* _pusIdx,uint _uiNumIdx,const CFEVect2& _oPos,const CFEVect2& _oScale,FEReal _rAngle,const CFEColor& _oColor)
{
	_poRenderer->PushTransform();

		_poRenderer->TranslateTransform(_oPos.x,_oPos.y);
		_poRenderer->ScaleTransform(_oScale.x,_oScale.y);
		_poRenderer->RotateTransform(_rAngle);
		_poRenderer->RenderMesh(_pusIdx,_poVX,_poUV,_oColor,(_pusIdx==NULL)?_uiNumVX:_uiNumIdx);

	_poRenderer->PopTransform();
}
// ----------------------------------------------------------------------------
void RenderMeshFrames(CFERenderer *_poRenderer,CFEMeshInst* _poMeshInstance,CFEMeshAction* _poAction,uint _uiSrcFrame,uint _uiDstFrame,FEReal _rFactor,const CFEVect2& _oPos,const CFEVect2& _oScale,FEReal _rAngle,const CFEColor& _oColor)
{
	// morph vertices...
	CFEVect2* poAVX = &_poAction->m_oSeq[_uiSrcFrame]->m_oVX[0];
	CFEVect2* poBVX = &_poAction->m_oSeq[_uiDstFrame]->m_oVX[0];
	CFEVect2* poDstVX = &_poMeshInstance->m_oVX[0];
	uint uiNumVX = _poAction->m_oSeq[_uiDstFrame]->m_oVX.size();

	for (uint i=0;i<uiNumVX;i++)
		poDstVX[i].Lerp(poAVX[i],poBVX[i],_rFactor);

    RenderMeshFrame(_poRenderer,
					poDstVX,
					&_poMeshInstance->m_poMesh->m_oUV[0],
					uiNumVX,
					(_poMeshInstance->m_poMesh->m_usIdx.size()>0)?&_poMeshInstance->m_poMesh->m_usIdx[0]:NULL,
					_poMeshInstance->m_poMesh->m_usIdx.size(),
					_oPos,_oScale,_rAngle,_oColor);
}
// ----------------------------------------------------------------------------
void SafeRenderFrame(CFERenderer *_poRenderer,CFEMeshInst* _poMeshInstance,CFEMeshAction* _poAction,uint _uiFrame,const CFEVect2& _oPos,FEReal _rDepth,const CFEVect2& _oScale,FEReal _rAngle,const CFEColor& _oColor)
{
	uint uiLoops = _poMeshInstance->m_rActionTime / _poAction->m_rActionTime;
	FEReal rTime = _poMeshInstance->m_rActionTime - (FEReal(uiLoops)*_poAction->m_rActionTime);

	CFEMeshFrame* poSF = _poAction->m_oSeq[ _uiFrame ];
	FEReal rRelTime  = rTime - poSF->m_rStartTime;
	uint uiNextFrame = _poAction->uiNextFrame( _uiFrame );

	if ((rRelTime <= poSF->m_rDelay) || (_uiFrame == uiNextFrame))
	{
		// Render only the current frame.
		RenderMeshFrame(_poRenderer,
						&poSF->m_oVX[0],
						&_poMeshInstance->m_poMesh->m_oUV[0],
						poSF->m_oVX.size(),
						(_poMeshInstance->m_poMesh->m_usIdx.size()>0)?&_poMeshInstance->m_poMesh->m_usIdx[0]:NULL,
						_poMeshInstance->m_poMesh->m_usIdx.size(),
						_oPos,_oScale,_rAngle,_oColor);
	}
	else
	{
		// Blend factor between frames.
		FEReal rFact = (rRelTime - poSF->m_rDelay) / poSF->m_rBlendTime;
		RenderMeshFrames(_poRenderer,_poMeshInstance,_poAction,_uiFrame,uiNextFrame,rFact,_oPos,_oScale,_rAngle,_oColor);
	}
}
// ----------------------------------------------------------------------------
void CFEMeshRenderer::Render(CFERenderer *_poRenderer,CFEMeshInst* _poInstance,const CFEVect2& _oPos,FEReal _rDepth,const CFEVect2& _oScale,FEReal _rAngle,const CFEColor& _oColor)
{
    // sanity checks
    if (( _poInstance == NULL ) || (_poInstance->m_poMesh == NULL) || (_poInstance->m_poMesh->m_oUV.size() == 0)) return;
    
	CFEMesh* poMesh	= _poInstance->m_poMesh;
	
    CFEMeshAction* poAction    = poMesh->m_poActions[_poInstance->m_uiMeshAction];
	CFESpriteFrame* poSprFrame = CFESpriteInstMgr::I()->poGetCurrentFrame(poMesh->m_hSpriteInst);
	// if (poSprFrame == NULL) return;

    // First set the blendmode
    _poRenderer->SetBlendMode( BM_ALPHA );

	if (poSprFrame != NULL)
		_poRenderer->SetMaterial( poSprFrame->m_hMaterial );
	else
		_poRenderer->SetMaterial(NULL);

    _poRenderer->SetDepth(_rDepth);

    // Render the frame
    uint uiCurFrame  = _poInstance->m_uiCurrentActionFrame;
    switch (poAction->m_ePlayMode)
    {
        // ------------------------------------------------
        case MAPM_NONE:
        case MAPM_ONESHOT:
        {
			SafeRenderFrame(_poRenderer,_poInstance,poAction,uiCurFrame,_oPos,_rDepth,_oScale,_rAngle,_oColor);
        }
        break;

        // ------------------------------------------------
        case MAPM_LOOP:
        {
			SafeRenderFrame(_poRenderer,_poInstance,poAction,uiCurFrame,_oPos,_rDepth,_oScale,_rAngle,_oColor);
        }
        break;

        // ------------------------------------------------
        case MAPM_PINGPONGSTOP:
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

        case MAPM_PINGPONG:
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
                ((poMesh->m_eBlendMode == BM_ALPHA) || (poMesh->m_eBlendMode == BM_ALPHAADD))
                )
                RenderMeshFrames(&poAction->m_oSeq[uiIFrame],&poAction->m_oSeq[iEFrame],rFactor,_oPos,_oScale,_rAngle,_oColor);
            else
                RenderMeshFrame(&poAction->m_oSeq[uiIFrame],_oPos,_oScale,_rAngle,_oColor,_1r);
			#endif
        }
        break;
    }
}
// ----------------------------------------------------------------------------
