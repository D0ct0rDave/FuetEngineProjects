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
#include "CFEHUDInstancer.h"

#include "CFEHUDAction.h"
#include "CFEHUD.h"
#include "CFEHUDelement.h"
#include "CFEHUDObject.h"
#include "CFEHUDGroup.h"
#include "CFEHUDLabel.h"
#include "CFEHUDIcon.h"
//-----------------------------------------------------------------------------
CFEHUD* CFEHUDInstancer::poCreateInstance(CFEHUD* _poHUDModel)
{
    // Temporary
    return( _poHUDModel );
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUD* _poObj)
{
/*
    for (uint i=0;i<_poObj->uiNumElements();i++)
        _poObj->poGetElement(i)->Accept(this);
*/
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDElement* _poObj)
{
/*
    for (uint i=0;i<_poObj->uiNumLayers();i++)
        _poObj->poGetLayer(i)->Accept(this);
*/
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDObject* _poObj)
{
    _poObj->Accept(this);
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDGroup* _poObj)
{
/*	
    if (! _poObj->bIsVisible()) return;
	
	m_poRenderer->PushTransform();

	m_poRenderer->TranslateTransform(_poObj->oGetPos().x,_poObj->oGetPos().y);
	m_poRenderer->RotateTransform(_poObj->rGetAngle());
	m_poRenderer->ScaleTransform(_poObj->oGetScale().x,_poObj->oGetScale().y);

	for (uint i=0;i<_poObj->uiNumObjs();i++)
	    _poObj->poGetObject(i)->Accept(this);

    m_poRenderer->PopTransform();
*/
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDLabel* _poObj)
{
/*	
    if (! _poObj->bIsVisible()) return;

	m_poRenderer->PushTransform();

	m_poRenderer->TranslateTransform(_poObj->oGetPos().x,_poObj->oGetPos().y);
	m_poRenderer->RotateTransform(_poObj->rGetAngle());
	m_poRenderer->ScaleTransform(_poObj->oGetSize().x,_poObj->oGetSize().y);

	    m_poRenderer->SetFont(_poObj->poGetFont());
        m_poRenderer->RenderText(_poObj->sGetText(),0,0,_poObj->oGetColor(),_poObj->eGetHAlignment(),_poObj->eGetVAlignment());

	m_poRenderer->PopTransform();    
*/
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDIcon* _poObj)
{
/*	
    if (! _poObj->bIsVisible()) return;
	
	CFESpriteInstMgr::SetColor(_poObj->hGetIcon(),_poObj->oGetColor());
	CFESpriteInstMgr::SetAngle(_poObj->hGetIcon(),_poObj->rGetAngle());
	CFESpriteInstMgr::SetSize (_poObj->hGetIcon(),_poObj->oGetSize());
	CFESpriteInstMgr::SetPos  (_poObj->hGetIcon(),_poObj->oGetPos());

    CFESpriteRenderer::Render(m_poRenderer,(CFESpriteInst*)_poObj->hGetIcon(),_poObj->oGetPos(),_poObj->oGetScale(),_poObj->rGetAngle(),_poObj->oGetColor());
*/
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDRect* _poObj)
{
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDElementAction* _poObj)
{
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDObjectAction* _poObj)
{
}
//-----------------------------------------------------------------------------
