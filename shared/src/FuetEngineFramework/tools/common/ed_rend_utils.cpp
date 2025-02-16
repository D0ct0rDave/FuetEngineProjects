//-----------------------------------------------------------------------------
/*! \class Editor Utilities
 *  \brief 
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License Rally
 */
//-----------------------------------------------------------------------------
#include "ed_rend_utils.h"
///-----------------------------------------------------------------
void RenderGrid(CFERenderer* _poRenderer,int _iGridSize,const CFEColor& _oGridColor,const CFEColor& _oGridHLColor)
{
	const FEReal INI = -2000.0f;
	const FEReal END =  2000.0f;

	FEReal rXIni = (int)(INI / (FEReal)_iGridSize)*_iGridSize;
	FEReal rYIni = (int)(INI / (FEReal)_iGridSize)*_iGridSize;
	FEReal rXEnd = (int)(END / (FEReal)_iGridSize)*_iGridSize;
	FEReal rYEnd = (int)(END / (FEReal)_iGridSize)*_iGridSize;
	
	uint uiHLines = (int)(rXEnd-rXIni) / _iGridSize;
    uint uiVLines = (int)(rYEnd-rYIni) / _iGridSize;
    uint i;

    for (i=0;i<=uiHLines;i++)
    {
        _poRenderer->RenderLine(rXIni,
                                rYIni + i*_iGridSize,
                                rXEnd,
                                rYIni + i*_iGridSize,
                                _oGridColor);
    }

    for (i=0;i<=uiVLines;i++)
    {
        _poRenderer->RenderLine(rXIni + i*_iGridSize,
                                rYIni,
                                rXIni + i*_iGridSize,
                                rYEnd,
                                _oGridColor);
    }
    
    _poRenderer->RenderLine(rXIni,0,rXEnd,0,_oGridHLColor);
    _poRenderer->RenderLine(0,rYIni,0,rYEnd,_oGridHLColor);
    
}
//---------------------------------------------------------------------------
void RenderPivot(CFERenderer* _poRenderer,const CFEVect2& _oPos,const CFEColor& _oColor)
{
	// Render Pivot
    _poRenderer->RenderLine(_oPos.x  ,_oPos.y-5,_oPos.x  ,_oPos.y+5,_oColor);
    _poRenderer->RenderLine(_oPos.x-5,_oPos.y  ,_oPos.x+5,_oPos.y  ,_oColor);
}
// ----------------------------------------------------------------------------
