//-----------------------------------------------------------------------------
/*! \class Editor Utilities
 *  \brief 
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par License Rally
 */
//-----------------------------------------------------------------------------
#include "ed_rend_utils.h"
///-----------------------------------------------------------------
void RenderGrid(CFERenderer* _poRenderer,int _iGridSize,const CFEColor& _oGridColor,const CFEColor& _oGridHLColor)
{
	const FEReal INI = -2000.0f;
	const FEReal END =  2000.0f;

	CFERect oRect;
	oRect.m_oIni.x = INI;
	oRect.m_oIni.y = INI;
	oRect.m_oEnd.x = END;
	oRect.m_oEnd.y = END;
	RenderGrid(_poRenderer,oRect,_iGridSize,_oGridColor,_oGridHLColor);
}

void RenderGrid(CFERenderer* _poRenderer,const CFERect& _oRect,int _iGridSize,const CFEColor& _oGridColor,const CFEColor& _oGridHLColor)
{
	_poRenderer->SetMaterial(NULL);

	FEReal rXIni = ((int)(_oRect.m_oIni.x / (FEReal)_iGridSize)-1)*_iGridSize;
	FEReal rYIni = ((int)(_oRect.m_oIni.y / (FEReal)_iGridSize)-1)*_iGridSize;
	FEReal rXEnd = ((int)(_oRect.m_oEnd.x / (FEReal)_iGridSize)+1)*_iGridSize;
	FEReal rYEnd = ((int)(_oRect.m_oEnd.y / (FEReal)_iGridSize)+1)*_iGridSize;
	
	uint uiHLines = (int)(rYEnd-rYIni) / _iGridSize;
    uint uiVLines = (int)(rXEnd-rXIni) / _iGridSize;
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
    
    
    // Rener coordinate axis.
    _poRenderer->RenderLine(rXIni,0,rXEnd,0,_oGridHLColor);
    _poRenderer->RenderLine(0,rYIni,0,rYEnd,_oGridHLColor);
    
}
//---------------------------------------------------------------------------
void RenderPivot(CFERenderer* _poRenderer,const CFEVect2& _oPos,const CFEColor& _oColor)
{
	// Render Pivot
	_poRenderer->SetMaterial(NULL);
    _poRenderer->RenderLine(_oPos.x  ,_oPos.y-5,_oPos.x  ,_oPos.y+5,_oColor);
    _poRenderer->RenderLine(_oPos.x-5,_oPos.y  ,_oPos.x+5,_oPos.y  ,_oColor);
}
// ----------------------------------------------------------------------------
