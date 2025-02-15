// -----------------------------------------------------------------------------
/*! \class CActionMoveEntityTo
 *  \brief This action moves an entity from its current position to a destination one in a given amount of time.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CActionMoveEntityTo.h"
#include "Game/Entity/CEntityRegistry.h"
// -----------------------------------------------------------------------------
void CActionMoveEntityTo::Init(const CFEString& _sEntityName,const CFEVect2& _oFinalPos,FEReal _rTime)
{
	m_sEntityName	= _sEntityName;
	m_oFinalPos		= _oFinalPos;
	m_rTotalTime    = _rTime;
}
// -----------------------------------------------------------------------------
void CActionMoveEntityTo::Reset()
{
	m_poEntity		= CEntityRegistry::poGetEntity(m_sEntityName);
	m_oInitialPos	= m_poEntity->oGetPos();
	m_rTime			= m_rTotalTime;
}
// -----------------------------------------------------------------------------
