// ----------------------------------------------------------------------------
/*! \class CFESkelAnimRenderer
 *  \brief A class for generic configuration settings reading
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
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
#include "CFESkelAnimNode.h"
#include "CFESkelAnimNodeVisitor.h"

#include "graphics\sprite\CFESprite.h"
#include "graphics\sprite\CFESpriteInstMgr.h"
#include "graphics\sprite\CFESpriteRenderer.h"
#include "support\graphics\CFERenderer.h"
#include "support\graphics\CFEMaterialMgr.h"

// #define BNB_SKEL_SYSTEM
const FEReal DEPTH_FACT = _01r;
//-----------------------------------------------------------------------------
class CFESkelAnimNodeRenderer : public CFESkelAnimNodeVisitor
{
	public:
		
		CFESkelAnimNodeRenderer(CFERenderer* _poRenderer,FEReal _rDepth,const CFEColor& _oColor)
		{
			m_rDepthBase	= _1r;
			m_poRenderer	= _poRenderer;
			m_rDepth		= _rDepth;
			m_rCurDepth		= _rDepth;
			m_oColor		= _oColor;
		}

		static void Render(CFESkelAnimNode* _poNode, CFERenderer* _poRenderer,FEReal _rDepth,const CFEColor& _oColor)
		{
			CFESkelAnimNodeRenderer oObj(_poRenderer,_rDepth,_oColor);
			_poNode->Accept(&oObj);
		}

		// avoid override function hiding
		// http://www.parashift.com/c++-faq-lite/hiding-rule.html
		using CFESkelAnimNodeVisitor::Visit;

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
		FEReal			m_rCurDepth;
		FEReal			m_rDepthBase;
		CFEColor		m_oColor;
};
//-----------------------------------------------------------------------------
void CFESkelAnimNodeRenderer::Visit(CFESkelAnimSprite* _poObj)
{
	if (! _poObj->bIsVisible()) return;
	if ( _poObj->hGetSprite() == NULL) return;
	
	FEReal rDepth		= m_rCurDepth + _poObj->rGetDepth()*m_rDepthBase*DEPTH_FACT;
	CFESpriteInstMgr::I()->SetAngle(_poObj->hGetSprite(),_poObj->rGetAngle());
	CFESpriteInstMgr::I()->SetPos  (_poObj->hGetSprite(),_poObj->oGetPos());

	#ifdef USE_SKELANIM_SCALE_COLOR
	CFEColor oResColor	= m_oColor * _poObj->oGetColor();
	CFESpriteInstMgr::I()->SetColor(_poObj->hGetSprite(),oResColor);
	CFESpriteInstMgr::I()->SetScale(_poObj->hGetSprite(),_poObj->oGetScale());
	CFESpriteRenderer::Render(m_poRenderer,(CFESpriteInst*)_poObj->hGetSprite(),_poObj->oGetPos(),rDepth,_poObj->oGetScale(),_poObj->rGetAngle(),oResColor);
	#else
	CFESpriteInstMgr::I()->SetColor(_poObj->hGetSprite(),m_oColor);
	CFESpriteRenderer::Render(m_poRenderer,(CFESpriteInst*)_poObj->hGetSprite(),_poObj->oGetPos(),rDepth,CFEVect2::ONE(),_poObj->rGetAngle(),m_oColor);
	#endif
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeRenderer::Visit(CFESkelAnimGroup* _poObj)
{
	if (! _poObj->bIsVisible()) return;

    #ifdef USE_SKELANIM_SCALE_COLOR
	CFEColor oOldColor = m_oColor;
	m_oColor *= _poObj->oGetColor();
	#endif

		m_poRenderer->PushTransform();
		FEReal rOldDepth		= m_rCurDepth;
		FEReal rOldBaseDepth	= m_rDepthBase;

			m_poRenderer->TranslateTransform(_poObj->oGetPos().x,_poObj->oGetPos().y);
			m_poRenderer->RotateTransform(_poObj->rGetAngle());

			#ifdef USE_SKELANIM_SCALE_COLOR
			m_poRenderer->ScaleTransform(_poObj->oGetScale().x,_poObj->oGetScale().y);
			#endif

			m_rDepthBase *= DEPTH_FACT;
			m_rCurDepth  += m_rDepthBase*_poObj->rGetDepth();

			for (uint i=0;i<_poObj->uiNumChildren();i++)
				_poObj->poGetChild(i)->Accept(this);

		m_rDepthBase  = rOldBaseDepth;
		m_rCurDepth   = rOldDepth;
		m_poRenderer->PopTransform();

	#ifdef USE_SKELANIM_SCALE_COLOR
	m_oColor = oOldColor;
	#endif
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeRenderer::Visit(CFESkelAnimBone* _poObj)
{
    if (! _poObj->bIsVisible()) return;
	if (_poObj->uiNumChildren() == 0) return;
		
    /// Attach this bone to the given mesh.
    #ifdef USE_SKELANIM_SCALE_COLOR
	CFEColor oOldColor = m_oColor;
	m_oColor *= _poObj->oGetColor();
	#endif

			CFEVect2 oPos    = _poObj->oGetBonePos() + _poObj->oGetPos();

			m_poRenderer->PushTransform();
			FEReal rOldDepth		= m_rCurDepth;
			FEReal rOldDepthBase	= m_rDepthBase;

				#ifdef BNB_SKEL_SYSTEM
					m_poRenderer->TranslateTransform(oAbsPos.x,oAbsPos.y);
					m_poRenderer->RotateTransform(_poObj->rGetAngle());
				#else
					m_poRenderer->TranslateTransform(oPos.x,oPos.y);
					m_poRenderer->RotateTransform(_poObj->rGetAngle());

					#ifdef USE_SKELANIM_SCALE_COLOR
					m_poRenderer->ScaleTransform(_poObj->oGetScale().x,_poObj->oGetScale().y);
					#endif
				#endif

				m_rDepthBase			= DEPTH_FACT;
				m_rCurDepth				= m_rDepth + m_rDepthBase*_poObj->rGetDepth();	// use the initial depth

				for (uint i=0;i<_poObj->uiNumChildren();i++)
					_poObj->poGetChild(i)->Accept(this);

			m_rDepthBase  = rOldDepthBase;
			m_rCurDepth   = rOldDepth;
			m_poRenderer->PopTransform();

	#ifdef USE_SKELANIM_SCALE_COLOR
	m_oColor = oOldColor;
	#endif
}
// ----------------------------------------------------------------------------
const FEReal JOINT_MARGIN = _1r / _32r;
//-----------------------------------------------------------------------------
void CFESkelAnimNodeRenderer::Visit(CFESkelAnimMesh* _poObj)
{
	if (! _poObj->bIsVisible()) return;

    FEHandler hSpr = _poObj->hGetSprite();
    if (hSpr == NULL) return;

    CFESpriteFrame* poSprFrame = CFESpriteInstMgr::I()->poGetCurrentFrame(hSpr);
    if (poSprFrame == NULL) return;

	// -------------------------
    /// Get vertex arrays base pointers
	// -------------------------
    CFEVect2* poLastSFVX = _poObj->poGetLastSFVX();
    CFEVect2* poVX		 = _poObj->poGetVX();

	if (poSprFrame != _poObj->poGetLastSpriteFrame())
	{
		CFEVect2* poUV       = _poObj->poGetUV();
    
		// -------------------------
		// Setup texcoords
		// -------------------------
		poUV[0].x = poSprFrame->m_oUV.m_oIni.x;
		poUV[0].y = poSprFrame->m_oUV.m_oIni.y;
		poUV[2].x = poSprFrame->m_oUV.m_oEnd.x;
		poUV[2].y = poSprFrame->m_oUV.m_oEnd.y;
		
		poUV[1].x = poUV[2].x;
		poUV[1].y = poUV[0].y;
		poUV[3].x = poUV[0].x;
		poUV[3].y = poUV[2].y;

		// -------------------------
		// Setup texcoords
		// -------------------------
		CFESpriteInstMgr::I()->GetGeometry(hSpr,poLastSFVX);

		for (uint i=0;i<4;i++)
			poVX[i] = poLastSFVX[i];

		_poObj->SetLastSpriteFrame(poSprFrame);
	}

	// -------------------------
	// Setup vertices
	// -------------------------
	for (uint i=0;( (i<4) && (i<_poObj->uiGetNumAttachedBones()) );i++)
	{
		if (_poObj->poGetAttachedBone(i) != NULL)
		{
			CFEVect2 oD = _poObj->poGetAttachedBone(i)->oGetPos() - _poObj->m_oIniBonePos[i];
			poVX[i] = poLastSFVX[i] + oD;
		}
	}

	// -------------------------
    /// Render mesh
    // -------------------------
    FEReal rDepth = m_rCurDepth + _poObj->rGetDepth()*m_rDepthBase*DEPTH_FACT;

    m_poRenderer->SetBlendMode(BM_ALPHA);
    m_poRenderer->SetMaterial( poSprFrame->m_hMaterial );
    m_poRenderer->SetDepth(rDepth);

	// -------------------------
	#ifdef USE_SKELANIM_SCALE_COLOR
	CFEColor oResColor	= m_oColor * _poObj->oGetColor();
    m_poRenderer->RenderMesh(_poObj->pusGetIdxs(),_poObj->poGetVX(),_poObj->poGetUV(),oResColor,6);
	#else
    m_poRenderer->RenderMesh(_poObj->pusGetIdxs(),_poObj->poGetVX(),_poObj->poGetUV(),m_oColor,6);
	#endif
}
//-----------------------------------------------------------------------------
void CFESkelAnimRenderer::Render(CFERenderer *_poRenderer,CFESkelAnimNode* _poNode,const CFEVect2& _oPos,FEReal _rDepth,const CFEVect2& _oScale,FEReal _rAngle,const CFEColor& _oColor)
{
	_poRenderer->PushTransform();

		_poRenderer->TranslateTransform(_oPos.x,_oPos.y);
		_poRenderer->RotateTransform(_rAngle);
		_poRenderer->ScaleTransform(_oScale.x,_oScale.y);

		CFESkelAnimNodeRenderer::Render(_poNode,_poRenderer,_rDepth,_oColor);

    _poRenderer->PopTransform();
}
//-----------------------------------------------------------------------------
