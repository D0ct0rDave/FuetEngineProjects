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
#ifndef ed_rend_utilsH
#define ed_rend_utilsH
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <wx/string.h>
#include <FuetEngine.h>
// ----------------------------------------------------------------------------

void RenderGrid(CFERenderer* _poRenderer,int _iGridSize,const CFEColor& _oGridColor,const CFEColor& _oGridHLColor);
void RenderPivot(CFERenderer* _poRenderer,const CFEVect2& _oPos,const CFEColor& _oColor);

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
