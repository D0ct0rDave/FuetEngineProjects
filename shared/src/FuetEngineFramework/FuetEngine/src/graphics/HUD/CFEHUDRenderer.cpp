// ----------------------------------------------------------------------------
/*! \class CFEHUDLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "support/graphics/CFERenderer.h"
#include "graphics/sprite/CFESpriteInstMgr.h"
#include "graphics/sprite/CFESpriteRenderer.h"
#include "graphics/mesh/CFEMeshInstMgr.h"
#include "graphics/mesh/CFEMeshRenderer.h"
#include "graphics/particlesys/CFEParticleSysInstMgr.h"
#include "graphics/particlesys/CFEParticleMgr.h"

#include "CFEHUDRenderer.h"
#include "CFEHUDAction.h"
#include "CFEHUD.h"
#include "CFEHUDElement.h"
#include "CFEHUDObject.h"
#include "CFEHUDGroup.h"
#include "CFEHUDLabel.h"
#include "CFEHUDIcon.h"
#include "CFEHUDRect.h"
#include "CFEHUDShape.h"
#include "CFEHUDPSys.h"
//-----------------------------------------------------------------------------
inline CFEColor oRenderColor(const CFEColor& _oModColor,CFEHUDObject* _poObj)
{
	return( _oModColor * _poObj->oGetColor() );
}
//-----------------------------------------------------------------------------
inline FEReal rRenderDepth(FEReal _rDepth,FEReal _rDepthFact,CFEHUDObject* _poObj)
{
	return(_rDepth + _poObj->rGetDepth()*_rDepthFact);
}
//-----------------------------------------------------------------------------
CFEColor CFEHUDRenderer::oGetRenderColor(CFEHUDObject* _poObj)
{
	return( oRenderColor(m_oModColor, _poObj) );
}
//-----------------------------------------------------------------------------
FEReal CFEHUDRenderer::rGetRenderDepth(CFEHUDObject* _poObj)
{
	return( rRenderDepth(m_rDepth, m_rDepthFact, _poObj) );
}
//-----------------------------------------------------------------------------
void CFEHUDRenderer::Render(CFEHUD* _poHUD,CFERenderer* _poRenderer,FEReal _rPSDeltaT)
{
	CFEHUDRenderer oHUDRenderer;
	oHUDRenderer.SetRenderer(_poRenderer);
	oHUDRenderer.SetPSDeltaT(_rPSDeltaT);
	_poHUD->Accept(&oHUDRenderer);
}
//-----------------------------------------------------------------------------
void CFEHUDRenderer::Render(CFEHUDElement* _poElement,CFERenderer* _poRenderer,FEReal _rPSDeltaT)
{
	CFEHUDRenderer oHUDRenderer;
	oHUDRenderer.SetRenderer(_poRenderer);
	oHUDRenderer.SetPSDeltaT(_rPSDeltaT);
	_poElement->Accept(&oHUDRenderer);
}
//-----------------------------------------------------------------------------
void CFEHUDRenderer::Visit(CFEHUD* _poObj)
{
    for (uint i=0;i<_poObj->uiNumElements();i++)
        _poObj->poGetElement(i)->Accept(this);
}
//-----------------------------------------------------------------------------
void CFEHUDRenderer::Visit(CFEHUDElement* _poObj)
{
    for (uint i=0;i<_poObj->uiNumLayers();i++)
        _poObj->poGetLayer(i)->Accept(this);
}
//-----------------------------------------------------------------------------
void CFEHUDRenderer::Visit(CFEHUDObject* _poObj)
{
    // Nothing to do on undefined objects.
    return;
}
//-----------------------------------------------------------------------------
void CFEHUDRenderer::Visit(CFEHUDGroup* _poObj)
{	
    if (! _poObj->bIsVisible()) return;

    CFEColor oNewModColor = m_oModColor * _poObj->oGetColor();
    if (oNewModColor.a<=_0r) return;

	m_poRenderer->PushTransform();

	m_poRenderer->TranslateTransform(_poObj->oGetPos().x,_poObj->oGetPos().y);
	m_poRenderer->RotateTransform(_poObj->rGetAngle());
	m_poRenderer->ScaleTransform(_poObj->oGetScale().x,_poObj->oGetScale().y);

	// Push current color & depth
	CFEColor oOldModColor = m_oModColor;
	FEReal	rOldDepth	  = m_rDepth;
	FEReal	rOldDepthFact = m_rDepthFact;

		m_oModColor   = oNewModColor;
		m_rDepth      = rGetRenderDepth(_poObj);
		m_rDepthFact *= _poObj->rGetDepthFact();

		for (uint i=0;i<_poObj->uiNumObjs();i++)
			if (_poObj->poGetObject(i) != NULL)
				_poObj->poGetObject(i)->Accept(this);

	// Pop color & depth
	m_oModColor = oOldModColor;
	m_rDepth    = rOldDepth;
	m_rDepthFact = rOldDepthFact;

    m_poRenderer->PopTransform();
}
//-----------------------------------------------------------------------------
void CFEHUDRenderer::Visit(CFEHUDLabel* _poObj)
{	
    if (! _poObj->bIsVisible()) return;

	CFEColor oResColor = oRenderColor(m_oModColor,_poObj);
	if (oResColor.a<=_0r) return;
	
	FEReal rDepth = rGetRenderDepth(_poObj);

	m_poRenderer->PushTransform();

	m_poRenderer->TranslateTransform(_poObj->oGetPos().x,_poObj->oGetPos().y);
	m_poRenderer->RotateTransform(_poObj->rGetAngle());
	m_poRenderer->ScaleTransform(_poObj->oGetScale().x,_poObj->oGetScale().y);

		m_poRenderer->SetBlendMode(BM_ALPHA);			
		m_poRenderer->SetDepth(rDepth);
	    m_poRenderer->SetFont(_poObj->poGetFont());
	    m_poRenderer->SetTextInterlining( _poObj->rGetInterlining() );
	    m_poRenderer->SetTextTracking( _poObj->rGetTracking() );
	    m_poRenderer->SetTextPointSize( _poObj->rGetPointSize() );

        m_poRenderer->RenderText(_poObj->sGetPrintableText(),0,0,oResColor,_poObj->eGetHAlignment(),_poObj->eGetVAlignment());

	m_poRenderer->PopTransform();
}
//-----------------------------------------------------------------------------
void CFEHUDRenderer::Visit(CFEHUDIcon* _poObj)
{	
    if (! _poObj->bIsVisible()) return;

	CFEColor oResColor = oRenderColor(m_oModColor,_poObj);
	if (oResColor.a<=_0r) return;

	FEReal rDepth = rGetRenderDepth(_poObj);

	#if 0
	// Update values. They are needed in some parts of the system.
	CFESpriteInstMgr::I()->SetColor(_poObj->hGetIcon(),oResColor);
	CFESpriteInstMgr::I()->SetAngle(_poObj->hGetIcon(),_poObj->rGetAngle());
	CFESpriteInstMgr::I()->SetScale(_poObj->hGetIcon(),_poObj->oGetScale());
	CFESpriteInstMgr::I()->SetPos  (_poObj->hGetIcon(),_poObj->oGetPos());
	CFESpriteInstMgr::I()->SetDepth(_poObj->hGetIcon(),rDepth);
	#endif

	int iSprAct = _poObj->iGetAction();

	if ((iSprAct != -1) && (CFESpriteInstMgr::I()->uiGetAction(_poObj->hGetIcon()) != iSprAct))
		CFESpriteInstMgr::I()->SetAction(_poObj->hGetIcon(),_poObj->iGetAction());

	CFESpriteRenderer::Render(m_poRenderer,(CFESpriteInst*)_poObj->hGetIcon(),_poObj->oGetPos(),rDepth,_poObj->oGetScale(),_poObj->rGetAngle(),oResColor);
}
//-----------------------------------------------------------------------------
void CFEHUDRenderer::Visit(CFEHUDRect* _poObj)
{
    if (! _poObj->bIsVisible()) return;

	CFEColor oResColor = oRenderColor(m_oModColor,_poObj);
	if (oResColor.a<=_0r) return;

	FEReal rDepth = rGetRenderDepth(_poObj);
	m_poRenderer->PushTransform();

	m_poRenderer->TranslateTransform(_poObj->oGetPos().x,_poObj->oGetPos().y);
	m_poRenderer->RotateTransform(_poObj->rGetAngle());
	m_poRenderer->ScaleTransform(_poObj->oGetScale().x,_poObj->oGetScale().y);

        FEReal rWidth  = _poObj->rGetWidth();
        FEReal rHeight = _poObj->rGetHeight();

        FEReal rXOfs = -_poObj->oGetPivot().x  * rWidth;
        FEReal rYOfs = -_poObj->oGetPivot().y  * rHeight;

        CFEVect2 oVX[4];
        oVX[0].x = rXOfs;
        oVX[0].y = rYOfs;

        oVX[1].x = rXOfs + rWidth;
        oVX[1].y = rYOfs;

        oVX[2].x = rXOfs + rWidth;
        oVX[2].y = rYOfs + rHeight;

        oVX[3].x = rXOfs;
        oVX[3].y = rYOfs + rHeight;

        CFEColor oVC[4];
        oVC[0] = _poObj->oGetCornerColor(0) * oResColor;
        oVC[1] = _poObj->oGetCornerColor(1) * oResColor;
        oVC[2] = _poObj->oGetCornerColor(2) * oResColor;
        oVC[3] = _poObj->oGetCornerColor(3) * oResColor;

        m_poRenderer->SetBlendMode(BM_ALPHA);
        m_poRenderer->SetMaterial(NULL);
		m_poRenderer->SetDepth(rDepth);
        m_poRenderer->RenderQuadList(oVX,NULL,oVC,1);

	m_poRenderer->PopTransform();
}
//-----------------------------------------------------------------------------
void CFEHUDRenderer::Visit(CFEHUDShape* _poObj)
{
    if (! _poObj->bIsVisible()) return;

	CFEColor oResColor = oRenderColor(m_oModColor,_poObj);
	if (oResColor.a<=_0r) return;

	FEReal rDepth = rGetRenderDepth(_poObj);

	// Update values. They are needed in some parts of the system.
	CFEMeshInstMgr::I()->SetColor(_poObj->hGetMesh(),oResColor);
	CFEMeshInstMgr::I()->SetAngle(_poObj->hGetMesh(),_poObj->rGetAngle());
	CFEMeshInstMgr::I()->SetScale(_poObj->hGetMesh(),_poObj->oGetScale());
	CFEMeshInstMgr::I()->SetPos  (_poObj->hGetMesh(),_poObj->oGetPos());
	CFEMeshInstMgr::I()->SetDepth(_poObj->hGetMesh(),rDepth);

	int iMeshAct = _poObj->iGetAction();
	if ((iMeshAct != -1) && (CFEMeshInstMgr::I()->uiGetAction(_poObj->hGetMesh()) != iMeshAct))
		CFEMeshInstMgr::I()->SetAction(_poObj->hGetMesh(),_poObj->iGetAction());

    CFEMeshRenderer::Render(m_poRenderer,(CFEMeshInst*)_poObj->hGetMesh(),_poObj->oGetPos(),rDepth,_poObj->oGetScale(),_poObj->rGetAngle(),oResColor);
}
//-----------------------------------------------------------------------------
void CFEHUDRenderer::Visit(CFEHUDPSys* _poObj)
{
	if ( CFEParticleSysInstMgr::I()->bIsEnabled(_poObj->hGetPSys()) )
	{
		// Update values. They are needed in some parts of the system.
		FEReal rDepth = rGetRenderDepth(_poObj);
		CFEVect2 oNewPos = m_poRenderer->oGetTransform().Transform( _poObj->oGetPos() );

		CFEParticleSysInstMgr::I()->SetPos  (_poObj->hGetPSys(),oNewPos);
		CFEParticleSysInstMgr::I()->SetDepth(_poObj->hGetPSys(),rDepth);

		CFEParticleSysInstMgr::I()->Update(_poObj->hGetPSys(), m_rPSDeltaT );
	}
}
//-----------------------------------------------------------------------------
void CFEHUDRenderer::Visit(CFEHUDElementAction* _poObj)
{
}
//-----------------------------------------------------------------------------
void CFEHUDRenderer::Visit(CFEHUDObjectAction* _poObj)
{
}
//-----------------------------------------------------------------------------
