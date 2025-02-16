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
//-----------------------------------------------------------------------------
void CFEHUDRenderer::Render(FEHandler _hHUDInstance,CFERenderer* _poRenderer)
{
    m_poRenderer = _poRenderer;
    ((CFEHUD*)_hHUDInstance)->Accept(this);
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

	for (uint i=0;i<_poObj->uiNumObjs();i++)
	    _poObj->poGetObject(i)->Accept(this);

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

		m_poRenderer->SetBlendMode(BM_ALPHA);			
	    m_poRenderer->SetFont(_poObj->poGetFont());
        m_poRenderer->RenderText(_poObj->sGetText(),0,0,_poObj->oGetColor(),_poObj->eGetHAlignment(),_poObj->eGetVAlignment());

	m_poRenderer->PopTransform();
}
//-----------------------------------------------------------------------------
void CFEHUDRenderer::Visit(CFEHUDIcon* _poObj)
{	
    if (! _poObj->bIsVisible()) return;
	
	CFESpriteInstMgr::SetColor(_poObj->hGetIcon(),_poObj->oGetColor());
	CFESpriteInstMgr::SetAngle(_poObj->hGetIcon(),_poObj->rGetAngle());
	CFESpriteInstMgr::SetScale(_poObj->hGetIcon(),_poObj->oGetScale());
	CFESpriteInstMgr::SetPos  (_poObj->hGetIcon(),_poObj->oGetPos());

    CFESpriteRenderer::Render(m_poRenderer,(CFESpriteInst*)_poObj->hGetIcon(),_poObj->oGetPos(),_0r,_poObj->oGetScale(),_poObj->rGetAngle(),_poObj->oGetColor());
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
        oVC[0] = _poObj->oGetCornerColor(0) * _poObj->oGetColor();
        oVC[1] = _poObj->oGetCornerColor(1) * _poObj->oGetColor();
        oVC[2] = _poObj->oGetCornerColor(2) * _poObj->oGetColor();
        oVC[3] = _poObj->oGetCornerColor(3) * _poObj->oGetColor();
    
        m_poRenderer->RenderQuad(oVX,NULL,oVC);

	m_poRenderer->PopTransform();    
}
//-----------------------------------------------------------------------------
