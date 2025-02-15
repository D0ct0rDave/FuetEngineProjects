// ----------------------------------------------------------------------------
/*! \class CFEClass
 *  \brief CFEClass Class Definition
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEScreenUtils.h"
#include "Types/CFEVect2.h"
#include "System/CFESystem.h"
#include "Support/Graphics/CFERenderer.h"
// ----------------------------------------------------------------------------
void CFEScreenUtils::FSRender(CFERenderer* _poRenderer,FEHandler _hMat,const CFEColor& _oColor,FEReal _rDepth)
{
	_poRenderer->SetDepth(_rDepth);
	_poRenderer->SetMaterial(_hMat);
	_poRenderer->RenderQuad(0,0,_poRenderer->uiGetScreenVWidth(),_poRenderer->uiGetScreenVHeight(),0,0,1,1,_oColor);
}
// ----------------------------------------------------------------------------
void CFEScreenUtils::FSRender(CFERenderer* _poRenderer,FEHandler _hMat,const CFEColor& _oColor,FEReal _rAlpha,FEReal _rDepth)
{
    CFEColor oColor = _oColor;
    oColor.a = _rAlpha;
    FSRender(_poRenderer,_hMat,oColor,_rDepth);
}
// ----------------------------------------------------------------------------
void CFEScreenUtils::FSRender(CFERenderer* _poRenderer,FEHandler _hMat,FEReal _rDepth)
{
    FSRender(_poRenderer,_hMat,CFEColor::WHITE(),_rDepth);
}
// ----------------------------------------------------------------------------
