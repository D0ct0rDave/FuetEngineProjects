// ----------------------------------------------------------------------------
/*! \class CFESkelAnimRenderer
 *  \brief A class for generic configuration settings reading
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESkelAnimRenderer.h"
#include "CFESkelAnim.h"
#include "CFESkelAnimSpriteModel.h"
#include "CFESkelAnimSprite.h"
#include "CFESkelAnimGroup.h"
#include "CFESkelAnimBone.h"
#include "CFESkelAnimMesh.h"
#include "CFESkelAnimNodeVisitor.h"

#include "graphics\sprite\CFESprite.h"
#include "graphics\sprite\CFESpriteInstMgr.h"
#include "graphics\sprite\CFESpriteRenderer.h"
#include "support\graphics\CFERenderer.h"
#include "support\graphics\CFEMaterialMgr.h"


//-----------------------------------------------------------------------------
class CFESkelAnimNodeRenderer : public CFESkelAnimNodeVisitor
{
	public:

		CFESkelAnimNodeRenderer(CFERenderer* _poRenderer,FEReal _rDepth,const CFEColor& _oColor)
		{
			m_poRenderer	= _poRenderer;
			m_rDepth		= _rDepth;
			m_oColor		= _oColor;
		}

		// Do nothing
        virtual void Visit(CFESkelAnimSpriteModel* _poObj){};
        /// 
        virtual void Visit(CFESkelAnimSprite* _poObj);
        /// 
        virtual void Visit(CFESkelAnimGroup* _poObj);
        /// 
        virtual void Visit(CFESkelAnimMesh* _poObj);
        /// 
        virtual void Visit(CFESkelAnimBone* _poObj);

	protected:

		CFERenderer*	m_poRenderer;
		FEReal			m_rDepth;
		CFEColor		m_oColor;
};
//-----------------------------------------------------------------------------
void CFESkelAnimNodeRenderer::Visit(CFESkelAnimSprite* _poObj)
{
	if (! _poObj->bIsVisible()) return;

	// WARNING: Really don't needed.
	CFEColor oResColor = m_oColor * _poObj->oGetColor();
	
	CFESpriteInstMgr::SetColor(_poObj->hGetSprite(),oResColor);
	CFESpriteInstMgr::SetAngle(_poObj->hGetSprite(),_poObj->rGetAngle());
	CFESpriteInstMgr::SetScale(_poObj->hGetSprite(),_poObj->oGetScale());
	CFESpriteInstMgr::SetPos  (_poObj->hGetSprite(),_poObj->oGetPos());
	
	/*
	CFESprite* poSpr = CFESpriteInstMgr::poGetSprite(_poObj->hGetSprite());	
	if (poSpr->m_sName != "lolo")
	{
		m_poRenderer->SetBlendMode(BM_ALPHA);
		m_poRenderer->SetMaterial(poSpr->poGetAction(0)->m_oSeq[0].m_hMaterial);
		m_poRenderer->RenderSprite(_poObj->oGetPos(),CFEVect2(0.5,0.5),_poObj->oGetScale()*poSpr->poGetAction(0)->m_oSeq[0].m_oSize,_poObj->rGetAngle(),CFEVect2(0,0),CFEVect2(1,1),_poObj->oGetColor(),0.25);
    }
    else
    */
    if ( _poObj->hGetSprite() )
        CFESpriteRenderer::Render(m_poRenderer,(CFESpriteInst*)_poObj->hGetSprite(),_poObj->oGetPos(),m_rDepth + _poObj->rGetDepth()*_01r,_poObj->oGetScale(),_poObj->rGetAngle(),oResColor);
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeRenderer::Visit(CFESkelAnimGroup* _poObj)
{
	if (! _poObj->bIsVisible()) return;

	m_poRenderer->PushTransform();
		
		m_poRenderer->TranslateTransform(_poObj->oGetPos().x,_poObj->oGetPos().y);
		m_poRenderer->RotateTransform(_poObj->rGetAngle());
		m_poRenderer->ScaleTransform(_poObj->oGetScale().x,_poObj->oGetScale().y);

        m_rDepth += _poObj->rGetDepth()*_01r;

		for (uint i=0;i<_poObj->uiNumObjs();i++)
			_poObj->poGetNode(i)->Accept(this);

        m_rDepth -= _poObj->rGetDepth()*_01r;

    m_poRenderer->PopTransform();
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeRenderer::Visit(CFESkelAnimBone* _poObj)
{
    if (! _poObj->bIsVisible()) return;

    /// Attach this bone to the given mesh.
    CFEVect2 oScale  = _poObj->oGetScale();
    CFEVect2 oAbsPos = _poObj->oGetPos()*oScale;
    _poObj->SetAbsPos(oAbsPos);

    if (_poObj->poGetAttachedNode() != NULL)
    {
	    m_poRenderer->PushTransform();

	        m_poRenderer->TranslateTransform(oAbsPos.x,oAbsPos.y);
		    m_poRenderer->RotateTransform(_poObj->rGetAngle());

                m_rDepth += _poObj->rGetDepth()*_01r;
			    _poObj->poGetAttachedNode()->Accept(this);
			    m_rDepth -= _poObj->rGetDepth()*_01r;

        m_poRenderer->PopTransform();
    }
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeRenderer::Visit(CFESkelAnimMesh* _poObj)
{
	if (! _poObj->bIsVisible()) return;

    if (_poObj->uiGetNumAttachedBones() < 2) return;
	uint uiNumAttachedBones = _poObj->uiGetNumAttachedBones()-1;

    FEHandler hSpr = _poObj->hGetSprite();
    if (hSpr == NULL) return;

    CFESpriteFrame* poSprFrame = CFESpriteInstMgr::poGetCurrentFrame(hSpr);
    if (poSprFrame == NULL) return;

        /// Get vertex array base pointer
        CFEVect2* poUV      = _poObj->poGetUV();
        CFEVect2* poVX      = _poObj->poGetVX();

		// -------------------------
		// Setup texcoords
		// -------------------------
		FEReal rIX = poSprFrame->m_oUV.m_oIni.x; 
		FEReal rFX = poSprFrame->m_oUV.m_oEnd.x; 
		FEReal rIY = poSprFrame->m_oUV.m_oIni.y; 
		FEReal rFY = poSprFrame->m_oUV.m_oEnd.y; 

		FEReal rStep = _1r / (FEReal)(uiNumAttachedBones);
		
		poUV[0].x = rIX;
		poUV[0].y = rIY;
		poUV[1].x = rFX;
		poUV[1].y = rIY;
		poUV+=2;

		// Inbetween texcoords
		FEReal rVal = _0r;
		for (uint i=1;i<uiNumAttachedBones;i++)
		{
			rVal += rStep;

			poUV[0].x = rIX;
			poUV[0].y = CFEMath::rLerp(rIY,rFY,rVal);
			poUV[1].x = rFX;
			poUV[1].y = poUV[0].y;
			poUV += 2;

			poUV[0].x = rIX;
			poUV[0].y = CFEMath::rLerp(rIY,rFY,rVal+rStep);
			poUV[1].x = rFX;
			poUV[1].y = poUV[0].y;
			poUV += 2;
		}

		// Last texcoords
		poUV[0].x = rIX;
		poUV[0].y = rFY;
		poUV[1].x = rFX;
		poUV[1].y = rFY;

		// -------------------------
		// Setup vertices
		// -------------------------
        FEHandler hMaterial = poSprFrame->m_hMaterial;

        uint uiWidth = 0;
        CFEMaterialMgr::bGetMaterialProperty(hMaterial,"DiffuseMap.Width",(FEPointer)&uiWidth);
        FEReal rHWidth = poSprFrame->m_oSize.x * _05r;

        // Setup vertices
        CFEVect2 oBonePos   = _poObj->poGetAttachedBone(0)->oGetAbsPos();
        CFEVect2 oPBonePos  = oBonePos; 
        CFEVect2 oNBonePos  = _poObj->poGetAttachedBone(1)->oGetAbsPos();
        CFEVect2 oDir       = oNBonePos - oBonePos;       
        oDir.Normalize();
        oDir.Orthogonalize();

        
        CFEVect2 oPDir      = oDir* rHWidth;
        // Setup first vertices.
        poVX[0] = oBonePos + oPDir;
        poVX[1] = oBonePos - oPDir;

        poVX += 2;

			const FEReal SEG_1_FACT = _1r / _3r;
			const FEReal SEG_2_FACT = _1r / _6r;

            // Setup inbetween vertices.
            for (uint i=1;i<uiNumAttachedBones;i++)
            {
                oPBonePos   = oBonePos;
                oBonePos    = oNBonePos;
                oNBonePos   = _poObj->poGetAttachedBone(i+1)->oGetAbsPos();

                oPDir   = oDir;
                oDir    = oNBonePos - oBonePos;
                oDir.Normalize();
                oDir.Orthogonalize();

                // Add previous segment vertices
                CFEVect2 oPPos;
                oPPos.Lerp(oPBonePos,oBonePos,_1r - JOINT_MARGIN);
                CFEVect2 oPW;
                oPW.Lerp(oPDir,oDir,SEG_1_FACT);
                oPW.Normalize();
                oPW *= rHWidth;

                poVX[0] = oPPos + oPW;
                poVX[1] = oPPos - oPW;
                poVX += 2;

                // Add next segment vertices
                CFEVect2 oNPos;
                oNPos.Lerp(oBonePos,oNBonePos,JOINT_MARGIN);
                CFEVect2 oNW;
                oNW.Lerp(oPDir,oDir,SEG_2_FACT);
                oNW.Normalize();
                oNW *= rHWidth;

                poVX[0] = oNPos + oNW;
                poVX[1] = oNPos - oNW;

                poVX += 2;
            }

        // Setup last vertices.
        oDir *= rHWidth;
        poVX[0] = oNBonePos + oDir;
        poVX[1] = oNBonePos - oDir;

		// -------------------------
		// Setup vertex color
		// -------------------------
        CFEColor* poVC      = _poObj->poGetVC();    
        for (uint i=0;i<_poObj->uiGetNumVXs();i++)
		{
			*poVC = m_oColor;
			poVC++;
		}

		// -------------------------
        /// Render mesh
        // -------------------------
        m_poRenderer->SetBlendMode(BM_ALPHA);
        m_poRenderer->SetMaterial( poSprFrame->m_hMaterial );
        m_poRenderer->SetDepth(m_rDepth + _poObj->rGetDepth()*_01r);
        m_poRenderer->RenderMesh(NULL,_poObj->poGetVX(),_poObj->poGetUV(),_poObj->poGetVC(),_poObj->uiGetNumVXs());

        /*
        poVX = _poObj->poGetVX();
        uint uiNumVXs = _poObj->uiGetNumVXs();
        for (uint i=0;i<uiNumVXs-1;i++)
        {
            m_poRenderer->RenderLine(poVX[0].x,poVX[0].y,poVX[1].x,poVX[1].y,CFEColor(1,0,0,1));
            poVX++;
        }
        */
}
//-----------------------------------------------------------------------------
void CFESkelAnimRenderer::Render(CFERenderer *_poRenderer,CFESkelAnimNode* _poNode,const CFEVect2& _oPos,FEReal _rDepth,const CFEVect2& _oScale,FEReal _rAngle,const CFEColor& _oColor)
{
	CFESkelAnimNodeRenderer oAnimNodeRenderer(_poRenderer,_rDepth,_oColor);

	_poRenderer->PushTransform();

		_poRenderer->TranslateTransform(_oPos.x,_oPos.y);
		_poRenderer->RotateTransform(_rAngle);		
		_poRenderer->ScaleTransform(_oScale.x,_oScale.y);

		_poNode->Accept(&oAnimNodeRenderer);

    _poRenderer->PopTransform();
}
//-----------------------------------------------------------------------------
