// ----------------------------------------------------------------------------
/*! \class CFEHUDLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "support/graphics/CFERenderer.h"
#include "graphics/sprite/CFESpriteInstMgr.h"
#include "graphics/sprite/CFESpriteRenderer.h"

#include "CFEHUDRenderer.h"
#include "CFEHUDAction.h"
#include "CFEHUD.h"
#include "CFEHUDelement.h"
#include "CFEHUDObject.h"
#include "CFEHUDGroup.h"
#include "CFEHUDLabel.h"
#include "CFEHUDIcon.h"
#include "CFEHUDRect.h"
#include "CFEHUDShape.h"
//-----------------------------------------------------------------------------
void CFEHUDRenderer::Render(FEHandler _hHUDInstance,CFERenderer* _poRenderer)
{	
	CFEHUDRenderer oHUDRenderer;
	oHUDRenderer.SetRenderer(_poRenderer);
	((CFEHUD*)_hHUDInstance)->Accept(&oHUDRenderer);
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
	
	m_poRenderer->PushTransform();

	m_poRenderer->TranslateTransform(_poObj->oGetPos().x,_poObj->oGetPos().y);
	m_poRenderer->RotateTransform(_poObj->rGetAngle());
	m_poRenderer->ScaleTransform(_poObj->oGetScale().x,_poObj->oGetScale().y);

	// Push current color & depth
	CFEColor oOldModColor = m_oModColor;
	FEReal	rOldDepth = m_rDepth;

	m_oModColor = m_oModColor * _poObj->oGetColor();
	m_rDepth    = _poObj->rGetDepth();					// Group depth are not cumulative.

		for (uint i=0;i<_poObj->uiNumObjs();i++)
			_poObj->poGetObject(i)->Accept(this);

	// Pop color & depth
	m_oModColor = oOldModColor;
	m_rDepth    = rOldDepth;
    m_poRenderer->PopTransform();
}
//-----------------------------------------------------------------------------
void CFEHUDRenderer::Visit(CFEHUDLabel* _poObj)
{	
    if (! _poObj->bIsVisible()) return;
	
	m_poRenderer->PushTransform();

	m_poRenderer->TranslateTransform(_poObj->oGetPos().x,_poObj->oGetPos().y);
	m_poRenderer->RotateTransform(_poObj->rGetAngle());
	m_poRenderer->ScaleTransform(_poObj->oGetScale().x,_poObj->oGetScale().y);
	CFEColor oResColor = m_oModColor * _poObj->oGetColor();

		m_poRenderer->SetBlendMode(BM_ALPHA);			
		m_poRenderer->SetDepth(m_rDepth + _poObj->rGetDepth()*_01r);
	    m_poRenderer->SetFont(_poObj->poGetFont());
        m_poRenderer->RenderText(_poObj->sGetText(),0,0,oResColor,_poObj->eGetHAlignment(),_poObj->eGetVAlignment());

	m_poRenderer->PopTransform();
}
//-----------------------------------------------------------------------------
void CFEHUDRenderer::Visit(CFEHUDIcon* _poObj)
{	
    if (! _poObj->bIsVisible()) return;
	
	// Update values. They are needed in some parts of the system.
	FEReal rDepth = m_rDepth + _poObj->rGetDepth()*_01r;
	CFESpriteInstMgr::SetColor(_poObj->hGetIcon(),_poObj->oGetColor());
	CFESpriteInstMgr::SetAngle(_poObj->hGetIcon(),_poObj->rGetAngle());
	CFESpriteInstMgr::SetScale(_poObj->hGetIcon(),_poObj->oGetScale());
	CFESpriteInstMgr::SetPos  (_poObj->hGetIcon(),_poObj->oGetPos());
	CFESpriteInstMgr::SetDepth(_poObj->hGetIcon(),rDepth);
	// CFESpriteInstMgr::SetAction(_poObj->hGetIcon(),_poObj->iGetAction());

	CFEColor oResColor = m_oModColor * _poObj->oGetColor();
    CFESpriteRenderer::Render(m_poRenderer,(CFESpriteInst*)_poObj->hGetIcon(),_poObj->oGetPos(),rDepth,_poObj->oGetScale(),_poObj->rGetAngle(),oResColor);
}
//-----------------------------------------------------------------------------
void CFEHUDRenderer::Visit(CFEHUDRect* _poObj)
{
    if (! _poObj->bIsVisible()) return;

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
        
        CFEColor oResColor = m_oModColor * _poObj->oGetColor();

        oVC[0] = _poObj->oGetCornerColor(0) * oResColor;
        oVC[1] = _poObj->oGetCornerColor(1) * oResColor;
        oVC[2] = _poObj->oGetCornerColor(2) * oResColor;
        oVC[3] = _poObj->oGetCornerColor(3) * oResColor;
        
		m_poRenderer->SetDepth(m_rDepth + _poObj->rGetDepth()*_01r);
        m_poRenderer->RenderQuad(oVX,NULL,oVC);

	m_poRenderer->PopTransform();    
}
//-----------------------------------------------------------------------------
void CFEHUDRenderer::Visit(CFEHUDShape* _poObj)
{
	
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
