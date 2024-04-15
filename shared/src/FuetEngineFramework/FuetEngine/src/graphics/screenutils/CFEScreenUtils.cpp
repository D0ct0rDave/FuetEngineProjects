// ----------------------------------------------------------------------------
/*! \class CFEClass
 *  \brief CFEClass Class Definition
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEScreenUtils.h"
#include "Types/CFEVect2.h"
#include "core/CFECore.h"
#include "Support/Graphics/CFERenderer.h"
// ----------------------------------------------------------------------------
void CFEScreenUtils::FSRender(CFERenderer* _poRenderer,FEHandler _hMat,const CFEColor& _oColor,FEReal _rDepth)
{
	CFEVect2 oViewTrans = _poRenderer->oGetViewTranslation();
	_poRenderer->TranslateView(_0r,_0r);

	FEReal rWMargin = _0r;
	FEReal rHMargin = _0r;

	// Safe margin for 3D displays
	if (_hMat == NULL)
	{
		rWMargin = _01r * _poRenderer->uiGetScreenVWidth() * _05r; // * _rDepth
		rHMargin = _01r * _poRenderer->uiGetScreenVHeight()* _05r; // * _rDepth
	}

	_poRenderer->SetDepth(_rDepth);
	_poRenderer->SetMaterial(_hMat);
	_poRenderer->RenderQuad(-rWMargin,
							-rHMargin,
							_poRenderer->uiGetScreenVWidth() +rWMargin,
							_poRenderer->uiGetScreenVHeight()+rHMargin,
							_oColor);
	
	_poRenderer->TranslateView(oViewTrans.x,oViewTrans.y);
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
