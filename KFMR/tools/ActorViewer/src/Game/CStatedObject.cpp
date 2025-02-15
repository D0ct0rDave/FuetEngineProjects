//-----------------------------------------------------------------------------
/*! \class CStatedObject
 *  \brief A class to derive from for objects with states
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#include "CStatedObject.h"
//-----------------------------------------------------------------------------
CStatedObject::CStatedObject() : m_uiState(0), m_uiPrevState(0), m_bAllowReentrance(false)
{
}
//-----------------------------------------------------------------------------
CStatedObject::~CStatedObject()
{
}
//-----------------------------------------------------------------------------