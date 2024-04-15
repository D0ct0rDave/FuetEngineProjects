//-----------------------------------------------------------------------------
/*! \class CFEStatedObject
 *  \brief A class to derive from for objects with states
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par PenguinWars License
 */
//-----------------------------------------------------------------------------
#include "CFEStatedObject.h"
//-----------------------------------------------------------------------------
CFEStatedObject::CFEStatedObject() : m_uiState(0), m_uiPrevState(0), m_bAllowReentrance(false), m_rStateTime(_0r)
{
}
//-----------------------------------------------------------------------------
CFEStatedObject::~CFEStatedObject()
{
}
//-----------------------------------------------------------------------------
void CFEStatedObject::OnEnterState(uint _uiState)
{
}
//-----------------------------------------------------------------------------		
void CFEStatedObject::OnExitState(uint _uiState, uint _uiNewState)
{
}
//-----------------------------------------------------------------------------
